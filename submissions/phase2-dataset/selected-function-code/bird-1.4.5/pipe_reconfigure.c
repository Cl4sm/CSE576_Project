static int
pipe_reconfigure(struct proto *P, struct proto_config *new)
{
  struct pipe_proto *p = (struct pipe_proto *)P;
  struct proto_config *old = P->cf;
  struct pipe_config *oc = (struct pipe_config *) old;
  struct pipe_config *nc = (struct pipe_config *) new;

  if ((oc->peer->table != nc->peer->table) || (oc->mode != nc->mode))
    return 0;

  /* Update output filters in ahooks */
  if (P->main_ahook)
    {
      P->main_ahook->out_filter = new->out_filter;
      P->main_ahook->in_limit = new->in_limit;
      proto_verify_limits(P->main_ahook);
    }

  if (p->peer_ahook)
    {
      p->peer_ahook->out_filter = new->in_filter;
      p->peer_ahook->in_limit = new->out_limit;
      proto_verify_limits(p->peer_ahook);
    }

  if ((P->proto_state != PS_UP) || (proto_reconfig_type == RECONFIG_SOFT))
    return 1;
  
  if ((new->preference != old->preference)
      || ! filter_same(new->in_filter, old->in_filter)
      || ! filter_same(new->out_filter, old->out_filter))
    proto_request_feeding(P);

  return 1;
}
