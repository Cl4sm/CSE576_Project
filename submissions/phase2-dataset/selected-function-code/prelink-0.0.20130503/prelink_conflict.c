struct prelink_conflict *
prelink_conflict (struct prelink_info *info, GElf_Word r_sym,
		  int reloc_type)
{
  GElf_Word symoff = info->symtab_start + r_sym * info->symtab_entsize;
  struct prelink_conflict *conflict;
  int reloc_class = info->dso->arch->reloc_class (reloc_type);
  size_t idx = 0;

  if (info->curconflicts->hash != &info->curconflicts->first)
    idx = symoff % 251;
  for (conflict = info->curconflicts->hash[idx]; conflict;
       conflict = conflict->next)
    if (conflict->symoff == symoff && conflict->reloc_class == reloc_class)
      {
	conflict->used = 1;
	return conflict;
      }

  return NULL;
}
