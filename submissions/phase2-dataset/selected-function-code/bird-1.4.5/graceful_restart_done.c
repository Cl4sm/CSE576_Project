static void
graceful_restart_done(struct timer *t UNUSED)
{
  struct proto *p;
  node *n;

  log(L_INFO "Graceful restart done");
  graceful_restart_state = GRS_DONE;

  WALK_LIST2(p, n, proto_list, glob_node)
    {
      if (!p->gr_recovery)
	continue;

      /* Resume postponed export of routes */
      if ((p->proto_state == PS_UP) && p->gr_wait)
      {
	proto_want_export_up(p);
	proto_log_state_change(p);
      }

      /* Cleanup */
      p->gr_recovery = 0;
      p->gr_wait = 0;
      p->gr_lock = 0;
    }

  graceful_restart_locks = 0;
}
