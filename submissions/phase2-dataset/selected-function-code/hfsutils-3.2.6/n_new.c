int n_new(node *np)
{
  btree *bt = np->bt;
  unsigned long num;

  if (bt->hdr.bthFree == 0)
    ERROR(EIO, "b*-tree full");

  num = 0;
  while (num < bt->hdr.bthNNodes && BMTST(bt->map, num))
    ++num;

  if (num == bt->hdr.bthNNodes)
    ERROR(EIO, "free b*-tree node not found");

  np->nnum = num;

  BMSET(bt->map, num);
  --bt->hdr.bthFree;

  bt->flags |= HFS_BT_UPDATE_HDR;

  return 0;

fail:
  return -1;
}
