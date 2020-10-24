void
mp_pool_clean(mp_pool_t *pool, int n_to_keep, int keep_recently_used)
{
  mp_chunk_t *chunk, **first_to_free;

  mp_pool_sort_used_chunks(pool);
  assert(n_to_keep >= 0);

  if (keep_recently_used)
  {
    int n_recently_used = pool->n_empty_chunks - pool->min_empty_chunks;

    if (n_to_keep < n_recently_used)
      n_to_keep = n_recently_used;
  }

  assert(n_to_keep >= 0);

  first_to_free = &pool->empty_chunks;

  while (*first_to_free && n_to_keep > 0)
  {
    first_to_free = &(*first_to_free)->next;
    --n_to_keep;
  }

  if (!*first_to_free)
  {
    pool->min_empty_chunks = pool->n_empty_chunks;
    return;
  }

  chunk = *first_to_free;

  while (chunk)
  {
    mp_chunk_t *next = chunk->next;
    chunk->magic = 0xdeadbeef;
    MyFree(chunk);
#ifdef MEMPOOL_STATS
    ++pool->total_chunks_freed;
#endif
    --pool->n_empty_chunks;
    chunk = next;
  }

  pool->min_empty_chunks = pool->n_empty_chunks;
  *first_to_free = NULL;
}
