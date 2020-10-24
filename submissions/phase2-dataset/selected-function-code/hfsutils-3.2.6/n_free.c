int n_free(node *np)
{
  btree *bt = np->bt;
  node sib;

  if (bt->hdr.bthFNode == np->nnum)
    bt->hdr.bthFNode = np->nd.ndFLink;

  if (bt->hdr.bthLNode == np->nnum)
    bt->hdr.bthLNode = np->nd.ndBLink;

  if (np->nd.ndFLink > 0)
    {
      if (bt_getnode(&sib, bt, np->nd.ndFLink) == -1)
	goto fail;

      sib.nd.ndBLink = np->nd.ndBLink;

      if (bt_putnode(&sib) == -1)
	goto fail;
    }

  if (np->nd.ndBLink > 0)
    {
      if (bt_getnode(&sib, bt, np->nd.ndBLink) == -1)
	goto fail;

      sib.nd.ndFLink = np->nd.ndFLink;

      if (bt_putnode(&sib) == -1)
	goto fail;
    }

  BMCLR(bt->map, np->nnum);
  ++bt->hdr.bthFree;

  bt->flags |= HFS_BT_UPDATE_HDR;

  return 0;

fail:
  return -1;
}
