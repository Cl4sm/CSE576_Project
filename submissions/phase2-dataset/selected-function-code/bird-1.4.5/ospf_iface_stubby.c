static int
ospf_iface_stubby(struct ospf_iface_patt *ip, struct ifa *addr)
{
  /* a host address */
  if (addr->flags & IA_HOST)
    return 1;

  /* a loopback iface */
  if (addr->iface->flags & IF_LOOPBACK)
    return 1;

  /*
   * For compatibility reasons on BSD systems, we force OSPF
   * interfaces with non-primary IP prefixes to be stub.
   */
#if defined(OSPFv2) && !defined(CONFIG_MC_PROPER_SRC)
  if (!ip->bsd_secondary && !(addr->flags & IA_PRIMARY))
    return 1;
#endif

  return ip->stub;
}
