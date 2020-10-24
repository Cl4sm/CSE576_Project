{
  hfsvol *vol = file->vol;

  if (vol->flags & HFS_VOL_READONLY)
    goto done;

  if (file->flags & HFS_FILE_UPDATE_CATREC)
    {
      node n;

      file->cat.u.fil.filStBlk  = file->cat.u.fil.filExtRec[0].xdrStABN;
      file->cat.u.fil.filRStBlk = file->cat.u.fil.filRExtRec[0].xdrStABN;

      if (v_catsearch(vol, file->parid, file->name, 0, 0, &n) <= 0 ||
	  v_putcatrec(&file->cat, &n) == -1)
	goto fail;

      file->flags &= ~HFS_FILE_UPDATE_CATREC;
    }

done:
  return 0;

fail:
  return -1;
}
