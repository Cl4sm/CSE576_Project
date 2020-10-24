{
  block b;
  byte *ptr = b;
  int i;

  d_storesw(&ptr, ddr->sbSig);
  d_storesw(&ptr, ddr->sbBlkSize);
  d_storesl(&ptr, ddr->sbBlkCount);
  d_storesw(&ptr, ddr->sbDevType);
  d_storesw(&ptr, ddr->sbDevId);
  d_storesl(&ptr, ddr->sbData);
  d_storesw(&ptr, ddr->sbDrvrCount);
  d_storesl(&ptr, ddr->ddBlock);
  d_storesw(&ptr, ddr->ddSize);
  d_storesw(&ptr, ddr->ddType);

  for (i = 0; i < 243; ++i)
    d_storesw(&ptr, ddr->ddPad[i]);

  ASSERT(ptr - b == HFS_BLOCKSZ);

  if (b_writepb(vol, 0, &b, 1) == -1)
    goto fail;

  return 0;

fail:
  return -1;
}
