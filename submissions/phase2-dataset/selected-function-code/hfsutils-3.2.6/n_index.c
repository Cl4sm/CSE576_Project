void n_index(const node *np, byte *record, unsigned int *reclen)
{
  const byte *key = HFS_NODEREC(*np, 0);

  if (np->bt == &np->bt->f.vol->cat)
    {
      /* force the key length to be 0x25 */

      HFS_SETKEYLEN(record, 0x25);
      memset(record + 1, 0, 0x25);
      memcpy(record + 1, key + 1, HFS_RECKEYLEN(key));
    }
  else
    memcpy(record, key, HFS_RECKEYSKIP(key));

  d_putul(HFS_RECDATA(record), np->nnum);

  if (reclen)
    *reclen = HFS_RECKEYSKIP(record) + 4;
}
