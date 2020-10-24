static
void compact(node *np)
{
  byte *ptr;
  int offset, nrecs, i;

  offset = 0x00e;
  ptr    = np->data + offset;
  nrecs  = 0;

  for (i = 0; i < np->nd.ndNRecs; ++i)
    {
      const byte *rec;
      int reclen;

      rec    = HFS_NODEREC(*np, i);
      reclen = HFS_RECLEN(*np, i);

      if (HFS_RECKEYLEN(rec) > 0)
	{
	  np->roff[nrecs++] = offset;
	  offset += reclen;

	  if (ptr == rec)
	    ptr += reclen;
	  else
	    {
	      while (reclen--)
		*ptr++ = *rec++;
	    }
	}
    }

  np->roff[nrecs] = offset;
  np->nd.ndNRecs  = nrecs;
}
