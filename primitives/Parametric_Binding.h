#ifndef PARAMETRIC_BINDING_H_
#define PARAMETRIC_BINDING_H_

#include "./String.h"

struct Parametric_Binding
{
	char parametric;
	struct String type_name;
	struct String type_instance;
};

struct Parametric_Binding Parametric_Binding_init(char c, char* type_name,
		char* type_instance);

#endif
