hfsdir *hfs_opendir(hfsvol *vol, const char *path)
{
  hfsdir *dir = 0;
  CatKeyRec key;
  CatDataRec data;
  byte pkey[HFS_CATKEYLEN];

  if (getvol(&vol) == -1)
    goto fail;

  dir = ALLOC(hfsdir, 1);
  if (dir == 0)
    ERROR(ENOMEM, 0);

  dir->vol = vol;

  if (*path == 0)
    {
      /* meta-directory containing root dirs from all mounted volumes */

      dir->dirid = 0;
      dir->vptr  = hfs_mounts;
    }
  else
    {
      if (v_resolve(&vol, path, &data, 0, 0, 0) <= 0)
	goto fail;

      if (data.cdrType != cdrDirRec)
	ERROR(ENOTDIR, 0);

      dir->dirid = data.u.dir.dirDirID;
      dir->vptr  = 0;

      r_makecatkey(&key, dir->dirid, "");
      r_packcatkey(&key, pkey, 0);

      if (bt_search(&vol->cat, pkey, &dir->n) <= 0)
	goto fail;
    }

  dir->prev = 0;
  dir->next = vol->dirs;

  if (vol->dirs)
    vol->dirs->prev = dir;

  vol->dirs = dir;

  return dir;

fail:
  FREE(dir);
  return 0;
}
