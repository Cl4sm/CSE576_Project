static inline ea_list *
ea_list_copy(ea_list *o)
{
  ea_list *n;
  unsigned i, len;

  if (!o)
    return NULL;
  ASSERT(!o->next);
  len = sizeof(ea_list) + sizeof(eattr) * o->count;
  n = mb_alloc(rta_pool, len);
  memcpy(n, o, len);
  n->flags |= EALF_CACHED;
  for(i=0; i<o->count; i++)
    {
      eattr *a = &n->attrs[i];
      if (!(a->type & EAF_EMBEDDED))
	{
	  unsigned size = sizeof(struct adata) + a->u.ptr->length;
	  struct adata *d = mb_alloc(rta_pool, size);
	  memcpy(d, a->u.ptr, size);
	  a->u.ptr = d;
	}
    }
  return n;
}
