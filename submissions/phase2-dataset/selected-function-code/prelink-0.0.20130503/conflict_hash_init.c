conflict_hash_init (struct prelink_conflicts *conflicts)
{
  struct prelink_conflict **hash
    = calloc (sizeof (struct prelink_conflict *), 251);
  struct prelink_conflict *conflict, *next;
  size_t idx;

  if (hash == NULL)
    return;

  for (conflict = conflicts->first; conflict; conflict = next)
    {
      next = conflict->next;
      idx = conflict->symoff % 251;
      conflict->next = hash[idx];
      hash[idx] = conflict;
    }
  conflicts->hash = hash;
}
