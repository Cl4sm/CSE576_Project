static_show_rt(struct static_route *r)
{
  byte via[STD_ADDRESS_P_LENGTH + 16];

  switch (r->dest)
    {
    case RTD_ROUTER:	bsprintf(via, "via %I%J", r->via, r->via_if); break;
    case RTD_DEVICE:	bsprintf(via, "dev %s", r->if_name); break;
    case RTD_BLACKHOLE:	bsprintf(via, "blackhole"); break;
    case RTD_UNREACHABLE: bsprintf(via, "unreachable"); break;
    case RTD_PROHIBIT:	bsprintf(via, "prohibited"); break;
    case RTD_MULTIPATH:	bsprintf(via, "multipath"); break;
    case RTDX_RECURSIVE: bsprintf(via, "recursive %I", r->via); break;
    default:		bsprintf(via, "???");
    }
  cli_msg(-1009, "%I/%d %s%s", r->net, r->masklen, via, r->installed ? "" : " (dormant)");

  struct static_route *r2;
  if (r->dest == RTD_MULTIPATH)
    for (r2 = r->mp_next; r2; r2 = r2->mp_next)
      cli_msg(-1009, "\tvia %I%J weight %d%s", r2->via, r2->via_if, r2->masklen + 1, /* really */
	      r2->installed ? "" : " (dormant)");
}
