void
_free_internal (ptr)
     __ptr_t ptr;
{
  int type;
  __malloc_size_t block, blocks;
  register __malloc_size_t i;
  struct list *prev, *next;
  __ptr_t curbrk;
  const __malloc_size_t lesscore_threshold
    /* Threshold of free space at which we will return some to the system.  */
    = FINAL_FREE_BLOCKS + 2 * __malloc_extra_blocks;

  register struct alignlist *l;

  if (ptr == NULL)
    return;

  for (l = _aligned_blocks; l != NULL; l = l->next)
    if (l->aligned == ptr)
      {
	l->aligned = NULL;	/* Mark the slot in the list as free.  */
	ptr = l->exact;
	break;
      }

  block = BLOCK (ptr);

  type = _heapinfo[block].busy.type;
  switch (type)
    {
    case 0:
      /* Get as many statistics as early as we can.  */
      --_chunks_used;
      _bytes_used -= _heapinfo[block].busy.info.size * BLOCKSIZE;
      _bytes_free += _heapinfo[block].busy.info.size * BLOCKSIZE;

      /* Find the free cluster previous to this one in the free list.
	 Start searching at the last block referenced; this may benefit
	 programs with locality of allocation.  */
      i = _heapindex;
      if (i > block)
	while (i > block)
	  i = _heapinfo[i].free.prev;
      else
	{
	  do
	    i = _heapinfo[i].free.next;
	  while (i > 0 && i < block);
	  i = _heapinfo[i].free.prev;
	}

      /* Determine how to link this block into the free list.  */
      if (block == i + _heapinfo[i].free.size)
	{
	  /* Coalesce this block with its predecessor.  */
	  _heapinfo[i].free.size += _heapinfo[block].busy.info.size;
	  block = i;
	}
      else
	{
	  /* Really link this block back into the free list.  */
	  _heapinfo[block].free.size = _heapinfo[block].busy.info.size;
	  _heapinfo[block].free.next = _heapinfo[i].free.next;
	  _heapinfo[block].free.prev = i;
	  _heapinfo[i].free.next = block;
	  _heapinfo[_heapinfo[block].free.next].free.prev = block;
	  ++_chunks_free;
	}

      /* Now that the block is linked in, see if we can coalesce it
	 with its successor (by deleting its successor from the list
	 and adding in its size).  */
      if (block + _heapinfo[block].free.size == _heapinfo[block].free.next)
	{
	  _heapinfo[block].free.size
	    += _heapinfo[_heapinfo[block].free.next].free.size;
	  _heapinfo[block].free.next
	    = _heapinfo[_heapinfo[block].free.next].free.next;
	  _heapinfo[_heapinfo[block].free.next].free.prev = block;
	  --_chunks_free;
	}

      /* How many trailing free blocks are there now?  */
      blocks = _heapinfo[block].free.size;

      /* Where is the current end of accessible core?  */
      curbrk = (*__morecore) (0);

      if (_heaplimit != 0 && curbrk == ADDRESS (_heaplimit))
	{
	  /* The end of the malloc heap is at the end of accessible core.
	     It's possible that moving _heapinfo will allow us to
	     return some space to the system.  */

	  __malloc_size_t info_block = BLOCK (_heapinfo);
	  __malloc_size_t info_blocks = _heapinfo[info_block].busy.info.size;
	  __malloc_size_t prev_block = _heapinfo[block].free.prev;
	  __malloc_size_t prev_blocks = _heapinfo[prev_block].free.size;
	  __malloc_size_t next_block = _heapinfo[block].free.next;
	  __malloc_size_t next_blocks = _heapinfo[next_block].free.size;

	  if (/* Win if this block being freed is last in core, the info table
		 is just before it, the previous free block is just before the
		 info table, and the two free blocks together form a useful
		 amount to return to the system.  */
	      (block + blocks == _heaplimit &&
	       info_block + info_blocks == block &&
	       prev_block != 0 && prev_block + prev_blocks == info_block &&
	       blocks + prev_blocks >= lesscore_threshold) ||
	      /* Nope, not the case.  We can also win if this block being
		 freed is just before the info table, and the table extends
		 to the end of core or is followed only by a free block,
		 and the total free space is worth returning to the system.  */
	      (block + blocks == info_block &&
	       ((info_block + info_blocks == _heaplimit &&
		 blocks >= lesscore_threshold) ||
		(info_block + info_blocks == next_block &&
		 next_block + next_blocks == _heaplimit &&
		 blocks + next_blocks >= lesscore_threshold)))
	      )
	    {
	      malloc_info *newinfo;
	      __malloc_size_t oldlimit = _heaplimit;

	      /* Free the old info table, clearing _heaplimit to avoid
		 recursion into this code.  We don't want to return the
		 table's blocks to the system before we have copied them to
		 the new location.  */
	      _heaplimit = 0;
	      _free_internal (_heapinfo);
	      _heaplimit = oldlimit;

	      /* Tell malloc to search from the beginning of the heap for
		 free blocks, so it doesn't reuse the ones just freed.  */
	      _heapindex = 0;

	      /* Allocate new space for the info table and move its data.  */
	      newinfo = (malloc_info *) _malloc_internal (info_blocks
							  * BLOCKSIZE);
	      memmove (newinfo, _heapinfo, info_blocks * BLOCKSIZE);
	      _heapinfo = newinfo;

	      /* We should now have coalesced the free block with the
		 blocks freed from the old info table.  Examine the entire
		 trailing free block to decide below whether to return some
		 to the system.  */
	      block = _heapinfo[0].free.prev;
	      blocks = _heapinfo[block].free.size;
	    }

	  /* Now see if we can return stuff to the system.  */
	  if (block + blocks == _heaplimit && blocks >= lesscore_threshold)
	    {
	      register __malloc_size_t bytes = blocks * BLOCKSIZE;
	      _heaplimit -= blocks;
	      (*__morecore) (-bytes);
	      _heapinfo[_heapinfo[block].free.prev].free.next
		= _heapinfo[block].free.next;
	      _heapinfo[_heapinfo[block].free.next].free.prev
		= _heapinfo[block].free.prev;
	      block = _heapinfo[block].free.prev;
	      --_chunks_free;
	      _bytes_free -= bytes;
	    }
	}

      /* Set the next search to begin at this block.  */
      _heapindex = block;
      break;

    default:
      /* Do some of the statistics.  */
      --_chunks_used;
      _bytes_used -= 1 << type;
      ++_chunks_free;
      _bytes_free += 1 << type;

      /* Get the address of the first free fragment in this block.  */
      prev = (struct list *) ((char *) ADDRESS (block) +
			      (_heapinfo[block].busy.info.frag.first << type));

      if (_heapinfo[block].busy.info.frag.nfree == (BLOCKSIZE >> type) - 1)
	{
	  /* If all fragments of this block are free, remove them
	     from the fragment list and free the whole block.  */
	  next = prev;
	  for (i = 1; i < (__malloc_size_t) (BLOCKSIZE >> type); ++i)
	    next = next->next;
	  prev->prev->next = next;
	  if (next != NULL)
	    next->prev = prev->prev;
	  _heapinfo[block].busy.type = 0;
	  _heapinfo[block].busy.info.size = 1;

	  /* Keep the statistics accurate.  */
	  ++_chunks_used;
	  _bytes_used += BLOCKSIZE;
	  _chunks_free -= BLOCKSIZE >> type;
	  _bytes_free -= BLOCKSIZE;

	  free (ADDRESS (block));
	}
      else if (_heapinfo[block].busy.info.frag.nfree != 0)
	{
	  /* If some fragments of this block are free, link this
	     fragment into the fragment list after the first free
	     fragment of this block. */
	  next = (struct list *) ptr;
	  next->next = prev->next;
	  next->prev = prev;
	  prev->next = next;
	  if (next->next != NULL)
	    next->next->prev = next;
	  ++_heapinfo[block].busy.info.frag.nfree;
	}
      else
	{
	  /* No fragments of this block are free, so link this
	     fragment into the fragment list and announce that
	     it is the first free fragment of this block. */
	  prev = (struct list *) ptr;
	  _heapinfo[block].busy.info.frag.nfree = 1;
	  _heapinfo[block].busy.info.frag.first = (unsigned long int)
	    ((unsigned long int) ((char *) ptr - (char *) NULL)
	     % BLOCKSIZE >> type);
	  prev->next = _fraghead[type].next;
	  prev->prev = &_fraghead[type];
	  prev->prev->next = prev;
	  if (prev->next != NULL)
	    prev->next->prev = prev;
	}
      break;
    }
}
