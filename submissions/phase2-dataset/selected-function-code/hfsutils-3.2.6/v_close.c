int v_close(hfsvol *vol)
{
  int result = 0;

  if (! (vol->flags & HFS_VOL_OPEN))
    goto done;

  if ((vol->flags & HFS_VOL_MOUNTED) &&
      flushvol(vol, 1) == -1)
    result = -1;

  if ((vol->flags & HFS_VOL_USINGCACHE) &&
      b_finish(vol) == -1)
    result = -1;

  if (os_close(&vol->priv) == -1)
    result = -1;

  vol->flags &= ~(HFS_VOL_OPEN | HFS_VOL_MOUNTED | HFS_VOL_USINGCACHE);

  /* free dynamically allocated structures */

  FREE(vol->vbm);

  vol->vbm   = 0;
  vol->vbmsz = 0;

  FREE(vol->ext.map);
  FREE(vol->cat.map);

  vol->ext.map = 0;
  vol->cat.map = 0;

done:
  return result;
}
