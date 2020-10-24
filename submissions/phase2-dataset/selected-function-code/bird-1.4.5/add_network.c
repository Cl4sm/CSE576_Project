static void
add_network(struct ospf_area *oa, ip_addr px, int pxlen, int metric, struct top_hash_entry *en, int pos)
{
  orta nf = {
    .type = RTS_OSPF,
    .options = 0,
    .metric1 = metric,
    .metric2 = LSINFINITY,
    .tag = 0,
    .rid = en->lsa.rt,
    .oa = oa,
    .nhs = en->nhs
  };

  if (pxlen < 0 || pxlen > MAX_PREFIX_LENGTH)
  {
    log(L_WARN "%s: Invalid prefix in LSA (Type: %04x, Id: %R, Rt: %R)",
	oa->po->proto.name, en->lsa.type, en->lsa.id, en->lsa.rt);
    return;
  }

  if (en == oa->rt)
  {
    /* 
     * Local stub networks does not have proper iface in en->nhi
     * (because they all have common top_hash_entry en).
     * We have to find iface responsible for that stub network.
     * Configured stubnets does not have any iface. They will
     * be removed in rt_sync().
     */

    struct ospf_iface *ifa;
#ifdef OSPFv2
    ifa = rt_pos_to_ifa(oa, pos);
#else /* OSPFv3 */
    ifa = px_pos_to_ifa(oa, pos);
#endif

    nf.nhs = ifa ? new_nexthop(oa->po, IPA_NONE, ifa->iface, ifa->ecmp_weight) : NULL;
  }

  ri_install_net(oa->po, px, pxlen, &nf);
}
