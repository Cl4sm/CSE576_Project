void **
htab_find_slot_with_hash (htab, element, hash, insert)
     htab_t htab;
     const void * element;
     hashval_t hash;
     enum insert_option insert;
{
  void **first_deleted_slot;
  unsigned int index;
  hashval_t hash2;
  size_t size;

  if (insert == INSERT && htab->size * 3 <= htab->n_elements * 4
      && htab_expand (htab) == 0)
    return NULL;

  size = htab->size;
  hash2 = 1 + hash % (size - 2);
  index = hash % size;

  htab->searches++;
  first_deleted_slot = NULL;

  for (;;)
    {
      void * entry = htab->entries[index];
      if (entry == EMPTY_ENTRY)
	{
	  if (insert == NO_INSERT)
	    return NULL;

	  htab->n_elements++;

	  if (first_deleted_slot)
	    {
	      *first_deleted_slot = EMPTY_ENTRY;
	      return first_deleted_slot;
	    }

	  return &htab->entries[index];
	}

      if (entry == DELETED_ENTRY)
	{
	  if (!first_deleted_slot)
	    first_deleted_slot = &htab->entries[index];
	}
      else  if ((*htab->eq_f) (entry, element))
	return &htab->entries[index];

      htab->collisions++;
      index += hash2;
      if (index >= size)
	index -= size;
    }
}
