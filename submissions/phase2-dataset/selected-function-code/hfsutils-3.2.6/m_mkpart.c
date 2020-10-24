int m_mkpart(hfsvol *vol,
	     const char *name, const char *type, unsigned long len)
{
  Partition map;
  unsigned int nparts, maxparts;
  unsigned long bnum, start, remain;
  int found;

  if (strlen(name) > 32 ||
      strlen(type) > 32)
    ERROR(EINVAL, "partition name/type can each be at most 32 chars");

  if (len == 0)
    ERROR(EINVAL, "partition length must be > 0");

  found = m_findpmentry(vol, "Apple_partition_map", &map, 0);
  if (found == -1)
    goto fail;

  if (! found)
    ERROR(EIO, "cannot find partition map's partition");

  nparts   = map.pmMapBlkCnt;
  maxparts = map.pmPartBlkCnt;

  bnum = 0;
  do
    {
      found = m_findpmentry(vol, "Apple_Free", &map, &bnum);
      if (found == -1)
	goto fail;

      if (! found)
	ERROR(ENOSPC, "no available partitions");
    }
  while (len > (unsigned long) map.pmPartBlkCnt);

  start  = (unsigned long) map.pmPyPartStart + len;
  remain = (unsigned long) map.pmPartBlkCnt  - len;

  if (remain && nparts >= maxparts)
    ERROR(EINVAL, "must allocate all blocks in free space");

  map.pmPartBlkCnt = len;

  strcpy((char *) map.pmPartName, name);
  strcpy((char *) map.pmParType,  type);

  map.pmLgDataStart = 0;
  map.pmDataCnt     = len;

  map.pmPartStatus  = 0;

  if (l_putpmentry(vol, &map, bnum) == -1)
    goto fail;

  if (remain)
    {
      map.pmPyPartStart = start;
      map.pmPartBlkCnt  = remain;

      strcpy((char *) map.pmPartName, "Extra");
      strcpy((char *) map.pmParType,  "Apple_Free");

      map.pmDataCnt = remain;

      if (l_putpmentry(vol, &map, ++nparts) == -1)
	goto fail;

      for (bnum = 1; bnum <= nparts; ++bnum)
	{
	  if (l_getpmentry(vol, &map, bnum) == -1)
	    goto fail;

	  map.pmMapBlkCnt = nparts;

	  if (l_putpmentry(vol, &map, bnum) == -1)
	    goto fail;
	}
    }

  return 0;

fail:
  return -1;
}
