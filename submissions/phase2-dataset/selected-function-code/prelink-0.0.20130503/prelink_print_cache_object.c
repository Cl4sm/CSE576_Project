prelink_print_cache_object (void **p, void *info)
{
  struct prelink_entry *e = * (struct prelink_entry **) p;
  int *psize = (int *) info, i;

  if (e->type == ET_UNPRELINKABLE)
    {
      printf ("%s (not prelinkable)%s\n", e->filename, e->ndepends ? ":" : "");
      for (i = 0; i < e->ndepends; i++)
	if (e->depends[i]->type == ET_UNPRELINKABLE)
	  printf ("    %s (not prelinkable)\n", e->depends[i]->filename);
	else
	  printf ("    %s [0x%08x]\n", e->depends[i]->filename,
		  e->depends[i]->checksum);
      return 1;
    }

  if (e->type == ET_CACHE_DYN)
    printf ("%s [0x%08x] 0x%0*llx-0x%0*llx%s\n", e->filename, e->checksum,
	    *psize, (long long) e->base, *psize, (long long) e->end,
	    e->ndepends ? ":" : "");
  else
    printf ("%s%s\n", e->filename, e->ndepends ? ":" : "");
  for (i = 0; i < e->ndepends; i++)
    printf ("    %s [0x%08x]\n", e->depends[i]->filename,
	    e->depends[i]->checksum);
  return 1;
}
