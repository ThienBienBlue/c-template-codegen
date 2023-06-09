#include <stdbool.h>
#include <stdlib.h>

#include "./Buffer_<T>.h"

struct Buffer_<T>* Buffer_<T>_init(int capacity)
{
	if (capacity < 0)
		capacity = 0;
	struct Buffer_<T>* retval = (struct Buffer_<T>*)malloc(sizeof(struct Buffer_<T>) + capacity * sizeof(T));
	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

bool Buffer_<T>_push(struct Buffer_<T>* self, T item)
{
	if (self == NULL || self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer_<T>_pop(struct Buffer_<T>* self)
{
	if (self == NULL || self->length <= 0)
		return false;
	self->length--;
	return true;
}

bool Buffer_<T>_swap(struct Buffer_<T>* self, int left_idx, int right_idx)
{
	if (self == NULL)
		return false;

	int buffer_length = self->length;
	T* buffer = self->buffer;
	if (!(0 <= left_idx && left_idx < buffer_length && 0 <= right_idx && right_idx < buffer_length))
		return false;

	T temp = buffer[left_idx];
	buffer[left_idx] = buffer[right_idx];
	buffer[right_idx] = temp;

	return true;
}
