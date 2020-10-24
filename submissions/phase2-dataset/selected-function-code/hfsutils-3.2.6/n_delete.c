{
  byte *rec;

  rec = HFS_NODEREC(*np, np->rnum);

  HFS_SETKEYLEN(rec, 0);
  compact(np);

  if (np->nd.ndNRecs == 0)
    {
      if (n_free(np) == -1)
	goto fail;

      HFS_SETKEYLEN(record, 0);
      *flag = 1;

      return 0;
    }

  /* see if we can join with our left sibling */

  if (np->nd.ndBLink > 0)
    {
      node left;

      if (bt_getnode(&left, np->bt, np->nd.ndBLink) == -1)
	goto fail;

      if (np->nd.ndNRecs + left.nd.ndNRecs <= HFS_MAX_NRECS &&
	  NODEUSED(*np) + 2 * np->nd.ndNRecs <= NODEFREE(left))
	return join(&left, np, record, flag);
    }

  if (np->rnum == 0)
    {
      /* special case: first record changed; update parent record key */

      n_index(np, record, 0);
      *flag = 1;
    }

  return bt_putnode(np);

fail:
  return -1;
}
