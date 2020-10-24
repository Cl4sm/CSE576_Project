{
  /* read the MDB, volume bitmap, and extents/catalog B*-tree headers */

  if (v_readmdb(vol) == -1 ||
      v_readvbm(vol) == -1 ||
      bt_readhdr(&vol->ext) == -1 ||
      bt_readhdr(&vol->cat) == -1)
    goto fail;

  if (! (vol->mdb.drAtrb & HFS_ATRB_UMOUNTED) &&
      v_scavenge(vol) == -1)
    goto fail;

  if (vol->mdb.drAtrb & HFS_ATRB_SLOCKED)
    vol->flags |= HFS_VOL_READONLY;
  else if (vol->flags & HFS_VOL_READONLY)
    vol->mdb.drAtrb |= HFS_ATRB_HLOCKED;
  else
    vol->mdb.drAtrb &= ~HFS_ATRB_HLOCKED;

  vol->flags |= HFS_VOL_MOUNTED;

  return 0;

fail:
  return -1;
}
