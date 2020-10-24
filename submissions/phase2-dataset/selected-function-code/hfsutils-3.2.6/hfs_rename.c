int hfs_rename(hfsvol *vol, const char *srcpath, const char *dstpath)
{
  hfsvol *srcvol;
  CatDataRec src, dst;
  unsigned long srcid, dstid;
  CatKeyRec key;
  char srcname[HFS_MAX_FLEN + 1], dstname[HFS_MAX_FLEN + 1];
  byte record[HFS_MAX_CATRECLEN];
  unsigned int reclen;
  int found, isdir, moving;
  node n;

  if (getvol(&vol) == -1 ||
      v_resolve(&vol, srcpath, &src, &srcid, srcname, 0) <= 0)
    goto fail;

  isdir  = (src.cdrType == cdrDirRec);
  srcvol = vol;

  found = v_resolve(&vol, dstpath, &dst, &dstid, dstname, 0);
  if (found == -1)
    goto fail;

  if (vol != srcvol)
    ERROR(EINVAL, "can't move across volumes");

  if (dstid == 0)
    ERROR(ENOENT, "bad destination path");

  if (found &&
      dst.cdrType == cdrDirRec &&
      dst.u.dir.dirDirID != src.u.dir.dirDirID)
    {
      dstid = dst.u.dir.dirDirID;
      strcpy(dstname, srcname);

      found = v_catsearch(vol, dstid, dstname, 0, 0, 0);
      if (found == -1)
	goto fail;
    }

  moving = (srcid != dstid);

  if (found)
    {
      const char *ptr;

      ptr = strrchr(dstpath, ':');
      if (ptr == 0)
	ptr = dstpath;
      else
	++ptr;

      if (*ptr)
	strcpy(dstname, ptr);

      if (! moving && strcmp(srcname, dstname) == 0)
	goto done;  /* source and destination are identical */

      if (moving || d_relstring(srcname, dstname))
	ERROR(EEXIST, "can't use destination name");
    }

  /* can't move anything into the root directory's parent */

  if (moving && dstid == HFS_CNID_ROOTPAR)
    ERROR(EINVAL, "can't move above root directory");

  if (moving && isdir)
    {
      unsigned long id;

      /* can't move root directory anywhere */

      if (src.u.dir.dirDirID == HFS_CNID_ROOTDIR)
	ERROR(EINVAL, "can't move root directory");

      /* make sure we aren't trying to move a directory inside itself */

      for (id = dstid; id != HFS_CNID_ROOTDIR; id = dst.u.dthd.thdParID)
	{
	  if (id == src.u.dir.dirDirID)
	    ERROR(EINVAL, "can't move directory inside itself");

	  if (v_getdthread(vol, id, &dst, 0) <= 0)
	    goto fail;
	}
    }

  if (vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  /* change volume name */

  if (dstid == HFS_CNID_ROOTPAR)
    {
      if (! validvname(dstname))
	goto fail;

      strcpy(vol->mdb.drVN, dstname);
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  /* remove source record */

  r_makecatkey(&key, srcid, srcname);
  r_packcatkey(&key, record, 0);

  if (bt_delete(&vol->cat, record) == -1)
    goto fail;

  /* insert destination record */

  r_makecatkey(&key, dstid, dstname);
  r_packcatrec(&key, &src, record, &reclen);

  if (bt_insert(&vol->cat, record, reclen) == -1)
    goto fail;

  /* update thread record */

  if (isdir)
    {
      if (v_getdthread(vol, src.u.dir.dirDirID, &dst, &n) <= 0)
	goto fail;

      dst.u.dthd.thdParID = dstid;
      strcpy(dst.u.dthd.thdCName, dstname);

      if (v_putcatrec(&dst, &n) == -1)
	goto fail;
    }
  else
    {
      found = v_getfthread(vol, src.u.fil.filFlNum, &dst, &n);
      if (found == -1)
	goto fail;

      if (found)
	{
	  dst.u.fthd.fthdParID = dstid;
	  strcpy(dst.u.fthd.fthdCName, dstname);

	  if (v_putcatrec(&dst, &n) == -1)
	    goto fail;
	}
    }

  /* update directory valences */

  if (moving)
    {
      if (v_adjvalence(vol, srcid, isdir, -1) == -1 ||
	  v_adjvalence(vol, dstid, isdir,  1) == -1)
	goto fail;
    }

done:
  return 0;

fail:
  return -1;
}
