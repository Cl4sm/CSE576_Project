int hfs_mkpart(const char *path, unsigned long len)
{
  hfsvol vol;

  v_init(&vol, HFS_OPT_NOCACHE);

  if (v_open(&vol, path, HFS_MODE_RDWR) == -1)
    goto fail;

  if (m_mkpart(&vol, "MacOS", "Apple_HFS", len) == -1)
    goto fail;

  if (v_close(&vol) == -1)
    goto fail;

  return 0;

fail:
  v_close(&vol);
  return -1;
}
