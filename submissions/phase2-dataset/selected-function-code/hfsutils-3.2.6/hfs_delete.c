{
  hfsfile file;
  CatKeyRec key;
  byte pkey[HFS_CATKEYLEN];
  int found;

  if (getvol(&vol) == -1 ||
      v_resolve(&vol, path, &file.cat, &file.parid, file.name, 0) <= 0)
    goto fail;

  if (file.cat.cdrType != cdrFilRec)
    ERROR(EISDIR, 0);

  if (file.parid == HFS_CNID_ROOTPAR)
    ERROR(EINVAL, 0);

  if (vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  /* free allocation blocks */

  file.vol   = vol;
  file.flags = 0;

  file.cat.u.fil.filLgLen  = 0;
  file.cat.u.fil.filRLgLen = 0;

  f_selectfork(&file, fkData);
  if (f_trunc(&file) == -1)
    goto fail;

  f_selectfork(&file, fkRsrc);
  if (f_trunc(&file) == -1)
    goto fail;

  /* delete file record */

  r_makecatkey(&key, file.parid, file.name);
  r_packcatkey(&key, pkey, 0);

  if (bt_delete(&vol->cat, pkey) == -1 ||
      v_adjvalence(vol, file.parid, 0, -1) == -1)
    goto fail;

  /* delete file thread, if any */

  found = v_getfthread(vol, file.cat.u.fil.filFlNum, 0, 0);
  if (found == -1)
    goto fail;

  if (found)
    {
      r_makecatkey(&key, file.cat.u.fil.filFlNum, "");
      r_packcatkey(&key, pkey, 0);

      if (bt_delete(&vol->cat, pkey) == -1)
	goto fail;
    }

  return 0;

fail:
  return -1;
}
