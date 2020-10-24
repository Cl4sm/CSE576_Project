int bt_search(btree *bt, const byte *key, node *np)
{
  int found = 0;
  unsigned long nnum;

  nnum = bt->hdr.bthRoot;

  if (nnum == 0)
    ERROR(ENOENT, 0);

  while (1)
    {
      const byte *rec;

      if (bt_getnode(np, bt, nnum) == -1)
	{
	  found = -1;
	  goto fail;
	}

      found = n_search(np, key);

      switch (np->nd.ndType)
	{
	case ndIndxNode:
	  if (np->rnum == -1)
	    ERROR(ENOENT, 0);

	  rec  = HFS_NODEREC(*np, np->rnum);
	  nnum = d_getul(HFS_RECDATA(rec));

	  break;

	case ndLeafNode:
	  if (! found)
	    ERROR(ENOENT, 0);

	  goto done;

	default:
	  found = -1;
	  ERROR(EIO, "unexpected b*-tree node");
	}
    }

done:
fail:
  return found;
}
