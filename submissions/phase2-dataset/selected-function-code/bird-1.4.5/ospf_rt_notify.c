ospf_rt_notify(struct proto *p, rtable *tbl UNUSED, net * n, rte * new, rte * old UNUSED, ea_list * attrs)
{
  struct proto_ospf *po = (struct proto_ospf *) p;
  struct ospf_area *oa = ospf_main_area(po);
  ort *nf = (ort *) fib_get(&po->rtf, &n->n.prefix, n->n.pxlen);
  struct fib_node *fn = &nf->fn;

  if (!new)
  {
    if (fn->x1 != EXT_EXPORT)
      return;

    flush_ext_lsa(oa, fn, oa_is_nssa(oa));

    /* Old external route might blocked some NSSA translation */
    if (po->areano > 1)
      schedule_rtcalc(po);

    return;
  }

  /* Get route attributes */
  u32 m1 = ea_get_int(attrs, EA_OSPF_METRIC1, LSINFINITY);
  u32 m2 = ea_get_int(attrs, EA_OSPF_METRIC2, 10000);
  u32 metric = (m1 != LSINFINITY) ? m1 : (m2 | LSA_EXT_EBIT);
  u32 tag = ea_get_int(attrs, EA_OSPF_TAG, 0);
  ip_addr gw = IPA_NONE;
  // FIXME check for gw should be per ifa, not per iface
  if ((new->attrs->dest == RTD_ROUTER) &&
      ipa_nonzero(new->attrs->gw) &&
      !ipa_has_link_scope(new->attrs->gw) &&
      (ospf_iface_find((struct proto_ospf *) p, new->attrs->iface) != NULL))
    gw = new->attrs->gw;

  originate_ext_lsa(oa, fn, EXT_EXPORT, metric, gw, tag, 1);
}
