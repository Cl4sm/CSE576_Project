int l_putbb(hfsvol *vol, const BootBlkHdr *bb, const byte *bootcode)
{
  block b;
  byte *ptr = b;

  d_storesw(&ptr, bb->bbID);
  d_storesl(&ptr, bb->bbEntry);
  d_storesw(&ptr, bb->bbVersion);
  d_storesw(&ptr, bb->bbPageFlags);

  d_storestr(&ptr, bb->bbSysName,    sizeof(bb->bbSysName));
  d_storestr(&ptr, bb->bbShellName,  sizeof(bb->bbShellName));
  d_storestr(&ptr, bb->bbDbg1Name,   sizeof(bb->bbDbg1Name));
  d_storestr(&ptr, bb->bbDbg2Name,   sizeof(bb->bbDbg2Name));
  d_storestr(&ptr, bb->bbScreenName, sizeof(bb->bbScreenName));
  d_storestr(&ptr, bb->bbHelloName,  sizeof(bb->bbHelloName));
  d_storestr(&ptr, bb->bbScrapName,  sizeof(bb->bbScrapName));

  d_storesw(&ptr, bb->bbCntFCBs);
  d_storesw(&ptr, bb->bbCntEvts);
  d_storesl(&ptr, bb->bb128KSHeap);
  d_storesl(&ptr, bb->bb256KSHeap);
  d_storesl(&ptr, bb->bbSysHeapSize);
  d_storesw(&ptr, bb->filler);
  d_storesl(&ptr, bb->bbSysHeapExtra);
  d_storesl(&ptr, bb->bbSysHeapFract);

  ASSERT(ptr - b == 148);

  if (bootcode)
    memcpy(ptr, bootcode, HFS_BOOTCODE1LEN);
  else
    memset(ptr, 0, HFS_BOOTCODE1LEN);

  if (b_writelb(vol, 0, &b) == -1)
    goto fail;

  if (bootcode)
    memcpy(&b, bootcode + HFS_BOOTCODE1LEN, HFS_BOOTCODE2LEN);
  else
    memset(&b, 0, HFS_BOOTCODE2LEN);

  if (b_writelb(vol, 1, &b) == -1)
    goto fail;

  return 0;

fail:
  return -1;
}
