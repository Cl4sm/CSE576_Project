static void
list_go_down_clicked (GtkButton *button,
		      GConfKeyEditor *editor)
{
	GtkTreeIter iter_first;
	GtkTreeIter iter_second;
	GtkTreeSelection *selection;
	
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->list_widget));

	if (gtk_tree_selection_get_selected (selection, NULL, &iter_first)) {
		GConfValue *first;
		GConfValue *second;

		iter_second = iter_first;

		gtk_tree_model_iter_next (GTK_TREE_MODEL (editor->list_model), &iter_second);

		gtk_tree_model_get (GTK_TREE_MODEL (editor->list_model), &iter_first, 0, &first, -1);
		gtk_tree_model_get (GTK_TREE_MODEL (editor->list_model), &iter_second, 0, &second, -1);
				
		gtk_list_store_set (editor->list_model, &iter_first, 0, second, -1);
		gtk_list_store_set (editor->list_model, &iter_second, 0, first, -1);
		gtk_tree_selection_select_iter(selection, &iter_second);
	}
}