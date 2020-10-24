{
  hfsvol vol;
  int nparts, found;
  Partition map;
  unsigned long bnum = 0;

  v_init(&vol, HFS_OPT_NOCACHE);

  if (v_open(&vol, path, HFS_MODE_RDONLY) == -1)
    goto fail;

  nparts = 0;
  while (1)
    {
      found = m_findpmentry(&vol, "Apple_HFS", &map, &bnum);
      if (found == -1)
	goto fail;

      if (! found)
	break;

      ++nparts;
    }

  if (v_close(&vol) == -1)
    goto fail;

  return nparts;

fail:
  v_close(&vol);
  return -1;
}
