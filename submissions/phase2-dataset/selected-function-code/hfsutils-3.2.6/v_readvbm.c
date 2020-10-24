int v_readvbm(hfsvol *vol)
{
  unsigned int vbmst = vol->mdb.drVBMSt;
  unsigned int vbmsz = (vol->mdb.drNmAlBlks + 0x0fff) >> 12;
  block *bp;

  ASSERT(vol->vbm == 0);

  if (vol->mdb.drAlBlSt - vbmst < vbmsz)
    ERROR(EIO, "volume bitmap collides with volume data");

  vol->vbm = ALLOC(block, vbmsz);
  if (vol->vbm == 0)
    ERROR(ENOMEM, 0);

  vol->vbmsz = vbmsz;

  for (bp = vol->vbm; vbmsz--; ++bp)
    {
      if (b_readlb(vol, vbmst++, bp) == -1)
	goto fail;
    }

  return 0;

fail:
  FREE(vol->vbm);

  vol->vbm   = 0;
  vol->vbmsz = 0;

  return -1;
}
