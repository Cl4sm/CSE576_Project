adjust_symtab_section_indices (DSO *dso, int n, int old_shnum, int *old_to_new)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Sym sym;
  int changed = 0, ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getsym (dso->elf, data, ndx, &sym);
	  if (sym.st_shndx > SHN_UNDEF && sym.st_shndx < SHN_LORESERVE)
	    {
	      if (sym.st_shndx >= old_shnum
		  || old_to_new[sym.st_shndx] == -1)
		{
		  if (! sym.st_size &&
		      sym.st_info == ELF32_ST_INFO (STB_LOCAL, STT_SECTION))
		    {
		      sym.st_value = 0;
		      sym.st_shndx = SHN_UNDEF;
		      gelfx_update_sym (dso->elf, data, ndx, &sym);
		      changed = 1;
		      continue;
		    }
		  else
		    {
		      if (sym.st_shndx >= old_shnum)
			{
			  error (0, 0, "%s: Symbol section index outside of section numbers",
				 dso->filename);
			  return 1;
			}
		      error (0, 0, "%s: Section symbol points into has been removed",
			     dso->filename);
		      return 1;
		    }
		}
	      if (old_to_new[sym.st_shndx] != sym.st_shndx)
		{
		  changed = 1;
		  sym.st_shndx = old_to_new[sym.st_shndx];
		  gelfx_update_sym (dso->elf, data, ndx, &sym);
		}
	    }
	}
    }

  if (changed)
    elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);

  return 0;
}
