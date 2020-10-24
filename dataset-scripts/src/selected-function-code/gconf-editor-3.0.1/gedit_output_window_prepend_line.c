void
gedit_output_window_prepend_line (GeditOutputWindow *ow, const gchar *line, gboolean scroll)
{
	GtkListStore *store;
	GtkTreeIter iter;
	GtkTreePath *path;

	g_return_if_fail (GEDIT_IS_OUTPUT_WINDOW (ow));
	g_return_if_fail (line != NULL);
	
	store = GTK_LIST_STORE (ow->priv->model);
	g_return_if_fail (store != NULL);

	gtk_list_store_prepend (store, &iter);

	gtk_list_store_set (store, &iter, COLUMN_LINES, line, -1);

	gtk_widget_set_sensitive (ow->priv->clear_button, TRUE);
	gtk_widget_set_sensitive (ow->priv->clear_menu_item, TRUE);

	if (!scroll)
		return;

	path = gtk_tree_model_get_path (GTK_TREE_MODEL (store), &iter);
	g_return_if_fail (path != NULL);
		
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (ow->priv->treeview),
				      path, 
				      NULL,
				      TRUE,
				      0.0,
				      0.0);

	gtk_tree_path_free (path);

}