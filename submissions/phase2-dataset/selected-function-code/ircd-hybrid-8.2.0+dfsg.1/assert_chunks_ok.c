assert_chunks_ok(mp_pool_t *pool, mp_chunk_t *chunk, int empty, int full)
{
  mp_allocated_t *allocated;
  int n = 0;

  if (chunk)
    assert(chunk->prev == NULL);

  while (chunk)
  {
    n++;
    assert(chunk->magic == MP_CHUNK_MAGIC);
    assert(chunk->pool == pool);

    for (allocated = chunk->first_free; allocated;
         allocated = allocated->u.next_free)
      assert(allocated->in_chunk == chunk);

    if (empty)
      assert(chunk->n_allocated == 0);
    else if (full)
      assert(chunk->n_allocated == chunk->capacity);
    else
      assert(chunk->n_allocated > 0 && chunk->n_allocated < chunk->capacity);

    assert(chunk->capacity == pool->new_chunk_capacity);

    assert(chunk->mem_size ==
           pool->new_chunk_capacity * pool->item_alloc_size);

    assert(chunk->next_mem >= chunk->mem &&
           chunk->next_mem <= chunk->mem + chunk->mem_size);

    if (chunk->next)
      assert(chunk->next->prev == chunk);

    chunk = chunk->next;
  }

  return n;
}
