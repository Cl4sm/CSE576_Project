{
  const btree *bt = np->bt;
  byte key1[HFS_MAX_KEYLEN], key2[HFS_MAX_KEYLEN];
  int i, comp = -1;

  bt->keyunpack(pkey, key2);

  for (i = np->nd.ndNRecs; i--; )
    {
      const byte *rec;

      rec = HFS_NODEREC(*np, i);

      if (HFS_RECKEYLEN(rec) == 0)
	continue;  /* deleted record */

      bt->keyunpack(rec, key1);
      comp = bt->keycompare(key1, key2);

      if (comp <= 0)
	break;
    }

  np->rnum = i;

  return comp == 0;
}
