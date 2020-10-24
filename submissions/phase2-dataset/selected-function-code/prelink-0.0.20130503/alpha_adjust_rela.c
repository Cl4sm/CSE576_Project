alpha_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		   GElf_Addr adjust)
{
  if (GELF_R_TYPE (rela->r_info) == R_ALPHA_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_ALPHA_JMP_SLOT)
    {
      GElf_Addr val = read_ule64 (dso, rela->r_offset);

      if (val >= start)
	{
	  write_le64 (dso, rela->r_offset, val + adjust);
	  if (val == rela->r_addend)
	    rela->r_addend += adjust;
	}
    }
  else if (GELF_R_TYPE (rela->r_info) == R_ALPHA_GLOB_DAT)
    {
      GElf_Addr val = read_ule64 (dso, rela->r_offset) - rela->r_addend;

      if (val && val >= start)
	write_le64 (dso, rela->r_offset, val + adjust + rela->r_addend);
    }
  return 0;
}
