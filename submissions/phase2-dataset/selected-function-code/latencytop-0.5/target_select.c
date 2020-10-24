static gint target_select(GtkTreeSelection *sel, void *data)
{
	GtkTreeModel		*model;
	GtkTreeIter		iter;
	struct process		*proc = NULL;

	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(targets_view));
	if (gtk_tree_selection_get_selected(sel, &model, &iter))
		gtk_tree_model_get(model, &iter, COL_T_PROC, &proc, -1);

	if (ignore_sel)
		return TRUE;

	if (proc && (force_results || proc->pid != cur_target_pid)) {
		cur_target_pid = proc->pid;
		remove_all_results();
		insert_results(proc->latencies);
	} else if (!proc && (force_results || cur_target_pid != 0)) {
		cur_target_pid = 0;
		remove_all_results();
		insert_results(lines);
	}

	/* We don't need to force updates anymore */
	force_results = 0;

	return FALSE;
}
