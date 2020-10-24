sparc_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		   GElf_Addr adjust)
{
  if (GELF_R_TYPE (rela->r_info) == R_SPARC_RELATIVE)
    {
      if (rela->r_addend)
	{
	  if ((Elf32_Addr) rela->r_addend >= start)
	    rela->r_addend += (Elf32_Sword) adjust;
	}
      else
	{
	  GElf_Addr val = read_ube32 (dso, rela->r_offset);

	  if (val >= start)
	    write_be32 (dso, rela->r_offset, val + adjust);
	}
    }
  return 0;
}
