proto_request_feeding(struct proto *p)
{
  ASSERT(p->proto_state == PS_UP);

  /* Do nothing if we are still waiting for feeding */
  if (p->export_state == ES_DOWN)
    return;

  /* If we are already feeding, we want to restart it */
  if (p->export_state == ES_FEEDING)
    {
      /* Unless feeding is in initial state */
      if (p->attn->hook == proto_feed_initial)
	return;

      rt_feed_baby_abort(p);
    }

  /* FIXME: This should be changed for better support of multitable protos */
  struct announce_hook *ah;
  for (ah = p->ahooks; ah; ah = ah->next)
    proto_reset_limit(ah->out_limit);

  /* Hack: reset exp_routes during refeed, and do not decrease it later */
  p->stats.exp_routes = 0;

  proto_schedule_feed(p, 0);
  proto_log_state_change(p);
}
