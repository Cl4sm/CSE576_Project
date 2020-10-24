int v_allocblocks(hfsvol *vol, ExtDescriptor *blocks)
{
  unsigned int request, found, foundat, start, end;
  register unsigned int pt;
  block *vbm;
  int wrap = 0;

  if (vol->mdb.drFreeBks == 0)
    ERROR(ENOSPC, "volume full");

  request = blocks->xdrNumABlks;
  found   = 0;
  foundat = 0;
  start   = vol->mdb.drAllocPtr;
  end     = vol->mdb.drNmAlBlks;
  vbm     = vol->vbm;

  ASSERT(request > 0);

  /* backtrack the start pointer to recover unused space */

  if (! BMTST(vbm, start))
    {
      while (start > 0 && ! BMTST(vbm, start - 1))
	--start;
    }

  /* find largest unused block which satisfies request */

  pt = start;

  while (1)
    {
      unsigned int mark;

      /* skip blocks in use */

      while (pt < end && BMTST(vbm, pt))
	++pt;

      if (wrap && pt >= start)
	break;

      /* count blocks not in use */

      mark = pt;
      while (pt < end && pt - mark < request && ! BMTST(vbm, pt))
	++pt;

      if (pt - mark > found)
	{
	  found   = pt - mark;
	  foundat = mark;
	}

      if (wrap && pt >= start)
	break;

      if (pt == end)
	pt = 0, wrap = 1;

      if (found == request)
	break;
    }

  if (found == 0 || found > vol->mdb.drFreeBks)
    ERROR(EIO, "bad volume bitmap or free block count");

  blocks->xdrStABN    = foundat;
  blocks->xdrNumABlks = found;

  if (v_dirty(vol) == -1)
    goto fail;

  vol->mdb.drAllocPtr = pt;
  vol->mdb.drFreeBks -= found;

  for (pt = foundat; pt < foundat + found; ++pt)
    BMSET(vbm, pt);

  vol->flags |= HFS_VOL_UPDATE_MDB | HFS_VOL_UPDATE_VBM;

  if (vol->flags & HFS_OPT_ZERO)
    {
      block b;
      unsigned int i;

      memset(&b, 0, sizeof(b));

      for (pt = foundat; pt < foundat + found; ++pt)
	{
	  for (i = 0; i < vol->lpa; ++i)
	    b_writeab(vol, pt, i, &b);
	}
    }

  return 0;

fail:
  return -1;
}
