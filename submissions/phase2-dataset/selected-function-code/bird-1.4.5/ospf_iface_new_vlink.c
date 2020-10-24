ospf_iface_new_vlink(struct proto_ospf *po, struct ospf_iface_patt *ip)
{
  struct proto *p = &po->proto;
  struct ospf_iface *ifa;
  struct pool *pool;

  if (!po->vlink_sk)
    return;

  OSPF_TRACE(D_EVENTS, "Adding vlink to %R via area %R", ip->vid, ip->voa);

  /* Vlink ifname is stored just after the ospf_iface structure */

  pool = rp_new(p->pool, "OSPF Vlink");
  ifa = mb_allocz(pool, sizeof(struct ospf_iface) + 16);
  ifa->oa = po->backbone;
  ifa->cf = ip;
  ifa->pool = pool;

  /* Assign iface ID, for vlinks, this is ugly hack */
  u32 vlink_id = po->last_vlink_id++;
  ifa->iface_id = vlink_id + OSPF_VLINK_ID_OFFSET;
  ifa->ifname = (void *) (ifa + 1);
  bsprintf(ifa->ifname, "vlink%d", vlink_id);

  ifa->voa = ospf_find_area(po, ip->voa);
  ifa->vid = ip->vid;
  ifa->sk = po->vlink_sk;

  ifa->helloint = ip->helloint;
  ifa->rxmtint = ip->rxmtint;
  ifa->waitint = ip->waitint;
  ifa->deadint = ip->deadint;
  ifa->inftransdelay = ip->inftransdelay;
  ifa->tx_length = OSPF_VLINK_MTU;

#ifdef OSPFv2
  ifa->autype = ip->autype;
  ifa->passwords = ip->passwords;
#endif

#ifdef OSPFv3
  ifa->instance_id = ip->instance_id;
#endif

  ifa->type = OSPF_IT_VLINK;

  ifa->state = OSPF_IS_DOWN;
  init_list(&ifa->neigh_list);
  init_list(&ifa->nbma_list);

  add_tail(&po->iface_list, NODE ifa);

  ifa->hello_timer = tm_new_set(ifa->pool, hello_timer_hook, ifa, 0, ifa->helloint);
}
