prelink_undo (DSO *dso)
{
  GElf_Ehdr ehdr;
  GElf_Shdr shdr[dso->ehdr.e_shnum + 20], old_shdr[dso->ehdr.e_shnum];
  GElf_Phdr phdr[dso->ehdr.e_phnum];
  Elf_Scn *scn;
  Elf_Data *d;
  int undo, i;
  struct section_move *move;
  struct reloc_info rinfo;

  for (undo = 1; undo < dso->ehdr.e_shnum; ++undo)
    if (! strcmp (strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[undo].sh_name),
		  ".gnu.prelink_undo"))
      break;

  if (undo == dso->ehdr.e_shnum)
    {
      if (undo_output)
	return 0;
      error (0, 0, "%s does not have .gnu.prelink_undo section", dso->filename);
      return 1;
    }

  memcpy (old_shdr, dso->shdr, sizeof (GElf_Shdr) * dso->ehdr.e_shnum);
  move = init_section_move (dso);
  if (move == NULL)
    return 1;

  if (find_reloc_sections (dso, &rinfo))
    goto error_out;

  if (undo_sections (dso, undo, move, &rinfo, &ehdr, phdr, shdr))
    goto error_out;

  if (reopen_dso (dso, move, (undo_output && strcmp (undo_output, "-") == 0)
			     ? "/tmp/undo" : undo_output))
    goto error_out;

  if (find_reloc_sections (dso, &rinfo))
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
	  if (undo_prelink_rel (dso, i))
	    goto error_out;
	  break;
	case SHT_RELA:
	  if (undo_prelink_rela (dso, i))
	    goto error_out;
	  break;
	}
    }

  if (dso->arch->arch_undo_prelink && dso->arch->arch_undo_prelink (dso))
    goto error_out;

  if (dso->ehdr.e_type == ET_DYN)
    {
      GElf_Addr adjust = 0, diff;

      for (i = dso->ehdr.e_shnum - 1; i > 0; --i)
	if (shdr[i].sh_flags & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR))
	  {
	    adjust = shdr[i].sh_addr - dso->shdr[i].sh_addr;
	    break;
	  }
      while (i > 0)
	{
	  int nsec = 1, j;
	  /* Change here PROGBITS .plt into NOBITS if needed.  */

	  /* Convert RELA to REL if needed.  */
	  if (dso->shdr[i].sh_type == SHT_RELA && shdr[i].sh_type == SHT_REL)
	    {
	      assert (dso->arch->rela_to_rel != NULL);
	      if (i == rinfo.plt)
		{
		  if (convert_rela_to_rel (dso, i))
		    goto error_out;
		  dso->shdr[i].sh_size = shdr[i].sh_size;
		}
	      else if (i == rinfo.last)
		{
		  GElf_Addr start = dso->shdr[rinfo.first].sh_addr;

		  for (j = rinfo.first; j <= rinfo.last; ++j)
		    {
		      if (convert_rela_to_rel (dso, j))
			goto error_out;
		      dso->shdr[j].sh_addr = start;
		      dso->shdr[j].sh_size = shdr[j].sh_size;
		      start += dso->shdr[j].sh_size;
		    }
		  nsec = rinfo.last - rinfo.first + 1;
		  i = rinfo.first;
		}
	      else
		{
		  error (0, 0, "%s: Cannot convert RELA to REL", dso->filename);
		  goto error_out;
		}
	    }
	  diff = shdr[i].sh_addr - dso->shdr[i].sh_addr;
	  if (diff != adjust)
	    {
	      assert (diff >= adjust);
	      if (adjust_dso (dso, dso->shdr[i + nsec].sh_addr, adjust - diff))
		goto error_out;
	      adjust = diff;
	    }
	  --i;
	}
      if (adjust && adjust_dso (dso, 0, adjust))
	goto error_out;
      for (i = 1; i < dso->ehdr.e_shnum; ++i)
	if (shdr[i].sh_flags & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR))
	  assert (shdr[i].sh_addr == dso->shdr[i].sh_addr);
    }
  else
    {
      /* Executable.  */
      for (i = 1; i < dso->ehdr.e_shnum; ++i)
	{
	  const char *name = strptr (dso, dso->ehdr.e_shstrndx,
				     dso->shdr[i].sh_name);

	  if (dso->shdr[i].sh_type == SHT_PROGBITS
	      && shdr[i].sh_type == SHT_NOBITS)
	    {
	      assert (strcmp (name, ".bss") == 0
		      || strcmp (name, ".sbss") == 0
		      || strcmp (name, ".plt") == 0
		      || strcmp (name, ".iplt") == 0);
	      scn = dso->scn[i];
	      d = elf_getdata (scn, NULL);
	      assert (d != NULL && elf_getdata (scn, d) == NULL);
	      assert (d->d_size == 0 || d->d_buf != NULL);
	      assert (d->d_size == dso->shdr[i].sh_size);
	      free (d->d_buf);
	      d->d_buf = NULL;
	      dso->shdr[i].sh_type = SHT_NOBITS;
	    }
	  else if (dso->shdr[i].sh_type == SHT_RELA
		   && shdr[i].sh_type == SHT_REL)
	    {
	      if (convert_rela_to_rel (dso, i))
		goto error_out;
	      dso->shdr[i].sh_size = shdr[i].sh_size;
	    }
	  else
	    assert (dso->shdr[i].sh_type == shdr[i].sh_type);
	  if (dso->shdr[i].sh_size != shdr[i].sh_size)
	    {
	      /* This is handled in code below for both ET_DYN and ET_EXEC.  */
	      if (i == dso->ehdr.e_shstrndx)
		continue;
	      assert (shdr[i].sh_type == SHT_NOBITS
		      || shdr[i].sh_size < dso->shdr[i].sh_size);
	      assert (strcmp (name, ".dynstr") == 0
		      || strcmp (name, ".bss") == 0
		      || strcmp (name, ".sbss") == 0);
	      scn = dso->scn[i];
	      d = elf_getdata (scn, NULL);
	      assert (d != NULL && elf_getdata (scn, d) == NULL);
	      d->d_size = shdr[i].sh_size;
	    }
	}

      if (update_dynamic_tags (dso, shdr, old_shdr, move))
	goto error_out;

      for (i = 1; i < dso->ehdr.e_shnum; ++i)
	if (shdr[i].sh_flags & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR))
	  dso->shdr[i].sh_addr = shdr[i].sh_addr;
    }

  /* Clear .dynamic entries added by prelink, update others.  */
  if (remove_dynamic_prelink_tags (dso)
      || update_dynamic_rel (dso, &rinfo))
    goto error_out;

  /* Shrink .shstrtab.  */
  i = dso->ehdr.e_shstrndx;
  if (shdr[i].sh_size < dso->shdr[i].sh_size)
    {
      scn = dso->scn[i];
      d = elf_getdata (scn, NULL);
      assert (d != NULL && elf_getdata (scn, d) == NULL);
      assert (d->d_size == dso->shdr[i].sh_size);
      d->d_size = shdr[i].sh_size;
    }

  /* Now restore the rest.  */
  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    dso->shdr[i] = shdr[i];
  if (dso->ehdr.e_phnum != ehdr.e_phnum)
    {
      assert (ehdr.e_phnum < dso->ehdr.e_phnum);
      if (gelf_newphdr (dso->elf, ehdr.e_phnum) == 0)
	{
	  error (0, 0, "Could not create new ELF headers");
	  goto error_out;
	}
    }
  for (i = 0; i < ehdr.e_phnum; ++i)
    dso->phdr[i] = phdr[i];
  dso->permissive = 1;
  assert (dso->ehdr.e_entry == ehdr.e_entry);
  assert (dso->ehdr.e_shnum == ehdr.e_shnum);
  assert (dso->ehdr.e_shstrndx == ehdr.e_shstrndx);
  dso->ehdr.e_phoff = ehdr.e_phoff;
  dso->ehdr.e_shoff = ehdr.e_shoff;
  dso->ehdr.e_phnum = ehdr.e_phnum;
  free (move);
  return 0;

error_out:
  free (move);
  return 1;
}
