		CatDataRec *data, char *cname, node *np)
{
  CatKeyRec key;
  byte pkey[HFS_CATKEYLEN];
  const byte *ptr;
  node n;
  int found;

  if (np == 0)
    np = &n;

  r_makecatkey(&key, parid, name);
  r_packcatkey(&key, pkey, 0);

  found = bt_search(&vol->cat, pkey, np);
  if (found <= 0)
    return found;

  ptr = HFS_NODEREC(*np, np->rnum);

  if (cname)
    {
      r_unpackcatkey(ptr, &key);
      strcpy(cname, key.ckrCName);
    }

  if (data)
    r_unpackcatdata(HFS_RECDATA(ptr), data);

  return 1;
}
