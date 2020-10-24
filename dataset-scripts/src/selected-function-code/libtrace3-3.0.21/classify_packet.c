static void classify_packet(struct tcp_opts opts, struct opt_counter *counts) {
	if (!opts.mss && !opts.sack && !opts.winscale && !opts.ts && !opts.ttcp && !opts.other)
	{
		counts->no_options ++;
		return;
	}

	if (opts.mss && !opts.sack && !opts.winscale && !opts.ts && !opts.ttcp && !opts.other)
	{
		counts->mss_only ++;
		return;
	}

	if (!opts.mss && !opts.sack && !opts.winscale && opts.ts && !opts.ttcp && !opts.other)
	{
		counts->ts_only ++;
		return;
	}

	if (opts.mss && opts.sack && !opts.winscale && !opts.ts)
		counts->ms ++;

	if (opts.mss && opts.winscale && !opts.sack && !opts.ts)
		counts->mw ++;

	if (opts.mss && opts.winscale && opts.sack && !opts.ts)
		counts->msw ++;
	
	if (opts.mss && opts.ts && !opts.winscale && !opts.sack)
		counts->mt ++;
	if (opts.ts && opts.sack && !opts.mss && !opts.winscale)
		counts->ts_and_sack ++;

	if (opts.ts && opts.winscale && !opts.mss && !opts.sack)
		counts->wt ++;

	if (opts.ts && opts.mss && opts.winscale && !opts.sack)
		counts->tmw ++;
	if (opts.ts && opts.mss && opts.sack && !opts.winscale)
		counts->tms ++;
	if (opts.ts && opts.sack && opts.winscale && !opts.mss)
		counts->tws ++;
	
	
	if (opts.mss && opts.sack && opts.winscale && opts.ts) {
		counts->all_four ++;
	}

	if (opts.ts && (opts.mss || opts.winscale || opts.sack)) {
		counts->ts_and_another ++;
	}
	
	if (opts.ttcp)
		counts->ttcp ++;
	if (opts.other)
		counts->other ++;	
}