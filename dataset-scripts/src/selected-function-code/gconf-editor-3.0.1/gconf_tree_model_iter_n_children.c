static gint
gconf_tree_model_iter_n_children (GtkTreeModel *tree_model, GtkTreeIter *iter)
{
	GConfTreeModel *model = (GConfTreeModel *)tree_model;
	Node *node;
	GtkTreeIter tmp;
	gint i = 0;

	g_return_val_if_fail (GCONF_IS_TREE_MODEL (tree_model), 0);
	if (iter) g_return_val_if_fail (model->stamp == iter->stamp, 0);

	if (iter == NULL)
		node = model->root;
	else
		node = ((Node *)iter->user_data)->children;

	if (!node && iter && gconf_tree_model_iter_children (tree_model, &tmp, iter)) {
		g_return_val_if_fail (tmp.stamp == model->stamp, 0);
		node = ((Node *)tmp.user_data);
	}

	if (!node)
		return 0;

	for (; node != NULL; node = node->next)
		i++;

	return i;
}