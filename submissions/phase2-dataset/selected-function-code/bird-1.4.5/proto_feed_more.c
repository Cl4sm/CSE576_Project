static void
proto_feed_more(void *P)
{
  struct proto *p = P;

  if (p->export_state != ES_FEEDING)
    return;

  DBG("Feeding protocol %s continued\n", p->name);
  if (rt_feed_baby(p))
    {
      DBG("Feeding protocol %s finished\n", p->name);
      p->export_state = ES_READY;
      proto_log_state_change(p);

      if (p->feed_done)
	p->feed_done(p);
    }
  else
    {
      p->attn->hook = proto_feed_more;
      ev_schedule(p->attn);		/* Will continue later... */
    }
}
