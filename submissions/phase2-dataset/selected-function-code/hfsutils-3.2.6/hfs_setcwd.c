int hfs_setcwd(hfsvol *vol, unsigned long id)
{
  if (getvol(&vol) == -1)
    goto fail;

  if (id == vol->cwd)
    goto done;

  /* make sure the directory exists */

  if (v_getdthread(vol, id, 0, 0) <= 0)
    goto fail;

  vol->cwd = id;

done:
  return 0;

fail:
  return -1;
}
