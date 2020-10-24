free_info (struct prelink_info *info)
{
  int i;

  free (info->symtab);
  free (info->dynbss);
  free (info->sdynbss);
  free (info->conflict_rela);
  if (info->conflicts)
    {
      for (i = 0; i < info->ent->ndepends + 1; ++i)
	{
	  if (info->conflicts[i].hash == &info->conflicts[i].first)
	    {
	      struct prelink_conflict *c = info->conflicts[i].first;
	      void *f;

	      while (c != NULL)
		{
		  f = c;
		  c = c->next;
		  free (f);
		}
	    }
	  else
	    {
	      int j;
	      for (j = 0; j < 251; j++)
	        {
		  struct prelink_conflict *c = info->conflicts[i].hash[j];
		  void *f;

		  while (c != NULL)
		    {
		      f = c;
		      c = c->next;
		      free (f);
		    }
		}
	      free (info->conflicts[i].hash);
	    }
	  if (info->conflicts[i].hash2 != NULL)
	    free (info->conflicts[i].hash2);
	}
      free (info->conflicts);
    }
  if (info->sonames)
    {
      for (i = 0; i < info->ent->ndepends + 1; ++i)
	free ((char *) info->sonames[i]);
      free (info->sonames);
    }
  free (info->tls);
  if (info->symbols)
    {
      for (i = 0; i < info->symbol_count; ++i)
	{
	  struct prelink_symbol *s = info->symbols[i].next;
	  void *f;

	  while (s != NULL)
	    {
	      f = s;
	      s = s->next;
	      free (f);
	    }
	}
      free (info->symbols);
    }
}
