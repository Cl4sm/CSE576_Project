void
ospf_sh_neigh_info(struct ospf_neighbor *n)
{
  struct ospf_iface *ifa = n->ifa;
  char *pos = "other";
  char etime[6];
  int exp, sec, min;

  exp = n->inactim->expires - now;
  sec = exp % 60;
  min = exp / 60;
  if (min > 59)
  {
    bsprintf(etime, "-Inf-");
  }
  else
  {
    bsprintf(etime, "%02u:%02u", min, sec);
  }

  if (n->rid == ifa->drid)
    pos = "dr   ";
  else if (n->rid == ifa->bdrid)
    pos = "bdr  ";
  else if ((n->ifa->type == OSPF_IT_PTP) || (n->ifa->type == OSPF_IT_PTMP) ||
	   (n->ifa->type == OSPF_IT_VLINK))
    pos = "ptp  ";

  cli_msg(-1013, "%-1R\t%3u\t%s/%s\t%-5s\t%-10s %-1I", n->rid, n->priority,
	  ospf_ns[n->state], pos, etime, ifa->ifname, n->ip);
}
