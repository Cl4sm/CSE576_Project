undo_sections (DSO *dso, int undo, struct section_move *move,
	       struct reloc_info *rinfo, GElf_Ehdr *ehdr,
	       GElf_Phdr *phdr, GElf_Shdr *shdr)
{
  Elf_Data src, dst, *d;
  Elf_Scn *scn;
  int i, j;

  scn = dso->scn[undo];
  d = elf_getdata (scn, NULL);
  assert (d != NULL && elf_getdata (scn, d) == NULL);

  src = *d;
  src.d_type = ELF_T_EHDR;
  src.d_align = dso->shdr[undo].sh_addralign;
  src.d_size = gelf_fsize (dso->elf, ELF_T_EHDR, 1, EV_CURRENT);
  dst = src;
  if (src.d_size > d->d_size)
    {
      error (0, 0, "%s: .gnu.prelink_undo section too small",
	     dso->filename);
      return 1;
    }
  switch (gelf_getclass (dso->elf))
    {
    case ELFCLASS32:
      dst.d_buf = alloca (dst.d_size);
      break;
    case ELFCLASS64:
      dst.d_buf = ehdr;
      break;
    default:
      return 1;
    }
  if (gelf_xlatetom (dso->elf, &dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
    {
      error (0, 0, "%s: Could not read .gnu.prelink_undo section",
	     dso->filename);
      return 1;
    }
  if (gelf_getclass (dso->elf) == ELFCLASS32)
    {
      Elf32_Ehdr *ehdr32 = (Elf32_Ehdr *) dst.d_buf;

      memcpy (ehdr->e_ident, ehdr32->e_ident, sizeof (ehdr->e_ident));
#define COPY(name) ehdr->name = ehdr32->name
      COPY (e_type);
      COPY (e_machine);
      COPY (e_version);
      COPY (e_entry);
      COPY (e_phoff);
      COPY (e_shoff);
      COPY (e_flags);
      COPY (e_ehsize);
      COPY (e_phentsize);
      COPY (e_phnum);
      COPY (e_shentsize);
      COPY (e_shnum);
      COPY (e_shstrndx);
#undef COPY
    }

  if (memcmp (ehdr->e_ident, dso->ehdr.e_ident, sizeof (ehdr->e_ident))
      || ehdr->e_type != dso->ehdr.e_type
      || ehdr->e_machine != dso->ehdr.e_machine
      || ehdr->e_version != dso->ehdr.e_version
      || ehdr->e_flags != dso->ehdr.e_flags
      || ehdr->e_ehsize != dso->ehdr.e_ehsize
      || ehdr->e_phentsize != dso->ehdr.e_phentsize
      || ehdr->e_shentsize != dso->ehdr.e_shentsize)
    {
      error (0, 0, "%s: ELF headers changed since prelinking",
	     dso->filename);
      return 1;
    }

  if (ehdr->e_phnum > dso->ehdr.e_phnum)
    {
      error (0, 0, "%s: Number of program headers is less than before prelinking",
	     dso->filename);
      return 1;
    }

  if (d->d_size != (src.d_size
		    + gelf_fsize (dso->elf, ELF_T_PHDR, ehdr->e_phnum,
				  EV_CURRENT)
		    + gelf_fsize (dso->elf, ELF_T_SHDR, ehdr->e_shnum - 1,
				  EV_CURRENT)))
    {
      error (0, 0, "%s: Incorrect size of .gnu.prelink_undo section",
	     dso->filename);
      return 1;
    }

  src.d_type = ELF_T_PHDR;
  src.d_buf += src.d_size;
  src.d_size = gelf_fsize (dso->elf, ELF_T_PHDR, ehdr->e_phnum, EV_CURRENT);
  dst = src;
  switch (gelf_getclass (dso->elf))
    {
    case ELFCLASS32:
      dst.d_buf = alloca (dst.d_size);
      break;
    case ELFCLASS64:
      dst.d_buf = phdr;
      break;
    }
  if (gelf_xlatetom (dso->elf, &dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
    {
      error (0, 0, "%s: Could not read .gnu.prelink_undo section",
	     dso->filename);
      return 1;
    }

  if (gelf_getclass (dso->elf) == ELFCLASS32)
    {
      Elf32_Phdr *phdr32 = (Elf32_Phdr *) dst.d_buf;

      for (i = 0; i < ehdr->e_phnum; ++i)
	{
#define COPY(name) phdr[i].name = phdr32[i].name
	  COPY(p_type);
	  COPY(p_flags);
	  COPY(p_offset);
	  COPY(p_vaddr);
	  COPY(p_paddr);
	  COPY(p_filesz);
	  COPY(p_memsz);
	  COPY(p_align);
#undef COPY
	}
    }

  memset (shdr, 0, sizeof (GElf_Shdr));
  src.d_type = ELF_T_SHDR;
  src.d_buf += src.d_size;
  src.d_size = gelf_fsize (dso->elf, ELF_T_SHDR, ehdr->e_shnum - 1, EV_CURRENT);
  dst = src;
  switch (gelf_getclass (dso->elf))
    {
    case ELFCLASS32:
      dst.d_buf = alloca (dst.d_size);
      break;
    case ELFCLASS64:
      dst.d_buf = shdr + 1;
      break;
    default:
      return 1;
    }
  if (gelf_xlatetom (dso->elf, &dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
    {
      error (0, 0, "%s: Could not read .gnu.prelink_undo section",
	     dso->filename);
      return 1;
    }

  if (gelf_getclass (dso->elf) == ELFCLASS32)
    {
      Elf32_Shdr *shdr32 = (Elf32_Shdr *) dst.d_buf;

      for (i = 1; i < ehdr->e_shnum; ++i)
	{
#define COPY(name) shdr[i].name = shdr32[i - 1].name
	  COPY (sh_name);
	  COPY (sh_type);
	  COPY (sh_flags);
	  COPY (sh_addr);
	  COPY (sh_offset);
	  COPY (sh_size);
	  COPY (sh_link);
	  COPY (sh_info);
	  COPY (sh_addralign);
	  COPY (sh_entsize);
#undef COPY
	}
    }

  move->new_shnum = ehdr->e_shnum;
  for (i = 1; i < move->old_shnum; ++i)
    move->old_to_new[i] = -1;
  for (i = 1; i < move->new_shnum; ++i)
    move->new_to_old[i] = -1;

  for (i = 1; i < move->old_shnum; ++i)
    {
      for (j = 1; j < move->new_shnum; ++j)
	if (dso->shdr[i].sh_name == shdr[j].sh_name
	    && dso->shdr[i].sh_type == shdr[j].sh_type
	    && dso->shdr[i].sh_flags == shdr[j].sh_flags
	    && dso->shdr[i].sh_addralign == shdr[j].sh_addralign
	    && dso->shdr[i].sh_entsize == shdr[j].sh_entsize
	    && dso->shdr[i].sh_size == shdr[j].sh_size
	    && move->new_to_old[j] == -1)
	  break;

      if (j == move->new_shnum)
	continue;

      move->old_to_new[i] = j;
      move->new_to_old[j] = i;
    }

  for (i = 1; i < move->old_shnum; ++i)
    if (move->old_to_new[i] == -1)
      {
	const char *name = strptr (dso, dso->ehdr.e_shstrndx,
				   dso->shdr[i].sh_name);

	if (! strcmp (name, ".gnu.prelink_undo")
	    || ! strcmp (name, ".gnu.conflict")
	    || ! strcmp (name, ".gnu.liblist")
	    || ! strcmp (name, ".gnu.libstr")
	    || ((! strcmp (name, ".dynbss") || ! strcmp (name, ".sdynbss"))
		&& dso->ehdr.e_type == ET_EXEC))
	  continue;

	if ((! strcmp (name, ".dynstr") && dso->ehdr.e_type == ET_EXEC)
	    || i == dso->ehdr.e_shstrndx)
	  {
	    for (j = 1; j < move->new_shnum; ++j)
	      if (dso->shdr[i].sh_name == shdr[j].sh_name
		  && dso->shdr[i].sh_type == shdr[j].sh_type
		  && dso->shdr[i].sh_flags == shdr[j].sh_flags
		  && dso->shdr[i].sh_addralign == shdr[j].sh_addralign
		  && dso->shdr[i].sh_entsize == shdr[j].sh_entsize
		  && dso->shdr[i].sh_size > shdr[j].sh_size
		  && move->new_to_old[j] == -1)
		break;

	    if (j < move->new_shnum)
	      {
		move->old_to_new[i] = j;
		move->new_to_old[j] = i;
		continue;
	      }
	  }

	if (((i >= rinfo->first && i <= rinfo->last) || i == rinfo->plt)
	    && dso->shdr[i].sh_type == SHT_RELA)
	  {
	    for (j = 1; j < move->new_shnum; ++j)
	      if (dso->shdr[i].sh_name == shdr[j].sh_name
		  && shdr[j].sh_type == SHT_REL
		  && dso->shdr[i].sh_flags == shdr[j].sh_flags
		  && dso->shdr[i].sh_addralign == shdr[j].sh_addralign
		  && 2 * dso->shdr[i].sh_entsize == 3 * shdr[j].sh_entsize
		  && 2 * dso->shdr[i].sh_size == 3 * shdr[j].sh_size
		  && move->new_to_old[j] == -1)
		break;

	    if (j < move->new_shnum)
	      {
		move->old_to_new[i] = j;
		move->new_to_old[j] = i;
		continue;
	      }
	  }

	if (! strcmp (name, ".bss")
	    || ! strcmp (name, ".sbss")
	    || ((! strcmp (name, ".plt") || ! strcmp (name, ".iplt"))
		&& dso->shdr[i].sh_type == SHT_PROGBITS))
	  {
	    int is_plt = ! strcmp (name, ".plt");

	    for (j = 1; j < move->new_shnum; ++j)
	      if (dso->shdr[i].sh_name == shdr[j].sh_name
		  && dso->shdr[i].sh_flags == shdr[j].sh_flags
		  && dso->shdr[i].sh_addralign == shdr[j].sh_addralign
		  && (is_plt || dso->shdr[i].sh_entsize == shdr[j].sh_entsize)
		  && move->new_to_old[j] == -1)
		{
		  if (is_plt)
		    {
		      if (dso->shdr[i].sh_size != shdr[j].sh_size)
			continue;
		      if (shdr[j].sh_type == SHT_NOBITS
			  && dso->shdr[i].sh_entsize == shdr[j].sh_entsize)
			break;
		      /* On Alpha prelink fixes bogus sh_entsize of .plt
			 sections.  */
		      if (shdr[j].sh_type == SHT_PROGBITS)
			break;
		    }
		  else
		    {
		      const char *pname;

		      if (dso->shdr[i].sh_type != shdr[j].sh_type
			  && (dso->shdr[i].sh_type != SHT_PROGBITS
			      || shdr[j].sh_type != SHT_NOBITS))
			continue;

		      if (dso->shdr[i].sh_size == shdr[j].sh_size)
			break;

		      pname = strptr (dso, dso->ehdr.e_shstrndx,
				      dso->shdr[i - 1].sh_name);
		      if (strcmp (pname, ".dynbss")
			  && strcmp (pname, ".sdynbss"))
			continue;

		      if (dso->shdr[i].sh_size + dso->shdr[i - 1].sh_size
			  == shdr[j].sh_size)
			break;
		    }
		}

	    if (j < move->new_shnum)
	      {
		move->old_to_new[i] = j;
		move->new_to_old[j] = i;
		continue;
	      }
	  }

	error (0, 0, "%s: Section %s created after prelinking",
	       dso->filename, name);
	return 1;
      }

  for (i = 1; i < move->new_shnum; ++i)
    if (move->new_to_old[i] == -1)
      {
	const char *name = strptr (dso, dso->ehdr.e_shstrndx, shdr[i].sh_name);

	error (0, 0, "%s: Section %s removed after prelinking", dso->filename,
	       name);
	return 1;
      }

  return 0;
}
