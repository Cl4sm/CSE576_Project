static int
x86_64_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		    GElf_Addr adjust)
{
  Elf64_Addr addr;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_X86_64_RELATIVE:
      if (rela->r_addend >= start)
	{
	  if (read_ule64 (dso, rela->r_offset) == rela->r_addend)
	    write_le64 (dso, rela->r_offset, rela->r_addend + adjust);
	  rela->r_addend += adjust;
	}
      break;
    case R_X86_64_IRELATIVE:
      if (rela->r_addend >= start)
	rela->r_addend += adjust;
      /* FALLTHROUGH */
    case R_X86_64_JUMP_SLOT:
      addr = read_ule64 (dso, rela->r_offset);
      if (addr >= start)
	write_le64 (dso, rela->r_offset, addr + adjust);
      break;
    }
  return 0;
}
