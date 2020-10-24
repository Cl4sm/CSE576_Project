ppc_layout_libs_pre (struct layout_libs *l)
{
  Elf32_Addr mmap_start = l->mmap_start - REG1S;
  Elf32_Addr first_start = REG0S, last_start = REG2S;
  struct prelink_entry *e, e0, *next = NULL;
  struct ppc_layout_data *pld;
  int cnt;

  if (l->max_page_size > 0x10000)
    error (EXIT_FAILURE, 0, "--layout-page-size too large");

  mmap_start = REG0E - (mmap_start & 0xff0000);
  for (cnt = 0, e = l->list; e != NULL; e = e->next, ++cnt)
    {
      if (e->base < mmap_start && e->layend > mmap_start)
	mmap_start = (e->layend + 0xffff) & ~0xffff;
      if (e->base < REG0S && e->layend > REG0S && first_start > e->base)
	first_start = e->base;
      if (e->base < REG0E && e->layend > REG2S && last_start < e->layend)
	last_start = e->layend;
    }
  if (mmap_start > REG0E)
    mmap_start = REG0E;

  pld = calloc (sizeof (*pld) + cnt * sizeof (pld->ents[0]), 1);
  if (pld == NULL)
    error (EXIT_FAILURE, ENOMEM, "Cannot lay libraries out");

  l->arch_data = pld;
  memset (&e0, 0, sizeof (e0));
  e0.prev = &e0;
  pld->cnt = cnt;
  pld->e[0].u.tmp = -1;
  pld->e[0].base = REG1S + REG0E - mmap_start;
  pld->e[0].end = pld->e[0].base;
  pld->e[0].layend = pld->e[0].end;
  pld->e[0].prev = &pld->e[0];
  pld->e[1].u.tmp = -1;
  pld->e[1].base = pld->e[0].end + mmap_start - REG0S;
  pld->e[1].end = pld->e[1].base;
  pld->e[1].layend = pld->e[1].end;
  pld->e[1].prev = &pld->e[1];
  pld->e[2].u.tmp = -1;
  pld->e[2].base = pld->e[1].end + first_start - REG1S;
  pld->e[2].end = pld->e[1].base;
  pld->e[2].layend = pld->e[2].end;
  pld->e[2].prev = &pld->e[2];
  for (cnt = 0, e = l->list; e != NULL; e = next, ++cnt)
    {
      next = e->next;
      pld->ents[cnt].e = e;
      pld->ents[cnt].base = e->base;
      pld->ents[cnt].end = e->end;
      pld->ents[cnt].layend = e->layend;
      if (e->layend <= REG0S)
	{
	  if (e->base < REG1S)
	    e->base = REG1S;
	  else if (e->base > first_start)
	    e->base = first_start;
	  if (e->layend < REG1S)
	    e->layend = REG1S;
	  else if (e->layend > first_start)
	    e->layend = first_start;
	  e->base += pld->e[1].end - REG1S;
	  e->layend += pld->e[1].end - REG1S;
	  list_append (&pld->e[1], e);
	}
      else if (e->base < mmap_start)
	{
	  if (e->base < REG0S)
	    e->base = REG0S;
	  if (e->layend > mmap_start)
	    e->layend = mmap_start;
	  e->base = pld->e[0].end + mmap_start - e->layend;
	  e->layend = pld->e[0].layend + mmap_start - pld->ents[cnt].base;
	  list_append (&pld->e[0], e);
	}
      else if (e->base < REG0E)
	{
	  if (e->layend > REG0E)
	    e->layend = REG0E;
	  e->base = REG1S + REG0E - e->layend;
	  e->layend = REG1S + REG0E - pld->ents[cnt].base;
	  list_append (&e0, e);
	}
      else if (e->layend >= last_start)
	{
	  if (e->base < last_start)
	    e->base = last_start;
	  e->base += pld->e[2].end - last_start;
	  e->layend += pld->e[2].end - last_start;
	  list_append (&pld->e[2], e);
	}
      e->end = e->layend;
    }

  list_sort (&pld->e[0]);
  if (e0.next == NULL)
    l->list = &pld->e[0];
  else
    {
      list_sort (&e0);
      l->list = e0.next;
      l->list->prev = pld->e[0].prev;
      e0.prev->next = &pld->e[0];
      pld->e[0].prev = e0.prev;
    }

  e0.prev = l->list->prev;
  l->list->prev = pld->e[1].prev;
  e0.prev->next = &pld->e[1];
  pld->e[1].prev = e0.prev;

  e0.prev = l->list->prev;
  l->list->prev = pld->e[2].prev;
  e0.prev->next = &pld->e[2];
  pld->e[2].prev = e0.prev;

  pld->mmap_start = mmap_start;
  pld->first_start = first_start;
  pld->last_start = last_start;

  l->mmap_start = REG1S;
  l->mmap_fin = pld->e[2].end + REG2E - last_start;
  l->mmap_end = l->mmap_fin;
  l->fakecnt = 3;
  l->fake = pld->e;

  return 0;
}
