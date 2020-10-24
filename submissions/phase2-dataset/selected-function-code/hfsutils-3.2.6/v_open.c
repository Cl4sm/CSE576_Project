int v_open(hfsvol *vol, const char *path, int mode)
{
  if (vol->flags & HFS_VOL_OPEN)
    ERROR(EINVAL, "volume already open");

  if (os_open(&vol->priv, path, mode) == -1)
    goto fail;

  vol->flags |= HFS_VOL_OPEN;

  /* initialize volume block cache (OK to fail) */

  if (! (vol->flags & HFS_OPT_NOCACHE) &&
      b_init(vol) != -1)
    vol->flags |= HFS_VOL_USINGCACHE;

  return 0;

fail:
  return -1;
}
