{
  CatKeyRec key;
  CatDataRec data;
  unsigned long parid;
  char name[HFS_MAX_FLEN + 1];
  byte pkey[HFS_CATKEYLEN];

  if (getvol(&vol) == -1 ||
      v_resolve(&vol, path, &data, &parid, name, 0) <= 0)
    goto fail;

  if (data.cdrType != cdrDirRec)
    ERROR(ENOTDIR, 0);

  if (data.u.dir.dirVal != 0)
    ERROR(ENOTEMPTY, 0);

  if (parid == HFS_CNID_ROOTPAR)
    ERROR(EINVAL, 0);

  if (vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  /* delete directory record */

  r_makecatkey(&key, parid, name);
  r_packcatkey(&key, pkey, 0);

  if (bt_delete(&vol->cat, pkey) == -1)
    goto fail;

  /* delete thread record */

  r_makecatkey(&key, data.u.dir.dirDirID, "");
  r_packcatkey(&key, pkey, 0);

  if (bt_delete(&vol->cat, pkey) == -1 ||
      v_adjvalence(vol, parid, 1, -1) == -1)
    goto fail;

  return 0;

fail:
  return -1;
}
