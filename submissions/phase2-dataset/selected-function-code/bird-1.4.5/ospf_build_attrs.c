ospf_build_attrs(ea_list * next, struct linpool *pool, u32 m1, u32 m2,
		 u32 tag, u32 rid)
{
  struct ea_list *l =
    lp_alloc(pool, sizeof(struct ea_list) + 4 * sizeof(eattr));

  l->next = next;
  l->flags = EALF_SORTED;
  l->count = 4;
  l->attrs[0].id = EA_OSPF_METRIC1;
  l->attrs[0].flags = 0;
  l->attrs[0].type = EAF_TYPE_INT | EAF_TEMP;
  l->attrs[0].u.data = m1;
  l->attrs[1].id = EA_OSPF_METRIC2;
  l->attrs[1].flags = 0;
  l->attrs[1].type = EAF_TYPE_INT | EAF_TEMP;
  l->attrs[1].u.data = m2;
  l->attrs[2].id = EA_OSPF_TAG;
  l->attrs[2].flags = 0;
  l->attrs[2].type = EAF_TYPE_INT | EAF_TEMP;
  l->attrs[2].u.data = tag;
  l->attrs[3].id = EA_OSPF_ROUTER_ID;
  l->attrs[3].flags = 0;
  l->attrs[3].type = EAF_TYPE_ROUTER_ID | EAF_TEMP;
  l->attrs[3].u.data = rid;
  return l;
}
