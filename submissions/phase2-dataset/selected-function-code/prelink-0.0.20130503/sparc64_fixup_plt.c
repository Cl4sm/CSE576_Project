static void
sparc64_fixup_plt (DSO *dso, GElf_Rela *rela, GElf_Addr value)
{
  Elf64_Sxword disp = value - rela->r_offset;

  if (rela->r_addend)
    {
      /* .plt[32768+]  */
      write_be64 (dso, rela->r_offset, value);
    }
  else if (disp >= -0x800000 && disp < 0x800000)
    {
      /* b,a value
	  nop
	 nop  */
      write_be32 (dso, rela->r_offset, 0x30800000 | ((disp >> 2) & 0x3fffff));
      write_be32 (dso, rela->r_offset + 4, 0x01000000);
      write_be32 (dso, rela->r_offset + 8, 0x01000000);
    }
  else if (! (value >> 32))
    {
      /* sethi %hi(value), %g1
	 jmpl %g1 + %lo(value), %g0
	  nop  */
      write_be32 (dso, rela->r_offset, 0x03000000 | ((value >> 10) & 0x3fffff));
      write_be32 (dso, rela->r_offset + 4, 0x81c06000 | (value & 0x3ff));
      write_be32 (dso, rela->r_offset + 8, 0x01000000);
    }
  else if ((rela->r_offset + 4 > value
	    && ((rela->r_offset - value) >> 31) == 0)
	   || (value > rela->r_offset + 4
	       && ((value - rela->r_offset - 4) >> 31) == 0))
    {
      /* mov %o7, %g1
	 call value
	  mov %g1, %o7  */
      write_be32 (dso, rela->r_offset, 0x8210000f);
      write_be32 (dso, rela->r_offset + 4, 0x40000000
		  | (((value - rela->r_offset - 4) >> 2) & 0x3fffffff));
      write_be32 (dso, rela->r_offset + 8, 0x9e100001);
    }
  else
    {
      unsigned int csts[4];
      int i = 0;

      /* sethi %hh(value), %g1
	 sethi %lm(value), %g5
	 or %g1, %hm(value), %g1
	 or %g5, %lo(value), %g5
	 sllx %g1, 32, %g1
	 jmpl %g1 + %g5, %g0
	  nop  */

      csts[0] = value >> 42;
      csts[1] = (value >> 32) & 0x3ff;
      csts[2] = (value >> 10) & 0x3fffff;
      csts[3] = value & 0x3ff;
      write_be32 (dso, rela->r_offset, 0x03000000 | csts[0]);
      write_be32 (dso, rela->r_offset + 4, 0x0b000000 | csts[2]);
      /* Sparc64 shared libs are often 0xfffff800XXXXXXXX, so optimize
	 for this common case.  */
      if (csts[1] == 0)
	write_be32 (dso, rela->r_offset + 8, 0x83287020);
      else
	write_be32 (dso, rela->r_offset + 8, 0x82106000 | csts[1]);
      write_be32 (dso, rela->r_offset + 12, 0x8a116000 | csts[3]);
      if (csts[1] != 0)
	write_be32 (dso, rela->r_offset + 16, 0x83287020), i = 4;
      write_be32 (dso, rela->r_offset + 16 + i, 0x81c04005);
      write_be32 (dso, rela->r_offset + 20 + i, 0x01000000);
    }
}
