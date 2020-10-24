static int
ospf_reconfigure(struct proto *p, struct proto_config *c)
{
  struct proto_ospf *po = (struct proto_ospf *) p;
  struct ospf_config *old = (struct ospf_config *) (p->cf);
  struct ospf_config *new = (struct ospf_config *) c;
  struct ospf_area_config *nac;
  struct ospf_area *oa, *oax;
  struct ospf_iface *ifa, *ifx;
  struct ospf_iface_patt *ip;

  if (proto_get_router_id(c) != po->router_id)
    return 0;

  if (po->rfc1583 != new->rfc1583)
    return 0;

  if (old->abr != new->abr)
    return 0;

  po->stub_router = new->stub_router;
  po->merge_external = new->merge_external;
  po->ecmp = new->ecmp;
  po->tick = new->tick;
  po->disp_timer->recurrent = po->tick;
  tm_start(po->disp_timer, 1);

  /* Mark all areas and ifaces */
  WALK_LIST(oa, po->area_list)
    oa->marked = 1;

  WALK_LIST(ifa, po->iface_list)
    ifa->marked = 1;

  /* Add and update areas */
  WALK_LIST(nac, new->area_list)
  {
    oa = ospf_find_area(po, nac->areaid);
    if (oa)
      ospf_area_reconfigure(oa, nac);
    else
      ospf_area_add(po, nac, 1);
  }

  /* Add and update vlinks */
  WALK_LIST(ip, new->vlink_list)
  {
    ifa = ospf_find_vlink(po, ip->voa, ip->vid);
    if (ifa)
      ospf_iface_reconfigure(ifa, ip);
    else
      ospf_iface_new_vlink(po, ip);
  }

  /* Delete remaining ifaces and areas */
  WALK_LIST_DELSAFE(ifa, ifx, po->iface_list)
    if (ifa->marked)
    {
      ospf_iface_shutdown(ifa);
      ospf_iface_remove(ifa);
    }

  WALK_LIST_DELSAFE(oa, oax, po->area_list)
    if (oa->marked)
      ospf_area_remove(oa);

  schedule_rtcalc(po);
  
  return 1;
}
