bgp_get_route_info(rte *e, byte *buf, ea_list *attrs)
{
  eattr *p = ea_find(attrs, EA_CODE(EAP_BGP, BA_AS_PATH));
  eattr *o = ea_find(attrs, EA_CODE(EAP_BGP, BA_ORIGIN));
  u32 origas;

  buf += bsprintf(buf, " (%d", e->pref);

  if (e->u.bgp.suppressed)
    buf += bsprintf(buf, "-");

  if (e->attrs->hostentry)
    {
      if (!rte_resolvable(e))
	buf += bsprintf(buf, "/-");
      else if (e->attrs->igp_metric >= IGP_METRIC_UNKNOWN)
	buf += bsprintf(buf, "/?");
      else
	buf += bsprintf(buf, "/%d", e->attrs->igp_metric);
    }
  buf += bsprintf(buf, ") [");

  if (p && as_path_get_last(p->u.ptr, &origas))
    buf += bsprintf(buf, "AS%u", origas);
  if (o)
    buf += bsprintf(buf, "%c", "ie?"[o->u.data]);
  strcpy(buf, "]");
}
