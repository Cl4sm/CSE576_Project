static int
cris_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		  GElf_Addr adjust)
{
  Elf32_Addr data;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_CRIS_RELATIVE:
      if ((Elf32_Addr) rela->r_addend >= start)
	rela->r_addend += (Elf32_Sword) adjust;
      break;
    case R_CRIS_JUMP_SLOT:
      data = read_ule32 (dso, rela->r_offset);
      if (data >= start)
	write_le32 (dso, rela->r_offset, data + adjust);
      break;
      break;
    }
  return 0;
}
