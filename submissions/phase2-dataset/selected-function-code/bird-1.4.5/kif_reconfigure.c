kif_reconfigure(struct proto *p, struct proto_config *new)
{
  struct kif_config *o = (struct kif_config *) p->cf;
  struct kif_config *n = (struct kif_config *) new;

  if (!kif_sys_reconfigure((struct kif_proto *) p, n, o))
    return 0;

  if (o->scan_time != n->scan_time)
    {
      tm_stop(kif_scan_timer);
      kif_scan_timer->recurrent = n->scan_time;
      kif_scan(kif_scan_timer);
      tm_start(kif_scan_timer, n->scan_time);
    }

  if (!EMPTY_LIST(o->primary) || !EMPTY_LIST(n->primary))
    {
      /* This is hack, we have to update a configuration
       * to the new value just now, because it is used
       * for recalculation of primary addresses.
       */
      p->cf = new;

      ifa_recalc_all_primary_addresses();
    }

  return 1;
}
