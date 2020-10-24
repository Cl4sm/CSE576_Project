int ck_btree(btree *bt)
{
  printf("*** Checking %s B*-tree\n", bt->f.name);

  if (bt_readhdr(bt) == -1)
    return -1;

  if (VERBOSE)
    {
      printf("  bthDepth    = %u\n",  bt->hdr.bthDepth);
      printf("  bthRoot     = %lu\n", bt->hdr.bthRoot);
      printf("  bthNRecs    = %lu\n", bt->hdr.bthNRecs);
      printf("  bthFNode    = %lu\n", bt->hdr.bthFNode);
      printf("  bthLNode    = %lu\n", bt->hdr.bthLNode);
      printf("  bthNodeSize = %u\n",  bt->hdr.bthNodeSize);
      printf("  bthKeyLen   = %u\n",  bt->hdr.bthKeyLen);
      printf("  bthNNodes   = %lu\n", bt->hdr.bthNNodes);
      printf("  bthFree     = %lu\n", bt->hdr.bthFree);
    }

# if 0
  if (VERBOSE)
    {
      node n;
      int i;

      n.bt   = bt;
      n.nnum = bt->hdr.bthFNode;

      while (n.nnum)
	{
	  if (bt_getnode(&n) == -1)
	    return -1;

	  if (n.nnum == bt->hdr.bthFNode)
	    printf("  { ");
	  else
	    printf("} { ");

	  for (i = 0; i < n.nd.ndNRecs; ++i)
	    {
	      byte *ptr;

	      ptr = HFS_NODEREC(n, i);

	      outhex(ptr, 1 + HFS_RECKEYLEN(ptr));
	      printf(": ");
	      outhex(HFS_RECDATA(ptr),
		     HFS_NODEREC(n, i + 1) - HFS_RECDATA(ptr));
	      printf("\n");

	      if (i < n.nd.ndNRecs - 1)
		printf("    ");
	    }

	  n.nnum = n.nd.ndFLink;
	}

      printf("}\n");
    }
# endif

  return 0;
}
