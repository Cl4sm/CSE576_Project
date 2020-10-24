int
adjust_dso (DSO *dso, GElf_Addr start, GElf_Addr adjust)
{
  int i;

  if (dso->arch->arch_adjust
      && dso->arch->arch_adjust (dso, start, adjust))
    return 1;

  if (dso->ehdr.e_entry >= start)
    {
      dso->ehdr.e_entry += adjust;
      gelf_update_ehdr (dso->elf, &dso->ehdr);
      elf_flagehdr (dso->elf, ELF_C_SET, ELF_F_DIRTY);
    }

  for (i = 0; i < dso->ehdr.e_phnum; i++)
    {
      /* Leave STACK segment alone, it has
	 p_vaddr == p_paddr == p_offset == p_filesz == p_memsz == 0.  */
      if (dso->phdr[i].p_type == PT_GNU_STACK)
	continue;
      if (! start)
	{
	  dso->phdr[i].p_vaddr += adjust;
	  dso->phdr[i].p_paddr += adjust;
	}
      else if (start <= dso->phdr[i].p_vaddr)
	{
	  dso->phdr[i].p_vaddr += adjust;
	  dso->phdr[i].p_paddr += adjust;
	  dso->phdr[i].p_offset += adjust;
	}
      else if (start < dso->phdr[i].p_vaddr + dso->phdr[i].p_filesz)
	{
	  dso->phdr[i].p_filesz += adjust;
	  dso->phdr[i].p_memsz += adjust;
	}
      else if (start < dso->phdr[i].p_vaddr + dso->phdr[i].p_memsz)
	dso->phdr[i].p_memsz += adjust;
      else
	continue;
      if (dso->phdr[i].p_type == PT_LOAD
	  && (dso->phdr[i].p_vaddr - dso->phdr[i].p_offset)
	     % dso->phdr[i].p_align)
	{
	  error (0, 0, "%s: PT_LOAD %08llx %08llx 0x%x would be not properly aligned",
		 dso->filename, (long long) dso->phdr[i].p_offset,
		 (long long) dso->phdr[i].p_vaddr, (int) dso->phdr[i].p_align);
	  return 1;
	}
      gelf_update_phdr (dso->elf, i, dso->phdr + i);
    }
  elf_flagphdr (dso->elf, ELF_C_SET, ELF_F_DIRTY);

  for (i = 1; i < dso->ehdr.e_shnum; i++)
    {
      const char *name;

      if (dso->arch->adjust_section)
	{
	  int ret = dso->arch->adjust_section (dso, i, start, adjust);

	  if (ret == 1)
	    return 1;
	  else if (ret)
	    continue;
	}
      switch (dso->shdr[i].sh_type)
	{
	case SHT_PROGBITS:
	  name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[i].sh_name);
	  if (strcmp (name, ".stab") == 0
	      && adjust_stabs (dso, i, start, adjust))
	    return 1;
	  if (strcmp (name, ".debug_info") == 0
	      && adjust_dwarf2 (dso, i, start, adjust))
	    return 1;
	  break;
	case SHT_HASH:
	case SHT_GNU_HASH:
	case SHT_NOBITS:
	case SHT_STRTAB:
	  break;
	case SHT_SYMTAB:
	case SHT_DYNSYM:
	  if (adjust_symtab (dso, i, start, adjust))
	    return 1;
	  break;
	case SHT_DYNAMIC:
	  if (adjust_dynamic (dso, i, start, adjust))
	    return 1;
	  break;
	case SHT_REL:
	  if (adjust_rel (dso, i, start, adjust))
	    return 1;
	  break;
	case SHT_RELA:
	  if (adjust_rela (dso, i, start, adjust))
	    return 1;
	  break;
	}
      if ((dso->arch->machine == EM_ALPHA
	   && dso->shdr[i].sh_type == SHT_ALPHA_DEBUG)
	  || (dso->arch->machine == EM_MIPS
	      && dso->shdr[i].sh_type == SHT_MIPS_DEBUG))
	if (adjust_mdebug (dso, i, start, adjust))
	  return 1;
    }

  for (i = 0; i < dso->ehdr.e_shnum; i++)
    {
      if (RELOCATE_SCN (dso->shdr[i].sh_flags))
	{
	  if (dso->shdr[i].sh_addr >= start)
	    {
	      Elf_Scn *scn = dso->scn[i];

	      dso->shdr[i].sh_addr += adjust;
	      if (start)
		dso->shdr[i].sh_offset += adjust;
	      gelfx_update_shdr (dso->elf, scn, dso->shdr + i);
	      elf_flagshdr (scn, ELF_C_SET, ELF_F_DIRTY);
	    }
	}
    }

  addr_adjust (dso->base, start, adjust);
  addr_adjust (dso->end, start, adjust);

  if (start)
    {
      start = adjust_new_to_old (dso, start);
      for (i = 0; i < dso->nadjust; i++)
	if (start < dso->adjust[i].start)
	  dso->adjust[i].adjust += adjust;
	else
	  break;
      if (i < dso->nadjust && start == dso->adjust[i].start)
	dso->adjust[i].adjust += adjust;
      else
	{
	  dso->adjust =
	    realloc (dso->adjust, (dso->nadjust + 1) * sizeof (*dso->adjust));
	  if (dso->adjust == NULL)
	    {
	      error (0, ENOMEM, "Cannot record the list of adjustements being made");
	      return 1;
	    }
	  memmove (dso->adjust + i + 1, dso->adjust + i, dso->nadjust - i);
	  dso->adjust[i].start = start;
	  dso->adjust[i].adjust = adjust;
	  ++dso->nadjust;
	}
    }

  return start ? adjust_dso_nonalloc (dso, 0, 0, adjust) : 0;
}
