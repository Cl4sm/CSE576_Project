prelink_load_cache (void)
{
  int fd, i, j;
  struct stat64 st;
  struct prelink_cache *cache;
  struct prelink_entry **ents;
  size_t cache_size;
  uint32_t string_start, *dep;

  fd = open (prelink_cache, O_RDONLY);
  if (fd < 0)
    return 0; /* The cache does not exist yet.  */

  if (fstat64 (fd, &st) < 0
      || st.st_size == 0)
    {
      close (fd);
      return 0;
    }

  cache = mmap (0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (cache == MAP_FAILED)
    error (EXIT_FAILURE, errno, "mmap of prelink cache file failed.");
  cache_size = st.st_size;
  if (memcmp (cache->magic, PRELINK_CACHE_MAGIC,
	      sizeof (PRELINK_CACHE_MAGIC) - 1))
    {
      if (memcmp (cache->magic, PRELINK_CACHE_NAME,
		  sizeof (PRELINK_CACHE_NAME) - 1))
	error (EXIT_FAILURE, 0, "%s: is not prelink cache file",
	       prelink_cache);
      munmap (cache, cache_size);
      return 0;
    }
  dep = (uint32_t *) & cache->entry[cache->nlibs];
  string_start = ((long) dep) - ((long) cache)
		 + cache->ndeps * sizeof (uint32_t);
  ents = (struct prelink_entry **)
	 alloca (cache->nlibs * sizeof (struct prelink_entry *));
  memset (ents, 0, cache->nlibs * sizeof (struct prelink_entry *));
  for (i = 0; i < cache->nlibs; i++)
    {
      /* Sanity checks.  */
      if (cache->entry[i].filename < string_start
	  || cache->entry[i].filename >= string_start + cache->len_strings
	  || cache->entry[i].depends >= cache->ndeps)
	error (EXIT_FAILURE, 0, "%s: bogus prelink cache file",
	       prelink_cache);

      ents[i] = prelink_load_entry (((char *) cache)
				    + cache->entry[i].filename);
    }

  for (i = 0; i < cache->nlibs; i++)
    {
      if (ents[i] == NULL)
	continue;

      if (ents[i]->type != ET_NONE)
	continue;

      ents[i]->checksum = cache->entry[i].checksum;
      ents[i]->base = cache->entry[i].base;
      ents[i]->end = cache->entry[i].end;
      ents[i]->type = (ents[i]->base == 0 && ents[i]->end == 0)
		      ? ET_CACHE_EXEC : ET_CACHE_DYN;
      ents[i]->flags = cache->entry[i].flags;

      if (ents[i]->flags == PCF_UNPRELINKABLE)
	ents[i]->type = (quick || print_cache) ? ET_UNPRELINKABLE : ET_NONE;

      /* If mtime is equal to ctime, assume the filesystem does not store
	 ctime.  */
      if (quick
	  && ((ents[i]->ctime == ents[i]->mtime
	       && ents[i]->type != ET_UNPRELINKABLE)
	      || ents[i]->ctime != cache->entry[i].ctime
	      || ents[i]->mtime != cache->entry[i].mtime))
	ents[i]->type = ET_NONE;

      for (j = cache->entry[i].depends; dep[j] != i; ++j)
	if (dep[j] >= cache->nlibs)
	  error (EXIT_FAILURE, 0, "%s: bogus prelink cache file",
		 prelink_cache);
	else if (ents[dep[j]] == NULL)
	  ents[i]->type = ET_NONE;

      if (ents[i]->type == ET_NONE)
	continue;

      ents[i]->ndepends = j - cache->entry[i].depends;
      if (ents[i]->ndepends)
	{
	  ents[i]->depends =
	    (struct prelink_entry **)
	    malloc (ents[i]->ndepends * sizeof (struct prelink_entry *));
	  if (ents[i]->depends == NULL)
	    error (EXIT_FAILURE, ENOMEM, "Cannot read cache file %s",
		   prelink_cache);

	  for (j = 0; j < ents[i]->ndepends; ++j)
	    ents[i]->depends[j] = ents[dep[cache->entry[i].depends + j]];
	}
    }

  if (quick)
    {
      qsort (ents, cache->nlibs, sizeof (struct prelink_entry *), deps_cmp);
      for (i = 0; i < cache->nlibs; ++i)
	{
	  if (ents[i] == NULL || ents[i]->type == ET_NONE)
	    continue;

	  for (j = 0; j < ents[i]->ndepends; ++j)
	    if (ents[i]->depends[j]->type == ET_NONE)
	      {
		ents[i]->type = ET_NONE;
		free (ents[i]->depends);
		ents[i]->depends = NULL;
		ents[i]->ndepends = 0;
		break;
	      }
	}
    }

  munmap (cache, cache_size);
  close (fd);
  return 0;
}
