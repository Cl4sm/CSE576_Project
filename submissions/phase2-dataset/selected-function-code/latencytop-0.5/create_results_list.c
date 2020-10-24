static GtkWidget *create_results_list(void)
{
	GtkWidget *scrollw;
	GtkCellRenderer *cell;
	GtkTreeViewColumn *column;
	GtkTreeSelection *sel;

	/* We Create a scrolled window with a treeview to hold the target
	 * list. Note that a lot of this is straight from the GTK tutorial :-)
	 */
	scrollw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrollw),
				       GTK_POLICY_AUTOMATIC, 
				       GTK_POLICY_AUTOMATIC);
   
	/* Create a tree view object */
	results_view = gtk_tree_view_new();
	gtk_container_add(GTK_CONTAINER(scrollw), results_view);

	/* We use a simple list store for 4 columns, one not visible */
	results_model = gtk_list_store_new(4,
					   G_TYPE_STRING,
					   G_TYPE_STRING,
					   G_TYPE_STRING,
					   G_TYPE_POINTER);
	gtk_tree_view_set_model(GTK_TREE_VIEW(results_view),
				GTK_TREE_MODEL(results_model));



	/* Add columns */
	cell = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Cause",
							   cell,
							   "text", COL_R_CAUSE,
							   NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(results_view),
				    GTK_TREE_VIEW_COLUMN(column));

	cell = gtk_cell_renderer_text_new ();
	g_object_set(G_OBJECT(cell),
		     "xalign", 1.0, 
		     NULL);
	column = gtk_tree_view_column_new_with_attributes ("Maximum",
							   cell,
							   "text", COL_R_MAX,
							   NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(results_view),
				    GTK_TREE_VIEW_COLUMN(column));

	cell = gtk_cell_renderer_text_new ();
	g_object_set(G_OBJECT(cell),
		     "xalign", 1.0, 
		     NULL);
	column = gtk_tree_view_column_new_with_attributes ("Percentage",
							   cell,
							   "text", COL_R_PERCENT,
							   NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(results_view),
				    GTK_TREE_VIEW_COLUMN(column));


	/* Set selection mode to single */
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(results_view));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

	/* Hookup selection changed message */
	g_signal_connect(G_OBJECT(sel), "changed",
			 G_CALLBACK(backtrace_select), NULL);
	return scrollw;
}
