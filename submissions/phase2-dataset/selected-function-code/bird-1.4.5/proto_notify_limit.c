void
proto_notify_limit(struct announce_hook *ah, struct proto_limit *l, int dir, u32 rt_count)
{
  const char *dir_name[PLD_MAX] = { "receive", "import" , "export" };
  const byte dir_down[PLD_MAX] = { PDC_RX_LIMIT_HIT, PDC_IN_LIMIT_HIT, PDC_OUT_LIMIT_HIT };
  struct proto *p = ah->proto;

  if (l->state == PLS_BLOCKED)
    return;

  /* For warning action, we want the log message every time we hit the limit */
  if (!l->state || ((l->action == PLA_WARN) && (rt_count == l->limit)))
    log(L_WARN "Protocol %s hits route %s limit (%d), action: %s",
	p->name, dir_name[dir], l->limit, proto_limit_name(l));

  switch (l->action)
    {
    case PLA_WARN:
      l->state = PLS_ACTIVE;
      break;

    case PLA_BLOCK:
      l->state = PLS_BLOCKED;
      break;

    case PLA_RESTART:
    case PLA_DISABLE:
      l->state = PLS_BLOCKED;
      if (p->proto_state == PS_UP)
	proto_schedule_down(p, l->action == PLA_RESTART, dir_down[dir]);
      break;
    }
}
