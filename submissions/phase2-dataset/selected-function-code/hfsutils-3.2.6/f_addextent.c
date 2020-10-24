int f_addextent(hfsfile *file, ExtDescriptor *blocks)
{
  hfsvol *vol = file->vol;
  ExtDataRec *extrec;
  unsigned long *pylen;
  unsigned int start, end;
  node n;
  int i;

  f_getptrs(file, &extrec, 0, &pylen);

  start  = file->fabn;
  end    = *pylen / vol->mdb.drAlBlkSiz;

  n.nnum = 0;
  i      = -1;

  while (start < end)
    {
      for (i = 0; i < 3; ++i)
	{
	  unsigned int num;

	  num    = file->ext[i].xdrNumABlks;
	  start += num;

	  if (start == end)
	    break;
	  else if (start > end)
	    ERROR(EIO, "file extents exceed file physical length");
	  else if (num == 0)
	    ERROR(EIO, "empty file extent");
	}

      if (start == end)
	break;

      if (v_extsearch(file, start, &file->ext, &n) <= 0)
	goto fail;

      file->fabn = start;
    }

  if (i >= 0 &&
      file->ext[i].xdrStABN + file->ext[i].xdrNumABlks == blocks->xdrStABN)
    file->ext[i].xdrNumABlks += blocks->xdrNumABlks;
  else
    {
      /* create a new extent descriptor */

      if (++i < 3)
	file->ext[i] = *blocks;
      else
	{
	  ExtKeyRec key;
	  byte record[HFS_MAX_EXTRECLEN];
	  unsigned int reclen;

	  /* record is full; create a new one */

	  file->ext[0] = *blocks;

	  for (i = 1; i < 3; ++i)
	    {
	      file->ext[i].xdrStABN    = 0;
	      file->ext[i].xdrNumABlks = 0;
	    }

	  file->fabn = start;

	  r_makeextkey(&key, file->fork, file->cat.u.fil.filFlNum, end);
	  r_packextrec(&key, &file->ext, record, &reclen);

	  if (bt_insert(&vol->ext, record, reclen) == -1)
	    goto fail;

	  i = -1;
	}
    }

  if (i >= 0)
    {
      /* store the modified extent record */

      if (file->fabn)
	{
	  if ((n.nnum == 0 &&
	       v_extsearch(file, file->fabn, 0, &n) <= 0) ||
	      v_putextrec(&file->ext, &n) == -1)
	    goto fail;
	}
      else
	memcpy(extrec, &file->ext, sizeof(ExtDataRec));
    }

  *pylen += blocks->xdrNumABlks * vol->mdb.drAlBlkSiz;

  file->flags |= HFS_FILE_UPDATE_CATREC;

  return 0;

fail:
  return -1;
}
