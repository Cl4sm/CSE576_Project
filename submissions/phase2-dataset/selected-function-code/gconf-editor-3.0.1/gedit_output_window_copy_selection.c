static void		 
gedit_output_window_copy_selection (GeditOutputWindow *ow)
{
	gboolean ret;
	GtkTreeIter iter;
	GtkTreeSelection *selection;

	GString *string = NULL;

	g_return_if_fail (GEDIT_IS_OUTPUT_WINDOW (ow));

	selection = gtk_tree_view_get_selection (
			GTK_TREE_VIEW (ow->priv->treeview));

	ret = gtk_tree_model_get_iter_first (ow->priv->model, &iter);
	
	while (ret)
	{
		if (gtk_tree_selection_iter_is_selected (selection, &iter))
		{
			gchar *line;
			
			gtk_tree_model_get (ow->priv->model, &iter, COLUMN_LINES, &line, -1);

			if (string == NULL)
				string = g_string_new (line);
			else
				string = g_string_append (string, line);
			
			string = g_string_append_c (string, '\n');
			
			g_free (line);
		}

		ret = gtk_tree_model_iter_next (ow->priv->model, &iter);
	}

	if (string != NULL)
	{
		gchar *text;

		pango_parse_markup (string->str, string->len, 0, NULL, &text, NULL, NULL);
		
		gtk_clipboard_set_text (gtk_widget_get_clipboard (
						GTK_WIDGET (ow), GDK_SELECTION_CLIPBOARD),
			      		text, 
					-1);

		g_free (text);
	}

	g_string_free (string, TRUE);
}