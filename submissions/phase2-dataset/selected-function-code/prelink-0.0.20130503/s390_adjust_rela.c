static int
s390_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		  GElf_Addr adjust)
{
  Elf32_Addr addr;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_390_RELATIVE:
      if ((Elf32_Addr) rela->r_addend >= start)
	{
	  addr = read_ube32 (dso, rela->r_offset);
	  if (addr == rela->r_addend)
	    write_be32 (dso, rela->r_offset, addr + adjust);
	  rela->r_addend += (Elf32_Sword) adjust;
	}
      break;
    case R_390_IRELATIVE:
      if (rela->r_addend >= start)
	/* Adjust the resolver function address.  */
	rela->r_addend += adjust;
      /* FALLTHROUGH */
    case R_390_JMP_SLOT:
      /* Adjust the address in the GOT slot.  */
      addr = read_ube32 (dso, rela->r_offset);
      if (addr >= start)
	write_be32 (dso, rela->r_offset, addr + adjust);
      break;
    }
  return 0;
}
