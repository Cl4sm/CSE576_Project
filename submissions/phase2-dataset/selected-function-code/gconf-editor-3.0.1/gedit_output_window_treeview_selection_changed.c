static void
gedit_output_window_treeview_selection_changed (GtkTreeSelection *selection, 
						GeditOutputWindow  *output_window)
{
	gboolean selected;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *line;
	
	g_return_if_fail (output_window != NULL);
	g_return_if_fail (selection != NULL);

	selected = (gtk_tree_selection_count_selected_rows (selection) > 0);
	
	gtk_widget_set_sensitive (output_window->priv->copy_menu_item, selected);
	gtk_widget_set_sensitive (output_window->priv->copy_button, selected);

	if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, COLUMN_LINES, &line, -1);	
		g_signal_emit (output_window, signals [SELECTION_CHANGED], 0, line);
	}
}