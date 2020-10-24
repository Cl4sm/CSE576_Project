krt_make_tmp_attrs(rte *rt, struct linpool *pool)
{
  struct ea_list *l = lp_alloc(pool, sizeof(struct ea_list) + 2 * sizeof(eattr));

  l->next = NULL;
  l->flags = EALF_SORTED;
  l->count = 2;

  l->attrs[0].id = EA_KRT_SOURCE;
  l->attrs[0].flags = 0;
  l->attrs[0].type = EAF_TYPE_INT | EAF_TEMP;
  l->attrs[0].u.data = rt->u.krt.proto;

  l->attrs[1].id = EA_KRT_METRIC;
  l->attrs[1].flags = 0;
  l->attrs[1].type = EAF_TYPE_INT | EAF_TEMP;
  l->attrs[1].u.data = rt->u.krt.metric;

  return l;
}
