prelink_entry_restore (htab_t htab, const char *filename)
{
  size_t i, j;
  struct prelink_entry *e;

  prelink_entry_count = 0;
  htab_restore (htab, filename, prelink_entry_restorefn);
  free (restore_line);
  for (i = 0; i < htab->size; ++i)
    if (htab->entries [i] && htab->entries [i] != (void *) 1)
      {
	e = (struct prelink_entry *) htab->entries [i];
	if (e->prev == (void *) -1)
	  e->prev = NULL;
	else
	  e->prev = (struct prelink_entry *)
		    htab->entries [(long) e->prev];
	if (e->next == (void *) -1)
	  e->next = NULL;
	else
	  e->next = (struct prelink_entry *)
		    htab->entries [(long) e->next];
	for (j = 0; j < e->ndepends; ++j)
	  {
	    e->depends [j] = (struct prelink_entry *)
			     htab->entries [(long) e->depends [j]];
	  }
      }
}
