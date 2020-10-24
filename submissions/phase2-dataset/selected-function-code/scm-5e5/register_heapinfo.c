__inline__
#endif
static void
register_heapinfo ()
{
  __malloc_size_t block, blocks;

  block = BLOCK (_heapinfo);
  blocks = BLOCKIFY (heapsize * sizeof (malloc_info));

  /* Account for the _heapinfo block itself in the statistics.  */
  _bytes_used += blocks * BLOCKSIZE;
  ++_chunks_used;

  /* Describe the heapinfo block itself in the heapinfo.  */
  _heapinfo[block].busy.type = 0;
  _heapinfo[block].busy.info.size = blocks;
  /* Leave back-pointers for malloc_find_address.  */
  while (--blocks > 0)
    _heapinfo[block + blocks].busy.info.size = -blocks;
}
