#include <stdbool.h>

#include "./codegen.h"
#include "./parsing.h"

#include "./primitives/String.h"

struct String codegen(struct Codegen_Args const args,
		struct String const template_string)
{
	int init_amount = (2 * template_string.length < 1024)
			? 1024
			: 2 * template_string.length;
	struct Arena arena = Arena_init(init_amount);
	struct Arena binding_zero = Arena_init(100);
	struct String retval = String_init(&arena, -1);

	// :include_state needs to be correctly set by end of each iteration.
	// If there are includes, :include_state is primed after every newline.
	struct Buffer_Parametric_Binding const* bindings = args.bindings;
	char* str = template_string.str;
	int str_len = template_string.length;

	enum {DONE, PRIMED, LATENT} include_state = DONE;

	if (0 < args.includes->length)
	{
		include_state = PRIMED;  // Also primed at the very beginning.
	}

	for (int i = 0; i < str_len;)
	{
		// Inject `#include`.
		if (include_state == PRIMED)
		{
			// First line of code is a good spot to inject.
			// Code is not comments, macros, or whitespace.
			char first_non_whitespace = 0;

			for (int j = i; j < str_len; j++)
			{
				first_non_whitespace = str[j];

				if (first_non_whitespace == '\n'
						|| !is_whitespace(first_non_whitespace))
				{
					break;
				}
			}

			bool insert_includes = first_non_whitespace != '\n'
					&& first_non_whitespace != '#'
					&& first_non_whitespace != '*'
					&& first_non_whitespace != '/';

			if (insert_includes)
			{
				struct Buffer_String const* includes = args.includes;
				struct String const* ibuf = includes->buffer;
				int ilen = includes->length;

				struct String prefix = String_from_cstring("#include \"");
				struct String suffix = String_from_cstring("\"\n");

				for (int j = 0; j < ilen; j++)
				{
					struct String include_path = ibuf[j];

					retval = String_append(retval, prefix);
					retval = String_append(retval, include_path);
					retval = String_append(retval, suffix);
				}

				retval = String_push(retval, '\n');
				include_state = DONE;
			}
			else
			{
				include_state = LATENT;  // Try again next newline.
			}
		}

		// Each iteration we either match on a type name, type instance, or
		// default to appending the current character.
		char c = str[i];
		struct Arena binding_arena = binding_zero;
		struct String_Offset append = match_type_name(bindings, args.style,
				&binding_arena, template_string, i);

		if (append.offset <= 0)
		{
			append = match_instance_name(bindings, template_string, i);
		}

		if (0 < append.offset)
		{
			retval = String_append(retval, append.string);
			i += append.offset;
		}
		else
		{
			retval = String_push(retval, c);
			i += 1;
		}

		if (include_state != DONE && c == '\n')
		{
			include_state = PRIMED;
		}
	}

	return retval;
}
