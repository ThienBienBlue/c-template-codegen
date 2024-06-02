#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "./String.h"
#include "./Parametric_Binding.h"
#include "./Buffer_Parametric_Binding.h"

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(int capacity)
{
	if (capacity < 0)
	{
		capacity = 0;
	}

	struct Buffer_Parametric_Binding* retval = (struct Buffer_Parametric_Binding*)malloc(sizeof(struct Buffer_Parametric_Binding) + capacity * sizeof(struct Parametric_Binding));

	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_filter(struct Buffer_Parametric_Binding* self, bool (*filter)(struct Parametric_Binding))
{
	int capacity = self->capacity;
	struct Parametric_Binding* buffer = self->buffer;
	struct Buffer_Parametric_Binding* retval = Buffer_Parametric_Binding_init(capacity);

	for (int idx = 0; idx < capacity; idx++)
	{
		struct Parametric_Binding item = buffer[idx];

		if (filter(item))
		{
			Buffer_Parametric_Binding_push(retval, item);
		}
	}

	return retval;
}

bool write_to(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item, int idx)
{
	// Assignment won't compile if struct Parametric_Binding has const fields.
	struct Parametric_Binding* write = self->buffer + idx;
	void* written = memmove(write, &item, sizeof(struct Parametric_Binding));

	return written != NULL;
}

bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item)
{

	if (self != NULL && self->length < self->capacity)
	{
		bool retval = write_to(self, item, self->length);

		if (retval)
		{
			self->length++;
		}

		return retval;
	}
	else
	{
		return false;
	}
}

bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self)
{
	if (self == NULL || self->length <= 0)
	{
		return false;
	}

	self->length--;

	return true;
}

bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, int left_idx, int right_idx)
{
	if (self == NULL)
	{
		return false;
	}

	int len = self->length;
	struct Parametric_Binding* buffer = self->buffer;

	if (left_idx < 0 || len <= left_idx || right_idx < 0 || len <= right_idx)
	{
		return false;
	}

	struct Parametric_Binding left = buffer[left_idx];
	struct Parametric_Binding right = buffer[right_idx];

	bool left_written = write_to(self, right, left_idx);
	bool right_written = write_to(self, left, right_idx);

	return left_written && right_written;
}
