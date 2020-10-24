prelink_prepare (DSO *dso)
{
  struct reloc_info rinfo;
  int liblist = 0, libstr = 0, newlibstr = 0, undo = 0, newundo = 0;
  int i;

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    {
      const char *name
	= strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[i].sh_name);
      if (! strcmp (name, ".gnu.liblist"))
	liblist = i;
      else if (! strcmp (name, ".gnu.libstr"))
	libstr = i;
      else if (! strcmp (name, ".gnu.prelink_undo"))
	undo = i;
    }

  if (undo == 0)
    {
      Elf32_Shdr *shdr32;
      Elf64_Shdr *shdr64;
      Elf_Data src, dst;

      dso->undo.d_size = gelf_fsize (dso->elf, ELF_T_EHDR, 1, EV_CURRENT)
			 + gelf_fsize (dso->elf, ELF_T_PHDR,
				       dso->ehdr.e_phnum, EV_CURRENT)
			 + gelf_fsize (dso->elf, ELF_T_SHDR,
				       dso->ehdr.e_shnum - 1, EV_CURRENT);
      dso->undo.d_buf = malloc (dso->undo.d_size);
      if (dso->undo.d_buf == NULL)
	{
	  error (0, ENOMEM, "%s: Could not create .gnu.prelink_undo section",
		 dso->filename);
	  return 1;
	}
      dso->undo.d_type = ELF_T_BYTE;
      dso->undo.d_off = 0;
      dso->undo.d_align = gelf_fsize (dso->elf, ELF_T_ADDR, 1, EV_CURRENT);
      dso->undo.d_version = EV_CURRENT;
      src = dso->undo;
      src.d_type = ELF_T_EHDR;
      src.d_size = gelf_fsize (dso->elf, ELF_T_EHDR, 1, EV_CURRENT);
      dst = src;
      switch (gelf_getclass (dso->elf))
	{
	case ELFCLASS32:
	  src.d_buf = elf32_getehdr (dso->elf);
	  if (elf32_xlatetof (&dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
	    {
	      error (0, 0, "%s: Failed to create .gnu.prelink_undo section",
		     dso->filename);
	      return 1;
	    }
	  break;
	case ELFCLASS64:
	  src.d_buf = elf64_getehdr (dso->elf);
	  if (elf64_xlatetof (&dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
	    {
	      error (0, 0, "%s: Failed to create .gnu.prelink_undo section",
		     dso->filename);
	      return 1;
	    }
	  break;
	default:
	  return 1;
	}
      src.d_buf = dst.d_buf + src.d_size;
      src.d_type = ELF_T_PHDR;
      src.d_size = gelf_fsize (dso->elf, ELF_T_PHDR, dso->ehdr.e_phnum,
			       EV_CURRENT);
      dst = src;
      switch (gelf_getclass (dso->elf))
	{
	case ELFCLASS32:
	  src.d_buf = elf32_getphdr (dso->elf);
	  if (elf32_xlatetof (&dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
	    {
	      error (0, 0, "%s: Failed to create .gnu.prelink_undo section",
		     dso->filename);
	      return 1;
	    }
	  break;
	case ELFCLASS64:
	  src.d_buf = elf64_getphdr (dso->elf);
	  if (elf64_xlatetof (&dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
	    {
	      error (0, 0, "%s: Failed to create .gnu.prelink_undo section",
		     dso->filename);
	      return 1;
	    }
	  break;
	}
      src.d_buf = dst.d_buf + src.d_size;
      src.d_type = ELF_T_SHDR;
      src.d_size = gelf_fsize (dso->elf, ELF_T_SHDR,
			       dso->ehdr.e_shnum - 1, EV_CURRENT);
      dst = src;
      switch (gelf_getclass (dso->elf))
	{
	case ELFCLASS32:
	  shdr32 = (Elf32_Shdr *) src.d_buf;
	  /* Note: cannot use dso->scn[i] below, since we want to save the
	     original section order before non-alloced sections were
	     sorted by sh_offset.  */
	  for (i = 1; i < dso->ehdr.e_shnum; ++i)
	    shdr32[i - 1] = *elf32_getshdr (elf_getscn (dso->elf, i));
	  if (elf32_xlatetof (&dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
	    {
	      error (0, 0, "%s: Failed to create .gnu.prelink_undo section",
		     dso->filename);
	      return 1;
	    }
	  break;
	case ELFCLASS64:
	  shdr64 = (Elf64_Shdr *) src.d_buf;
	  /* Note: cannot use dso->scn[i] below, since we want to save the
	     original section order before non-alloced sections were
	     sorted by sh_offset.  */
	  for (i = 1; i < dso->ehdr.e_shnum; ++i)
	    shdr64[i - 1] = *elf64_getshdr (elf_getscn (dso->elf, i));
	  if (elf64_xlatetof (&dst, &src, dso->ehdr.e_ident[EI_DATA]) == NULL)
	    {
	      error (0, 0, "%s: Failed to create .gnu.prelink_undo section",
		     dso->filename);
	      return 1;
	    }
	  break;
	}
    }

  if (dso->ehdr.e_type != ET_DYN)
    return 0;

  if (find_reloc_sections (dso, &rinfo))
    return 1;

  if (is_ldso_soname (dso->soname))
    {
      liblist = -1;
      libstr = -1;
    }

  if (liblist && libstr && undo
      && ! rinfo.rel_to_rela && ! rinfo.rel_to_rela_plt)
      return 0;

  if (! liblist || ! libstr || ! undo)
    {
      struct section_move *move;

      move = init_section_move (dso);
      if (move == NULL)
	return 1;

      if (! liblist)
	{
	  liblist = move->old_to_new [dso->ehdr.e_shstrndx];
	  add_section (move, liblist);
	}
      else
	liblist = 0;

      if (! libstr)
	{
	  add_section (move, liblist + 1);
	  libstr = liblist + 1;
	  newlibstr = 1;
	}
      else if (libstr != -1)
	libstr = move->old_to_new[libstr];

      if (! undo)
	{
	  if (libstr == -1)
	    {
	      undo = move->old_to_new [dso->ehdr.e_shstrndx];
	      add_section (move, undo);
	    }
	  else
	    {
	      add_section (move, libstr + 1);
	      undo = libstr + 1;
	    }
	  newundo = 1;
	}
      else
	undo = move->old_to_new[undo];

      if (reopen_dso (dso, move, NULL))
	{
	  free (move);
	  return 1;
	}

      free (move);
      if (liblist)
	{
	  memset (&dso->shdr[liblist], 0, sizeof (GElf_Shdr));
	  dso->shdr[liblist].sh_name = shstrtabadd (dso, ".gnu.liblist");
	  if (dso->shdr[liblist].sh_name == 0)
	    return 1;
	  dso->shdr[liblist].sh_type = SHT_GNU_LIBLIST;
	  dso->shdr[liblist].sh_offset = dso->shdr[liblist - 1].sh_offset;
	  if (dso->shdr[liblist - 1].sh_type != SHT_NOBITS)
	    dso->shdr[liblist].sh_offset += dso->shdr[liblist - 1].sh_size;
	  dso->shdr[liblist].sh_link = libstr;
	  dso->shdr[liblist].sh_addralign = sizeof (GElf_Word);
	  dso->shdr[liblist].sh_entsize = sizeof (Elf32_Lib);
	}

      if (newlibstr)
	{
	  memset (&dso->shdr[libstr], 0, sizeof (GElf_Shdr));
	  dso->shdr[libstr].sh_name = shstrtabadd (dso, ".gnu.libstr");
	  if (dso->shdr[libstr].sh_name == 0)
	    return 1;
	  dso->shdr[libstr].sh_type = SHT_STRTAB;
	  dso->shdr[libstr].sh_offset = dso->shdr[libstr - 1].sh_offset;
	  if (dso->shdr[libstr - 1].sh_type != SHT_NOBITS)
	    dso->shdr[libstr].sh_offset += dso->shdr[libstr - 1].sh_size;
	  dso->shdr[libstr].sh_addralign = 1;
	}

      if (newundo)
	{
	  Elf_Scn *scn;
	  Elf_Data *data;
	  GElf_Addr newoffset;

	  memset (&dso->shdr[undo], 0, sizeof (GElf_Shdr));
	  dso->shdr[undo].sh_name = shstrtabadd (dso, ".gnu.prelink_undo");
	  if (dso->shdr[undo].sh_name == 0)
	    return 1;
	  dso->shdr[undo].sh_type = SHT_PROGBITS;
	  dso->shdr[undo].sh_offset = dso->shdr[undo - 1].sh_offset;
	  if (dso->shdr[undo - 1].sh_type != SHT_NOBITS)
	    dso->shdr[undo].sh_offset += dso->shdr[undo - 1].sh_size;
	  dso->shdr[undo].sh_addralign = dso->undo.d_align;
	  dso->shdr[undo].sh_entsize = 1;
	  dso->shdr[undo].sh_size = dso->undo.d_size;
	  newoffset = dso->shdr[undo].sh_offset + dso->undo.d_align - 1;
	  newoffset &= ~(dso->shdr[undo].sh_addralign - 1);
	  if (adjust_dso_nonalloc (dso, undo + 1, dso->shdr[undo].sh_offset,
				   dso->undo.d_size + newoffset
				   - dso->shdr[undo].sh_offset))
	    return 1;
	  dso->shdr[undo].sh_offset = newoffset;
	  scn = dso->scn[undo];
	  data = elf_getdata (scn, NULL);
	  assert (data != NULL && elf_getdata (scn, data) == NULL);
	  free (data->d_buf);
	  *data = dso->undo;
	  dso->undo.d_buf = NULL;
	}
    }
  else if (reopen_dso (dso, NULL, NULL))
    return 1;

  if (rinfo.rel_to_rela || rinfo.rel_to_rela_plt)
    {
      /* On REL architectures, we might need to convert some REL
	 relocations to RELA relocs.  */

      int safe = 1, align = 0, last;
      GElf_Addr start, adjust, adjust1, adjust2;

      for (i = 1; i < (rinfo.plt ? rinfo.plt : rinfo.first); i++)
	switch (dso->shdr[i].sh_type)
	  {
	  case SHT_HASH:
	  case SHT_GNU_HASH:
	  case SHT_DYNSYM:
	  case SHT_REL:
	  case SHT_RELA:
	  case SHT_STRTAB:
	  case SHT_NOTE:
	  case SHT_GNU_verdef:
	  case SHT_GNU_verneed:
	  case SHT_GNU_versym:
	    /* These sections are safe, no relocations should point
	       to it, therefore enlarging a section after sections
	       from this set only (and SHT_REL) in ET_DYN just needs
	       adjusting the rest of the library.  */
	    break;
	  case SHT_DYNAMIC:
	  case SHT_MIPS_REGINFO:
	    /* The same applies to these sections on MIPS.  The convention
	       is to put .dynamic and .reginfo near the beginning of the
	       read-only segment, before the program text.  No relocations
	       may refer to them.  */
	    if (dso->ehdr.e_machine == EM_MIPS)
	      break;
	    /* FALLTHROUGH */
	  default:
	    /* The rest of sections are not safe.  */
	    safe = 0;
	    break;
	  }

      if (! safe)
	{
	  error (0, 0, "%s: Cannot safely convert %s' section from REL to RELA",
		 dso->filename, strptr (dso, dso->ehdr.e_shstrndx,
					dso->shdr[rinfo.rel_to_rela
					? rinfo.first : rinfo.plt].sh_name));
	  return 1;
	}

      for (i = rinfo.plt ? rinfo.plt : rinfo.first; i < dso->ehdr.e_shnum; i++)
	{
	  if (dso->shdr[i].sh_addralign > align)
	    align = dso->shdr[i].sh_addralign;
	}

      if (rinfo.plt)
	start = dso->shdr[rinfo.plt].sh_addr + dso->shdr[rinfo.plt].sh_size;
      else
	start = dso->shdr[rinfo.last].sh_addr + dso->shdr[rinfo.last].sh_size;

      adjust1 = 0;
      adjust2 = 0;
      assert (sizeof (Elf32_Rel) * 3 == sizeof (Elf32_Rela) * 2);
      assert (sizeof (Elf64_Rel) * 3 == sizeof (Elf64_Rela) * 2);
      if (rinfo.rel_to_rela)
	{
	  for (i = rinfo.first; i <= rinfo.last; ++i)
	    {
	      GElf_Addr size = dso->shdr[i].sh_size / 2 * 3;
	      adjust1 += size - dso->shdr[i].sh_size;
	      if (convert_rel_to_rela (dso, i))
		return 1;
	    }
	}
      if (rinfo.rel_to_rela_plt)
	{
	  GElf_Addr size = dso->shdr[rinfo.plt].sh_size / 2 * 3;
	  adjust2 = size - dso->shdr[rinfo.plt].sh_size;
	  if (convert_rel_to_rela (dso, rinfo.plt))
	    return 1;
	}

      adjust = adjust1 + adjust2;

      /* Need to make sure that all the remaining sections are properly
	 aligned.  */
      if (align)
	adjust = (adjust + align - 1) & ~(align - 1);

      /* Need to make sure adjust doesn't cause different Phdr segments
	 to overlap on the same page.  */
      last = -1;
      for (i = 0; i < dso->ehdr.e_phnum; ++i)
	if (dso->phdr[i].p_type == PT_LOAD
	    && dso->phdr[i].p_vaddr + dso->phdr[i].p_memsz >= start)
	  {
	    if (last != -1
		&& (((dso->phdr[last].p_vaddr + dso->phdr[last].p_memsz - 1)
		     ^ dso->phdr[i].p_vaddr)
		    & ~(dso->arch->max_page_size - 1))
		&& !(((dso->phdr[last].p_vaddr + dso->phdr[last].p_memsz
		       + adjust - 1)
		      ^ (dso->phdr[i].p_vaddr + adjust))
		     & ~(dso->arch->max_page_size - 1)))
	      {
		if (align >= dso->arch->max_page_size)
		  {
		    error (0, 0, "%s: Cannot grow reloc sections", dso->filename);
		    return 1;
		  }
		adjust = (adjust + dso->arch->max_page_size - 1)
			 & ~(dso->arch->max_page_size - 1);
	      }
	    last = i;
	  }

      /* Adjust all addresses pointing into remaining sections.  */
      if (adjust_dso (dso, start - 1, adjust))
	return 1;

      if (rinfo.rel_to_rela)
	{
	  GElf_Addr adjust3 = 0;
	  for (i = rinfo.first; i <= rinfo.last; ++i)
	    {
	      GElf_Addr size = dso->shdr[i].sh_size / 2 * 3;

	      dso->shdr[i].sh_addr += adjust3;
	      dso->shdr[i].sh_offset += adjust3;
	      adjust3 += size - dso->shdr[i].sh_size;
	      dso->shdr[i].sh_size = size;
	    }
	  assert (adjust1 == adjust3);
	  if (rinfo.plt)
	    {
	      dso->shdr[rinfo.plt].sh_addr += adjust1;
	      dso->shdr[rinfo.plt].sh_offset += adjust1;
	    }
	}
      if (rinfo.rel_to_rela_plt)
	dso->shdr[rinfo.plt].sh_size += adjust2;

      if (update_dynamic_rel (dso, &rinfo))
	return 1;
    }

  return 0;
}
