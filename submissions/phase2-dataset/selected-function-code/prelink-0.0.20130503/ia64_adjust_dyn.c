static int
ia64_adjust_dyn (DSO *dso, int n, GElf_Dyn *dyn, GElf_Addr start,
		 GElf_Addr adjust)
{
  if (dyn->d_tag == DT_IA_64_PLT_RESERVE)
    {
      int sec = addr_to_sec (dso, dyn->d_un.d_ptr);
      Elf64_Addr data;

      if (sec != -1)
	{
	  data = read_ule64 (dso, dyn->d_un.d_ptr + 8);

	  /* If .got[1] points to .plt + 0x30, it needs to be adjusted.  */
	  if (data && data >= start)
	    {
	      int i;

	      for (i = 1; i < dso->ehdr.e_shnum; i++)
		if (data == dso->shdr[i].sh_addr + 0x30
		    && dso->shdr[i].sh_type == SHT_PROGBITS
		    && strcmp (strptr (dso, dso->ehdr.e_shstrndx,
				       dso->shdr[i].sh_name), ".plt") == 0)
		  {
		    write_le64 (dso, dyn->d_un.d_ptr + 8, data + adjust);
		    break;
		  }
	    }
	}

      if (dyn->d_un.d_ptr >= start)
	dyn->d_un.d_ptr += adjust;
      return 1;
    }

  return 0;
}
