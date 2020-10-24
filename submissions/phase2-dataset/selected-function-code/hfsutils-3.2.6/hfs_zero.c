int hfs_zero(const char *path, unsigned int maxparts, unsigned long *blocks)
{
  hfsvol vol;

  v_init(&vol, HFS_OPT_NOCACHE);

  if (maxparts < 1)
    ERROR(EINVAL, "must allow at least 1 partition");

  if (v_open(&vol, path, HFS_MODE_RDWR) == -1 ||
      v_geometry(&vol, 0) == -1)
    goto fail;

  if (m_zeroddr(&vol) == -1 ||
      m_zeropm(&vol, 1 + maxparts) == -1)
    goto fail;

  if (blocks)
    {
      Partition map;
      int found;

      found = m_findpmentry(&vol, "Apple_Free", &map, 0);
      if (found == -1)
	goto fail;

      if (! found)
	ERROR(EIO, "unable to determine free partition space");

      *blocks = map.pmPartBlkCnt;
    }

  if (v_close(&vol) == -1)
    goto fail;

  return 0;

fail:
  v_close(&vol);
  return -1;
}
