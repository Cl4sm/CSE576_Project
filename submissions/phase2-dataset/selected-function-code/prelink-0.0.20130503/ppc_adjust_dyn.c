ppc_adjust_dyn (DSO *dso, int n, GElf_Dyn *dyn, GElf_Addr start,
		GElf_Addr adjust)
{
  if (dyn->d_tag == DT_PPC_GOT)
    {
      Elf32_Addr data;

      data = read_ube32 (dso, dyn->d_un.d_ptr);
      /* DT_PPC_GOT[0] points to _DYNAMIC, it needs to be adjusted.  */
      if (data == dso->shdr[n].sh_addr && data >= start)
	write_be32 (dso, dyn->d_un.d_ptr, data + adjust);

      data = read_ube32 (dso, dyn->d_un.d_ptr + 4);
      /* DT_PPC_GOT[1] points to .glink in prelinked libs.  */
      if (data && data >= start)
	write_be32 (dso, dyn->d_un.d_ptr + 4, data + adjust);

      if (dyn->d_un.d_ptr >= start)
	{
	  dyn->d_un.d_ptr += adjust;
	  return 1;
	}
    }
  else if (dyn->d_tag == DT_PLTGOT
	   && !dynamic_info_is_set (dso, DT_PPC_GOT_BIT))
    {
      int i;

      for (i = 1; i < dso->ehdr.e_shnum; ++i)
	if (! strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			      dso->shdr[i].sh_name), ".got")
	    && dso->shdr[i].sh_size >= 16)
	  {
	    Elf32_Addr data, addr;
	    int step;

	    /* If .got[1] points to _DYNAMIC, it needs to be adjusted.
	       Other possible locations of the .got header are at the
	       end of .got or around offset 32768 in it.  */
	    for (addr = dso->shdr[i].sh_addr, step = 0; step < 18; step++)
	      {
		if (read_ube32 (dso, addr) == 0x4e800021
		    && (data = read_ube32 (dso, addr + 4))
		       == dso->shdr[n].sh_addr
		    && data >= start
		    && read_ube32 (dso, addr + 8) == 0
		    && read_ube32 (dso, addr + 12) == 0)
		  {
		    /* Probably should use here a check that neither of
		       the 4 addresses contains a dynamic relocation against
		       it.  */
		    write_be32 (dso, addr + 4, data + adjust);
		    break;
		  }
		if (step == 0)
		  addr = dso->shdr[i].sh_addr + dso->shdr[i].sh_size - 16;
		else if (step == 1)
		  {
		    if (dso->shdr[i].sh_size >= 32768 - 32)
		      addr = dso->shdr[i].sh_addr + 32768 - 32 - 16;
		    else
		      break;
		  }
		else
		  {
		    addr += 4;
		    if (addr + 16
			> dso->shdr[i].sh_addr + dso->shdr[i].sh_size)
		      break;
		  }
	      }
	    break;
	  }
    }

  return 0;
}
