int
gconf_tree_model_build_match_list (GConfTreeModel *tree_model, GeditOutputWindow *output_window,
				   const char *pattern, gboolean search_keys, gboolean search_values,
				   GObject *dialog)
{
	GtkTreeIter iter_root;
	int res;
	SearchIter *st;

	st = g_new0 (SearchIter, 1);
	st->pattern = pattern;
	st->search_keys = search_keys;
	st->search_values = search_values;
	st->output_window = output_window; 
	st->res = 0;
	st->searching = dialog;

	g_object_add_weak_pointer (st->searching, (gpointer)&st->searching);

	if (!gtk_tree_model_get_iter_first (GTK_TREE_MODEL (tree_model), &iter_root)) {

		/* Ugh, something is terribly wrong */
		return 0;
	}

	gtk_tree_model_foreach (GTK_TREE_MODEL (tree_model),
				gconf_tree_model_search_iter_foreach, st);

	res = st->res;
#if 0
	g_free (st); /* This causes invalid memory access according to valgrind */
#endif		     /* FIXME: This introduces a small leak (24 bytes) */
	return res;

}