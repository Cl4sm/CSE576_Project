int v_adjvalence(hfsvol *vol, unsigned long parid, int isdir, int adj)
{
  node n;
  CatDataRec data;
  int result = 0;

  if (isdir)
    vol->mdb.drDirCnt += adj;
  else
    vol->mdb.drFilCnt += adj;

  vol->flags |= HFS_VOL_UPDATE_MDB;

  if (parid == HFS_CNID_ROOTDIR)
    {
      if (isdir)
	vol->mdb.drNmRtDirs += adj;
      else
	vol->mdb.drNmFls    += adj;
    }
  else if (parid == HFS_CNID_ROOTPAR)
    goto done;

  if (v_getdthread(vol, parid, &data, 0) <= 0 ||
      v_catsearch(vol, data.u.dthd.thdParID, data.u.dthd.thdCName,
		  &data, 0, &n) <= 0 ||
      data.cdrType != cdrDirRec)
    ERROR(EIO, "can't find parent directory");

  data.u.dir.dirVal  += adj;
  data.u.dir.dirMdDat = d_mtime(time(0));

  result = v_putcatrec(&data, &n);

done:
  return result;

fail:
  return -1;
}
