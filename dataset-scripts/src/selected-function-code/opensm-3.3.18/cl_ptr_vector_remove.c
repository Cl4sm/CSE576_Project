void *cl_ptr_vector_remove(IN cl_ptr_vector_t * const p_vector,
			   IN const size_t index)
{
	size_t src;
	const void *element;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);
	CL_ASSERT(p_vector->size > index);

	/* Store a copy of the element to return. */
	element = p_vector->p_ptr_array[index];
	/* Shift all items above the removed item down. */
	if (index < --p_vector->size) {
		for (src = index; src < p_vector->size; src++)
			p_vector->p_ptr_array[src] =
			    p_vector->p_ptr_array[src + 1];
	}
	/* Clear the entry for the element just outside of the new upper bound. */
	p_vector->p_ptr_array[p_vector->size] = NULL;

	return ((void *)element);
}