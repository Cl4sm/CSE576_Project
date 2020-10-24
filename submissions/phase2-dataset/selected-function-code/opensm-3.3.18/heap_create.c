static int heap_create(vertex_t * adj_list, uint32_t adj_list_size,
		       binary_heap_t ** binheap)
{
	binary_heap_t *heap = NULL;
	uint32_t i = 0;

	/* allocate the memory for the heap object */
	heap = (binary_heap_t *) malloc(sizeof(binary_heap_t));
	if (!heap)
		return 1;

	/* the heap size is equivalent to the size of the adj_list */
	heap->size = adj_list_size;

	/* allocate the pointer array, fill with the pointers to the elements of the adj_list and set the initial heap_id */
	heap->nodes = (vertex_t **) malloc(heap->size * sizeof(vertex_t *));
	if (!heap->nodes) {
		free(heap);
		return 1;
	}
	for (i = 0; i < heap->size; i++) {
		heap->nodes[i] = &adj_list[i];
		heap->nodes[i]->heap_id = i;
	}

	/* sort elements */
	for (i = heap->size; i > 0; i--)
		heap_down(heap, i - 1);

	*binheap = heap;
	return 0;
}