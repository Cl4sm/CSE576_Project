void
bgp_update_startup_delay(struct bgp_proto *p)
{
  struct bgp_config *cf = p->cf;

  DBG("BGP: Updating startup delay\n");

  if (p->last_proto_error && ((now - p->last_proto_error) >= (int) cf->error_amnesia_time))
    p->startup_delay = 0;

  p->last_proto_error = now;

  if (cf->disable_after_error)
    {
      p->startup_delay = 0;
      p->p.disabled = 1;
      return;
    }

  if (!p->startup_delay)
    p->startup_delay = cf->error_delay_time_min;
  else
    p->startup_delay = MIN(2 * p->startup_delay, cf->error_delay_time_max);
}
