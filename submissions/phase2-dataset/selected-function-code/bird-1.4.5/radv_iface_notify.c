void
radv_iface_notify(struct radv_iface *ifa, int event)
{
  struct proto_radv *ra = ifa->ra;

  if (!ifa->sk)
    return;

  RADV_TRACE(D_EVENTS, "Event %s on %s", ev_name[event], ifa->iface->name);

  switch (event)
  {
  case RA_EV_CHANGE:
    ifa->plen = 0;
  case RA_EV_INIT:
    ifa->initial = MAX_INITIAL_RTR_ADVERTISEMENTS;
    break;

  case RA_EV_RS:
    break;
  }

  /* Update timer */
  unsigned delta = now - ifa->last;
  unsigned after = 0;

  if (delta < ifa->cf->min_delay)
    after = ifa->cf->min_delay - delta;

  tm_start(ifa->timer, after);
}
