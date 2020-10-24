ospf_iface_info(struct ospf_iface *ifa)
{
  char *more = "";

  if (ifa->strictnbma &&
      ((ifa->type == OSPF_IT_NBMA) || (ifa->type == OSPF_IT_PTMP)))
    more = " (strict)";

  if (ifa->cf->real_bcast &&
      ((ifa->type == OSPF_IT_BCAST) || (ifa->type == OSPF_IT_PTP)))
    more = " (real)";

  if (ifa->type == OSPF_IT_VLINK)
  {
    cli_msg(-1015, "Virtual link %s to %R:", ifa->ifname, ifa->vid);
    cli_msg(-1015, "\tPeer IP: %I", ifa->vip);
    cli_msg(-1015, "\tTransit area: %R (%u)", ifa->voa->areaid, ifa->voa->areaid);
  }
  else
  {
#ifdef OSPFv2
    if (ifa->addr->flags & IA_PEER)
      cli_msg(-1015, "Interface %s (peer %I)", ifa->ifname, ifa->addr->opposite);
    else
      cli_msg(-1015, "Interface %s (%I/%d)", ifa->ifname, ifa->addr->prefix, ifa->addr->pxlen);
#else /* OSPFv3 */
    cli_msg(-1015, "Interface %s (IID %d)", ifa->ifname, ifa->instance_id);
#endif
    cli_msg(-1015, "\tType: %s%s", ospf_it[ifa->type], more);
    cli_msg(-1015, "\tArea: %R (%u)", ifa->oa->areaid, ifa->oa->areaid);
  }
  cli_msg(-1015, "\tState: %s%s", ospf_is[ifa->state], ifa->stub ? " (stub)" : "");
  cli_msg(-1015, "\tPriority: %u", ifa->priority);
  cli_msg(-1015, "\tCost: %u", ifa->cost);
  if (ifa->oa->po->ecmp)
    cli_msg(-1015, "\tECMP weight: %d", ((int) ifa->ecmp_weight) + 1);
  cli_msg(-1015, "\tHello timer: %u", ifa->helloint);

  if (ifa->type == OSPF_IT_NBMA)
  {
    cli_msg(-1015, "\tPoll timer: %u", ifa->pollint);
  }
  cli_msg(-1015, "\tWait timer: %u", ifa->waitint);
  cli_msg(-1015, "\tDead timer: %u", ifa->deadint);
  cli_msg(-1015, "\tRetransmit timer: %u", ifa->rxmtint);
  if ((ifa->type == OSPF_IT_BCAST) || (ifa->type == OSPF_IT_NBMA))
  {
    cli_msg(-1015, "\tDesigned router (ID): %R", ifa->drid);
    cli_msg(-1015, "\tDesigned router (IP): %I", ifa->drip);
    cli_msg(-1015, "\tBackup designed router (ID): %R", ifa->bdrid);
    cli_msg(-1015, "\tBackup designed router (IP): %I", ifa->bdrip);
  }
}
