ppc64_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		   GElf_Addr adjust)
{
  if (GELF_R_TYPE (rela->r_info) == R_PPC64_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_PPC64_IRELATIVE)
    {
      GElf_Addr val = read_ube64 (dso, rela->r_offset);

      if (val == rela->r_addend && val >= start)
	write_be64 (dso, rela->r_offset, val + adjust);
      if (rela->r_addend >= start)
	rela->r_addend += adjust;
    }
  else if (GELF_R_TYPE (rela->r_info) == R_PPC64_JMP_IREL)
    {
      if (rela->r_addend >= start)
	rela->r_addend += adjust;
    }
  return 0;
}
