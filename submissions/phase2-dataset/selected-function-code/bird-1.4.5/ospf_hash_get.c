ospf_hash_get(struct top_graph *f, u32 domain, u32 lsa, u32 rtr, u32 type)
{
  struct top_hash_entry **ee;
  struct top_hash_entry *e;

  ee = f->hash_table + ospf_top_hash(f, domain, lsa, rtr, type);
  e = *ee;

  while (e && (e->lsa.id != lsa || e->lsa.rt != rtr || e->lsa.type != type || e->domain != domain))
    e = e->next;

  if (e)
    return e;

  e = sl_alloc(f->hash_slab);
  e->color = OUTSPF;
  e->dist = LSINFINITY;
  e->nhs = NULL;
  e->lb = IPA_NONE;
  e->lsa.id = lsa;
  e->lsa.rt = rtr;
  e->lsa.type = type;
  e->lsa_body = NULL;
  e->domain = domain;
  e->next = *ee;
  *ee = e;
  if (f->hash_entries++ > f->hash_entries_max)
    ospf_top_rehash(f, HASH_HI_STEP);
  return e;
}
