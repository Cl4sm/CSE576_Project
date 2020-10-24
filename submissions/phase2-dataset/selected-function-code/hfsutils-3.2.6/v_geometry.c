{
  Partition map;
  unsigned long bnum = 0;
  int found;

  vol->pnum = pnum;

  if (pnum == 0)
    {
      vol->vstart = 0;
      vol->vlen   = b_size(vol);

      if (vol->vlen == 0)
	goto fail;
    }
  else
    {
      while (pnum--)
	{
	  found = m_findpmentry(vol, "Apple_HFS", &map, &bnum);
	  if (found == -1 || ! found)
	    goto fail;
	}

      vol->vstart = map.pmPyPartStart;
      vol->vlen   = map.pmPartBlkCnt;

      if (map.pmDataCnt)
	{
	  if ((unsigned long) map.pmLgDataStart +
	      (unsigned long) map.pmDataCnt > vol->vlen)
	    ERROR(EINVAL, "partition data overflows partition");

	  vol->vstart += (unsigned long) map.pmLgDataStart;
	  vol->vlen    = map.pmDataCnt;
	}

      if (vol->vlen == 0)
	ERROR(EINVAL, "volume partition is empty");
    }

  if (vol->vlen < 800 * (1024 >> HFS_BLOCKSZ_BITS))
    ERROR(EINVAL, "volume is smaller than 800K");

  return 0;

fail:
  return -1;
}
