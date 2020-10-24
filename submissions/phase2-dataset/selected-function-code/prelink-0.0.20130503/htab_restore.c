void
htab_restore (htab, name, restorefn)
     htab_t htab;
     const char *name;
     htab_restorefn restorefn;
{
  FILE *f = fopen (name, "r");
  size_t size, n_elements, n_deleted, i, j, k;
  int c;

  if (f == NULL)
    abort ();
  if (fscanf (f, "size %zd n_elements %zd n_deleted %zd\n",
	      &size, &n_elements, &n_deleted) != 3)
    abort ();
  htab_empty (htab);
  free (htab->entries);
  htab->entries = (void **) calloc (size, sizeof (void *));
  if (htab->entries == NULL)
    abort ();
  htab->size = size;
  htab->n_elements = n_elements;
  htab->n_deleted = n_deleted;
  for (i = 0; i < htab->size; ++i)
    {
      switch ((c = fgetc (f)))
	{
	case 'E':
	case 'D':
	  if (fscanf (f, "%zd\n", &j) != 1)
	    abort ();
	  if (i + j > htab->size)
	    abort ();
	  if (c == 'D')
	    for (k = i; k < i + j; ++k)
	      htab->entries [k] = DELETED_ENTRY;
	  i += j - 1;
	  break;
	case 'V':
	  htab->entries [i] = (*restorefn) (f);
	  break;
	default:
	  abort ();
	}
    }
  fclose (f);
}
