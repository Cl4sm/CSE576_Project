sparc64_adjust_dyn (DSO *dso, int n, GElf_Dyn *dyn, GElf_Addr start,
		    GElf_Addr adjust)
{
  if (dyn->d_tag == DT_PLTGOT)
    {
      int i;

      for (i = 1; i < dso->ehdr.e_shnum; ++i)
	if (! strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			      dso->shdr[i].sh_name), ".got"))
	  {
	    Elf64_Addr data;

	    data = read_ube64 (dso, dso->shdr[i].sh_addr);
	    /* .got[0] points to _DYNAMIC, it needs to be adjusted.  */
	    if (data == dso->shdr[n].sh_addr && data >= start)
	      write_be64 (dso, dso->shdr[i].sh_addr, data + adjust);
	    break;
	  }
    }

  return 0;
}
