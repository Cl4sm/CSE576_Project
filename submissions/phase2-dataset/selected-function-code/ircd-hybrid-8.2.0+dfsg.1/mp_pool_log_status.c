void
mp_pool_log_status(mp_pool_t *pool)
{
  uint64_t bytes_used = 0;
  uint64_t bytes_allocated = 0;
  uint64_t bu = 0, ba = 0;
  mp_chunk_t *chunk;
  int n_full = 0, n_used = 0;

  assert(pool);

  for (chunk = pool->empty_chunks; chunk; chunk = chunk->next)
    bytes_allocated += chunk->mem_size;

  ilog(LOG_TYPE_DEBUG, "%llu bytes in %d empty chunks",
       bytes_allocated, pool->n_empty_chunks);
  for (chunk = pool->used_chunks; chunk; chunk = chunk->next)
  {
    ++n_used;
    bu += chunk->n_allocated * pool->item_alloc_size;
    ba += chunk->mem_size;

    ilog(LOG_TYPE_DEBUG, "   used chunk: %d items allocated",
         chunk->n_allocated);
  }

  ilog(LOG_TYPE_DEBUG, "%llu/%llu bytes in %d partially full chunks",
       bu, ba, n_used);
  bytes_used += bu;
  bytes_allocated += ba;
  bu = ba = 0;

  for (chunk = pool->full_chunks; chunk; chunk = chunk->next)
  {
    ++n_full;
    bu += chunk->n_allocated * pool->item_alloc_size;
    ba += chunk->mem_size;
  }

  ilog(LOG_TYPE_DEBUG, "%llu/%llu bytes in %d full chunks",
       bu, ba, n_full);
  bytes_used += bu;
  bytes_allocated += ba;

  ilog(LOG_TYPE_DEBUG, "Total: %llu/%llu bytes allocated "
       "for cell pools are full.",
       bytes_used, bytes_allocated);

#ifdef MEMPOOL_STATS
  ilog(LOG_TYPE_DEBUG, "%llu cell allocations ever; "
       "%llu chunk allocations ever; "
       "%llu chunk frees ever.",
       pool->total_items_allocated,
       pool->total_chunks_allocated,
       pool->total_chunks_freed);
#endif
}
