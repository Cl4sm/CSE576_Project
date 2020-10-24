static void
list_remove_clicked (GtkButton *button,
		     GConfKeyEditor *editor)
{
	GtkTreeIter iter;
	GtkTreeIter tmp;
	GtkTreeSelection *selection;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->list_widget));

	if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
		tmp = iter;
		if (gtk_tree_model_iter_next (GTK_TREE_MODEL (editor->list_model), &tmp)) {
			gtk_tree_selection_select_iter (selection, &tmp);
		} else {
			GtkTreePath *path;
			path = gtk_tree_model_get_path (GTK_TREE_MODEL (editor->list_model), &iter);
			if (gtk_tree_path_prev (path)) {
				gtk_tree_selection_select_path (selection, path);
			}
			gtk_tree_path_free (path);
		}
		gtk_list_store_remove (editor->list_model, &iter);
	}
}