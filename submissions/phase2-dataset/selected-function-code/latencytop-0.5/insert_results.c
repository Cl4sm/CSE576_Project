static void insert_results(GList *list)
{
	GList *entry;
	struct latency_line *line;
	GtkTreeIter iter;
	gchar *reasonstr;
	int l;

	entry = g_list_first(list);
	while(entry) {
		char maxstr[32];
		char percentstr[32];

		line = entry->data;
		entry = g_list_next(entry);

		/* Skip too small values */
		if (line->max * 0.001 < 0.1) 
			continue;

		/* Remove crap \n at end of line */
		reasonstr = g_strdup(line->reason);
		l = strlen(reasonstr);
		while(l) {
			if (reasonstr[--l] != '\n')
				break;
			reasonstr[l] = 0;
		}

		/* Store it all into the model */
		gtk_list_store_append(GTK_LIST_STORE(results_model), &iter);
		sprintf(maxstr, "%.1f ms", line->max * 0.001);
		sprintf(percentstr, "%.1f %%", (line->time * 100 + 0.0001) / total_time);
		gtk_list_store_set(GTK_LIST_STORE(results_model),
				   &iter,
				   COL_R_CAUSE, reasonstr,
				   COL_R_MAX, maxstr,
				   COL_R_PERCENT, percentstr,
				   COL_R_LINE, line,
				   -1);
		g_free(reasonstr);
	}
}
