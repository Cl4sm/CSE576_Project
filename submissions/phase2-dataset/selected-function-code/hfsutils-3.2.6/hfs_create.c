hfsfile *hfs_create(hfsvol *vol, const char *path,
		    const char *type, const char *creator)
{
  hfsfile *file = 0;
  unsigned long parid;
  char name[HFS_MAX_FLEN + 1];
  CatKeyRec key;
  byte record[HFS_MAX_CATRECLEN];
  unsigned reclen;
  int found;

  if (getvol(&vol) == -1)
    goto fail;

  file = ALLOC(hfsfile, 1);
  if (file == 0)
    ERROR(ENOMEM, 0);

  found = v_resolve(&vol, path, &file->cat, &parid, name, 0);
  if (found == -1 || parid == 0)
    goto fail;

  if (found)
    ERROR(EEXIST, 0);

  if (parid == HFS_CNID_ROOTPAR)
    ERROR(EINVAL, 0);

  if (vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  /* create file `name' in parent `parid' */

  if (bt_space(&vol->cat, 1) == -1)
    goto fail;

  f_init(file, vol, vol->mdb.drNxtCNID++, name);
  vol->flags |= HFS_VOL_UPDATE_MDB;

  file->parid = parid;

  /* create catalog record */

  file->cat.u.fil.filUsrWds.fdType =
    d_getsl((const unsigned char *) type);
  file->cat.u.fil.filUsrWds.fdCreator =
    d_getsl((const unsigned char *) creator);

  file->cat.u.fil.filCrDat = d_mtime(time(0));
  file->cat.u.fil.filMdDat = file->cat.u.fil.filCrDat;

  r_makecatkey(&key, file->parid, file->name);
  r_packcatrec(&key, &file->cat, record, &reclen);

  if (bt_insert(&vol->cat, record, reclen) == -1 ||
      v_adjvalence(vol, file->parid, 0, 1) == -1)
    goto fail;

  /* package file handle for user */

  file->next = vol->files;

  if (vol->files)
    vol->files->prev = file;

  vol->files = file;

  return file;

fail:
  FREE(file);
  return 0;
}
