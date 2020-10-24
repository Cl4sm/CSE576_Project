static int
find_cxx_sym (struct prelink_info *info, GElf_Addr addr,
	      struct find_cxx_sym *fcs, int reloc_size,
	      struct find_cxx_sym_cache **cache)
{
  int n, ndeps = info->ent->ndepends + 1;
  unsigned int hi, lo, mid;
  DSO *dso = NULL;
  struct find_cxx_sym_cache *c;

  if (fcs->dso == NULL
      || addr < fcs->dso->base
      || addr >= fcs->dso->end)
    {
      for (n = 1; n < ndeps; ++n)
	{
	  dso = info->dsos[n];
	  if (addr >= dso->base
	      && addr < dso->end)
	    break;
	}

      if (n == ndeps
	  && addr >= info->dso->base
	  && addr < info->dso->end)
	{
	  n = 0;
	  dso = info->dso;
	}

      assert (n < ndeps);

      if (cache[n] == NULL)
	{
	  cache[n] = create_cache (dso, 0);
	  if (cache[n] == NULL)
	    return -2;
	}
      if (cache[n] == (struct find_cxx_sym_cache *) -1UL)
	return -1;

      fcs->n = n;
      fcs->ent = n ? info->ent->depends[n - 1] : info->ent;
      fcs->dso = dso;
      fcs->cache = cache[n];
      fcs->symsec = fcs->cache->symsec;
      fcs->symtab = fcs->cache->symtab;
      fcs->strsec = fcs->cache->strsec;
      fcs->strtab = fcs->cache->strtab;
      fcs->lastndx = -1;
    }
  else
    dso = fcs->dso;

  c = fcs->cache;
  lo = 0;
  hi = c->count;
  if (fcs->lastndx != -1)
    {
      if (c->vals[fcs->lastndx].start <= addr)
	{
	  lo = fcs->lastndx;
	  if (hi - lo >= 16)
	    {
	      if (c->vals[lo + 2].start > addr)
		hi = lo + 2;
	      else if (c->vals[lo + 15].start > addr)
		hi = lo + 15;
	    }
	}
      else
	{
	  hi = fcs->lastndx;
	  if (hi >= 15)
	    {
	      if (c->vals[hi - 2].start <= addr)
		lo = hi - 2;
	      else if (c->vals[hi - 15].start <= addr)
		lo = hi - 15;
	    }
	}
    }
  while (lo < hi)
    {
      mid = (lo + hi) / 2;
      if (c->vals[mid].start <= addr)
	{
	  if (c->vals[mid].end >= addr + reloc_size)
	    {
	      gelfx_getsym (dso->elf, fcs->symtab, c->vals[mid].idx,
			    &fcs->sym);
	      fcs->lastndx = mid;
	      return c->vals[mid].idx;
	    }
	  lo = mid + 1;
	}
      else
	hi = mid;
    }

  return -1;
}
