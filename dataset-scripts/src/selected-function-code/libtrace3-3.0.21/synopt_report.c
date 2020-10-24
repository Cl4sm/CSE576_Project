void synopt_report(void)
{
	
	FILE *out = fopen("tcpopt_syn.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}


	
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"No Options",
			(double)(syn_counts.no_options) / total_syns * 100.0,
			(double)(synack_counts.no_options) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"M Only",
			(double)(syn_counts.mss_only) / total_syns * 100.0,
			(double)(synack_counts.mss_only) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"T Only",
			(double)(syn_counts.ts_only) / total_syns * 100.0,
			(double)(synack_counts.ts_only) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"M and S",
			(double)(syn_counts.ms) / total_syns * 100.0,
			(double)(synack_counts.ms) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"M and W",
			(double)(syn_counts.mw) / total_syns * 100.0,
			(double)(synack_counts.mw) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"M, S and W",
			(double)(syn_counts.msw) / total_syns * 100.0,
			(double)(synack_counts.msw) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"M, T",
			(double)(syn_counts.mt) / total_syns * 100.0,
			(double)(synack_counts.mt) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"W, T",
			(double)(syn_counts.wt) / total_syns * 100.0,
			(double)(synack_counts.wt) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"S, T",
			(double)(syn_counts.ts_and_sack) / total_syns * 100.0,
			(double)(synack_counts.ts_and_sack) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"S, M, T",
			(double)(syn_counts.tms) / total_syns * 100.0,
			(double)(synack_counts.tms) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"W, M, T",
			(double)(syn_counts.tmw) / total_syns * 100.0,
			(double)(synack_counts.tmw) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"S, W, T",
			(double)(syn_counts.tws) / total_syns * 100.0,
			(double)(synack_counts.tws) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"M, S, W and T",
			(double)(syn_counts.all_four) / total_syns * 100.0,
			(double)(synack_counts.all_four) / total_synacks * 100.0);
	//fprintf(out, "%-20s\t%.2f%%\n",
	//		"T and (M or S or W)",
	//		(double)(counts.ts_and_another) / total_syns * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"T/TCP",
			(double)(syn_counts.ttcp) / total_syns * 100.0,
			(double)(synack_counts.ttcp) / total_synacks * 100.0);
	fprintf(out, "%-20s\t%.2f%%\t%.2f%%\n",
			"Other options",
			(double)(syn_counts.other) / total_syns * 100.0,
			(double)(synack_counts.other) / total_synacks * 100.0);
	
	
	fclose(out);
}