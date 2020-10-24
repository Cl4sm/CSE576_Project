read_dynamic (DSO *dso)
{
  int i;

  memset (dso->info, 0, sizeof(dso->info));
  dso->info_set_mask = 0;
  for (i = 0; i < dso->ehdr.e_shnum; i++)
    if (dso->shdr[i].sh_type == SHT_DYNAMIC)
      {
	Elf_Data *data = NULL;
	Elf_Scn *scn = dso->scn[i];
	GElf_Dyn dyn;

	dso->dynamic = i;
	while ((data = elf_getdata (scn, data)) != NULL)
	  {
	    int ndx, maxndx;

	    maxndx = data->d_size / dso->shdr[i].sh_entsize;
	    for (ndx = 0; ndx < maxndx; ++ndx)
	      {
		gelfx_getdyn (dso->elf, data, ndx, &dyn);
		if (dyn.d_tag == DT_NULL)
		  break;
		else if ((GElf_Xword) dyn.d_tag < DT_NUM)
		  {
		    dso->info[dyn.d_tag] = dyn.d_un.d_val;
		    if (dyn.d_tag < 50)
		      dso->info_set_mask |= (1ULL << dyn.d_tag);
		  }
		else if (dyn.d_tag == DT_CHECKSUM)
		  {
		    dso->info_DT_CHECKSUM = dyn.d_un.d_val;
		    dso->info_set_mask |= (1ULL << DT_CHECKSUM_BIT);
		  }
		else if (dyn.d_tag == DT_GNU_PRELINKED)
		  {
		    dso->info_DT_GNU_PRELINKED = dyn.d_un.d_val;
		    dso->info_set_mask |= (1ULL << DT_GNU_PRELINKED_BIT);
		  }
		else if (dyn.d_tag == DT_VERDEF)
		  {
		    dso->info_DT_VERDEF = dyn.d_un.d_val;
		    dso->info_set_mask |= (1ULL << DT_VERDEF_BIT);
		  }
		else if (dyn.d_tag == DT_VERNEED)
		  {
		    dso->info_DT_VERNEED = dyn.d_un.d_val;
		    dso->info_set_mask |= (1ULL << DT_VERNEED_BIT);
		  }
		else if (dyn.d_tag == DT_VERSYM)
		  {
		    dso->info_DT_VERSYM = dyn.d_un.d_val;
		    dso->info_set_mask |= (1ULL << DT_VERSYM_BIT);
		  }
		else if (dyn.d_tag == DT_FILTER)
		  dso->info_set_mask |= (1ULL << DT_FILTER_BIT);
		else if (dyn.d_tag == DT_AUXILIARY)
		  dso->info_set_mask |= (1ULL << DT_AUXILIARY_BIT);
		else if (dyn.d_tag == DT_LOPROC)
		  dso->info_set_mask |= (1ULL << DT_LOPROC_BIT);
		else if (dyn.d_tag == DT_GNU_HASH)
		  {
		    dso->info_DT_GNU_HASH = dyn.d_un.d_val;
		    dso->info_set_mask |= (1ULL << DT_GNU_HASH_BIT);
		  }
		if (dso->ehdr.e_machine == EM_MIPS)
		  {
		    if (dyn.d_tag == DT_MIPS_LOCAL_GOTNO)
		      dso->info_DT_MIPS_LOCAL_GOTNO = dyn.d_un.d_val;
		    else if (dyn.d_tag == DT_MIPS_GOTSYM)
		      dso->info_DT_MIPS_GOTSYM = dyn.d_un.d_val;
		    else if (dyn.d_tag == DT_MIPS_SYMTABNO)
		      dso->info_DT_MIPS_SYMTABNO = dyn.d_un.d_val;
		  }
	      }
	    if (ndx < maxndx)
	      break;
	  }
      }
}
