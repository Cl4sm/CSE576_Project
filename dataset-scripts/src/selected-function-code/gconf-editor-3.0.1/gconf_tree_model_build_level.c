static gboolean
gconf_tree_model_build_level (GConfTreeModel *model, Node *parent_node, gboolean emit_signals)
{
	GSList *list, *tmp;
	Node *tmp_node = NULL;
	gint i = 0;

	if (parent_node->children)
		return FALSE;

	list = gconf_client_all_dirs (model->client, parent_node->path, NULL);

	if (!list)
		return FALSE;

	for (tmp = list; tmp; tmp = tmp->next, i++) {
		Node *node;

		node = g_new0 (Node, 1);
		node->offset = i;
		node->parent = parent_node;
		node->path = tmp->data;

		if (tmp_node) {
			tmp_node->next = node;
			node->prev = tmp_node;
		} else {
			/* set parent node's children */
			parent_node->children = node;
		}

		tmp_node = node;

		/* let the model know things have changed */
		if (emit_signals) {
			GtkTreeIter tmp_iter;
			GtkTreePath *tmp_path;

			model->stamp++;
			tmp_iter.stamp = model->stamp;
			tmp_iter.user_data = tmp_node;
			tmp_path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), &tmp_iter);
			gtk_tree_model_row_inserted (GTK_TREE_MODEL (model), tmp_path, &tmp_iter);
			gtk_tree_path_free (tmp_path);
		}
	}

	g_slist_free (list);

	return TRUE;
}