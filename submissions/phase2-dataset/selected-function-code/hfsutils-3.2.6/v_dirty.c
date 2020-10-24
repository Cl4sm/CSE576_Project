int v_dirty(hfsvol *vol)
{
  if (vol->mdb.drAtrb & HFS_ATRB_UMOUNTED)
    {
      vol->mdb.drAtrb &= ~HFS_ATRB_UMOUNTED;
      ++vol->mdb.drWrCnt;

      if (v_writemdb(vol) == -1)
	goto fail;

      if ((vol->flags & HFS_VOL_USINGCACHE) &&
	  b_flush(vol) == -1)
	goto fail;
    }

  return 0;

fail:
  return -1;
}
