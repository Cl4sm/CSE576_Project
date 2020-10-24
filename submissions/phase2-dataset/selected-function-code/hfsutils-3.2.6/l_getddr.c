{
  block b;
  const byte *ptr = b;
  int i;

  if (b_readpb(vol, 0, &b, 1) == -1)
    goto fail;

  d_fetchsw(&ptr, &ddr->sbSig);
  d_fetchsw(&ptr, &ddr->sbBlkSize);
  d_fetchsl(&ptr, &ddr->sbBlkCount);
  d_fetchsw(&ptr, &ddr->sbDevType);
  d_fetchsw(&ptr, &ddr->sbDevId);
  d_fetchsl(&ptr, &ddr->sbData);
  d_fetchsw(&ptr, &ddr->sbDrvrCount);
  d_fetchsl(&ptr, &ddr->ddBlock);
  d_fetchsw(&ptr, &ddr->ddSize);
  d_fetchsw(&ptr, &ddr->ddType);

  for (i = 0; i < 243; ++i)
    d_fetchsw(&ptr, &ddr->ddPad[i]);

  ASSERT(ptr - b == HFS_BLOCKSZ);

  return 0;

fail:
  return -1;
}
