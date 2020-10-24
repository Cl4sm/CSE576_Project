{
  unsigned int start, len, pt;
  block *vbm;

  start = blocks->xdrStABN;
  len   = blocks->xdrNumABlks;
  vbm   = vol->vbm;

  if (v_dirty(vol) == -1)
    goto fail;

  vol->mdb.drFreeBks += len;

  for (pt = start; pt < start + len; ++pt)
    BMCLR(vbm, pt);

  vol->flags |= HFS_VOL_UPDATE_MDB | HFS_VOL_UPDATE_VBM;

  return 0;

fail:
  return -1;
}
