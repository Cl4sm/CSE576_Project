int
find_reloc_sections (DSO *dso, struct reloc_info *rinfo)
{
  int first, last, rela, i;
  GElf_Addr start, end, pltstart, pltend;

  memset (rinfo, 0, sizeof (*rinfo));

  if (dynamic_info_is_set (dso, DT_REL)
      && dynamic_info_is_set (dso, DT_RELA))
    {
      error (0, 0, "%s: Cannot prelink object with both DT_REL and DT_RELA tags",
	     dso->filename);
      return 1;
    }

  rela = dynamic_info_is_set (dso, DT_RELA);

  if (rela)
    {
      start = dso->info[DT_RELA];
      end = dso->info[DT_RELA] + dso->info[DT_RELASZ];
    }
  else
    {
      start = dso->info[DT_REL];
      end = dso->info[DT_REL] + dso->info[DT_RELSZ];
    }
  rinfo->reldyn_rela = rela;

  if (dso->info[DT_JMPREL])
    {
      pltstart = dso->info[DT_JMPREL];
      pltend = dso->info[DT_JMPREL] + dso->info[DT_PLTRELSZ];
      first = addr_to_sec (dso, pltstart);
      last = addr_to_sec (dso, pltend - 1);
      if (first == -1
	  || last == -1
	  || first != last
	  || dso->shdr[first].sh_addr != pltstart
	  || dso->shdr[first].sh_addr + dso->shdr[first].sh_size != pltend
	  || (dso->info[DT_PLTREL] != DT_REL
	      && dso->info[DT_PLTREL] != DT_RELA)
	  || dso->shdr[first].sh_type
	     != (dso->info[DT_PLTREL] == DT_RELA ? SHT_RELA : SHT_REL)
	  || strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			     dso->shdr[first].sh_name),
		     dso->info[DT_PLTREL] == DT_RELA
		     ? ".rela.plt" : ".rel.plt"))
	{
	  error (0, 0, "%s: DT_JMPREL tags don't surround .rel%s.plt section",
		 dso->filename, dso->info[DT_PLTREL] == DT_RELA ? "a" : "");
	  return 1;
	}
      rinfo->plt = first;
      rinfo->plt_rela = (dso->shdr[first].sh_type == SHT_RELA);
      if (dso->shdr[first].sh_type == SHT_REL
	  && dso->arch->need_rel_to_rela != NULL
	  && dso->arch->need_rel_to_rela (dso, first, first))
	rinfo->rel_to_rela_plt = 1;
    }
  else
    {
      pltstart = end;
      pltend = end;
    }

  if (start == 0 && end == 0)
    {
      /* No non-PLT relocations.  */
      return 0;
    }

  if (start == end)
    {
      first = 0;
      last = 0;
    }
  else
    {
      first = addr_to_sec (dso, start);
      last = addr_to_sec (dso, end - 1);

      if (first == -1
	  || last == -1
	  || dso->shdr[first].sh_addr != start
	  || dso->shdr[last].sh_addr + dso->shdr[last].sh_size != end)
	{
	  error (0, 0, "%s: DT_REL%s tags don't surround whole relocation sections",
		 dso->filename, rela ? "A" : "");
	  return 1;
	}

      for (i = first; i <= last; i++)
	if (dso->shdr[i].sh_type != (rela ? SHT_RELA : SHT_REL))
	  {
	    error (0, 0, "%s: DT_REL%s tags don't surround relocation sections of expected type",
		   dso->filename, rela ? "A" : "");
	    return 1;
	  }
    }

  if (pltstart != end && pltend != end)
    {
      error (0, 0, "%s: DT_JMPREL tag not adjacent to DT_REL%s relocations",
	     dso->filename, rela ? "A" : "");
      return 1;
    }

  if (pltstart == start && pltend == end)
    {
      /* No non-PLT relocations.  */
      rinfo->overlap = 1;
      return 0;
    }

  if (pltstart != end && pltend == end)
    {
      rinfo->overlap = 1;
      --last;
    }

  rinfo->first = first;
  rinfo->last = last;
  if (! rela
      && first
      && dso->arch->need_rel_to_rela != NULL
      && dso->arch->need_rel_to_rela (dso, first, last))
    rinfo->rel_to_rela = 1;
  return 0;
}
