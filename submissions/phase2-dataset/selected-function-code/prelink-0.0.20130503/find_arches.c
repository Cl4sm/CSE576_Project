find_arches (void **p, void *info)
{
  struct layout_libs *l = (struct layout_libs *) info;
  struct prelink_entry *e = * (struct prelink_entry **) p;
  int i;

  if (e->type == ET_DYN || e->type == ET_EXEC
      || e->type == ET_CACHE_DYN || e->type == ET_CACHE_EXEC)
    {
      for (i = 0; i < l->nbinlibs; ++i)
	if ((l->binlibs[i]->flags & (PCF_ELF64 | PCF_MACHINE))
	    == (e->flags & (PCF_ELF64 | PCF_MACHINE)))
	  return 1;

      l->binlibs[l->nbinlibs++] = e;
    }

  return 1;
}
