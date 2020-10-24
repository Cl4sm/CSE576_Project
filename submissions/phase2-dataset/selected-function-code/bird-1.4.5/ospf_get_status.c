ospf_get_status(struct proto *p, byte * buf)
{
  struct proto_ospf *po = (struct proto_ospf *) p;

  if (p->proto_state == PS_DOWN)
    buf[0] = 0;
  else
  {
    struct ospf_iface *ifa;
    struct ospf_neighbor *n;
    int adj = 0;

    WALK_LIST(ifa, po->iface_list)
      WALK_LIST(n, ifa->neigh_list) if (n->state == NEIGHBOR_FULL)
      adj = 1;

    if (adj == 0)
      strcpy(buf, "Alone");
    else
      strcpy(buf, "Running");
  }
}
