pipe_start(struct proto *P)
{
  struct pipe_config *cf = (struct pipe_config *) P->cf;
  struct pipe_proto *p = (struct pipe_proto *) P;

  /* Lock both tables, unlock is handled in pipe_cleanup() */
  rt_lock_table(P->table);
  rt_lock_table(p->peer_table);

  /* Going directly to PS_UP - prepare for feeding,
     connect the protocol to both routing tables */

  P->main_ahook = proto_add_announce_hook(P, P->table, &P->stats);
  P->main_ahook->out_filter = cf->c.out_filter;
  P->main_ahook->in_limit = cf->c.in_limit;
  proto_reset_limit(P->main_ahook->in_limit);

  p->peer_ahook = proto_add_announce_hook(P, p->peer_table, &p->peer_stats);
  p->peer_ahook->out_filter = cf->c.in_filter;
  p->peer_ahook->in_limit = cf->c.out_limit;
  proto_reset_limit(p->peer_ahook->in_limit);

  if (p->mode == PIPE_OPAQUE)
    {
      P->main_source = rt_get_source(P, 0);
      rt_lock_source(P->main_source);
    }

  return PS_UP;
}
