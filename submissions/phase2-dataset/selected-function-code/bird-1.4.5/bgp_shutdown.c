bgp_shutdown(struct proto *P)
{
  struct bgp_proto *p = (struct bgp_proto *) P;
  unsigned subcode = 0;

  BGP_TRACE(D_EVENTS, "Shutdown requested");

  switch (P->down_code)
    {
    case PDC_CF_REMOVE:
    case PDC_CF_DISABLE:
      subcode = 3; // Errcode 6, 3 - peer de-configured
      break;

    case PDC_CF_RESTART:
      subcode = 6; // Errcode 6, 6 - other configuration change
      break;

    case PDC_CMD_DISABLE:
    case PDC_CMD_SHUTDOWN:
      subcode = 2; // Errcode 6, 2 - administrative shutdown
      break;

    case PDC_CMD_RESTART:
      subcode = 4; // Errcode 6, 4 - administrative reset
      break;

    case PDC_RX_LIMIT_HIT:
    case PDC_IN_LIMIT_HIT:
      subcode = 1; // Errcode 6, 1 - max number of prefixes reached
      /* log message for compatibility */
      log(L_WARN "%s: Route limit exceeded, shutting down", p->p.name);
      goto limit;

    case PDC_OUT_LIMIT_HIT:
      subcode = proto_restart ? 4 : 2; // Administrative reset or shutdown

    limit:
      bgp_store_error(p, NULL, BE_AUTO_DOWN, BEA_ROUTE_LIMIT_EXCEEDED);
      if (proto_restart)
	bgp_update_startup_delay(p);
      else
	p->startup_delay = 0;
      goto done;
    }

  bgp_store_error(p, NULL, BE_MAN_DOWN, 0);
  p->startup_delay = 0;

 done:
  bgp_stop(p, subcode);
  return p->p.proto_state;
}
