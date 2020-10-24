get_relocated_mem (struct prelink_info *info, DSO *dso, GElf_Addr addr,
		   char *buf, GElf_Word size, GElf_Addr dest_addr)
{
  int sec = addr_to_sec (dso, addr), j;
  Elf_Scn *scn;
  Elf_Data *data;
  off_t off;

  if (sec == -1)
    return 1;

  memset (buf, 0, size);
  if (dso->shdr[sec].sh_type != SHT_NOBITS)
    {
      scn = dso->scn[sec];
      data = NULL;
      off = addr - dso->shdr[sec].sh_addr;
      while ((data = elf_rawdata (scn, data)) != NULL)
	{
	  if (data->d_off < off + size
	      && data->d_off + data->d_size > off)
	    {
	      off_t off2 = off - data->d_off;
	      size_t len = size;

	      if (off2 < 0)
		{
		  len += off2;
		  off2 = 0;
		}
	      if (off2 + len > data->d_size)
		len = data->d_size - off2;
	      assert (off2 + len <= data->d_size);
	      assert (len <= size);
	      memcpy (buf + off2 - off, data->d_buf + off2, len);
	    }
	}
    }

  if (info->dso != dso)
    {
      /* This is tricky. We need to apply any conflicts
	 against memory area which we've copied to the COPY
	 reloc offset.  */
      for (j = 0; j < info->conflict_rela_size; ++j)
	{
	  int reloc_type, reloc_size, ret;
	  off_t off;

	  if (info->conflict_rela[j].r_offset >= addr + size)
	    continue;
	  if (info->conflict_rela[j].r_offset + dso->arch->max_reloc_size
	      <= addr)
	    continue;

	  reloc_type = GELF_R_TYPE (info->conflict_rela[j].r_info);
	  reloc_size = dso->arch->reloc_size (reloc_type);
	  if (info->conflict_rela[j].r_offset + reloc_size <= addr)
	    continue;

	  off = info->conflict_rela[j].r_offset - addr;

	  /* Check if whole relocation fits into the area.
	     Punt if not.  */
	  if (off < 0 || size - off < reloc_size)
	    return 2;
	  /* Note that apply_conflict_rela shouldn't rely on R_SYM
	     field of conflict to be 0.  */
	  ret
	    = dso->arch->apply_conflict_rela (info, info->conflict_rela + j,
					      buf + off,
					      dest_addr ? dest_addr + off : 0);
	  if (ret)
	    return ret;
	}
    }
  else
    {
      int i, ndx, maxndx;
      int reloc_type, reloc_size;
      union { GElf_Rel rel; GElf_Rela rela; } u;
      off_t off;

      if (addr + size > info->dynbss_base
	  && addr < info->dynbss_base + info->dynbss_size)
	{
	  if (addr < info->dynbss_base
	      || addr + size > info->dynbss_base + info->dynbss_size)
	    return 4;

	  memcpy (buf, info->dynbss + (addr - info->dynbss_base), size);
	  return 0;
	}

      if (addr + size > info->sdynbss_base
	  && addr < info->sdynbss_base + info->sdynbss_size)
	{
	  if (addr < info->sdynbss_base
	      || addr + size > info->sdynbss_base + info->sdynbss_size)
	    return 4;

	  memcpy (buf, info->sdynbss + (addr - info->sdynbss_base), size);
	  return 0;
	}

      for (i = 1; i < dso->ehdr.e_shnum; ++i)
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
	    case SHT_RELA:
	      break;
	    default:
	      continue;
	    }
	  scn = dso->scn[i];
	  data = NULL;
	  while ((data = elf_getdata (scn, data)) != NULL)
	    {
	      maxndx = data->d_size / dso->shdr[i].sh_entsize;
	      for (ndx = 0; ndx < maxndx; ++ndx)
		{
		  if (dso->shdr[i].sh_type == SHT_REL)
		    gelfx_getrel (dso->elf, data, ndx, &u.rel);
		  else
		    gelfx_getrela (dso->elf, data, ndx, &u.rela);

		  if (u.rel.r_offset >= addr + size)
		    continue;
		  if (u.rel.r_offset + dso->arch->max_reloc_size <= addr)
		    continue;

		  reloc_type = GELF_R_TYPE (u.rel.r_info);
		  reloc_size = dso->arch->reloc_size (reloc_type);
		  if (u.rel.r_offset + reloc_size <= addr)
		    continue;

		  if (reloc_type == dso->arch->R_COPY)
		    return 3;

		  off = u.rel.r_offset - addr;

		  /* Check if whole relocation fits into the area.
		     Punt if not.  */
		  if (off < 0 || size - off < reloc_size)
		    return 2;

		  if (dso->shdr[i].sh_type == SHT_REL)
		    dso->arch->apply_rel (info, &u.rel, buf + off);
		  else
		    dso->arch->apply_rela (info, &u.rela, buf + off);
		}
	    }
	}
    }

  return 0;
}
