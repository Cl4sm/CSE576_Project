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
	targets_view = gtk_tree_view_new();
	gtk_container_add(GTK_CONTAINER(scrollw), targets_view);

	/* We use a simple list store for 4 colums, one is not displayed */
	targets_model = gtk_list_store_new(4,
					   GDK_TYPE_PIXBUF,
					   G_TYPE_STRING,
					   G_TYPE_STRING,
					   G_TYPE_POINTER);
	gtk_tree_view_set_model(GTK_TREE_VIEW(targets_view),
				GTK_TREE_MODEL(targets_model));

	/* Create the column with icon & text */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, "Targets");
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_attributes(column, renderer,
					    "pixbuf", COL_T_ICON,
					    NULL);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_set_attributes(column, renderer,
					    "markup", COL_T_NAME,
					    NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(targets_view),
				    GTK_TREE_VIEW_COLUMN(column));

	/* Add a column for the max latency */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(G_OBJECT(renderer),
		     "xalign", 1.0, 
		     NULL);
	column = gtk_tree_view_column_new_with_attributes("Max",
							  renderer,
							  "text", COL_T_MAX,
							  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(targets_view),
				    GTK_TREE_VIEW_COLUMN(column));

	/* Set selection mode to single */
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(targets_view));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

	/* Hookup selection changed message */
	g_signal_connect(G_OBJECT(sel), "changed",
			 G_CALLBACK(target_select), NULL);
	return scrollw;
}
