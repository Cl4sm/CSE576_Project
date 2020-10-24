static struct find_cxx_sym_cache *
create_cache (DSO *dso, int plt)
{
  Elf_Data *symtab, *strtab;
  Elf_Scn *scn;
  int symsec, strsec, ndx, dndx, maxndx;
  struct find_cxx_sym_cache *cache;
  GElf_Addr top;

  symsec = addr_to_sec (dso, dso->info[DT_SYMTAB]);
  if (symsec == -1)
    return (struct find_cxx_sym_cache *) -1UL;
  scn = dso->scn[symsec];
  symtab = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, symtab) == NULL);
  strsec = addr_to_sec (dso, dso->info[DT_STRTAB]);
  if (strsec == -1)
    return (struct find_cxx_sym_cache *) -1UL;
  scn = dso->scn[strsec];
  strtab = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, strtab) == NULL);
  maxndx = symtab->d_size / dso->shdr[symsec].sh_entsize;

  cache = malloc (sizeof (*cache) + sizeof (cache->vals[0]) * maxndx);
  if (cache == NULL)
    {
      error (0, ENOMEM, "%s: Could load symbol table", dso->filename);
      return NULL;
    }

  cache->symsec = symsec;
  cache->strsec = strsec;
  cache->symtab = symtab;
  cache->strtab = strtab;
  for (ndx = 0, dndx = 0; ndx < maxndx; ++ndx)
    {
      GElf_Sym sym;
      const char *name;
      int k;

      gelfx_getsym (dso->elf, symtab, ndx, &sym);
      if (plt)
	{
	  if (sym.st_shndx != SHN_UNDEF || sym.st_value == 0)
	    continue;
	}
      else if (sym.st_shndx == SHN_UNDEF)
	continue;
      cache->vals[dndx].start = sym.st_value;
      cache->vals[dndx].end = sym.st_value + sym.st_size;
      cache->vals[dndx].idx = ndx;
      cache->vals[dndx].mark = 0;
      name = (const char *) strtab->d_buf + sym.st_name;
      if (!plt && ELF32_ST_VISIBILITY (sym.st_other) == STV_DEFAULT)
	for (k = 0; specials[k].prefix; ++k)
	  if (sym.st_info == specials[k].st_info
	      && strncmp (name, specials[k].prefix,
			  specials[k].prefix_len) == 0)
	    {
	      cache->vals[dndx].mark = 1;
	      break;
	    }
      ++dndx;
    }

  maxndx = dndx;
  qsort (cache->vals, maxndx, sizeof (cache->vals[0]), cachecmp);

  if (!plt)
    {
      for (top = 0, ndx = 0; ndx < maxndx; ++ndx)
	{
	  if (cache->vals[ndx].start < top
	      || (ndx < maxndx - 1
		  && cache->vals[ndx].end > cache->vals[ndx + 1].start))
	    cache->vals[ndx].mark = 0;
	  if (cache->vals[ndx].end > top)
	    top = cache->vals[ndx].end;
	}

      for (ndx = dndx = 0; ndx < maxndx; ++ndx)
	if (cache->vals[ndx].mark)
	  cache->vals[dndx++] = cache->vals[ndx];
    }
  cache->count = dndx;
  return cache;
}
