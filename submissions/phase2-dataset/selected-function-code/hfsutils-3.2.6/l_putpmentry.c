int l_putpmentry(hfsvol *vol, const Partition *map, unsigned long bnum)
{
  block b;
  byte *ptr = b;
  int i;

  d_storesw(&ptr, map->pmSig);
  d_storesw(&ptr, map->pmSigPad);
  d_storesl(&ptr, map->pmMapBlkCnt);
  d_storesl(&ptr, map->pmPyPartStart);
  d_storesl(&ptr, map->pmPartBlkCnt);

  memset(ptr, 0, 32);
  strncpy((char *) ptr, (const char *) map->pmPartName, 32);
  ptr += 32;

  memset(ptr, 0, 32);
  strncpy((char *) ptr, (const char *) map->pmParType, 32);
  ptr += 32;

  d_storesl(&ptr, map->pmLgDataStart);
  d_storesl(&ptr, map->pmDataCnt);
  d_storesl(&ptr, map->pmPartStatus);
  d_storesl(&ptr, map->pmLgBootStart);
  d_storesl(&ptr, map->pmBootSize);
  d_storesl(&ptr, map->pmBootAddr);
  d_storesl(&ptr, map->pmBootAddr2);
  d_storesl(&ptr, map->pmBootEntry);
  d_storesl(&ptr, map->pmBootEntry2);
  d_storesl(&ptr, map->pmBootCksum);

  memset(ptr, 0, 16);
  strncpy((char *) ptr, (const char *) map->pmProcessor, 16);
  ptr += 16;

  for (i = 0; i < 188; ++i)
    d_storesw(&ptr, map->pmPad[i]);

  ASSERT(ptr - b == HFS_BLOCKSZ);

  if (b_writepb(vol, bnum, &b, 1) == -1)
    goto fail;

  return 0;

fail:
  return -1;
}
