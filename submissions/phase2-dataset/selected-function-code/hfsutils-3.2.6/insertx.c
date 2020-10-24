int insertx(node *np, byte *record, int *reclen)
{
  node child;
  byte *rec;
  int result = 0;

  if (n_search(np, record))
    ERROR(EIO, "b*-tree record already exists");

  switch (np->nd.ndType)
    {
    case ndIndxNode:
      if (np->rnum == -1)
	rec = HFS_NODEREC(*np, 0);
      else
	rec = HFS_NODEREC(*np, np->rnum);

      if (bt_getnode(&child, np->bt, d_getul(HFS_RECDATA(rec))) == -1 ||
	  insertx(&child, record, reclen) == -1)
	goto fail;

      if (np->rnum == -1)
	{
	  n_index(&child, rec, 0);
	  if (*reclen == 0)
	    {
	      result = bt_putnode(np);
	      goto done;
	    }
	}

      if (*reclen)
	result = n_insert(np, record, reclen);

      break;

    case ndLeafNode:
      result = n_insert(np, record, reclen);
      break;

    default:
      ERROR(EIO, "unexpected b*-tree node");
    }

done:
  return result;

fail:
  return -1;
}
