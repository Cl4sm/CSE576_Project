mips_adjust_dyn (DSO *dso, int n, GElf_Dyn *dyn, GElf_Addr start,
		 GElf_Addr adjust)
{
  switch (dyn->d_tag)
    {
    case DT_MIPS_TIME_STAMP:
    case DT_MIPS_ICHECKSUM:
    case DT_MIPS_IVERSION:
    case DT_MIPS_CONFLICT:
    case DT_MIPS_CONFLICTNO:
    case DT_MIPS_LIBLIST:
    case DT_MIPS_LIBLISTNO:
      error (0, 0, "%s: File contains QuickStart information", dso->filename);
      return 1;

    case DT_MIPS_BASE_ADDRESS:
    case DT_MIPS_RLD_MAP:
    case DT_MIPS_OPTIONS:
      if (dyn->d_un.d_ptr >= start)
	dyn->d_un.d_ptr += adjust;
      return 1;

    case DT_MIPS_LOCAL_GOTNO:
    case DT_MIPS_UNREFEXTNO:
    case DT_MIPS_SYMTABNO:
    case DT_MIPS_HIPAGENO:
    case DT_MIPS_GOTSYM:
      /* We don't change the layout of the GOT or symbol table.  */
      return 1;

    case DT_MIPS_RLD_VERSION:
    case DT_MIPS_FLAGS:
      /* We don't change these properties.  */
      return 1;
    }
  return 0;
}
