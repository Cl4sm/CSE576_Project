static void
pipe_show_stats(struct pipe_proto *p)
{
  struct proto_stats *s1 = &p->p.stats;
  struct proto_stats *s2 = &p->peer_stats;

  /*
   * Pipe stats (as anything related to pipes) are a bit tricky. There
   * are two sets of stats - s1 for ahook to the primary routing and
   * s2 for the ahook to the secondary routing table. The user point
   * of view is that routes going from the primary routing table to
   * the secondary routing table are 'exported', while routes going in
   * the other direction are 'imported'.
   *
   * Each route going through a pipe is, technically, first exported
   * to the pipe and then imported from that pipe and such operations
   * are counted in one set of stats according to the direction of the
   * route propagation. Filtering is done just in the first part
   * (export). Therefore, we compose stats for one directon for one
   * user direction from both import and export stats, skipping
   * immediate and irrelevant steps (exp_updates_accepted,
   * imp_updates_received, imp_updates_filtered, ...).
   *
   * Rule of thumb is that stats s1 have the correct 'polarity'
   * (imp/exp), while stats s2 have switched 'polarity'.
   */

  cli_msg(-1006, "  Routes:         %u imported, %u exported", 
	  s1->imp_routes, s2->imp_routes);
  cli_msg(-1006, "  Route change stats:     received   rejected   filtered    ignored   accepted");
  cli_msg(-1006, "    Import updates:     %10u %10u %10u %10u %10u",
	  s2->exp_updates_received, s2->exp_updates_rejected + s1->imp_updates_invalid,
	  s2->exp_updates_filtered, s1->imp_updates_ignored, s1->imp_updates_accepted);
  cli_msg(-1006, "    Import withdraws:   %10u %10u        --- %10u %10u",
	  s2->exp_withdraws_received, s1->imp_withdraws_invalid,
	  s1->imp_withdraws_ignored, s1->imp_withdraws_accepted);
  cli_msg(-1006, "    Export updates:     %10u %10u %10u %10u %10u",
	  s1->exp_updates_received, s1->exp_updates_rejected + s2->imp_updates_invalid,
	  s1->exp_updates_filtered, s2->imp_updates_ignored, s2->imp_updates_accepted);
  cli_msg(-1006, "    Export withdraws:   %10u %10u        --- %10u %10u",
	  s1->exp_withdraws_received, s2->imp_withdraws_invalid,
	  s2->imp_withdraws_ignored, s2->imp_withdraws_accepted);
}
