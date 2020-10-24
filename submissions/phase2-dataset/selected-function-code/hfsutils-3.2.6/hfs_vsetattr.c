int hfs_vsetattr(hfsvol *vol, hfsvolent *ent)
{
  if (getvol(&vol) == -1)
    goto fail;

  if (ent->clumpsz % vol->mdb.drAlBlkSiz != 0)
    ERROR(EINVAL, "illegal clump size");

  /* make sure "blessed" folder exists */

  if (ent->blessed &&
      v_getdthread(vol, ent->blessed, 0, 0) <= 0)
    ERROR(EINVAL, "illegal blessed folder");

  if (vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  vol->mdb.drClpSiz      = ent->clumpsz;

  vol->mdb.drCrDate      = d_mtime(ent->crdate);
  vol->mdb.drLsMod       = d_mtime(ent->mddate);
  vol->mdb.drVolBkUp     = d_mtime(ent->bkdate);

  vol->mdb.drFndrInfo[0] = ent->blessed;

  vol->flags |= HFS_VOL_UPDATE_MDB;

  return 0;

fail:
  return -1;
}
