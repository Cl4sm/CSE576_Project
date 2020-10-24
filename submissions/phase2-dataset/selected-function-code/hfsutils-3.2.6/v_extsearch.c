int v_extsearch(hfsfile *file, unsigned int fabn,
		ExtDataRec *data, node *np)
{
  ExtKeyRec key;
  ExtDataRec extsave;
  unsigned int fabnsave;
  byte pkey[HFS_EXTKEYLEN];
  const byte *ptr;
  node n;
  int found;

  if (np == 0)
    np = &n;

  r_makeextkey(&key, file->fork, file->cat.u.fil.filFlNum, fabn);
  r_packextkey(&key, pkey, 0);

  /* in case bt_search() clobbers these */

  memcpy(&extsave, &file->ext, sizeof(ExtDataRec));
  fabnsave = file->fabn;

  found = bt_search(&file->vol->ext, pkey, np);

  memcpy(&file->ext, &extsave, sizeof(ExtDataRec));
  file->fabn = fabnsave;

  if (found <= 0)
    return found;

  if (data)
    {
      ptr = HFS_NODEREC(*np, np->rnum);
      r_unpackextdata(HFS_RECDATA(ptr), data);
    }

  return 1;
}
