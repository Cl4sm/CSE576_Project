alpha_is_indirect_plt (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  Elf64_Addr pltaddr;
  uint32_t plt[3];
  int32_t hi, lo;

  relaaddr -= dso->info[DT_JMPREL];
  relaaddr /= sizeof (Elf64_Rela);
  relaaddr *= 12;
  pltaddr = dso->info[DT_PLTGOT] + 32 + relaaddr;
  hi = rela->r_offset - pltaddr;
  lo = (int16_t) hi;
  hi = (hi - lo) >> 16;
  plt[0] = read_ule32 (dso, pltaddr);
  plt[1] = read_ule32 (dso, pltaddr + 4);
  plt[2] = read_ule32 (dso, pltaddr + 8);
  if (plt[0] == (0x277b0000 | (hi & 0xffff))
      && plt[1] == (0xa77b0000 | (lo & 0xffff))
      && plt[2] == 0x6bfb0000)
    return 1;
  return 0;
}
