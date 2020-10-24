hfsvol *hfs_mount(const char *path, int pnum, int mode)
{
  hfsvol *vol, *check;

  /* see if the volume is already mounted */

  for (check = hfs_mounts; check; check = check->next)
    {
      if (check->pnum == pnum && v_same(check, path) == 1)
	{
	  /* verify compatible read/write mode */

	  if (((check->flags & HFS_VOL_READONLY) &&
	       ! (mode & HFS_MODE_RDWR)) ||
	      (! (check->flags & HFS_VOL_READONLY) &&
	       (mode & (HFS_MODE_RDWR | HFS_MODE_ANY))))
	    {
	      vol = check;
	      goto done;
	    }
	}
    }

  vol = ALLOC(hfsvol, 1);
  if (vol == 0)
    ERROR(ENOMEM, 0);

  v_init(vol, mode);

  /* open the medium */

  switch (mode & HFS_MODE_MASK)
    {
    case HFS_MODE_RDWR:
    case HFS_MODE_ANY:
      if (v_open(vol, path, HFS_MODE_RDWR) != -1)
	break;

      if ((mode & HFS_MODE_MASK) == HFS_MODE_RDWR)
	goto fail;

    case HFS_MODE_RDONLY:
    default:
      vol->flags |= HFS_VOL_READONLY;

      if (v_open(vol, path, HFS_MODE_RDONLY) == -1)
	goto fail;
    }

  /* mount the volume */

  if (v_geometry(vol, pnum) == -1 ||
      v_mount(vol) == -1)
    goto fail;

  /* add to linked list of volumes */

  vol->prev = 0;
  vol->next = hfs_mounts;

  if (hfs_mounts)
    hfs_mounts->prev = vol;

  hfs_mounts = vol;

done:
  ++vol->refs;
  curvol = vol;

  return vol;

fail:
  if (vol)
    {
      v_close(vol);
      FREE(vol);
    }

  return 0;
}
