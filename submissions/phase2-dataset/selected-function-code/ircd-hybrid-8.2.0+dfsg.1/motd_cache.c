motd_cache(struct Motd *motd)
{
  FILE *file = NULL;
  struct MotdCache *cache = NULL;
  struct stat sb;
  char line[MOTD_LINESIZE + 2];  /* +2 for \r\n */
  char *tmp = NULL;
  unsigned int i = 0;
  dlink_node *ptr = NULL;

  assert(motd);
  assert(motd->path);

  if (motd->cache)
    return motd->cache;

  /* Try to find it in the list of cached files */
  DLINK_FOREACH(ptr, MotdList.cachelist.head)
  {
    cache = ptr->data;

    if (!strcmp(cache->path, motd->path) && cache->maxcount == motd->maxcount)
    {
      cache->ref++;  /* Increase reference count */
      motd->cache = cache;  /* Remember cache */
      return motd->cache;  /* Return it */
    }
  }

  /* Need the file's modification time */
  if (stat(motd->path, &sb) == -1)
  {
    ilog(LOG_TYPE_IRCD, "Couldn't stat \"%s\": %s", motd->path,
         strerror(errno));
    return 0;
  }

  /* Gotta read in the file, now */
  if ((file = fopen(motd->path, "r")) == NULL)
  {
    ilog(LOG_TYPE_IRCD, "Couldn't open \"%s\": %s", motd->path,
         strerror(errno));
    return 0;
  }

  /* Ok, allocate a structure; we'll realloc later to trim memory */
  cache = MyCalloc(sizeof(struct MotdCache) + (MOTD_LINESIZE * MOTD_MAXLINES));
  cache->ref = 1;
  cache->path = xstrdup(motd->path);
  cache->maxcount = motd->maxcount;
  cache->modtime = *localtime((const time_t *)&sb.st_mtime);  /* Store modtime */

  while (cache->count < cache->maxcount && fgets(line, sizeof(line), file))
  {
    /* Copy over line, stopping when we overflow or hit line end */
    for (tmp = line, i = 0; i < (MOTD_LINESIZE - 1) && *tmp && *tmp != '\r' && *tmp != '\n'; ++tmp, ++i)
      cache->motd[cache->count][i] = *tmp;
    cache->motd[cache->count][i] = '\0';

    cache->count++;
  }

  fclose(file);  /* Close the file */

  /* Trim memory usage a little */
  motd->cache = MyCalloc(sizeof(struct MotdCache) +
                         (MOTD_LINESIZE * cache->count));
  memcpy(motd->cache, cache, sizeof(struct MotdCache) +
         (MOTD_LINESIZE * cache->count));
  MyFree(cache);

  /* Now link it in */
  dlinkAdd(motd->cache, &motd->cache->node, &MotdList.cachelist);

  return motd->cache;
}
