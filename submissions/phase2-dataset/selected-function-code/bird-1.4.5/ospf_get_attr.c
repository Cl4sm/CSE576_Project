static int
ospf_get_attr(eattr * a, byte * buf, int buflen UNUSED)
{
  switch (a->id)
  {
  case EA_OSPF_METRIC1:
    bsprintf(buf, "metric1");
    return GA_NAME;
  case EA_OSPF_METRIC2:
    bsprintf(buf, "metric2");
    return GA_NAME;
  case EA_OSPF_TAG:
    bsprintf(buf, "tag: 0x%08x", a->u.data);
    return GA_FULL;
  case EA_OSPF_ROUTER_ID:
    bsprintf(buf, "router_id");
    return GA_NAME;
  default:
    return GA_UNKNOWN;
  }
}
