htab_expand (htab)
     htab_t htab;
{
  void **oentries;
  void **olimit;
  void **p;

  oentries = htab->entries;
  olimit = oentries + htab->size;

  htab->size = higher_prime_number (htab->size * 2);

  if (htab->return_allocation_failure)
    {
      void **nentries = (void **) calloc (htab->size, sizeof (void **));
      if (nentries == NULL)
	return 0;
      htab->entries = nentries;
    }

  htab->n_elements -= htab->n_deleted;
  htab->n_deleted = 0;

  p = oentries;
  do
    {
      void * x = *p;

      if (x != EMPTY_ENTRY && x != DELETED_ENTRY)
	{
	  void **q = find_empty_slot_for_expand (htab, (*htab->hash_f) (x));

	  *q = x;
	}

      p++;
    }
  while (p < olimit);

  free (oentries);
  return 1;
}
