int
prelink_save_cache (int do_warn)
{
  struct prelink_cache cache;
  struct collect_ents l;
  struct prelink_cache_entry *data;
  uint32_t *deps, ndeps = 0, i, j, k;
  char *strings;
  int fd, len;
  struct prelink_entry *ents_array[prelink_entry_count];

  memset (&cache, 0, sizeof (cache));
  memcpy ((char *) & cache, PRELINK_CACHE_MAGIC,
	  sizeof (PRELINK_CACHE_MAGIC) - 1);
  l.ents = ents_array;
  l.nents = 0;
  l.ndeps = 0;
  l.len_strings = 0;
  htab_traverse (prelink_filename_htab, find_ents, &l);
  cache.nlibs = l.nents;
  cache.ndeps = l.ndeps;
  cache.len_strings = l.len_strings;

  len = cache.nlibs * sizeof (struct prelink_cache_entry)
	+ cache.ndeps * sizeof (uint32_t) + cache.len_strings;
  char data_buf[len];
  data = (struct prelink_cache_entry *) data_buf;
  deps = (uint32_t *) & data[cache.nlibs];
  strings = (char *) & deps[cache.ndeps];

  for (i = 0; i < l.nents; ++i)
    {
      data[i].filename = (strings - (char *) data) + sizeof (cache);
      strings = stpcpy (strings, l.ents[i]->canon_filename) + 1;
      data[i].checksum = l.ents[i]->checksum;
      data[i].flags = l.ents[i]->flags & ~PCF_PRELINKED;
      data[i].ctime = l.ents[i]->ctime;
      data[i].mtime = l.ents[i]->mtime;
      if (l.ents[i]->type == ET_EXEC || l.ents[i]->type == ET_CACHE_EXEC)
	{
	  data[i].base = 0;
	  data[i].end = 0;
	}
      else if (l.ents[i]->type == ET_UNPRELINKABLE)
	{
	  data[i].base = 0;
	  data[i].end = 0;
	  data[i].checksum = 0;
	  data[i].flags = PCF_UNPRELINKABLE;
	}
      else
	{
	  data[i].base = l.ents[i]->base;
	  data[i].end = l.ents[i]->end;
	}
    }

  for (i = 0; i < cache.nlibs; i++)
    {
      data[i].depends = ndeps;
      for (j = 0; j < l.ents[i]->ndepends; j++)
	{
	  for (k = 0; k < cache.nlibs; k++)
	    if (l.ents[k] == l.ents[i]->depends[j])
	      break;
	  if (k == cache.nlibs)
	    abort ();
	  deps[ndeps++] = k;
	}
      deps[ndeps++] = i;
    }

  size_t prelink_cache_len = strlen (prelink_cache);
  char prelink_cache_tmp [prelink_cache_len + sizeof (".XXXXXX")];
  memcpy (mempcpy (prelink_cache_tmp, prelink_cache, prelink_cache_len),
	  ".XXXXXX", sizeof (".XXXXXX"));
  fd = mkstemp (prelink_cache_tmp);
  if (fd < 0)
    {
      error (0, errno, "Could not write prelink cache");
      return 1;
    }

  if (write (fd, &cache, sizeof (cache)) != sizeof (cache)
      || write (fd, data, len) != len
      || fchmod (fd, 0644)
      || fsync (fd)
      || close (fd)
      || rename (prelink_cache_tmp, prelink_cache))
    {
      error (0, errno, "Could not write prelink cache");
      unlink (prelink_cache_tmp);
      return 1;
    }
  return 0;
}
