proto_want_core_up(struct proto *p)
{
  ASSERT(p->core_state == FS_HUNGRY);

  if (!p->proto->multitable)
    {
      p->main_source = rt_get_source(p, 0);
      rt_lock_source(p->main_source);

      /* Connect protocol to routing table */
      p->main_ahook = proto_add_announce_hook(p, p->table, &p->stats);
      p->main_ahook->in_filter = p->cf->in_filter;
      p->main_ahook->out_filter = p->cf->out_filter;
      p->main_ahook->rx_limit = p->cf->rx_limit;
      p->main_ahook->in_limit = p->cf->in_limit;
      p->main_ahook->out_limit = p->cf->out_limit;
      p->main_ahook->in_keep_filtered = p->cf->in_keep_filtered;

      proto_reset_limit(p->main_ahook->rx_limit);
      proto_reset_limit(p->main_ahook->in_limit);
      proto_reset_limit(p->main_ahook->out_limit);
    }

  p->core_state = FS_HAPPY;
  proto_relink(p);
}
