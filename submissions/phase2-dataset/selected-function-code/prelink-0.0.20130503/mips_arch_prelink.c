static int
mips_arch_prelink (struct prelink_info *info)
{
  struct mips_global_got_iterator ggi;
  DSO *dso;
  GElf_Addr value;

  dso = info->dso;
  if (dso->info[DT_PLTGOT] == 0)
    return 0;

  /* Install Quickstart values for all global GOT entries of type A-D
     in the table above.  */
  mips_init_global_got_iterator (&ggi, dso);
  while (mips_get_global_got_entry (&ggi))
    {
      value = info->resolve (info, ggi.sym_index, R_MIPS_REL32);
      if (ggi.sym.st_shndx == SHN_UNDEF
	  || ggi.sym.st_shndx == SHN_COMMON)
	buf_write_ne32 (dso, ggi.got_entry, value);
      else
	{
	  /* Type E and F in the table above.  We cannot install Quickstart
	     values for type E, but we should never need to in executables,
	     because an executable should not use lazy binding stubs for
	     symbols it defines itself.  Although we could in theory just
	     discard any such stub address, it goes against the principle
	     that prelinking should be reversible.

	     When type E entries occur in shared libraries, we can fix
	     them up using conflicts.

	     Type F entries should never need a Quickstart value -- the
	     current value should already be correct.  However, the conflict
	     code will cope correctly with malformed type F entries in
	     shared libraries, so we only complain about executables here.  */
	  if (dso->ehdr.e_type == ET_EXEC
	      && value != buf_read_une32 (dso, ggi.got_entry))
	    {
	      error (0, 0, "%s: The global GOT entries for defined symbols"
		     " do not match their st_values\n", dso->filename);
	      return 1;
	    }
	}
    }
  return ggi.failed;
}
