void
htab_dump (htab, name, dumpfn)
     htab_t htab;
     const char *name;
     htab_dumpfn dumpfn;
{
  FILE *f = fopen (name, "w");
  size_t i, j;

  if (f == NULL)
    abort ();
  fprintf (f, "size %zd n_elements %zd n_deleted %zd\n",
	   htab->size, htab->n_elements, htab->n_deleted);
  for (i = 0; i < htab->size; ++i)
    {
      if (htab->entries [i] == EMPTY_ENTRY
	  || htab->entries [i] == DELETED_ENTRY)
	{
	  for (j = i + 1; j < htab->size; ++j)
	    if (htab->entries [j] != htab->entries [i])
	      break;
	  fprintf (f, "%c%zd\n",
		   htab->entries [i] == EMPTY_ENTRY ? 'E' : 'D',
		   j - i);
	  i = j - 1;
	}
      else
	{
	  fputc ('V', f);
	  (*dumpfn) (f, htab->entries [i]);
	}
    }
  fclose (f);
}
