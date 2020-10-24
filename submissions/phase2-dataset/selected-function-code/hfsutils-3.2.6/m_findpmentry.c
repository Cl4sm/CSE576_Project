		  Partition *map, unsigned long *start)
{
  unsigned long bnum;
  int found = 0;

  if (start && *start > 0)
    {
      bnum = *start;

      if (bnum++ >= (unsigned long) map->pmMapBlkCnt)
	ERROR(EINVAL, "partition not found");
    }
  else
    bnum = 1;

  while (1)
    {
      if (l_getpmentry(vol, map, bnum) == -1)
	{
	  found = -1;
	  goto fail;
	}

      if (map->pmSig != HFS_PM_SIGWORD)
	{
	  found = -1;

	  if (map->pmSig == HFS_PM_SIGWORD_OLD)
	    ERROR(EINVAL, "old partition map format not supported");
	  else
	    ERROR(EINVAL, "invalid partition map");
	}

      if (strcmp((char *) map->pmParType, type) == 0)
	{
	  found = 1;
	  goto done;
	}

      if (bnum++ >= (unsigned long) map->pmMapBlkCnt)
	ERROR(EINVAL, "partition not found");
    }

done:
  if (start)
    *start = bnum;

fail:
  return found;
}
