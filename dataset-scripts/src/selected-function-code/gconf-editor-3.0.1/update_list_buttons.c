static void
update_list_buttons (GConfKeyEditor *editor)
{
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	GtkTreePath *path;
	GtkTreeModel *model;
	gint selected;
	gboolean can_edit = FALSE, can_remove = FALSE, can_go_up = FALSE, can_go_down = FALSE;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->list_widget));

	if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
		path = gtk_tree_model_get_path (model, &iter);

		selected = gtk_tree_path_get_indices (path)[0];

		can_edit = can_remove = TRUE;
		can_go_up = selected > 0;
		can_go_down =  selected < gtk_tree_model_iter_n_children (model, NULL) - 1;

		gtk_tree_path_free (path);
	}

	gtk_widget_set_sensitive (editor->remove_button, can_remove);
	gtk_widget_set_sensitive (editor->edit_button, can_edit);
	gtk_widget_set_sensitive (editor->go_up_button, can_go_up);
	gtk_widget_set_sensitive (editor->go_down_button, can_go_down);

}