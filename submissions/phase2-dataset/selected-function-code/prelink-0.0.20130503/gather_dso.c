static int
gather_dso (DSO *dso, struct prelink_entry *ent)
{
  int prelinked;

  if (verbose > 5)
    printf ("Checking shared library %s\n", ent->canon_filename);

  if (dso->ehdr.e_type != ET_DYN)
    {
      error (0, 0, "%s is not a shared library", ent->filename);
      close_dso (dso);
      return 1;
    }

  prelinked = (dynamic_info_is_set (dso, DT_GNU_PRELINKED_BIT)
	       && dynamic_info_is_set (dso, DT_CHECKSUM_BIT));
  ent->timestamp = dso->info_DT_GNU_PRELINKED;
  ent->checksum = dso->info_DT_CHECKSUM;
  ent->base = dso->base;
  ent->end = dso->end;
  if (dso->arch->need_rel_to_rela != NULL && ! prelinked)
    {
      /* If the library has not been prelinked yet and we need
	 to convert REL to RELA, then make room for it.  */
      struct reloc_info rinfo;
      GElf_Addr adjust = 0;
      int sec = dso->ehdr.e_shnum;

      if (find_reloc_sections (dso, &rinfo))
	{
	  close_dso (dso);
	  return 1;
	}

      assert (sizeof (Elf32_Rel) * 3 == sizeof (Elf32_Rela) * 2);
      assert (sizeof (Elf64_Rel) * 3 == sizeof (Elf64_Rela) * 2);
      if (rinfo.rel_to_rela)
	{
	  sec = rinfo.first;
	  adjust = (dso->shdr[rinfo.last].sh_addr
		    + dso->shdr[rinfo.last].sh_size
		    - dso->shdr[rinfo.first].sh_addr) / 2;
	}
      if (rinfo.rel_to_rela_plt)
	{
	  if (rinfo.plt < sec)
	    sec = rinfo.plt;
	  adjust += dso->shdr[rinfo.plt].sh_size / 2;
	}
      if (adjust)
	{
	  int align = 0, i, last;
	  GElf_Addr start;

	  for (i = rinfo.plt ? rinfo.plt : rinfo.first;
	       i < dso->ehdr.e_shnum; i++)
	    {
	      if (dso->shdr[i].sh_addralign > align)
		align = dso->shdr[i].sh_addralign;
	    }

	  if (rinfo.plt)
	    start = dso->shdr[rinfo.plt].sh_addr
		    + dso->shdr[rinfo.plt].sh_size;
	  else
	    start = dso->shdr[rinfo.first].sh_addr
		    + dso->shdr[rinfo.first].sh_size;

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
		    && (((dso->phdr[last].p_vaddr + dso->phdr[last].p_memsz
			  - 1) ^ dso->phdr[i].p_vaddr)
			& ~(dso->arch->max_page_size - 1))
		    && !(((dso->phdr[last].p_vaddr + dso->phdr[last].p_memsz
			   + adjust - 1)
			  ^ (dso->phdr[i].p_vaddr + adjust))
			 & ~(dso->arch->max_page_size - 1)))
		  {
		    if (align >= dso->arch->max_page_size)
		      {
			error (0, 0, "%s: Cannot grow reloc sections",
			       ent->filename);
			close_dso (dso);
			return 1;
		      }
		    adjust = (adjust + dso->arch->max_page_size - 1)
			     & ~(dso->arch->max_page_size - 1);
		  }
		last = i;
	      }

	  ent->end += adjust;
	}
    }

  if (gather_deps (dso, ent))
    return 1;

  if (ent->done && ! prelinked && ! undo)
    ent->done = 0;
  ent->type = ET_DYN;
  return 0;
}
