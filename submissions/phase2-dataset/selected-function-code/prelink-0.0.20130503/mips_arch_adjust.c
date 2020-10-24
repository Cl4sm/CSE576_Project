static int
mips_arch_adjust (DSO *dso, GElf_Addr start, GElf_Addr adjust)
{
  struct mips_local_got_iterator lgi;
  struct mips_global_got_iterator ggi;
  GElf_Addr value;

  if (dso->info[DT_PLTGOT] == 0)
    return 0;

  /* Adjust every local GOT entry by ADJUST.  Every adjustment moves
     the code and data, so we do not need to check START here.  */
  mips_init_local_got_iterator (&lgi, dso);
  while (mips_get_local_got_entry (&lgi))
    {
      value = buf_read_une32 (dso, lgi.got_entry);
      buf_write_ne32 (dso, lgi.got_entry, value + adjust);
    }

  /* Adjust every global GOT entry.  Referring to the table above:

     For [A, B, C]: Adjust the GOT entry if it contains st_value
     and if the symbol's value will be adjusted.

     For [D]: Do nothing.  SHN_COMMON entries never need adjusting.

     For [E, F]: Adjust the GOT entry if it does not contain st_value
     -- in other words, if it is a type E entry that points to a lazy
     binding stub -- or if the symbol's value will also be adjusted.  */
  mips_init_global_got_iterator (&ggi, dso);
  while (mips_get_global_got_entry (&ggi))
    {
      value = buf_read_une32 (dso, ggi.got_entry);
      if (ggi.sym.st_shndx != SHN_COMMON
	  && value >= start
	  && (value == ggi.sym.st_value
	      ? adjust_symbol_p (dso, &ggi.sym)
	      : ggi.sym.st_shndx != SHN_UNDEF))
	buf_write_ne32 (dso, ggi.got_entry, value + adjust);
    }

  return lgi.failed || ggi.failed;
}
