prelink_entry_dumpfn (FILE *f, const void *ptr)
{
  struct prelink_entry *e = (struct prelink_entry *) ptr;
  struct prelink_link *l;
  int i;

  fprintf (f, "%s|%s|%s|%x|%x|%llx|%llx|%llx|%llx|%llx|%d|%d|%d|%d|%d|%d|%d|",
	   e->filename,
	   strcmp (e->canon_filename, e->filename) ? e->canon_filename : "",
	   e->soname && strcmp (e->soname, e->filename) ? e->soname : "",
	   e->timestamp, e->checksum,
	   (long long) e->base, (long long) e->end, (long long) e->pltgot,
	   (long long) e->dev, (long long) e->ino,
	   e->type, e->done, e->ndepends, e->refs, e->flags,
	   e->prev ? e->prev->u.tmp : -1, e->next ? e->next->u.tmp : -1);
  for (i = 0; i < e->ndepends; ++i)
    fprintf (f, "%d-", e->depends [i]->u.tmp);
  fputc ('|', f);
  for (l = e->hardlink; l; l = l->next)
    fprintf (f, "%s|", l->canon_filename);
  fputs ("\n", f);
}
