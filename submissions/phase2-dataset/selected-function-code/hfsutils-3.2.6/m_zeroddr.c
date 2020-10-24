int m_zeroddr(hfsvol *vol)
{
  Block0 ddr;
  int i;

  ASSERT(vol->pnum == 0 && vol->vlen != 0);

  ddr.sbSig       = HFS_DDR_SIGWORD;
  ddr.sbBlkSize   = HFS_BLOCKSZ;
  ddr.sbBlkCount  = vol->vlen;

  ddr.sbDevType   = 0;
  ddr.sbDevId     = 0;
  ddr.sbData      = 0;

  ddr.sbDrvrCount = 0;

  ddr.ddBlock     = 0;
  ddr.ddSize      = 0;
  ddr.ddType      = 0;

  for (i = 0; i < 243; ++i)
    ddr.ddPad[i] = 0;

  return l_putddr(vol, &ddr);
}
