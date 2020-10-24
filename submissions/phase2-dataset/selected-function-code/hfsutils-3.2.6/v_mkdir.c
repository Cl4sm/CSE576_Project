int v_mkdir(hfsvol *vol, unsigned long parid, const char *name)
{
  CatKeyRec key;
  CatDataRec data;
  unsigned long id;
  byte record[HFS_MAX_CATRECLEN];
  unsigned int reclen;
  int i;

  if (bt_space(&vol->cat, 2) == -1)
    goto fail;

  id = vol->mdb.drNxtCNID++;
  vol->flags |= HFS_VOL_UPDATE_MDB;

  /* create directory record */

  data.cdrType   = cdrDirRec;
  data.cdrResrv2 = 0;

  data.u.dir.dirFlags = 0;
  data.u.dir.dirVal   = 0;
  data.u.dir.dirDirID = id;
  data.u.dir.dirCrDat = d_mtime(time(0));
  data.u.dir.dirMdDat = data.u.dir.dirCrDat;
  data.u.dir.dirBkDat = 0;

  memset(&data.u.dir.dirUsrInfo,  0, sizeof(data.u.dir.dirUsrInfo));
  memset(&data.u.dir.dirFndrInfo, 0, sizeof(data.u.dir.dirFndrInfo));
  for (i = 0; i < 4; ++i)
    data.u.dir.dirResrv[i] = 0;

  r_makecatkey(&key, parid, name);
  r_packcatrec(&key, &data, record, &reclen);

  if (bt_insert(&vol->cat, record, reclen) == -1)
    goto fail;

  /* create thread record */

  data.cdrType   = cdrThdRec;
  data.cdrResrv2 = 0;

  data.u.dthd.thdResrv[0] = 0;
  data.u.dthd.thdResrv[1] = 0;
  data.u.dthd.thdParID    = parid;
  strcpy(data.u.dthd.thdCName, name);

  r_makecatkey(&key, id, "");
  r_packcatrec(&key, &data, record, &reclen);

  if (bt_insert(&vol->cat, record, reclen) == -1 ||
      v_adjvalence(vol, parid, 1, 1) == -1)
    goto fail;

  return 0;

fail:
  return -1;
}
