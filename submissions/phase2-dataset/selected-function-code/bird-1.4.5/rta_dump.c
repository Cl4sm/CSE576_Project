rta_dump(rta *a)
{
  static char *rts[] = { "RTS_DUMMY", "RTS_STATIC", "RTS_INHERIT", "RTS_DEVICE",
			 "RTS_STAT_DEV", "RTS_REDIR", "RTS_RIP",
			 "RTS_OSPF", "RTS_OSPF_IA", "RTS_OSPF_EXT1",
                         "RTS_OSPF_EXT2", "RTS_BGP" };
  static char *rtc[] = { "", " BC", " MC", " AC" };
  static char *rtd[] = { "", " DEV", " HOLE", " UNREACH", " PROHIBIT" };

  debug("p=%s uc=%d %s %s%s%s h=%04x",
	a->src->proto->name, a->uc, rts[a->source], ip_scope_text(a->scope), rtc[a->cast],
	rtd[a->dest], a->hash_key);
  if (!(a->aflags & RTAF_CACHED))
    debug(" !CACHED");
  debug(" <-%I", a->from);
  if (a->dest == RTD_ROUTER)
    debug(" ->%I", a->gw);
  if (a->dest == RTD_DEVICE || a->dest == RTD_ROUTER)
    debug(" [%s]", a->iface ? a->iface->name : "???" );
  if (a->eattrs)
    {
      debug(" EA: ");
      ea_dump(a->eattrs);
    }
}
