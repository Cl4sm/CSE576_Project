find_empty_slot_for_expand (htab, hash)
     htab_t htab;
     hashval_t hash;
{
  size_t size = htab->size;
  hashval_t hash2 = 1 + hash % (size - 2);
  unsigned int index = hash % size;

  for (;;)
    {
      void **slot = htab->entries + index;

      if (*slot == EMPTY_ENTRY)
	return slot;
      else if (*slot == DELETED_ENTRY)
	abort ();

      index += hash2;
      if (index >= size)
	index -= size;
    }
}
