	      int (*func)(hfsvol *, unsigned int, unsigned int, block *))
{
  unsigned int abnum;
  unsigned int blnum;
  unsigned int fabn;
  int i;

  abnum = num / file->vol->lpa;
  blnum = num % file->vol->lpa;

  /* locate the appropriate extent record */

  fabn = file->fabn;

  if (abnum < fabn)
    {
      ExtDataRec *extrec;

      f_getptrs(file, &extrec, 0, 0);

      fabn = file->fabn = 0;
      memcpy(&file->ext, extrec, sizeof(ExtDataRec));
    }
  else
    abnum -= fabn;

  while (1)
    {
      unsigned int n;

      for (i = 0; i < 3; ++i)
	{
	  n = file->ext[i].xdrNumABlks;

	  if (abnum < n)
	    return func(file->vol, file->ext[i].xdrStABN + abnum, blnum, bp);

	  fabn  += n;
	  abnum -= n;
	}

      if (v_extsearch(file, fabn, &file->ext, 0) <= 0)
	goto fail;

      file->fabn = fabn;
    }

fail:
  return -1;
}
