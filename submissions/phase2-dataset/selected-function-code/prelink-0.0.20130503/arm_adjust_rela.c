arm_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		 GElf_Addr adjust)
{
  Elf32_Addr data;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ARM_RELATIVE:
      if ((Elf32_Addr) rela->r_addend >= start)
	{
	  rela->r_addend += (Elf32_Sword) adjust;
	  /* Write it to the memory location as well.
	     Not necessary, but we can do it.  */
	  write_ne32 (dso, rela->r_offset, rela->r_addend);
	}
      break;
    case R_ARM_JUMP_SLOT:
      data = read_une32 (dso, rela->r_offset);
      if (data >= start)
	write_ne32 (dso, rela->r_offset, data + adjust);
      break;
      break;
    }
  return 0;
}
