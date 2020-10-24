static int
ospf_start(struct proto *p)
{
  struct proto_ospf *po = (struct proto_ospf *) p;
  struct ospf_config *c = (struct ospf_config *) (p->cf);
  struct ospf_area_config *ac;

  po->router_id = proto_get_router_id(p->cf);
  po->rfc1583 = c->rfc1583;
  po->stub_router = c->stub_router;
  po->merge_external = c->merge_external;
  po->ebit = 0;
  po->ecmp = c->ecmp;
  po->tick = c->tick;
  po->disp_timer = tm_new(p->pool);
  po->disp_timer->data = po;
  po->disp_timer->randomize = 0;
  po->disp_timer->hook = ospf_disp;
  po->disp_timer->recurrent = po->tick;
  tm_start(po->disp_timer, 1);
  po->lsab_size = 256;
  po->lsab_used = 0;
  po->lsab = mb_alloc(p->pool, po->lsab_size);
  po->nhpool = lp_new(p->pool, 12*sizeof(struct mpnh));
  init_list(&(po->iface_list));
  init_list(&(po->area_list));
  fib_init(&po->rtf, p->pool, sizeof(ort), 0, ospf_rt_initort);
  po->areano = 0;
  po->gr = ospf_top_new(p->pool);
  s_init_list(&(po->lsal));

  WALK_LIST(ac, c->area_list)
    ospf_area_add(po, ac, 0);

  if (c->abr)
    ospf_open_vlink_sk(po);

  /* Add all virtual links */
  struct ospf_iface_patt *ic;
  WALK_LIST(ic, c->vlink_list)
    ospf_iface_new_vlink(po, ic);

  return PS_UP;
}
