prelink_build_conflicts (struct prelink_info *info)
{
  int i, ndeps = info->ent->ndepends + 1;
  struct prelink_entry *ent;
  int ret = 0;
  DSO *dso;
  struct copy_relocs cr;

  info->dsos = alloca (sizeof (struct DSO *) * ndeps);
  memset (info->dsos, 0, sizeof (struct DSO *) * ndeps);
  memset (&cr, 0, sizeof (cr));
  info->dsos[0] = info->dso;
  for (i = 1; i < ndeps; ++i)
    {
      ent = info->ent->depends[i - 1];
      if ((dso = open_dso (ent->filename)) == NULL)
	goto error_out;
      info->dsos[i] = dso;
      /* Now check that the DSO matches what we recorded about it.  */
      if (ent->timestamp != dso->info_DT_GNU_PRELINKED
	  || ent->checksum != dso->info_DT_CHECKSUM
	  || ent->base != dso->base)
	{
	  error (0, 0, "%s: Library %s has changed since it has been prelinked",
		 info->dso->filename, ent->filename);
	  goto error_out;
	}
    }

  for (i = 0; i < ndeps; ++i)
    {
      int j, sec, first_conflict, maxidx;
      struct prelink_conflict *conflict;

      dso = info->dsos[i];
      ent = i ? info->ent->depends[i - 1] : info->ent;

      /* Verify .gnu.liblist sections of all dependent libraries.  */
      if (i && ent->ndepends > 0)
	{
	  const char *name;
	  int nliblist;
	  Elf32_Lib *liblist;
	  Elf_Scn *scn;
	  Elf_Data *data;

	  for (j = 1; j < dso->ehdr.e_shnum; ++j)
	    if (dso->shdr[j].sh_type == SHT_GNU_LIBLIST
		&& (name = strptr (dso, dso->ehdr.e_shstrndx,
				   dso->shdr[j].sh_name))
		&& ! strcmp (name, ".gnu.liblist")
		&& (dso->shdr[j].sh_size % sizeof (Elf32_Lib)) == 0)
	      break;

	  if (j == dso->ehdr.e_shnum)
	    {
	      error (0, 0, "%s: Library %s has dependencies, but doesn't contain .gnu.liblist section",
		     info->dso->filename, ent->filename);
	      goto error_out;
	    }

	  nliblist = dso->shdr[j].sh_size / sizeof (Elf32_Lib);
	  scn = dso->scn[j];
	  data = elf_getdata (scn, NULL);
	  if (data == NULL || elf_getdata (scn, data)
	      || data->d_buf == NULL || data->d_off
	      || data->d_size != dso->shdr[j].sh_size)
	    {
	      error (0, 0, "%s: Could not read .gnu.liblist section from %s",
		     info->dso->filename, ent->filename);
	      goto error_out;
	    }

	  if (nliblist != ent->ndepends)
	    {
	      error (0, 0, "%s: Library %s has different number of libs in .gnu.liblist than expected",
		     info->dso->filename, ent->filename);
	      goto error_out;
	    }
	  liblist = (Elf32_Lib *) data->d_buf;
	  for (j = 0; j < nliblist; ++j)
	    if (liblist[j].l_time_stamp != ent->depends[j]->timestamp
		|| liblist[j].l_checksum != ent->depends[j]->checksum)
	      {
		error (0, 0, "%s: .gnu.liblist in library %s is inconsistent with recorded dependencies",
		       info->dso->filename, ent->filename);
		goto error_out;
	      }

	  /* Extra check, maybe not needed.  */
	  for (j = 0; j < nliblist; ++j)
	    {
	      int k;
	      for (k = 0; k < info->ent->ndepends; ++k)
		if (liblist[j].l_time_stamp == info->ent->depends[k]->timestamp
		    && liblist[j].l_checksum == info->ent->depends[k]->checksum)
		  break;

	      if (k == info->ent->ndepends)
		abort ();
	    }
	}

      info->curconflicts = &info->conflicts[i];
      info->curtls = info->tls[i].modid ? info->tls + i : NULL;
      first_conflict = info->conflict_rela_size;
      sec = addr_to_sec (dso, dso->info[DT_SYMTAB]);
      /* DT_SYMTAB should be found and should point to
	 start of .dynsym section.  */
      if (sec == -1 || dso->info[DT_SYMTAB] != dso->shdr[sec].sh_addr)
	{
	  error (0, 0, "Bad symtab");
	  goto error_out;
	}
      info->symtab_start = dso->shdr[sec].sh_addr - dso->base;
      info->symtab_end = info->symtab_start + dso->shdr[sec].sh_size;
      for (j = 0; j < dso->ehdr.e_shnum; ++j)
	{
	  if (! (dso->shdr[j].sh_flags & SHF_ALLOC))
	    continue;
	  switch (dso->shdr[j].sh_type)
	    {
	    case SHT_REL:
	      if (i == 0
		  && strcmp (strptr (dso, dso->ehdr.e_shstrndx,
				     dso->shdr[j].sh_name),
			     ".gnu.conflict") == 0)
		break;
	      if (prelink_conflict_rel (dso, j, info))
		goto error_out;
	      break;
	    case SHT_RELA:
	      if (i == 0
		  && strcmp (strptr (dso, dso->ehdr.e_shstrndx,
				     dso->shdr[j].sh_name),
			     ".gnu.conflict") == 0)
		break;
	      if (prelink_conflict_rela (dso, j, info))
		goto error_out;
	      break;
	    }
	}

      if (dso->arch->arch_prelink_conflict
	  && dso->arch->arch_prelink_conflict (dso, info))
	goto error_out;

      maxidx = 1;
      if (info->curconflicts->hash != &info->curconflicts->first)
	maxidx = 251;
      for (j = 0; j < maxidx; j++)
	for (conflict = info->curconflicts->hash[j]; conflict;
	     conflict = conflict->next)
	  if (! conflict->used && (i || conflict->ifunc))
	    {
	      error (0, 0, "%s: Conflict %08llx not found in any relocation",
		     dso->filename, (unsigned long long) conflict->symoff);
	      ret = 1;
	    }

      /* Record library's position in search scope into R_SYM field.  */
      for (j = first_conflict; j < info->conflict_rela_size; ++j)
	info->conflict_rela[j].r_info
	  = GELF_R_INFO (i, GELF_R_TYPE (info->conflict_rela[j].r_info));

      if (dynamic_info_is_set (dso, DT_TEXTREL)
	  && info->conflict_rela_size > first_conflict)
	{
	  /* We allow prelinking against non-PIC libraries, as long as
	     no conflict is against read-only segment.  */
	  int k;

	  for (j = first_conflict; j < info->conflict_rela_size; ++j)
	    for (k = 0; k < dso->ehdr.e_phnum; ++k)
	      if (dso->phdr[k].p_type == PT_LOAD
		  && (dso->phdr[k].p_flags & PF_W) == 0
		  && dso->phdr[k].p_vaddr
		     <= info->conflict_rela[j].r_offset
		  && dso->phdr[k].p_vaddr + dso->phdr[k].p_memsz
		     > info->conflict_rela[j].r_offset)
		{
		  error (0, 0, "%s: Cannot prelink against non-PIC shared library %s",
			 info->dso->filename, dso->filename);
		  goto error_out;
		}
	}
    }

  dso = info->dso;
  for (i = 0; i < dso->ehdr.e_shnum; ++i)
    {
      if (! (dso->shdr[i].sh_flags & SHF_ALLOC))
	continue;
      switch (dso->shdr[i].sh_type)
	{
	case SHT_REL:
	  if (prelink_find_copy_rel (dso, i, &cr))
	    goto error_out;
	  break;
	case SHT_RELA:
	  if (prelink_find_copy_rela (dso, i, &cr))
	    goto error_out;
	  break;
	}
    }

  if (cr.count)
    {
      int bss1, bss2, firstbss2 = 0;
      const char *name;

      qsort (cr.rela, cr.count, sizeof (GElf_Rela), rela_cmp);
      bss1 = addr_to_sec (dso, cr.rela[0].r_offset);
      bss2 = addr_to_sec (dso, cr.rela[cr.count - 1].r_offset);
      if (bss1 != bss2)
	{
	  for (i = 1; i < cr.count; ++i)
	    if (cr.rela[i].r_offset
		>= dso->shdr[bss1].sh_addr + dso->shdr[bss1].sh_size)
	      break;
	  if (cr.rela[i].r_offset < dso->shdr[bss2].sh_addr)
	    {
	      error (0, 0, "%s: Copy relocs against 3 or more sections",
		     dso->filename);
	      goto error_out;
	    }
	  firstbss2 = i;
	  info->sdynbss_size = cr.rela[i - 1].r_offset - cr.rela[0].r_offset;
	  info->sdynbss_size += cr.rela[i - 1].r_addend;
	  info->sdynbss = calloc (info->sdynbss_size, 1);
	  info->sdynbss_base = cr.rela[0].r_offset;
	  if (info->sdynbss == NULL)
	    {
	      error (0, ENOMEM, "%s: Cannot build .sdynbss", dso->filename);
	      goto error_out;
	    }

	  for (i = 0; i < dso->ehdr.e_phnum; ++i)
	    if (dso->phdr[i].p_type == PT_LOAD
		&& dso->shdr[bss1].sh_addr >= dso->phdr[i].p_vaddr
		&& dso->shdr[bss1].sh_addr
		   < dso->phdr[i].p_vaddr + dso->phdr[i].p_memsz)
	      break;
	  if (i == dso->ehdr.e_phnum
	      || dso->shdr[bss2].sh_addr + dso->shdr[bss2].sh_size
		 > dso->phdr[i].p_vaddr + dso->phdr[i].p_memsz)
	    {
	      error (0, 0, "%s: Copy relocs against more than one segment",
		     dso->filename);
	      goto error_out;
	    }
	}

      info->dynbss_size = cr.rela[cr.count - 1].r_offset
			  - cr.rela[firstbss2].r_offset;
      info->dynbss_size += cr.rela[cr.count - 1].r_addend;
      info->dynbss = calloc (info->dynbss_size, 1);
      info->dynbss_base = cr.rela[firstbss2].r_offset;
      if (info->dynbss == NULL)
	{
	  error (0, ENOMEM, "%s: Cannot build .dynbss", dso->filename);
	  goto error_out;
	}

      /* emacs apparently has .rel.bss relocations against .data section,
	 crap.  */
      if (dso->shdr[bss1].sh_type != SHT_NOBITS
	  && strcmp (name = strptr (dso, dso->ehdr.e_shstrndx,
				    dso->shdr[bss1].sh_name),
		     ".dynbss") != 0
	  && strcmp (name, ".sdynbss") != 0)
	{
	  error (0, 0, "%s: COPY relocations don't point into .bss or .sbss section",
		 dso->filename);
	  goto error_out;
	}
      if (bss1 != bss2
	  && dso->shdr[bss2].sh_type != SHT_NOBITS
	  && strcmp (name = strptr (dso, dso->ehdr.e_shstrndx,
				    dso->shdr[bss2].sh_name),
		     ".dynbss") != 0
	  && strcmp (name, ".sdynbss") != 0)
	{
	  error (0, 0, "%s: COPY relocations don't point into .bss or .sbss section",
		 dso->filename);
	  goto error_out;
	}

      for (i = 0; i < cr.count; ++i)
	{
	  struct prelink_symbol *s;
	  DSO *ndso = NULL;
	  int j, reloc_class;

	  reloc_class
	    = dso->arch->reloc_class (GELF_R_TYPE (cr.rela[i].r_info));

	  assert (reloc_class != RTYPE_CLASS_TLS);

	  for (s = & info->symbols[GELF_R_SYM (cr.rela[i].r_info)]; s;
	       s = s->next)
	    if (s->reloc_class == reloc_class)
	      break;

	  if (s == NULL || s->u.ent == NULL)
	    {
	      error (0, 0, "%s: Could not find symbol copy reloc is against",
		     dso->filename);
	      goto error_out;
	    }

	  for (j = 1; j < ndeps; ++j)
	    if (info->ent->depends[j - 1] == s->u.ent)
	      {
		ndso = info->dsos[j];
		break;
	      }

	  assert (j < ndeps);
	  if (i < firstbss2)
	    j = get_relocated_mem (info, ndso, s->u.ent->base + s->value,
				   info->sdynbss + cr.rela[i].r_offset
				   - info->sdynbss_base, cr.rela[i].r_addend,
				   cr.rela[i].r_offset);
	  else
	    j = get_relocated_mem (info, ndso, s->u.ent->base + s->value,
				   info->dynbss + cr.rela[i].r_offset
				   - info->dynbss_base, cr.rela[i].r_addend,
				   cr.rela[i].r_offset);

	  switch (j)
	    {
	    case 1:
	      error (0, 0, "%s: Could not find variable copy reloc is against",
		     dso->filename);
	      goto error_out;
	    case 2:
	      error (0, 0, "%s: Conflict partly overlaps with %08llx-%08llx area",
		     dso->filename,
		     (long long) cr.rela[i].r_offset,
		     (long long) (cr.rela[i].r_offset + cr.rela[i].r_addend));
	      goto error_out;
	    }
	}
    }

  if (info->conflict_rela_size)
    {
      qsort (info->conflict_rela, info->conflict_rela_size, sizeof (GElf_Rela),
	     conflict_rela_cmp);

      /* Now make sure all conflict RELA's are against absolute 0 symbol.  */
      for (i = 0; i < info->conflict_rela_size; ++i)
	info->conflict_rela[i].r_info
	  = GELF_R_INFO (0, GELF_R_TYPE (info->conflict_rela[i].r_info));

      if (enable_cxx_optimizations && remove_redundant_cxx_conflicts (info))
	goto error_out;
    }

  for (i = 1; i < ndeps; ++i)
    if (info->dsos[i])
      close_dso (info->dsos[i]);

  info->dsos = NULL;
  free (cr.rela);
  return ret;

error_out:
  free (cr.rela);
  free (info->dynbss);
  free (info->sdynbss);
  info->dynbss = NULL;
  info->sdynbss = NULL;
  for (i = 1; i < ndeps; ++i)
    if (info->dsos[i])
      close_dso (info->dsos[i]);
  return 1;
}
