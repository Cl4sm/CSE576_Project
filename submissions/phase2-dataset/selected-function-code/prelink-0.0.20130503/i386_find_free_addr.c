i386_find_free_addr (struct layout_libs *l, Elf32_Addr *ret,
		     Elf32_Addr beg, Elf32_Addr end, Elf32_Addr start)
{
  struct prelink_entry *e;
  Elf32_Addr low, hi;

  ret[0] = beg;
  ret[3] = end;
  for (e = l->list; e != NULL; e = e->next)
    if (e->base >= start)
      break;
  if (e == l->list)
    {
      ret[1] = ret[2] = start;
      return;
    }

  if (e == NULL)
    e = l->list;
  low = start;
  for (e = e->prev; ; e = e->prev)
    {
      if (e->base < beg)
	break;
      if (e->layend > low)
	low = e->base;
      if (e == l->list)
	break;
    }

  if (low == start)
    {
      ret[1] = ret[2] = start;
      return;
    }

  hi = start;
  for (; e; e = e->next)
    {
      if (e->base >= end)
	break;
      if (e->base >= hi)
	break;
      if (e->layend > hi)
	hi = e->layend;
    }

  assert (low >= beg && hi <= end);

  if (hi - start > start - low)
    start = low;
  else
    start = hi;

  ret[1] = ret[2] = start;
}
