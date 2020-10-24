adjust_dynamic (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Dyn dyn;
  int ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getdyn (dso->elf, data, ndx, &dyn);
	  if (dso->arch->adjust_dyn (dso, n, &dyn, start, adjust) == 0)
	    switch (dyn.d_tag)
	      {
	      case DT_REL:
	      case DT_RELA:
		/* On some arches DT_REL* may be 0 indicating no relocations
		   (if DT_REL*SZ is also 0).  Don't adjust it in that case.  */
		if (dyn.d_un.d_ptr && dyn.d_un.d_ptr >= start)
		  {
		    dyn.d_un.d_ptr += adjust;
		    gelfx_update_dyn (dso->elf, data, ndx, &dyn);
		  }
		break;
	      default:
		if (dyn.d_tag < DT_ADDRRNGLO || dyn.d_tag > DT_ADDRRNGHI)
		  break;
		/* FALLTHROUGH */
	      case DT_INIT:
	      case DT_FINI:
	      case DT_HASH:
	      case DT_STRTAB:
	      case DT_SYMTAB:
	      case DT_JMPREL:
	      case DT_INIT_ARRAY:
	      case DT_FINI_ARRAY:
	      case DT_PREINIT_ARRAY:
	      case DT_VERDEF:
	      case DT_VERNEED:
	      case DT_VERSYM:
	      case DT_PLTGOT:
		if (dyn.d_un.d_ptr >= start)
		  {
		    dyn.d_un.d_ptr += adjust;
		    gelfx_update_dyn (dso->elf, data, ndx, &dyn);
		  }
		break;
	      }
	  else
	    gelfx_update_dyn (dso->elf, data, ndx, &dyn);
	}
    }

  elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);

  /* Update the cached dynamic info as well.  */
  read_dynamic (dso);
  return 0;
}
