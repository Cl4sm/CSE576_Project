static vertex_t *heap_getmin(binary_heap_t * heap)
{
	vertex_t *min = NULL;

	if (heap->size > 0)
		min = heap->nodes[0];

	if (min == NULL)
		return min;

	if (heap->size > 0) {
		if (heap->size > 1) {
			heap_exchange(heap, 0, heap->size - 1);
			heap->size--;
			heap_down(heap, 0);
		} else {
			heap->size--;
		}
	}

	return min;
}