static void
ospf_get_route_info(rte * rte, byte * buf, ea_list * attrs UNUSED)
{
  char *type = "<bug>";

  switch (rte->attrs->source)
  {
    case RTS_OSPF:
      type = "I";
      break;
    case RTS_OSPF_IA:
      type = "IA";
      break;
    case RTS_OSPF_EXT1:
      type = "E1";
      break;
    case RTS_OSPF_EXT2:
      type = "E2";
      break;
  }

  buf += bsprintf(buf, " %s", type);
  buf += bsprintf(buf, " (%d/%d", rte->pref, rte->u.ospf.metric1);
  if (rte->attrs->source == RTS_OSPF_EXT2)
    buf += bsprintf(buf, "/%d", rte->u.ospf.metric2);
  buf += bsprintf(buf, ")");
  if ((rte->attrs->source == RTS_OSPF_EXT1 || rte->attrs->source == RTS_OSPF_EXT2) && rte->u.ospf.tag)
  {
    buf += bsprintf(buf, " [%x]", rte->u.ospf.tag);
  }
  if (rte->u.ospf.router_id)
    buf += bsprintf(buf, " [%R]", rte->u.ospf.router_id);
}
