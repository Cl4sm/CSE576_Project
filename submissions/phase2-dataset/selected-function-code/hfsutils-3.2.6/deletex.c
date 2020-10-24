static
int deletex(node *np, const byte *key, byte *record, int *flag)
{
  node child;
  byte *rec;
  int found, result = 0;

  found = n_search(np, key);

  switch (np->nd.ndType)
    {
    case ndIndxNode:
      if (np->rnum == -1)
	ERROR(EIO, "b*-tree record not found");

      rec = HFS_NODEREC(*np, np->rnum);

      if (bt_getnode(&child, np->bt, d_getul(HFS_RECDATA(rec))) == -1 ||
	  deletex(&child, key, rec, flag) == -1)
	goto fail;

      if (*flag)
	{
	  *flag = 0;

	  if (HFS_RECKEYLEN(rec) == 0)
	    {
	      result = n_delete(np, record, flag);
	      break;
	    }

	  if (np->rnum == 0)
	    {
	      /* propagate index record change into parent */

	      n_index(np, record, 0);
	      *flag = 1;
	    }

	  result = bt_putnode(np);
	}

      break;

    case ndLeafNode:
      if (found == 0)
	ERROR(EIO, "b*-tree record not found");

      result = n_delete(np, record, flag);
      break;

    default:
      ERROR(EIO, "unexpected b*-tree node");
    }

  return result;

fail:
  return -1;
}
