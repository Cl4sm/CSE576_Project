prelink_entry_restorefn (FILE *f)
{
  struct prelink_entry *e;
  struct prelink_link **plink;
  char *p, *q, *s;
  long long ll[5];
  int ii[5];
  int i;

  if (getline (&restore_line, &restore_size, f) < 0)
    abort ();
  e = (struct prelink_entry *) calloc (1, sizeof (struct prelink_entry));
  if (e == NULL)
    abort ();
  p = restore_line;
  q = strchr (p, '|');
  s = malloc (q - p + 1);
  memcpy (s, p, q - p);
  s [q - p] = '\0';
  e->filename = s;
  ++q;
  p = q;
  if (*p == '|')
    e->canon_filename = strdup (e->filename);
  else
    {
      q = strchr (p, '|');
      s = malloc (q - p + 1);
      memcpy (s, p, q - p);
      s [q - p] = '\0';
      e->canon_filename = s;
    }
  ++q;
  p = q;
  if (*p == '|')
    e->soname = strdup (e->filename);
  else
    {
      q = strchr (p, '|');
      s = malloc (q - p + 1);
      memcpy (s, p, q - p);
      s [q - p] = '\0';
      e->soname = s;
    }
  p = q + 1;
  if (sscanf (p, "%x|%x|%llx|%llx|%llx|%llx|%llx|%d|%d|%d|%d|%d|%d|%d|%n",
	      ii, ii + 1, ll, ll + 1, ll + 2, ll + 3, ll + 4,
	      &e->type, &e->done, &e->ndepends, &e->refs, &e->flags,
	      ii + 2, ii + 3, ii + 4) < 14)
    abort ();
  e->timestamp = ii[0];
  e->checksum = ii[1];
  e->base = ll[0];
  e->end = ll[1];
  e->pltgot = ll[2];
  e->dev = ll[3];
  e->ino = ll[4];
  e->prev = (void *) (long) ii[2];
  e->next = (void *) (long) ii[3];
  e->depends = (struct prelink_entry **)
	       malloc (e->ndepends * sizeof (struct prelink_entry *));
  p += ii[4];
  for (i = 0; i < e->ndepends; ++i)
    {
      e->depends [i] = (void *) strtol (p, &q, 0);
      if (p == q || *q != '-')
	abort ();
      p = q + 1;
    }
  if (*p++ != '|')
    abort ();
  plink = &e->hardlink;
  while (*p != '\n')
    {
      struct prelink_link *link = (struct prelink_link *)
				  malloc (sizeof (struct prelink_link));
      q = strchr (p, '|');
      *plink = link;
      plink = &link->next;
      s = malloc (q - p + 1);
      memcpy (s, p, q - p);
      s [q - p] = '\0';
      e->soname = s;
      link->canon_filename = s;
      p = q + 1;
    }
  *plink = NULL;
  ++prelink_entry_count;
  return e;
}
