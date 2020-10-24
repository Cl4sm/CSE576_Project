int flushvol(hfsvol *vol, int umount)
{
  if (vol->flags & HFS_VOL_READONLY)
    goto done;

  if ((vol->ext.flags & HFS_BT_UPDATE_HDR) &&
      bt_writehdr(&vol->ext) == -1)
    goto fail;

  if ((vol->cat.flags & HFS_BT_UPDATE_HDR) &&
      bt_writehdr(&vol->cat) == -1)
    goto fail;

  if ((vol->flags & HFS_VOL_UPDATE_VBM) &&
      v_writevbm(vol) == -1)
    goto fail;

  if (umount && ! (vol->mdb.drAtrb & HFS_ATRB_UMOUNTED))
    {
      vol->mdb.drAtrb |= HFS_ATRB_UMOUNTED;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  if ((vol->flags & (HFS_VOL_UPDATE_MDB | HFS_VOL_UPDATE_ALTMDB)) &&
      v_writemdb(vol) == -1)
    goto fail;

done:
  return 0;

fail:
  return -1;
}
