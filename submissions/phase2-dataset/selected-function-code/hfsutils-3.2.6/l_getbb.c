int l_getbb(hfsvol *vol, BootBlkHdr *bb, byte *bootcode)
{
  block b;
  const byte *ptr = b;

  if (b_readlb(vol, 0, &b) == -1)
    goto fail;

  d_fetchsw(&ptr, &bb->bbID);
  d_fetchsl(&ptr, &bb->bbEntry);
  d_fetchsw(&ptr, &bb->bbVersion);
  d_fetchsw(&ptr, &bb->bbPageFlags);

  d_fetchstr(&ptr, bb->bbSysName,    sizeof(bb->bbSysName));
  d_fetchstr(&ptr, bb->bbShellName,  sizeof(bb->bbShellName));
  d_fetchstr(&ptr, bb->bbDbg1Name,   sizeof(bb->bbDbg1Name));
  d_fetchstr(&ptr, bb->bbDbg2Name,   sizeof(bb->bbDbg2Name));
  d_fetchstr(&ptr, bb->bbScreenName, sizeof(bb->bbScreenName));
  d_fetchstr(&ptr, bb->bbHelloName,  sizeof(bb->bbHelloName));
  d_fetchstr(&ptr, bb->bbScrapName,  sizeof(bb->bbScrapName));

  d_fetchsw(&ptr, &bb->bbCntFCBs);
  d_fetchsw(&ptr, &bb->bbCntEvts);
  d_fetchsl(&ptr, &bb->bb128KSHeap);
  d_fetchsl(&ptr, &bb->bb256KSHeap);
  d_fetchsl(&ptr, &bb->bbSysHeapSize);
  d_fetchsw(&ptr, &bb->filler);
  d_fetchsl(&ptr, &bb->bbSysHeapExtra);
  d_fetchsl(&ptr, &bb->bbSysHeapFract);

  ASSERT(ptr - b == 148);

  if (bootcode)
    {
      memcpy(bootcode, ptr, HFS_BOOTCODE1LEN);

      if (b_readlb(vol, 1, &b) == -1)
	goto fail;

      memcpy(bootcode + HFS_BOOTCODE1LEN, b, HFS_BOOTCODE2LEN);
    }

  return 0;

fail:
  return -1;
}
