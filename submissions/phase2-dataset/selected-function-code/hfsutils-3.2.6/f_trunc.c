{
  hfsvol *vol = file->vol;
  ExtDataRec *extrec;
  unsigned long *lglen, *pylen, alblksz, newpylen;
  unsigned int dlen, start, end;
  node n;
  int i;

  if (vol->flags & HFS_VOL_READONLY)
    goto done;

  f_getptrs(file, &extrec, &lglen, &pylen);

  alblksz  = vol->mdb.drAlBlkSiz;
  newpylen = (*lglen / alblksz + (*lglen % alblksz != 0)) * alblksz;

  if (newpylen > *pylen)
    ERROR(EIO, "file size exceeds physical length");
  else if (newpylen == *pylen)
    goto done;

  dlen  = (*pylen - newpylen) / alblksz;

  start = file->fabn;
  end   = newpylen / alblksz;

  if (start >= end)
    {
      start = file->fabn = 0;
      memcpy(&file->ext, extrec, sizeof(ExtDataRec));
    }

  n.nnum = 0;
  i      = -1;

  while (start < end)
    {
      for (i = 0; i < 3; ++i)
	{
	  unsigned int num;

	  num    = file->ext[i].xdrNumABlks;
	  start += num;

	  if (start >= end)
	    break;
	  else if (num == 0)
	    ERROR(EIO, "empty file extent");
	}

      if (start >= end)
	break;

      if (v_extsearch(file, start, &file->ext, &n) <= 0)
	goto fail;

      file->fabn = start;
    }

  if (start > end)
    {
      ExtDescriptor blocks;

      file->ext[i].xdrNumABlks -= start - end;
      dlen -= start - end;

      blocks.xdrStABN    = file->ext[i].xdrStABN + file->ext[i].xdrNumABlks;
      blocks.xdrNumABlks = start - end;

      if (v_freeblocks(vol, &blocks) == -1)
	goto fail;
    }

  *pylen = newpylen;

  file->flags |= HFS_FILE_UPDATE_CATREC;

  do
    {
      while (dlen && ++i < 3)
	{
	  unsigned int num;

	  num    = file->ext[i].xdrNumABlks;
	  start += num;

	  if (num == 0)
	    ERROR(EIO, "empty file extent");
	  else if (num > dlen)
	    ERROR(EIO, "file extents exceed physical size");

	  dlen -= num;

	  if (v_freeblocks(vol, &file->ext[i]) == -1)
	    goto fail;

	  file->ext[i].xdrStABN    = 0;
	  file->ext[i].xdrNumABlks = 0;
	}

      if (file->fabn)
	{
	  if (n.nnum == 0 &&
	      v_extsearch(file, file->fabn, 0, &n) <= 0)
	    goto fail;

	  if (file->ext[0].xdrNumABlks)
	    {
	      if (v_putextrec(&file->ext, &n) == -1)
		goto fail;
	    }
	  else
	    {
	      if (bt_delete(&vol->ext, HFS_NODEREC(n, n.rnum)) == -1)
		goto fail;

	      n.nnum = 0;
	    }
	}
      else
	memcpy(extrec, &file->ext, sizeof(ExtDataRec));

      if (dlen)
	{
	  if (v_extsearch(file, start, &file->ext, &n) <= 0)
	    goto fail;

	  file->fabn = start;
	  i = -1;
	}
    }
  while (dlen);

done:
  return 0;

fail:
  return -1;
}
