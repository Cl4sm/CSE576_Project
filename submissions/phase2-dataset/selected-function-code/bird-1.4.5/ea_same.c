int
ea_same(ea_list *x, ea_list *y)
{
  int c;

  if (!x || !y)
    return x == y;
  ASSERT(!x->next && !y->next);
  if (x->count != y->count)
    return 0;
  for(c=0; c<x->count; c++)
    {
      eattr *a = &x->attrs[c];
      eattr *b = &y->attrs[c];

      if (a->id != b->id ||
	  a->flags != b->flags ||
	  a->type != b->type ||
	  ((a->type & EAF_EMBEDDED) ? a->u.data != b->u.data : !adata_same(a->u.ptr, b->u.ptr)))
	return 0;
    }
  return 1;
}
