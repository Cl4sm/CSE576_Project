sparc_fixup_plt (DSO *dso, GElf_Rela *rela, GElf_Addr value)
{
  Elf32_Sword disp = value - rela->r_offset;

  if (disp >= -0x800000 && disp < 0x800000)
    {
      /* b,a value
	  nop
	 nop  */
      write_be32 (dso, rela->r_offset, 0x30800000 | ((disp >> 2) & 0x3fffff));
      write_be32 (dso, rela->r_offset + 4, 0x01000000);
      write_be32 (dso, rela->r_offset + 8, 0x01000000);
    }
  else
    {
      /* sethi %hi(value), %g1
	 jmpl %g1 + %lo(value), %g0
	  nop  */
      write_be32 (dso, rela->r_offset, 0x03000000 | ((value >> 10) & 0x3fffff));
      write_be32 (dso, rela->r_offset + 4, 0x81c06000 | (value & 0x3ff));
      write_be32 (dso, rela->r_offset + 8, 0x01000000);
    }
}
