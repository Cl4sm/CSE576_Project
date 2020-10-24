static void
mp_pool_sort_used_chunks(mp_pool_t *pool)
{
  int i, n = 0, inverted = 0;
  mp_chunk_t **chunks, *chunk;

  for (chunk = pool->used_chunks; chunk; chunk = chunk->next)
  {
    ++n;
    if (chunk->next && chunk->next->n_allocated > chunk->n_allocated)
      ++inverted;
  }

  if (!inverted)
    return;

  chunks = MyCalloc(sizeof(mp_chunk_t *) * n);

  for (i = 0, chunk = pool->used_chunks; chunk; chunk = chunk->next)
    chunks[i++] = chunk;

  qsort(chunks, n, sizeof(mp_chunk_t *), mp_pool_sort_used_chunks_helper);
  pool->used_chunks = chunks[0];
  chunks[0]->prev = NULL;

  for (i = 1; i < n; ++i)
  {
    chunks[i - 1]->next = chunks[i];
    chunks[i]->prev = chunks[i - 1];
  }

  chunks[n - 1]->next = NULL;
  MyFree(chunks);
  mp_pool_assert_ok(pool);
}
