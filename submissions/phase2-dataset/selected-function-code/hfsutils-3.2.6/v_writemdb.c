{
  vol->mdb.drLsMod = d_mtime(time(0));

  vol->mdb.drXTFlSize = vol->ext.f.cat.u.fil.filPyLen;
  memcpy(&vol->mdb.drXTExtRec,
	 &vol->ext.f.cat.u.fil.filExtRec, sizeof(ExtDataRec));

  vol->mdb.drCTFlSize = vol->cat.f.cat.u.fil.filPyLen;
  memcpy(&vol->mdb.drCTExtRec,
	 &vol->cat.f.cat.u.fil.filExtRec, sizeof(ExtDataRec));

  if (l_putmdb(vol, &vol->mdb, vol->flags & HFS_VOL_UPDATE_ALTMDB) == -1)
    goto fail;

  vol->flags &= ~(HFS_VOL_UPDATE_MDB | HFS_VOL_UPDATE_ALTMDB);

  return 0;

fail:
  return -1;
}
