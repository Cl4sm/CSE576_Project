int hfs_vstat(hfsvol *vol, hfsvolent *ent)
{
  if (getvol(&vol) == -1)
    goto fail;

  strcpy(ent->name, vol->mdb.drVN);

  ent->flags     = (vol->flags & HFS_VOL_READONLY) ? HFS_ISLOCKED : 0;

  ent->totbytes  = vol->mdb.drNmAlBlks * vol->mdb.drAlBlkSiz;
  ent->freebytes = vol->mdb.drFreeBks  * vol->mdb.drAlBlkSiz;

  ent->alblocksz = vol->mdb.drAlBlkSiz;
  ent->clumpsz   = vol->mdb.drClpSiz;

  ent->numfiles  = vol->mdb.drFilCnt;
  ent->numdirs   = vol->mdb.drDirCnt;

  ent->crdate    = d_ltime(vol->mdb.drCrDate);
  ent->mddate    = d_ltime(vol->mdb.drLsMod);
  ent->bkdate    = d_ltime(vol->mdb.drVolBkUp);

  ent->blessed   = vol->mdb.drFndrInfo[0];

  return 0;

fail:
  return -1;
}
