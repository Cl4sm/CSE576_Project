find_libs (void **p, void *info)
{
  struct layout_libs *l = (struct layout_libs *) info;
  struct prelink_entry *e = * (struct prelink_entry **) p;

  if ((e->flags & (PCF_ELF64 | PCF_MACHINE)) != l->flags)
    return 1;

  if (e->type == ET_DYN || e->type == ET_EXEC
      || e->type == ET_CACHE_DYN || e->type == ET_CACHE_EXEC)
    l->binlibs[l->nbinlibs++] = e;
  if (e->type == ET_DYN || e->type == ET_CACHE_DYN)
    l->libs[l->nlibs++] = e;
  if (force)
    e->done = 0;
  if (e->type == ET_CACHE_DYN || e->type == ET_CACHE_EXEC)
    e->done = 2;
  if (e->base & (l->max_page_size - 1))
    {
      e->done = 0;
      e->end -= e->base;
      e->base = 0;
    }

  return 1;
}
