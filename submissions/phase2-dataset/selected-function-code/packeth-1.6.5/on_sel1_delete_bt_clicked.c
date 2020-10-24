void
on_sel1_delete_bt_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *clist;
	
	GtkTreeModel *treestore;
        //GtkTreeIter toplevel;

        clist = lookup_widget(GTK_WIDGET (selection1_dialog), "clist1");
        treestore = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
	
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
        GtkTreeIter iter;
        gtk_tree_selection_get_selected(selection,&treestore,&iter);
	gtk_tree_store_remove(GTK_TREE_STORE(treestore),&iter);

	gtk_tree_model_get_iter_first(treestore,&iter);

	gtk_tree_selection_select_iter(gtk_tree_view_get_selection(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1"))),&iter);

	
}