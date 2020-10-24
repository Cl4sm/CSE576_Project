mp_pool_get(mp_pool_t *pool)
{
  mp_chunk_t *chunk;
  mp_allocated_t *allocated;
  void *ptr = NULL;

  if (pool->used_chunks)
  {
    /*
     * Common case: there is some chunk that is neither full nor empty. Use
     * that one. (We can't use the full ones, obviously, and we should fill
     * up the used ones before we start on any empty ones.
     */
    chunk = pool->used_chunks;

  }
  else if (pool->empty_chunks)
  {
    /*
     * We have no used chunks, but we have an empty chunk that we haven't
     * freed yet: use that. (We pull from the front of the list, which should
     * get us the most recently emptied chunk.)
     */
    chunk = pool->empty_chunks;

    /* Remove the chunk from the empty list. */
    pool->empty_chunks = chunk->next;
    if (chunk->next)
      chunk->next->prev = NULL;

    /* Put the chunk on the 'used' list*/
    add_newly_used_chunk_to_used_list(pool, chunk);

    assert(!chunk->prev);
    --pool->n_empty_chunks;
    if (pool->n_empty_chunks < pool->min_empty_chunks)
      pool->min_empty_chunks = pool->n_empty_chunks;
  }
  else
  {
    /* We have no used or empty chunks: allocate a new chunk. */
    chunk = mp_chunk_new(pool);

    /* Add the new chunk to the used list. */
    add_newly_used_chunk_to_used_list(pool, chunk);
  }

  assert(chunk->n_allocated < chunk->capacity);

  if (chunk->first_free)
  {
    /* If there's anything on the chunk's freelist, unlink it and use it. */
    allocated = chunk->first_free;
    chunk->first_free = allocated->u.next_free;
    allocated->u.next_free = NULL; /* For debugging; not really needed. */
    assert(allocated->in_chunk == chunk);
  }
  else
  {
    /* Otherwise, the chunk had better have some free space left on it. */
    assert(chunk->next_mem + pool->item_alloc_size <=
           chunk->mem + chunk->mem_size);

    /* Good, it did.  Let's carve off a bit of that free space, and use
     * that. */
    allocated = (void *)chunk->next_mem;
    chunk->next_mem += pool->item_alloc_size;
    allocated->in_chunk = chunk;
    allocated->u.next_free = NULL; /* For debugging; not really needed. */
  }

  ++chunk->n_allocated;
#ifdef MEMPOOL_STATS
  ++pool->total_items_allocated;
#endif

  if (chunk->n_allocated == chunk->capacity)
  {
    /* This chunk just became full. */
    assert(chunk == pool->used_chunks);
    assert(chunk->prev == NULL);

    /* Take it off the used list. */
    pool->used_chunks = chunk->next;
    if (chunk->next)
      chunk->next->prev = NULL;

    /* Put it on the full list. */
    chunk->next = pool->full_chunks;
    if (chunk->next)
      chunk->next->prev = chunk;
    pool->full_chunks = chunk;
  }

  /* And return the memory portion of the mp_allocated_t. */
  ptr = A2M(allocated);
  memset(ptr, 0, pool->item_size);

  return ptr;
}
