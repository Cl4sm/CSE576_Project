static void
alpha_fixup_plt (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr,
		 GElf_Addr value)
{
  Elf64_Sxword disp;
  Elf64_Addr plt;

  relaaddr -= dso->info[DT_JMPREL];
  relaaddr /= sizeof (Elf64_Rela);
  relaaddr *= 12;
  plt = dso->info[DT_PLTGOT] + 32 + relaaddr;
  disp = ((Elf64_Sxword) (value - plt - 12)) / 4;
  if (disp >= -0x100000 && disp < 0x100000)
    {
      int32_t hi, lo;

      hi = value - plt;
      lo = (int16_t) hi;
      hi = (hi - lo) >> 16;

      /* ldah $27,hi($27)
	 lda $27,lo($27)
	 br $31,value  */
      write_le32 (dso, plt, 0x277b0000 | (hi & 0xffff));
      write_le32 (dso, plt + 4, 0x237b0000 | (lo & 0xffff));
      write_le32 (dso, plt + 8, 0xc3e00000 | (disp & 0x1fffff));
    }
  else
    {
      int32_t hi, lo;

      hi = rela->r_offset - plt;
      lo = (int16_t) hi;
      hi = (hi - lo) >> 16;

      /* ldah $27,hi($27)
	 ldq $27,lo($27)
	 jmp $31,($27)  */
      write_le32 (dso, plt, 0x277b0000 | (hi & 0xffff));
      write_le32 (dso, plt + 4, 0xa77b0000 | (lo & 0xffff));
      write_le32 (dso, plt + 8, 0x6bfb0000);
    }
}
