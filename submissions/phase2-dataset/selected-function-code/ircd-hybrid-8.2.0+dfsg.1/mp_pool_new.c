mp_pool_new(size_t item_size, size_t chunk_capacity)
{
  mp_pool_t *pool;
  size_t alloc_size, new_chunk_cap;

/*  assert(item_size < SIZE_T_CEILING);
  assert(chunk_capacity < SIZE_T_CEILING);
  assert(SIZE_T_CEILING / item_size > chunk_capacity);
*/
  pool = MyCalloc(sizeof(mp_pool_t));
  /*
   * First, we figure out how much space to allow per item. We'll want to
   * use make sure we have enough for the overhead plus the item size.
   */
  alloc_size = (size_t)(offsetof(mp_allocated_t, u.mem) + item_size);
  /*
   * If the item_size is less than sizeof(next_free), we need to make
   * the allocation bigger.
   */
  if (alloc_size < sizeof(mp_allocated_t))
    alloc_size = sizeof(mp_allocated_t);

  /* If we're not an even multiple of ALIGNMENT, round up. */
  if (alloc_size % ALIGNMENT)
    alloc_size = alloc_size + ALIGNMENT - (alloc_size % ALIGNMENT);
  if (alloc_size < ALIGNMENT)
    alloc_size = ALIGNMENT;

  assert((alloc_size % ALIGNMENT) == 0);

  /*
   * Now we figure out how many items fit in each chunk. We need to fit at
   * least 2 items per chunk. No chunk can be more than MAX_CHUNK bytes long,
   * or less than MIN_CHUNK.
   */
  if (chunk_capacity > MAX_CHUNK)
    chunk_capacity = MAX_CHUNK;

  /*
   * Try to be around a power of 2 in size, since that's what allocators like
   * handing out. 512K-1 byte is a lot better than 512K+1 byte.
   */
  chunk_capacity = (size_t) round_to_power_of_2(chunk_capacity);

  while (chunk_capacity < alloc_size * 2 + CHUNK_OVERHEAD)
    chunk_capacity *= 2;
  if (chunk_capacity < MIN_CHUNK)
    chunk_capacity = MIN_CHUNK;

  new_chunk_cap = (chunk_capacity-CHUNK_OVERHEAD) / alloc_size;
  assert(new_chunk_cap < INT_MAX);
  pool->new_chunk_capacity = (int)new_chunk_cap;

  pool->item_size = item_size;
  pool->item_alloc_size = alloc_size;

  pool->next = mp_allocated_pools;
  mp_allocated_pools = pool;

  ilog(LOG_TYPE_DEBUG, "Capacity is %lu, item size is %lu, alloc size is %lu",
       (unsigned long)pool->new_chunk_capacity,
       (unsigned long)pool->item_alloc_size,
       (unsigned long)(pool->new_chunk_capacity*pool->item_alloc_size));

  return pool;
}
