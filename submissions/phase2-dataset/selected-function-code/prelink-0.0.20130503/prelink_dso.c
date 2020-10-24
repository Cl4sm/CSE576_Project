static int
prelink_dso (struct prelink_info *info)
{
  int liblist = 0, libstr = 0, nobits_plt = 0;
  int i, ndeps = info->ent->ndepends + 1;
  DSO *dso = info->dso;
  Elf32_Lib *list = NULL;
  Elf_Scn *scn;
  Elf_Data *data;
  GElf_Addr oldsize, oldoffset;
  size_t strsize;

  if (dso->ehdr.e_type != ET_DYN)
    return 0;

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    {
      const char *name
	= strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[i].sh_name);
      if (! strcmp (name, ".gnu.liblist"))
	liblist = i;
      else if (! strcmp (name, ".gnu.libstr"))
	libstr = i;
      else if (! strcmp (name, ".plt") && dso->shdr[i].sh_type == SHT_NOBITS)
	nobits_plt = i;
#if 0
      else if (dso->arch->create_opd && ! strcmp (name, ".opd"))
	opd = i;
#endif
    }

  if (nobits_plt)
    {
      int j, first;
      GElf_Addr adj, last_offset;

      for (i = 0; i < dso->ehdr.e_phnum; ++i)
	if (dso->phdr[i].p_type == PT_LOAD
	    && dso->phdr[i].p_vaddr <= dso->shdr[nobits_plt].sh_addr
	    && dso->phdr[i].p_vaddr + dso->phdr[i].p_memsz
	       >= dso->shdr[nobits_plt].sh_addr
		  + dso->shdr[nobits_plt].sh_size)
	  break;

      if (i == dso->ehdr.e_phnum)
	{
	  error (0, 0, "%s: .plt section not contained within a segment",
		 dso->filename);
	  return 1;
	}

      for (j = i + 1; j < dso->ehdr.e_phnum; ++j)
	if (dso->phdr[j].p_type == PT_LOAD)
	  {
	    error (0, 0, "%s: library's NOBITS .plt section not in loadable last segment",
		   dso->filename);
	    return 1;
	  }

      for (j = nobits_plt - 1; j > 0; --j)
	if (dso->shdr[j].sh_addr < dso->phdr[i].p_vaddr
	    || dso->shdr[j].sh_type != SHT_NOBITS)
	  break;
      first = j + 1;

      for (j = first; j <= nobits_plt; ++j)
	{
	  Elf_Data *data = elf_getdata (dso->scn[j], NULL);

	  assert (data->d_buf == NULL);
	  assert (data->d_size == dso->shdr[j].sh_size);
	  if (data->d_size)
	    {
	      data->d_buf = calloc (data->d_size, 1);
	      if (data->d_buf == NULL)
		{
		  error (0, ENOMEM, "%s: Could not convert NOBITS section into PROGBITS",
			 dso->filename);
		  return 1;
		}
	    }
	  data->d_type = ELF_T_BYTE;
	  dso->shdr[j].sh_type = SHT_PROGBITS;
	  dso->shdr[j].sh_offset = dso->phdr[i].p_offset + dso->shdr[j].sh_addr
				   - dso->phdr[i].p_vaddr;
	}

      adj = dso->shdr[nobits_plt].sh_offset + dso->shdr[nobits_plt].sh_size
	    - dso->phdr[i].p_offset;
      assert (adj <= dso->phdr[i].p_memsz);
      if (adj > dso->phdr[i].p_filesz)
	{
	  adj -= dso->phdr[i].p_filesz;
	  dso->phdr[i].p_filesz += adj;
	  if (adjust_dso_nonalloc (dso, nobits_plt + 1,
				   dso->shdr[first].sh_offset, adj))
	    return 1;
	}

      last_offset = dso->shdr[nobits_plt].sh_offset
		    + dso->shdr[nobits_plt].sh_size;
      for (j = nobits_plt + 1; j < dso->ehdr.e_shnum; ++j)
	if (!(dso->shdr[j].sh_flags & (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR)))
	  break;
	else
	  {
	    last_offset += dso->shdr[j].sh_addralign - 1;
	    last_offset &= ~(dso->shdr[j].sh_addralign - 1);
	    if (last_offset > dso->phdr[i].p_offset + dso->phdr[i].p_filesz)
	      last_offset = dso->phdr[i].p_offset + dso->phdr[i].p_filesz;
	    dso->shdr[j].sh_offset = last_offset;
	  }
    }

  if (ndeps <= 1)
    return 0;

  assert (liblist != 0);
  assert (libstr != 0);

  list = calloc (ndeps - 1, sizeof (Elf32_Lib));
  if (list == NULL)
    {
      error (0, ENOMEM, "%s: Cannot build .gnu.liblist section",
	     dso->filename);
      goto error_out;
    }

  strsize = 1;
  for (i = 0; i < ndeps - 1; ++i)
    {
      struct prelink_entry *ent = info->ent->depends[i];

      strsize += strlen (info->sonames[i + 1]) + 1;
      list[i].l_time_stamp = ent->timestamp;
      list[i].l_checksum = ent->checksum;
    }

  scn = dso->scn[libstr];
  data = elf_getdata (scn, NULL);
  if (data == NULL)
    data = elf_newdata (scn);
  assert (elf_getdata (scn, data) == NULL);

  data->d_type = ELF_T_BYTE;
  data->d_size = 1;
  data->d_off = 0;
  data->d_align = 1;
  data->d_version = EV_CURRENT;
  data->d_buf = realloc (data->d_buf, strsize);
  if (data->d_buf == NULL)
    {
      error (0, ENOMEM, "%s: Could not build .gnu.libstr section",
	     dso->filename);
      goto error_out;
    }

  oldsize = dso->shdr[libstr].sh_size;
  dso->shdr[libstr].sh_size = 1;
  *(char *)data->d_buf = '\0';
  for (i = 0; i < ndeps - 1; ++i)
    {
      const char *name = info->sonames[i + 1];

      list[i].l_name = strtabfind (dso, liblist, name);
      if (list[i].l_name == 0)
	{
	  size_t len = strlen (name) + 1;

	  memcpy (data->d_buf + data->d_size, name, len);
	  list[i].l_name = data->d_size;
	  data->d_size += len;
	  dso->shdr[libstr].sh_size += len;
	}
    }
  if (oldsize != dso->shdr[libstr].sh_size)
    {
      GElf_Addr adjust = dso->shdr[libstr].sh_size - oldsize;

      oldoffset = dso->shdr[libstr].sh_offset;
      if (adjust_dso_nonalloc (dso, libstr + 1, oldoffset, adjust))
	goto error_out;
    }

  scn = dso->scn[liblist];
  data = elf_getdata (scn, NULL);
  if (data == NULL)
    data = elf_newdata (scn);
  assert (elf_getdata (scn, data) == NULL);

  data->d_type = ELF_T_WORD;
  data->d_size = (ndeps - 1) * sizeof (Elf32_Lib);
  data->d_off = 0;
  data->d_align = sizeof (GElf_Word);
  data->d_version = EV_CURRENT;
  free (data->d_buf);
  data->d_buf = list;
  list = NULL;

  if (data->d_size != dso->shdr[liblist].sh_size)
    {
      GElf_Addr adjust = data->d_size - dso->shdr[liblist].sh_size;
      GElf_Addr newoffset;

      oldoffset = dso->shdr[liblist].sh_offset;
      newoffset = oldoffset;
      if (newoffset & (data->d_align - 1))
	{
	  newoffset = (newoffset + data->d_align - 1) & ~(data->d_align - 1);
	  adjust += newoffset - dso->shdr[liblist].sh_offset;
	}
      if (adjust_dso_nonalloc (dso, liblist + 1, oldoffset, adjust))
	goto error_out;
      dso->shdr[liblist].sh_offset = newoffset;
      dso->shdr[liblist].sh_size = data->d_size;
    }

  recompute_nonalloc_offsets (dso);
  return 0;

error_out:
  free (list);
  return 1;
}
