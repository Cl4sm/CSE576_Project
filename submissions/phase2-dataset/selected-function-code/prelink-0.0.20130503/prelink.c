prelink (DSO *dso, struct prelink_entry *ent)
{
  int i;
  Elf_Scn *scn;
  Elf_Data *data;
  struct prelink_info info;

  ent->pltgot = dso->info[DT_PLTGOT];

  if (! dso->info[DT_SYMTAB])
    return 0;

  if (! dso_is_rdwr (dso) && dso->ehdr.e_type == ET_DYN)
    {
      if (reopen_dso (dso, NULL, NULL))
	return 1;
    }

  i = addr_to_sec (dso, dso->info[DT_SYMTAB]);
  /* DT_SYMTAB should be found and should point to
     start of .dynsym section.  */
  if (i == -1
      || dso->info[DT_SYMTAB] != dso->shdr[i].sh_addr)
    {
      error (0, 0, "%s: Bad symtab", dso->filename);
      return 1;
    }

  memset (&info, 0, sizeof (info));
  info.ent = ent;
  info.symtab_entsize = dso->shdr[i].sh_entsize;
  info.symtab = calloc (dso->shdr[i].sh_size / dso->shdr[i].sh_entsize,
			sizeof (GElf_Sym));
  if (info.symtab == NULL)
    {
      error (0, ENOMEM, "%s: Cannot convert .dynsym section", dso->filename);
      return 1;
    }

  scn = dso->scn[i];
  data = NULL;
  while ((data = elf_getdata (scn, data)) != NULL)
    {
      int ndx, maxndx, loc;

      loc = data->d_off / info.symtab_entsize;
      maxndx = data->d_size / info.symtab_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	gelfx_getsym (dso->elf, data, ndx, info.symtab + loc + ndx);
    }
  info.symtab_start =
    adjust_new_to_old (dso, dso->shdr[i].sh_addr - dso->base);
  info.symtab_end = info.symtab_start + dso->shdr[i].sh_size;
  info.dso = dso;
  switch (prelink_get_relocations (&info))
    {
    case 0:
      goto error_out;
    case 1:
      info.resolve = resolve_ldso;
      break;
    case 2:
      info.resolve = resolve_dso;
      break;
    }

  if (dso->arch->arch_pre_prelink && dso->arch->arch_pre_prelink (dso))
    goto error_out;

  if (dso->ehdr.e_type == ET_EXEC)
    {
      if (prelink_exec (&info))
	goto error_out;
    }
  else if (prelink_dso (&info))
    goto error_out;

  for (i = 1; i < dso->ehdr.e_shnum; i++)
    {
      if (! (dso->shdr[i].sh_flags & SHF_ALLOC))
	continue;
      if (! strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			    dso->shdr[i].sh_name),
		    ".gnu.conflict"))
	continue;
      switch (dso->shdr[i].sh_type)
	{
	case SHT_REL:
	  if (prelink_rel (dso, i, &info))
	    goto error_out;
	  break;
	case SHT_RELA:
	  if (prelink_rela (dso, i, &info))
	    goto error_out;
	  break;
	}
    }

  if (dso->arch->arch_prelink && dso->arch->arch_prelink (&info))
    goto error_out;

  if (dso->arch->read_opd && dso->arch->read_opd (dso, ent))
    goto error_out;

  /* Must be last.  */
  if (dso->ehdr.e_type == ET_DYN
      && prelink_set_timestamp (&info))
    goto error_out;

  free_info (&info);
  return 0;

error_out:
  free_info (&info);
  return 1;
}
