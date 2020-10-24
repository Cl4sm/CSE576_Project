{
  Partition map;
  unsigned int i;

  ASSERT(vol->pnum == 0 && vol->vlen != 0);

  if (maxparts < 2)
    ERROR(EINVAL, "must allow at least 2 partitions");

  /* first entry: partition map itself */

  map.pmSig         = HFS_PM_SIGWORD;
  map.pmSigPad      = 0;
  map.pmMapBlkCnt   = 2;

  map.pmPyPartStart = 1;
  map.pmPartBlkCnt  = maxparts;

  strcpy((char *) map.pmPartName, "Apple");
  strcpy((char *) map.pmParType,  "Apple_partition_map");

  map.pmLgDataStart = 0;
  map.pmDataCnt     = map.pmPartBlkCnt;

  map.pmPartStatus  = 0;

  map.pmLgBootStart = 0;
  map.pmBootSize    = 0;
  map.pmBootAddr    = 0;
  map.pmBootAddr2   = 0;
  map.pmBootEntry   = 0;
  map.pmBootEntry2  = 0;
  map.pmBootCksum   = 0;

  strcpy((char *) map.pmProcessor, "");

  for (i = 0; i < 188; ++i)
    map.pmPad[i] = 0;

  if (l_putpmentry(vol, &map, 1) == -1)
    goto fail;

  /* second entry: rest of medium */

  map.pmPyPartStart = 1 + maxparts;
  map.pmPartBlkCnt  = vol->vlen - 1 - maxparts;

  strcpy((char *) map.pmPartName, "Extra");
  strcpy((char *) map.pmParType,  "Apple_Free");

  map.pmDataCnt = map.pmPartBlkCnt;

  if (l_putpmentry(vol, &map, 2) == -1)
    goto fail;

  /* zero rest of partition map's partition */

  if (maxparts > 2)
    {
      block b;

      memset(&b, 0, sizeof(b));

      for (i = 3; i <= maxparts; ++i)
	{
	  if (b_writepb(vol, i, &b, 1) == -1)
	    goto fail;
	}
    }

  return 0;

fail:
  return -1;
}
