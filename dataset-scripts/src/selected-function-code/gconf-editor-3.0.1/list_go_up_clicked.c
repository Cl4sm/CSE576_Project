static void
list_go_up_clicked (GtkButton *button,
		    GConfKeyEditor *editor)
{
	GtkTreeIter iter_first;
	GtkTreeIter iter_second;
	GtkTreeSelection *selection;
	
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->list_widget));

	if (gtk_tree_selection_get_selected (selection, NULL, &iter_second)) {
		GConfValue *first;
		GConfValue *second;
		GtkTreePath *path;
		
		path = gtk_tree_model_get_path (GTK_TREE_MODEL (editor->list_model), &iter_second);
		gtk_tree_path_prev (path);
		gtk_tree_model_get_iter (GTK_TREE_MODEL (editor->list_model), &iter_first, path);
		
		gtk_tree_model_get (GTK_TREE_MODEL (editor->list_model), &iter_first, 0, &first, -1);
		gtk_tree_model_get (GTK_TREE_MODEL (editor->list_model), &iter_second, 0, &second, -1);
				
		gtk_list_store_set (editor->list_model, &iter_first, 0, second, -1);
		gtk_list_store_set (editor->list_model, &iter_second, 0, first, -1);

		gtk_tree_path_free (path);

		gtk_tree_selection_select_iter(selection, &iter_first);
	}
}