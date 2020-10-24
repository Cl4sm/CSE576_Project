int l_getpmentry(hfsvol *vol, Partition *map, unsigned long bnum)
{
  block b;
  const byte *ptr = b;
  int i;

  if (b_readpb(vol, bnum, &b, 1) == -1)
    goto fail;

  d_fetchsw(&ptr, &map->pmSig);
  d_fetchsw(&ptr, &map->pmSigPad);
  d_fetchsl(&ptr, &map->pmMapBlkCnt);
  d_fetchsl(&ptr, &map->pmPyPartStart);
  d_fetchsl(&ptr, &map->pmPartBlkCnt);

  strncpy((char *) map->pmPartName, (const char *) ptr, 32);
  map->pmPartName[32] = 0;
  ptr += 32;

  strncpy((char *) map->pmParType, (const char *) ptr, 32);
  map->pmParType[32] = 0;
  ptr += 32;

  d_fetchsl(&ptr, &map->pmLgDataStart);
  d_fetchsl(&ptr, &map->pmDataCnt);
  d_fetchsl(&ptr, &map->pmPartStatus);
  d_fetchsl(&ptr, &map->pmLgBootStart);
  d_fetchsl(&ptr, &map->pmBootSize);
  d_fetchsl(&ptr, &map->pmBootAddr);
  d_fetchsl(&ptr, &map->pmBootAddr2);
  d_fetchsl(&ptr, &map->pmBootEntry);
  d_fetchsl(&ptr, &map->pmBootEntry2);
  d_fetchsl(&ptr, &map->pmBootCksum);

  strncpy((char *) map->pmProcessor, (const char *) ptr, 16);
  map->pmProcessor[16] = 0;
  ptr += 16;

  for (i = 0; i < 188; ++i)
    d_fetchsw(&ptr, &map->pmPad[i]);

  ASSERT(ptr - b == HFS_BLOCKSZ);

  return 0;

fail:
  return -1;
}
