static GtkWidget *create_backtrace_list(void)
{
	GtkWidget *scrollw;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *sel;

	/* We Create a scrolled window with a treeview to hold the target
	 * list. Note that a lot of this is straight from the GTK tutorial :-)
	 */
	scrollw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollw),
				       GTK_POLICY_AUTOMATIC, 
				       GTK_POLICY_AUTOMATIC);
   
	/* Create a tree view object */
	backtrace_view = gtk_tree_view_new();
	gtk_container_add(GTK_CONTAINER(scrollw), backtrace_view);

	/* We use a simple list store */
	backtrace_model = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(backtrace_view),
				GTK_TREE_MODEL(backtrace_model));



	/* Add columns */
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Backtrace",
							   renderer,
							   "text", COL_B_FUNC,
							   NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(backtrace_view),
				    GTK_TREE_VIEW_COLUMN(column));

	/* Set selection mode to single */
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(backtrace_view));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
	return scrollw;
}
