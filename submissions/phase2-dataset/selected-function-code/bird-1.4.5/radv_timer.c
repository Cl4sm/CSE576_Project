radv_timer(timer *tm)
{
  struct radv_iface *ifa = tm->data;
  struct proto_radv *ra = ifa->ra;

  RADV_TRACE(D_EVENTS, "Timer fired on %s", ifa->iface->name);

  radv_send_ra(ifa, 0);

  /* Update timer */
  ifa->last = now;
  unsigned after = ifa->cf->min_ra_int;
  after += random() % (ifa->cf->max_ra_int - ifa->cf->min_ra_int + 1);

  if (ifa->initial)
    ifa->initial--;

  if (ifa->initial)
    after = MIN(after, MAX_INITIAL_RTR_ADVERT_INTERVAL);

  tm_start(ifa->timer, after);
}
