static int
set_stt_section_values (DSO *dso, int n)
{
  Elf_Data *data;
  Elf_Scn *scn = dso->scn[n];
  GElf_Sym sym;
  int ndx, maxndx, sec;
  char seen[dso->ehdr.e_shnum];

  memset (seen, 0, dso->ehdr.e_shnum);
  data = elf_getdata (scn, NULL);
  assert (data != NULL);
  assert (elf_getdata (scn, data) == NULL);
  assert (data->d_off == 0);

  maxndx = data->d_size / dso->shdr[n].sh_entsize;
  gelfx_getsym (dso->elf, data, 0, &sym);
  if (sym.st_info != ELF32_ST_INFO (STB_LOCAL, STT_NOTYPE)
      || sym.st_size != 0 || sym.st_other != 0
      || sym.st_value != 0 || sym.st_shndx != SHN_UNDEF
      || sym.st_name != 0)
    return 0;

  for (ndx = 1; ndx < maxndx; ++ndx)
    {
      gelfx_getsym (dso->elf, data, ndx, &sym);
      if (sym.st_info == ELF32_ST_INFO (STB_LOCAL, STT_SECTION)
	  && sym.st_size == 0 && sym.st_other == 0
	  && sym.st_name == 0)
	{
	  if (sym.st_shndx > SHN_UNDEF && sym.st_shndx < SHN_LORESERVE)
	    {
	      seen[sym.st_shndx] = 1;
	      sym.st_value = dso->shdr[sym.st_shndx].sh_addr;
	      gelfx_update_sym (dso->elf, data, ndx, &sym);
	    }
	}
      else
	break;
    }

  for (ndx = 1, sec = 1; ndx < maxndx; ++ndx)
    {
      gelfx_getsym (dso->elf, data, ndx, &sym);
      if (sym.st_info == ELF32_ST_INFO (STB_LOCAL, STT_SECTION)
	  && sym.st_size == 0 && sym.st_other == 0
	  && sym.st_name == 0)
	{
	  if (sym.st_shndx == SHN_UNDEF)
	    {
	      while (sec < dso->ehdr.e_shnum && seen[sec])
		++sec;

	      if (sec >= dso->ehdr.e_shnum)
		sym.st_value = 0;
	      else
		sym.st_value = dso->shdr[sec].sh_addr;
	      sym.st_shndx = sec++;
	      gelfx_update_sym (dso->elf, data, ndx, &sym);
	    }
	}
      else
	break;
    }

  return 0;
}
