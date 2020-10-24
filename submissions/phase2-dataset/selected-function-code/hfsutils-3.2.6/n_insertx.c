{
  int rnum, i;
  byte *ptr;

  rnum = np->rnum + 1;

  /* push other records down to make room */

  for (ptr = HFS_NODEREC(*np, np->nd.ndNRecs) + reclen;
       ptr > HFS_NODEREC(*np, rnum) + reclen; --ptr)
    *(ptr - 1) = *(ptr - 1 - reclen);

  ++np->nd.ndNRecs;

  for (i = np->nd.ndNRecs; i > rnum; --i)
    np->roff[i] = np->roff[i - 1] + reclen;

  /* write the new record */

  memcpy(HFS_NODEREC(*np, rnum), record, reclen);
}
