bdr_election(struct ospf_iface *ifa)
{
  struct proto_ospf *po = ifa->oa->po;
  u32 myid = po->router_id;
  struct ospf_neighbor *neigh, *ndr, *nbdr, me;
  int doadj;

  DBG("(B)DR election.\n");

  me.state = NEIGHBOR_2WAY;
  me.rid = myid;
  me.priority = ifa->priority;
  me.ip = ifa->addr->ip;

#ifdef OSPFv2
  me.dr = ipa_to_u32(ifa->drip);
  me.bdr = ipa_to_u32(ifa->bdrip);
#else /* OSPFv3 */
  me.dr = ifa->drid;
  me.bdr = ifa->bdrid;
  me.iface_id = ifa->iface_id;
#endif

  add_tail(&ifa->neigh_list, NODE & me);

  nbdr = electbdr(ifa->neigh_list);
  ndr = electdr(ifa->neigh_list);

  if (ndr == NULL)
    ndr = nbdr;

  /* 9.4. (4) */
  if (((ifa->drid == myid) && (ndr != &me))
      || ((ifa->drid != myid) && (ndr == &me))
      || ((ifa->bdrid == myid) && (nbdr != &me))
      || ((ifa->bdrid != myid) && (nbdr == &me)))
  {
#ifdef OSPFv2
    me.dr = ndr ? ipa_to_u32(ndr->ip) : 0;
    me.bdr = nbdr ? ipa_to_u32(nbdr->ip) : 0;
#else /* OSPFv3 */
    me.dr = ndr ? ndr->rid : 0;
    me.bdr = nbdr ? nbdr->rid : 0;
#endif

    nbdr = electbdr(ifa->neigh_list);
    ndr = electdr(ifa->neigh_list);

    if (ndr == NULL)
      ndr = nbdr;
  }

  u32 odrid = ifa->drid;
  u32 obdrid = ifa->bdrid;
 
  ifa->drid = ndr ? ndr->rid : 0;
  ifa->drip = ndr ? ndr->ip  : IPA_NONE;
  ifa->bdrid = nbdr ? nbdr->rid : 0;
  ifa->bdrip = nbdr ? nbdr->ip  : IPA_NONE;

#ifdef OSPFv3
  ifa->dr_iface_id = ndr ? ndr->iface_id : 0;
#endif

  DBG("DR=%R, BDR=%R\n", ifa->drid, ifa->bdrid);

  doadj = ((ifa->drid != odrid) || (ifa->bdrid != obdrid));

  if (myid == ifa->drid)
    ospf_iface_chstate(ifa, OSPF_IS_DR);
  else
  {
    if (myid == ifa->bdrid)
      ospf_iface_chstate(ifa, OSPF_IS_BACKUP);
    else
      ospf_iface_chstate(ifa, OSPF_IS_DROTHER);
  }

  rem_node(NODE & me);

  if (doadj)
  {
    WALK_LIST(neigh, ifa->neigh_list)
    {
      ospf_neigh_sm(neigh, INM_ADJOK);
    }
  }
}
