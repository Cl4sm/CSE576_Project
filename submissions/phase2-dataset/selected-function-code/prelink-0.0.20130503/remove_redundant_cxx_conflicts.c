int
remove_redundant_cxx_conflicts (struct prelink_info *info)
{
  int i, j, k, n, o, state, removed = 0;
  int ndx, sec;
  unsigned int hi, lo, mid;
  int reloc_type, reloc_size;
  struct find_cxx_sym fcs1, fcs2;
  char *mem1, *mem2;
  const char *name = NULL, *secname = NULL;
  GElf_Addr symtab_start;
  GElf_Word symoff;
  Elf_Data *binsymtab = NULL;
  int binsymtabsec;
  struct prelink_conflict *conflict;
  struct find_cxx_sym_cache **cache;
  struct find_cxx_sym_cache *binsymcache = NULL;
  int ret = 0;
  int rtype_class_valid;

  /* Don't bother doing this for non-C++ programs.  */
  for (i = 0; i < info->ent->ndepends; ++i)
    if (strstr (info->ent->depends[i]->canon_filename, "libstdc++"))
      break;
  if (i == info->ent->ndepends)
    return 0;

  binsymtabsec = addr_to_sec (info->dso, info->dso->info[DT_SYMTAB]);
  if (binsymtabsec != -1)
    {
      Elf_Scn *scn = info->dso->scn[binsymtabsec];

      binsymtab = elf_getdata (scn, NULL);
      assert (elf_getdata (scn, binsymtab) == NULL);
    }

  rtype_class_valid = info->dso->arch->rtype_class_valid;

  state = 0;
  memset (&fcs1, 0, sizeof (fcs1));
  memset (&fcs2, 0, sizeof (fcs2));
  cache = alloca (sizeof (struct find_cxx_sym_cache *)
		  * (info->ent->ndepends + 1));
  memset (cache, '\0', sizeof (struct find_cxx_sym_cache *)
		       * (info->ent->ndepends + 1));
  for (i = 0; i < info->conflict_rela_size; ++i)
    {
      size_t cidx;

      reloc_type = GELF_R_TYPE (info->conflict_rela[i].r_info);
      reloc_size = info->dso->arch->reloc_size (reloc_type);

      if (GELF_R_SYM (info->conflict_rela[i].r_info) != 0)
	continue;

      if (state
	  && fcs1.sym.st_value <= info->conflict_rela[i].r_offset
	  && fcs1.sym.st_value + fcs1.sym.st_size
	     >= info->conflict_rela[i].r_offset + reloc_size)
	{
	  if (state == 3)
	    goto remove_noref;
	  if (state == 2)
	    goto check_pltref;
	  continue;
	}

      n = find_cxx_sym (info, info->conflict_rela[i].r_offset,
			&fcs1, reloc_size, cache);

      state = 0;
      if (n == -1)
	continue;
      if (n == -2)
	{
	  ret = 1;
	  goto out_free_cache;
	}
      state = 1;
      sec = addr_to_sec (fcs1.dso, fcs1.sym.st_value);
      if (sec == -1)
	continue;
      secname = strptr (fcs1.dso, fcs1.dso->ehdr.e_shstrndx,
			fcs1.dso->shdr[sec].sh_name);
      if (secname == NULL)
	continue;

      name = (const char *) fcs1.strtab->d_buf + fcs1.sym.st_name;

      for (k = 0; specials[k].prefix; ++k)
	if (ELF32_ST_VISIBILITY (fcs1.sym.st_other) == STV_DEFAULT
	    && fcs1.sym.st_info == specials[k].st_info
	    && strncmp (name, specials[k].prefix, specials[k].prefix_len) == 0)
	  break;

      if (specials[k].prefix == NULL)
	continue;

      if (strcmp (secname, ".data") != 0
	  && strcmp (secname, ".data.rel.ro") != 0
	  && strcmp (secname, ".sdata") != 0)
	continue;

      if (specials[k].check_pltref)
	state = 2;

      symtab_start = fcs1.dso->shdr[fcs1.symsec].sh_addr - fcs1.dso->base;
      symoff = symtab_start + n * fcs1.dso->shdr[fcs1.symsec].sh_entsize;

      cidx = 0;
      if (info->conflicts[fcs1.n].hash != &info->conflicts[fcs1.n].first)
	cidx = symoff % 251;
      for (conflict = info->conflicts[fcs1.n].hash[cidx]; conflict;
	   conflict = conflict->next)
	if (conflict->symoff == symoff
	    && conflict->reloc_class == rtype_class_valid)
	  break;

      if (conflict == NULL)
	goto check_pltref;

      if (conflict->conflict.ent != fcs1.ent
	  || fcs1.dso->base + conflict->conflictval != fcs1.sym.st_value)
	goto check_pltref;

      if (verbose > 4)
	error (0, 0, "Possible C++ conflict removal from unreferenced table at %s:%s+%d",
	       fcs1.dso->filename, name,
	       (int) (info->conflict_rela[i].r_offset - fcs1.sym.st_value));

      /* Limit size slightly.  */
      if (fcs1.sym.st_size > 16384)
	goto check_pltref;

      o = find_cxx_sym (info, conflict->lookup.ent->base + conflict->lookupval,
			&fcs2, fcs1.sym.st_size, cache);

      if (o == -2)
	{
	  ret = 1;
	  goto out_free_cache;
	}

      if (o == -1
	  || fcs1.sym.st_size != fcs2.sym.st_size
	  || fcs1.sym.st_info != fcs2.sym.st_info
	  || ELF32_ST_VISIBILITY (fcs2.sym.st_other) != STV_DEFAULT
	  || strcmp (name, (char *) fcs2.strtab->d_buf + fcs2.sym.st_name) != 0)
	goto check_pltref;

      mem1 = malloc (fcs1.sym.st_size * 2);
      if (mem1 == NULL)
	{
	  error (0, ENOMEM, "%s: Could not compare %s arrays",
		 info->dso->filename, name);
	  ret = 1;
	  goto out_free_cache;
	}

      mem2 = mem1 + fcs1.sym.st_size;

      if (get_relocated_mem (info, fcs1.dso, fcs1.sym.st_value, mem1,
			     fcs1.sym.st_size, 0)
	  || get_relocated_mem (info, fcs2.dso, fcs2.sym.st_value, mem2,
				fcs1.sym.st_size, 0)
	  || memcmp (mem1, mem2, fcs1.sym.st_size) != 0)
	{
	  free (mem1);
	  goto check_pltref;
	}

      free (mem1);

      state = 3;

remove_noref:
      if (verbose > 3)
	error (0, 0, "Removing C++ conflict from unreferenced table at %s:%s+%d",
	       fcs1.dso->filename, name,
	       (int) (info->conflict_rela[i].r_offset - fcs1.sym.st_value));

      info->conflict_rela[i].r_info =
	GELF_R_INFO (1, GELF_R_TYPE (info->conflict_rela[i].r_info));
      ++removed;
      continue;

check_pltref:
      /* If the binary calls directly (or takes its address) one of the
	 methods in a virtual table, but doesn't define it, there is no
	 need to leave conflicts in the virtual table which will only
	 slow down the code (as it has to hop through binary's .plt
	 back to the method).  */
      if (state != 2
	  || info->conflict_rela[i].r_addend < info->dso->base
	  || info->conflict_rela[i].r_addend >= info->dso->end
	  || binsymtab == NULL)
	continue;

      if (binsymcache == NULL)
	{
	  binsymcache = create_cache (info->dso, 1);
	  if (binsymcache == NULL)
	    {
	      ret = 1;
	      goto out_free_cache;
	    }
	}
      if (binsymcache == (struct find_cxx_sym_cache *) -1UL)
	continue;

      lo = 0;
      mid = 0;
      hi = binsymcache->count;
      while (lo < hi)
	{
	  mid = (lo + hi) / 2;
	  if (binsymcache->vals[mid].start < info->conflict_rela[i].r_addend)
	    lo = mid + 1;
	  else if (binsymcache->vals[mid].start
		   > info->conflict_rela[i].r_addend)
	    hi = mid;
	  else
	    break;
	}
      if (lo >= hi)
	continue;

      while (mid > 0 && binsymcache->vals[mid - 1].start
			== info->conflict_rela[i].r_addend)
	--mid;

      while (mid < binsymcache->count
	     && binsymcache->vals[mid].start
		== info->conflict_rela[i].r_addend)
	{
	  GElf_Sym sym;

	  ndx = binsymcache->vals[mid].idx;
	  mid++;
	  gelfx_getsym (info->dso->elf, binsymtab, ndx, &sym);
	  assert (sym.st_value == info->conflict_rela[i].r_addend);
	  if (sym.st_shndx == SHN_UNDEF && sym.st_value)
	    {
	      struct prelink_symbol *s;
	      size_t maxidx, l;

	      if (verbose > 4)
		error (0, 0, "Possible C++ conflict removal due to reference to binary's .plt at %s:%s+%d",
		       fcs1.dso->filename, name,
		       (int) (info->conflict_rela[i].r_offset
			      - fcs1.sym.st_value));

	      for (s = &info->symbols[ndx]; s; s = s->next)
		if (s->reloc_class == RTYPE_CLASS_PLT)
		  break;

	      if (s == NULL)
		break;

	      maxidx = 1;
	      if (info->conflicts[fcs1.n].hash
		  != &info->conflicts[fcs1.n].first)
		{
		  if (info->conflicts[fcs1.n].hash2 == NULL)
		    {
		      info->conflicts[fcs1.n].hash2
			= calloc (sizeof (struct prelink_conflict *), 251);
		      if (info->conflicts[fcs1.n].hash2 != NULL)
			{
			  for (l = 0; l < 251; l++)
			    for (conflict = info->conflicts[fcs1.n].hash[l];
				 conflict; conflict = conflict->next)
			      if (conflict->reloc_class == rtype_class_valid
				  && conflict->conflict.ent)
				{
				  size_t ccidx
				    = (conflict->lookup.ent->base
				       + conflict->lookupval) % 251;
				  conflict->next2
				    = info->conflicts[fcs1.n].hash2[ccidx];
				  info->conflicts[fcs1.n].hash2[ccidx]
				    = conflict;
				}
			}
		    }
		  if (info->conflicts[fcs1.n].hash2 != NULL)
		    {
		      size_t ccidx = info->conflict_rela[i].r_addend % 251;
		      for (conflict = info->conflicts[fcs1.n].hash2[ccidx];
			   conflict; conflict = conflict->next2)
			if (conflict->lookup.ent->base + conflict->lookupval
			    == info->conflict_rela[i].r_addend
			    && (conflict->conflict.ent->base
				+ conflict->conflictval
				== s->u.ent->base + s->value))
			  goto pltref_remove;
		      break;
		    }
		  maxidx = 251;
		}

	      for (l = 0; l < maxidx; l++)
		for (conflict = info->conflicts[fcs1.n].hash[l];
		     conflict; conflict = conflict->next)
		  if (conflict->lookup.ent->base + conflict->lookupval
		      == info->conflict_rela[i].r_addend
		      && conflict->conflict.ent
		      && (conflict->conflict.ent->base
			  + conflict->conflictval == s->u.ent->base + s->value)
		      && conflict->reloc_class == rtype_class_valid)
		    {
pltref_remove:
		      if (verbose > 3)
			error (0, 0, "Removing C++ conflict due to reference to binary's .plt at %s:%s+%d",
			       fcs1.dso->filename, name,
			       (int) (info->conflict_rela[i].r_offset
				      - fcs1.sym.st_value));

		      info->conflict_rela[i].r_info =
			GELF_R_INFO (1, GELF_R_TYPE (info->conflict_rela[i].r_info));
		      ++removed;
		      goto pltref_check_done;
		    }

pltref_check_done:
	      break;
	    }
	}
    }

  if (removed)
    {
      for (i = 0, j = 0; i < info->conflict_rela_size; ++i)
	if (GELF_R_SYM (info->conflict_rela[i].r_info) == 0)
	  {
	    if (i != j)
	      info->conflict_rela[j] = info->conflict_rela[i];
	    ++j;
	  }
      info->conflict_rela_size = j;
    }

out_free_cache:
  for (i = 0; i < info->ent->ndepends + 1; i++)
    if (cache[i] && cache[i] != (struct find_cxx_sym_cache *) -1UL)
      free (cache[i]);
  if (binsymcache && binsymcache != (struct find_cxx_sym_cache *) -1UL)
    free (binsymcache);
  return ret;
}
