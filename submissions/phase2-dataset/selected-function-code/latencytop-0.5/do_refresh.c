{
	/* Ignore selection messages triggerd by the list during those updates */
	ignore_sel = TRUE;

	/* Empty list views */
	remove_all_targets();
	remove_all_results();

	/* Call core to update the glists */
	update_list();

	/* Stop ignoring selection changes */
	ignore_sel = FALSE;

	/* Insert all targets again */
	insert_all_targets();

	/* Make them look good */
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(targets_view));
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(results_view));
}
