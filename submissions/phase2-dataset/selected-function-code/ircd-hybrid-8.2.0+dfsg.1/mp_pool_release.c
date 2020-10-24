void
mp_pool_release(void *item)
{
  mp_allocated_t *allocated = (void *)M2A(item);
  mp_chunk_t *chunk = allocated->in_chunk;

  assert(chunk);
  assert(chunk->magic == MP_CHUNK_MAGIC);
  assert(chunk->n_allocated > 0);

  allocated->u.next_free = chunk->first_free;
  chunk->first_free = allocated;

  if (chunk->n_allocated == chunk->capacity)
  {
    /* This chunk was full and is about to be used. */
    mp_pool_t *pool = chunk->pool;
    /* unlink from the full list  */
    if (chunk->prev)
      chunk->prev->next = chunk->next;
    if (chunk->next)
      chunk->next->prev = chunk->prev;
    if (chunk == pool->full_chunks)
      pool->full_chunks = chunk->next;

    /* link to the used list. */
    chunk->next = pool->used_chunks;
    chunk->prev = NULL;

    if (chunk->next)
      chunk->next->prev = chunk;
    pool->used_chunks = chunk;
  }
  else if (chunk->n_allocated == 1)
  {
    /* This was used and is about to be empty. */
    mp_pool_t *pool = chunk->pool;

    /* Unlink from the used list */
    if (chunk->prev)
      chunk->prev->next = chunk->next;
    if (chunk->next)
      chunk->next->prev = chunk->prev;
    if (chunk == pool->used_chunks)
      pool->used_chunks = chunk->next;

    /* Link to the empty list */
    chunk->next = pool->empty_chunks;
    chunk->prev = NULL;
    if (chunk->next)
      chunk->next->prev = chunk;
    pool->empty_chunks = chunk;

    /* Reset the guts of this chunk to defragment it, in case it gets
     * used again. */
    chunk->first_free = NULL;
    chunk->next_mem = chunk->mem;

    ++pool->n_empty_chunks;
  }

  --chunk->n_allocated;
}
