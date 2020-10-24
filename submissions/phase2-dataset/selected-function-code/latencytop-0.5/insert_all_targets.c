static void insert_all_targets(void)
{
	GList *entry;
	struct process *proc;
	GtkTreeIter iter;
	gchar *esctext;
	GdkPixbuf *icon;
	GtkTreePath *path = NULL;
	int preferred;

	/* Force the selection change to update the results */
	force_results = 1;

	/* Add the "system" target */
        gtk_list_store_append (GTK_LIST_STORE(targets_model), &iter);
        gtk_list_store_set (GTK_LIST_STORE(targets_model), 
	                    &iter,
			    COL_T_ICON, global_icon,
                            COL_T_NAME, "<b>Global</b>",
			    COL_T_MAX, "",
			    COL_T_PROC, NULL,
	                    -1);

	/* Select it if it's our current target */
	if (cur_target_pid == 0)
		path = gtk_tree_model_get_path(GTK_TREE_MODEL(targets_model), &iter);

	/* The core doesn't sort the list, we do here */
	entry = g_list_sort(procs, compare_process);
	while(entry) {
		char targetstr[64];
		char maxstr[32];

		proc = entry->data;
		entry = g_list_next(entry);

		preferred = proc->name && prefered_process &&
			strcmp(proc->name, prefered_process) == 0;

		/* Skip too small values */
		if (!preferred && (proc->max * 0.001 < 0.1))
			continue;

		if (preferred)
			gtk_list_store_insert(GTK_LIST_STORE(targets_model), &iter, 1);
		else
			gtk_list_store_append(GTK_LIST_STORE(targets_model), &iter);
		esctext = g_markup_escape_text(proc->name, -1);
		icon = proc->kernelthread ? kthread_icon : process_icon;
		sprintf(maxstr, "%.1f", proc->max * 0.001);
		targetstr[63] = 0;
		if (preferred)
			snprintf(targetstr, 63, "<u>%s</u>", esctext);
		else
			strncpy(targetstr, esctext, 63);
		gtk_list_store_set(GTK_LIST_STORE(targets_model), 
				   &iter,
				   COL_T_ICON, icon,
				   COL_T_NAME, targetstr,
				   COL_T_MAX, maxstr,
				   COL_T_PROC, proc,
				   -1);
		if (cur_target_pid == proc->pid)
			path = gtk_tree_model_get_path(GTK_TREE_MODEL(targets_model), &iter);
		g_free(esctext);
	}

	if (path)
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(targets_view),
					 path, NULL, FALSE);
}
