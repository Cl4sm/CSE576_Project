execstack_set (DSO *dso, int flag)
{
  int i, null = -1, last, ret;
  GElf_Addr lowoff = ~(GElf_Addr) 0, start = 0, align = 0;
  GElf_Addr adjust;

  for (i = 0; i < dso->ehdr.e_phnum; ++i)
    if (dso->phdr[i].p_type == PT_GNU_STACK)
      {
	/* Found PT_GNU_STACK.  Check if we need any change or not.  */
	if (flag ^ ((dso->phdr[i].p_flags & PF_X) != 0))
	  {
	    ret = execstack_make_rdwr (dso, flag);
	    if (ret != -1)
	      return ret;
	    dso->phdr[i].p_flags ^= PF_X;
	    goto out_write;
	  }
	else
	  goto out_close;
      }
    else if (dso->phdr[i].p_type == PT_NULL)
      null = i;

  if (null != -1)
    {
      /* Overwrite PT_NULL segment with PT_GNU_STACK.  */
      ret = execstack_make_rdwr (dso, flag);
      if (ret != -1)
	return ret;
      execstack_fill_phdr (dso, i, flag);
      goto out_write;
    }

  if (dso->ehdr.e_shnum == 0)
    {
      error (0, 0, "%s: Section header table missing", dso->filename);
      goto error_out;
    }

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    {
      if (lowoff > dso->shdr[i].sh_offset)
	{
	  if (dso->shdr[i].sh_flags & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR))
	    {
	      lowoff = dso->shdr[i].sh_offset;
	      start = dso->shdr[i].sh_addr;
	    }
	  else
	    {
	      error (0, 0, "%s: Non-alloced sections before alloced ones",
		     dso->filename);
	      goto error_out;
	    }
	}

      if (dso->shdr[i].sh_addralign > align)
	align = dso->shdr[i].sh_addralign;
    }

  if (dso->ehdr.e_phoff >= lowoff)
    {
      error (0, 0, "%s: Program header table not before all sections",
	     dso->filename);
      goto error_out;
    }

  if (dso->ehdr.e_shoff <= lowoff)
    {
      error (0, 0, "%s: Section header table before first section",
	     dso->filename);
      goto error_out;
    }

  if (dso->ehdr.e_phoff + (dso->ehdr.e_phnum + 1) * dso->ehdr.e_phentsize
      <= lowoff)
    {
      /* There is enough space for the headers even without reshuffling
	 anything.  */
      for (i = 0; i < dso->ehdr.e_phnum; ++i)
	if (dso->phdr[i].p_type == PT_PHDR)
	  {
	    if (dso->phdr[i].p_filesz
		== dso->ehdr.e_phnum * dso->ehdr.e_phentsize)
	      dso->phdr[i].p_filesz += dso->ehdr.e_phentsize;
	    if (dso->phdr[i].p_memsz
		== dso->ehdr.e_phnum * dso->ehdr.e_phentsize)
	      dso->phdr[i].p_memsz += dso->ehdr.e_phentsize;
	  }
      i = dso->ehdr.e_phnum++;
      ret = execstack_make_rdwr (dso, flag);
      if (ret != -1)
	return ret;
      execstack_fill_phdr (dso, i, flag);
      goto out_write;
    }

  if (dso->ehdr.e_type != ET_DYN)
    {
      error (0, 0, "%s: Reshuffling of objects to make room for\n"
		   "program header entry only supported for shared libraries",
	     dso->filename);
      goto error_out;
    }

  adjust = dso->ehdr.e_phoff + (dso->ehdr.e_phnum + 1) * dso->ehdr.e_phentsize
	   - lowoff;
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
		goto error_out;
	      }
	    adjust = (adjust + dso->arch->max_page_size - 1)
		     & ~(dso->arch->max_page_size - 1);
	  }
	last = i;
      }

  for (i = 0; i < dso->ehdr.e_phnum; ++i)
    if (dso->phdr[i].p_type == PT_PHDR)
      {
	if (dso->phdr[i].p_filesz == dso->ehdr.e_phnum * dso->ehdr.e_phentsize)
	  dso->phdr[i].p_filesz += dso->ehdr.e_phentsize;
	if (dso->phdr[i].p_memsz == dso->ehdr.e_phnum * dso->ehdr.e_phentsize)
	  dso->phdr[i].p_memsz += dso->ehdr.e_phentsize;
      }

  i = dso->ehdr.e_phnum++;
  ret = execstack_make_rdwr (dso, flag);
  if (ret != -1)
    return ret;

  if (adjust_dso (dso, start, adjust))
    goto error_out;

  execstack_fill_phdr (dso, i, flag);

out_write:
  if (dynamic_info_is_set (dso, DT_CHECKSUM_BIT)
      && dso_is_rdwr (dso)
      && prelink_set_checksum (dso))
    goto error_out;

  dso->permissive = 1;

  return update_dso (dso, NULL);

out_close:
  close_dso (dso);
  return 0;

error_out:
  close_dso (dso);
  return 1;
}
