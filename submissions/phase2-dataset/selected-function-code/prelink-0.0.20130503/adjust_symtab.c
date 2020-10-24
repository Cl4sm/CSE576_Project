static int
adjust_symtab (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Sym sym;
  int ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getsym (dso->elf, data, ndx, &sym);
	  if (adjust_symbol_p (dso, &sym) && sym.st_value >= start)
	    {
	      sym.st_value += adjust;
	      gelfx_update_sym (dso->elf, data, ndx, &sym);
	    }
	}
    }

  elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
  return 0;
}
