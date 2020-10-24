fdopen_dso (int fd, const char *name)
{
  Elf *elf = NULL;
  GElf_Ehdr ehdr;
  GElf_Addr last_off;
  int i, j, k, last, *sections, *invsections;
  DSO *dso = NULL;
  struct PLArch *plarch;
  extern struct PLArch __start_pl_arch[], __stop_pl_arch[];

  elf = elf_begin (fd, ELF_C_READ, NULL);
  if (elf == NULL)
    {
      error (0, 0, "cannot open ELF file: %s", elf_errmsg (-1));
      goto error_out;
    }

  if (elf_kind (elf) != ELF_K_ELF)
    {
      error (0, 0, "\"%s\" is not an ELF file", name);
      goto error_out;
    }

  if (gelf_getehdr (elf, &ehdr) == NULL)
    {
      error (0, 0, "cannot get the ELF header: %s",
	     elf_errmsg (-1));
      goto error_out;
    }

  if (ehdr.e_type != ET_DYN && ehdr.e_type != ET_EXEC)
    {
      error (0, 0, "\"%s\" is not a shared library", name);
      goto error_out;
    }

  if (ehdr.e_shnum == 0)
    {
      GElf_Phdr phdr;

      /* Check for UPX compressed executables.  */
      if (ehdr.e_type == ET_EXEC
	  && ehdr.e_phnum > 0
	  && (gelf_getphdr (elf, 0, &phdr), phdr.p_type == PT_LOAD)
	  && phdr.p_filesz >= 256
	  && phdr.p_filesz <= 4096
	  && phdr.p_offset == 0
	  && ehdr.e_phoff + ehdr.e_phnum * ehdr.e_phentsize < phdr.p_filesz)
	{
	  char *buf = alloca (phdr.p_filesz);
	  size_t start = ehdr.e_phoff + ehdr.e_phnum * ehdr.e_phentsize;

	  if (pread (fd, buf, phdr.p_filesz, 0) == phdr.p_filesz
	      && memmem (buf + start, phdr.p_filesz - start,
			 "UPX!", 4) != NULL)
	    {
	      error (0, 0, "\"%s\" is UPX compressed executable", name);
	      goto error_out;
	    }
	}
      error (0, 0, "\"%s\" has no section headers", name);
      goto error_out;
    }

  /* Allocate DSO structure. Leave place for additional 20 new section
     headers.  */
  dso = (DSO *)
	malloc (sizeof(DSO) + (ehdr.e_shnum + 20) * sizeof(GElf_Shdr)
		+ (ehdr.e_phnum + 1) * sizeof(GElf_Phdr)
		+ (ehdr.e_shnum + 20) * sizeof(Elf_Scn *));
  if (!dso)
    {
      error (0, ENOMEM, "Could not open DSO");
      goto error_out;
    }

  elf_flagelf (elf, ELF_C_SET, ELF_F_LAYOUT | ELF_F_PERMISSIVE);

  memset (dso, 0, sizeof(DSO));
  dso->elf = elf;
  dso->ehdr = ehdr;
  dso->phdr = (GElf_Phdr *) &dso->shdr[ehdr.e_shnum + 20];
  dso->scn = (Elf_Scn **) &dso->phdr[ehdr.e_phnum + 1];
  switch (ehdr.e_ident[EI_CLASS])
    {
    case ELFCLASS32:
      dso->mask = 0xffffffff; break;
    case ELFCLASS64:
      dso->mask = 0xffffffffffffffffULL; break;
    }
  for (i = 0; i < ehdr.e_phnum; ++i)
    gelf_getphdr (elf, i, dso->phdr + i);
  dso->fd = fd;

  for (i = 0, j = 0; i < ehdr.e_shnum; ++i)
    {
      dso->scn[i] = elf_getscn (elf, i);
      gelfx_getshdr (elf, dso->scn[i], dso->shdr + i);
      if ((dso->shdr[i].sh_flags & SHF_ALLOC) && dso->shdr[i].sh_type != SHT_NOBITS)
	j = 1;
    }
  if (j == 0)
    {
      /* If all ALLOC sections are SHT_NOBITS, then this is a
	 stripped-to-file debuginfo.  Skip it silently.  */
      goto error_out;
    }

  sections = (int *) alloca (dso->ehdr.e_shnum * sizeof (int) * 2);
  sections[0] = 0;
  for (i = 1, j = 1, k = dso->ehdr.e_shnum, last = -1;
       i < dso->ehdr.e_shnum; ++i)
    if (RELOCATE_SCN (dso->shdr[i].sh_flags))
      {
	last = i;
	sections[j++] = i;
      }
    else
      sections[--k] = i;
  assert (j == k);

  section_cmp_dso = dso;
  qsort (sections + k, dso->ehdr.e_shnum - k, sizeof (*sections), section_cmp);
  invsections = sections + dso->ehdr.e_shnum;
  invsections[0] = 0;
  for (i = 1, j = 0; i < ehdr.e_shnum; ++i)
    {
      if (i != sections[i])
	{
	  j = 1;
	  dso->scn[i] = elf_getscn (elf, sections[i]);
	  gelfx_getshdr (elf, dso->scn[i], dso->shdr + i);
	}
      invsections[sections[i]] = i;
    }

  if (j)
    {
      dso->move = init_section_move (dso);
      if (dso->move == NULL)
	goto error_out;
      memcpy (dso->move->old_to_new, invsections, dso->ehdr.e_shnum * sizeof (int));
      memcpy (dso->move->new_to_old, sections, dso->ehdr.e_shnum * sizeof (int));
    }

  last_off = 0;
  for (i = 1; i < ehdr.e_shnum; ++i)
    {
      if (dso->shdr[i].sh_link >= ehdr.e_shnum)
	{
	  error (0, 0, "%s: bogus sh_link value %d", name,
		 dso->shdr[i].sh_link);
	  goto error_out;
	}
      dso->shdr[i].sh_link = invsections[dso->shdr[i].sh_link];
      if (dso->shdr[i].sh_type == SHT_REL
	  || dso->shdr[i].sh_type == SHT_RELA
	  || (dso->shdr[i].sh_flags & SHF_INFO_LINK))
	{
	  if (dso->shdr[i].sh_info >= ehdr.e_shnum)
	    {
	      error (0, 0, "%s: bogus sh_info value %d", name,
		     dso->shdr[i].sh_info);
	      goto error_out;
	    }
	  dso->shdr[i].sh_info = invsections[dso->shdr[i].sh_info];
	}

      /* Some linkers mess up sh_offset fields for empty or nobits
	 sections.  */
      if (RELOCATE_SCN (dso->shdr[i].sh_flags)
	  && (dso->shdr[i].sh_size == 0
	      || dso->shdr[i].sh_type == SHT_NOBITS))
	{
	  for (j = i + 1; j < ehdr.e_shnum; ++j)
	    if (! RELOCATE_SCN (dso->shdr[j].sh_flags))
	      break;
	    else if (dso->shdr[j].sh_size != 0
		     && dso->shdr[j].sh_type != SHT_NOBITS)
	      break;
	  dso->shdr[i].sh_offset = (last_off + dso->shdr[i].sh_addralign - 1)
				   & ~(dso->shdr[i].sh_addralign - 1);
	  if (j < ehdr.e_shnum
	      && dso->shdr[i].sh_offset > dso->shdr[j].sh_offset)
	    {
	      GElf_Addr k;

	      for (k = dso->shdr[i].sh_addralign - 1; k; )
		{
		  k >>= 1;
		  dso->shdr[i].sh_offset = (last_off + k) & ~k;
		  if (dso->shdr[i].sh_offset <= dso->shdr[j].sh_offset)
		    break;
		}
	    }
	  last_off = dso->shdr[i].sh_offset;
	}
      else
	last_off = dso->shdr[i].sh_offset + dso->shdr[i].sh_size;
    }
  dso->ehdr.e_shstrndx = invsections[dso->ehdr.e_shstrndx];

  for (plarch = __start_pl_arch; plarch < __stop_pl_arch; plarch++)
    if (plarch->class == ehdr.e_ident[EI_CLASS]
	&& (plarch->machine == ehdr.e_machine
	    || plarch->alternate_machine[0] == ehdr.e_machine
	    || plarch->alternate_machine[1] == ehdr.e_machine
	    || plarch->alternate_machine[2] == ehdr.e_machine))
      break;

  if (plarch == __stop_pl_arch || ehdr.e_machine == EM_NONE)
    {
      error (0, 0, "\"%s\"'s architecture is not supported", name);
      goto error_out;
    }

  dso->arch = plarch;

  dso->base = ~(GElf_Addr) 0;
  dso->align = 0;
  dso->end = 0;
  for (i = 0; i < dso->ehdr.e_phnum; i++)
    if (dso->phdr[i].p_type == PT_LOAD)
      {
	GElf_Addr base, end;

	if (dso->phdr[i].p_align > dso->align)
	  dso->align = dso->phdr[i].p_align;
	base = dso->phdr[i].p_vaddr & ~(dso->phdr[i].p_align - 1);
	end = dso->phdr[i].p_vaddr + dso->phdr[i].p_memsz;
	if (base < dso->base)
	  dso->base = base;
	if (end > dso->end)
	  dso->end = end;
      }

  if (dso->base == ~(GElf_Addr) 0)
    {
      error (0, 0, "%s: cannot find loadable segment", name);
      goto error_out;
    }

  read_dynamic (dso);

  dso->filename = (const char *) strdup (name);
  dso->soname = dso->filename;
  if (dso->info[DT_STRTAB] && dso->info[DT_SONAME])
    {
      const char *soname;

      soname = get_data (dso, dso->info[DT_STRTAB] + dso->info[DT_SONAME],
			 NULL);
      if (soname && soname[0] != '\0')
	dso->soname = (const char *) strdup (soname);
    }

  if (dso->arch->machine == EM_ALPHA
      || dso->arch->machine == EM_MIPS)
    for (i = 1; i < ehdr.e_shnum; ++i)
      {
	if ((dso->shdr[i].sh_type == SHT_ALPHA_DEBUG
	     && dso->arch->machine == EM_ALPHA)
	    || (dso->shdr[i].sh_type == SHT_MIPS_DEBUG
		&& dso->arch->machine == EM_MIPS))
	  {
	    const char *name
	      = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[i].sh_name);
	    if (! strcmp (name, ".mdebug"))
	      dso->mdebug_orig_offset = dso->shdr[i].sh_offset;
	    break;
	  }
      }

  return dso;

error_out:
  if (dso)
    {
      free (dso->move);
      if (dso->soname != dso->filename)
	free ((char *) dso->soname);
      free ((char *) dso->filename);
      free (dso);
    }
  if (elf)
    elf_end (elf);
  if (fd != -1)
    close (fd);
  return NULL;
}
