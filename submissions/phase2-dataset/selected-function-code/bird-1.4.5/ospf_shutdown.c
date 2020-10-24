static int
ospf_shutdown(struct proto *p)
{
  struct proto_ospf *po = (struct proto_ospf *) p;
  struct ospf_iface *ifa;
  OSPF_TRACE(D_EVENTS, "Shutdown requested");

  /* And send to all my neighbors 1WAY */
  WALK_LIST(ifa, po->iface_list)
    ospf_iface_shutdown(ifa);

  /* Cleanup locked rta entries */
  FIB_WALK(&po->rtf, nftmp)
  {
    rta_free(((ort *) nftmp)->old_rta);
  }
  FIB_WALK_END;

  return PS_DOWN;
}
