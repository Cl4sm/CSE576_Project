int hfs_umount(hfsvol *vol)
{
  int result = 0;

  if (getvol(&vol) == -1)
    goto fail;

  if (--vol->refs)
    {
      result = v_flush(vol);
      goto done;
    }

  /* close all open files and directories */

  while (vol->files)
    {
      if (hfs_close(vol->files) == -1)
	result = -1;
    }

  while (vol->dirs)
    {
      if (hfs_closedir(vol->dirs) == -1)
	result = -1;
    }

  /* close medium */

  if (v_close(vol) == -1)
    result = -1;

  /* remove from linked list of volumes */

  if (vol->prev)
    vol->prev->next = vol->next;
  if (vol->next)
    vol->next->prev = vol->prev;

  if (vol == hfs_mounts)
    hfs_mounts = vol->next;
  if (vol == curvol)
    curvol = 0;

  FREE(vol);

done:
  return result;

fail:
  return -1;
}
