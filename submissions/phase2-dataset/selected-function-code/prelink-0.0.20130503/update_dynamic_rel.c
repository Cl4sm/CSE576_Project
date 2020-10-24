int
update_dynamic_rel (DSO *dso, struct reloc_info *rinfo)
{
  GElf_Dyn *info[DT_NUM], *info_DT_RELCOUNT, *info_DT_RELACOUNT;
  GElf_Dyn *dynamic = NULL;
  int rel = rinfo->first, plt = rinfo->plt, overlap = rinfo->overlap;
  int dynsec, count = 0, loc;
  Elf_Data *data;
  Elf_Scn *scn = NULL;

  memset (&info, 0, sizeof (info));
  info_DT_RELCOUNT = NULL;
  info_DT_RELACOUNT = NULL;
  for (dynsec = 0; dynsec < dso->ehdr.e_shnum; dynsec++)
    if (dso->shdr[dynsec].sh_type == SHT_DYNAMIC)
      {
	scn = dso->scn[dynsec];
	dynamic = alloca (dso->shdr[dynsec].sh_size
			  / dso->shdr[dynsec].sh_entsize * sizeof (GElf_Dyn));
	loc = 0;
	data = NULL;
	while ((data = elf_getdata (scn, data)) != NULL)
	  {
	    int ndx, maxndx;

	    maxndx = data->d_size / dso->shdr[dynsec].sh_entsize;
	    for (ndx = 0; ndx < maxndx; ++ndx, ++loc)
	      {
		gelfx_getdyn (dso->elf, data, ndx, dynamic + loc);
		if (dynamic[loc].d_tag == DT_NULL)
		  break;
		else if ((GElf_Xword) dynamic[loc].d_tag < DT_NUM)
		  info[dynamic[loc].d_tag] = dynamic + loc;
		else if (dynamic[loc].d_tag == DT_RELCOUNT)
		  info_DT_RELCOUNT = dynamic + loc;
		else if (dynamic[loc].d_tag == DT_RELACOUNT)
		  info_DT_RELACOUNT = dynamic + loc;
	      }
	    if (ndx < maxndx)
	      break;
	  }
	count = loc;
	break;
      }

  if (rel && plt && overlap)
    {
      if (dso->shdr[rel].sh_type != dso->shdr[plt].sh_type)
	overlap = 0;
    }

  if (rel || (plt && overlap))
    {
      int dt_RELENT, dt_REL, dt_RELSZ;

      if (rinfo->reldyn_rela)
	{
	  dt_RELENT = DT_RELAENT;
	  dt_REL = DT_RELA;
	  dt_RELSZ = DT_RELASZ;
	}
      else
	{
	  dt_RELENT = DT_RELENT;
	  dt_REL = DT_REL;
	  dt_RELSZ = DT_RELSZ;
	}

      assert (dso->info[dt_RELENT]
	      == gelf_fsize (dso->elf, rinfo->reldyn_rela
			     ? ELF_T_RELA : ELF_T_REL, 1, EV_CURRENT));
      assert (dso->info[dt_REL] != 0);
      assert (dso->info[dt_RELSZ] != 0);

      info[dt_REL]->d_un.d_ptr = dso->shdr[rel ?: plt].sh_addr;
      if (plt && overlap)
	info[dt_RELSZ]->d_un.d_val =
	  dso->shdr[plt].sh_addr + dso->shdr[plt].sh_size;
      else
	info[dt_RELSZ]->d_un.d_val =
	  dso->shdr[rinfo->last].sh_addr + dso->shdr[rinfo->last].sh_size;
      info[dt_RELSZ]->d_un.d_val -= info[dt_REL]->d_un.d_ptr;

      if (!rinfo->reldyn_rela && dso->shdr[rel ?: plt].sh_type == SHT_RELA)
	{
	  info[DT_RELENT]->d_un.d_val =
	    gelf_fsize (dso->elf, ELF_T_RELA, 1, EV_CURRENT);
	  info[DT_REL]->d_tag = DT_RELA;
	  info[DT_RELSZ]->d_tag = DT_RELASZ;
	  info[DT_RELENT]->d_tag = DT_RELAENT;
	  if (info_DT_RELCOUNT)
	    info_DT_RELCOUNT->d_tag = DT_RELACOUNT;
	}
      else if (rinfo->reldyn_rela && dso->shdr[rel ?: plt].sh_type == SHT_REL)
	{
	  info[DT_RELAENT]->d_un.d_val =
	    gelf_fsize (dso->elf, ELF_T_REL, 1, EV_CURRENT);
	  info[DT_RELA]->d_tag = DT_REL;
	  info[DT_RELASZ]->d_tag = DT_RELSZ;
	  info[DT_RELAENT]->d_tag = DT_RELENT;
	  if (info_DT_RELACOUNT)
	    info_DT_RELACOUNT->d_tag = DT_RELCOUNT;
	}
    }

  if (plt)
    {
      assert (dso->info[DT_JMPREL] != 0);
      assert (dso->info[DT_PLTREL] == rinfo->plt_rela ? DT_RELA : DT_REL);

      info[DT_JMPREL]->d_un.d_ptr = dso->shdr[plt].sh_addr;
      if (!rinfo->plt_rela && dso->shdr[plt].sh_type == SHT_RELA)
	{
	  info[DT_PLTREL]->d_un.d_val = DT_RELA;
	  info[DT_PLTRELSZ]->d_un.d_val = dso->shdr[plt].sh_size;
	}
      else if (rinfo->plt_rela && dso->shdr[plt].sh_type == SHT_REL)
	{
	  info[DT_PLTREL]->d_un.d_val = DT_REL;
	  info[DT_PLTRELSZ]->d_un.d_val = dso->shdr[plt].sh_size;
	}

      if (!rel && !overlap)
	{
	  int dt_REL = rinfo->reldyn_rela ? DT_RELA : DT_REL;

	  if (info[dt_REL] && info[dt_REL]->d_un.d_ptr)
	    info[dt_REL]->d_un.d_ptr = info[DT_JMPREL]->d_un.d_ptr;
	}
    }

  loc = 0;
  data = NULL;
  while ((data = elf_getdata (scn, data)) != NULL)
    {
      int ndx, maxndx;

      maxndx = data->d_size / dso->shdr[dynsec].sh_entsize;
      for (ndx = 0; ndx < maxndx && loc < count; ++ndx, ++loc)
	if ((GElf_Xword) dynamic[loc].d_tag < DT_NUM
	    || dynamic[loc].d_tag == DT_RELCOUNT
	    || dynamic[loc].d_tag == DT_RELACOUNT)
	  gelfx_update_dyn (dso->elf, data, ndx, dynamic + loc);
      if (ndx < maxndx)
	break;
    }

  read_dynamic (dso);
  return 0;
}
