proto_show_stats(struct proto_stats *s, int in_keep_filtered)
{
  if (in_keep_filtered)
    cli_msg(-1006, "  Routes:         %u imported, %u filtered, %u exported, %u preferred", 
	    s->imp_routes, s->filt_routes, s->exp_routes, s->pref_routes);
  else
    cli_msg(-1006, "  Routes:         %u imported, %u exported, %u preferred", 
	    s->imp_routes, s->exp_routes, s->pref_routes);

  cli_msg(-1006, "  Route change stats:     received   rejected   filtered    ignored   accepted");
  cli_msg(-1006, "    Import updates:     %10u %10u %10u %10u %10u",
	  s->imp_updates_received, s->imp_updates_invalid,
	  s->imp_updates_filtered, s->imp_updates_ignored,
	  s->imp_updates_accepted);
  cli_msg(-1006, "    Import withdraws:   %10u %10u        --- %10u %10u",
	  s->imp_withdraws_received, s->imp_withdraws_invalid,
	  s->imp_withdraws_ignored, s->imp_withdraws_accepted);
  cli_msg(-1006, "    Export updates:     %10u %10u %10u        --- %10u",
	  s->exp_updates_received, s->exp_updates_rejected,
	  s->exp_updates_filtered, s->exp_updates_accepted);
  cli_msg(-1006, "    Export withdraws:   %10u        ---        ---        --- %10u",
	  s->exp_withdraws_received, s->exp_withdraws_accepted);
}
