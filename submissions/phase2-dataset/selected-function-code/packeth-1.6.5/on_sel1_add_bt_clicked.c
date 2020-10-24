void
on_sel1_add_bt_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *clist, *en_ip, *en_mac, *en_name;
	gchar *en_ip_t, *en_mac_t, *en_name_t;
	gchar *text_ip, *text_mac, *text_name;
	gchar *tmp[3];
	//gint number;
	GtkTreeModel *treestore;
	GtkTreeIter toplevel;
	
	clist = lookup_widget(GTK_WIDGET (selection1_dialog), "clist1");
	treestore = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
        en_ip = lookup_widget(GTK_WIDGET(clist), "sel1_IP_entry");
        en_mac = lookup_widget(GTK_WIDGET(clist), "sel1_mac_entry");

	en_ip = lookup_widget(GTK_WIDGET(clist), "sel1_IP_entry");
	en_mac = lookup_widget(GTK_WIDGET(clist), "sel1_mac_entry");
	en_name = lookup_widget(GTK_WIDGET(clist), "entry153");
	
	/* is there any other elegant way to get the row number but with global variable? */
	en_ip_t = (char *)gtk_entry_get_text(GTK_ENTRY(en_ip));
	en_mac_t = (char *)gtk_entry_get_text(GTK_ENTRY(en_mac));
	en_name_t = (char *)gtk_entry_get_text(GTK_ENTRY(en_name));

	/* we still have the value of the row number, so we can check wheather this is
	 * the same as in the entry fields. so in case the user selects an entry and then 
	 * presses add button we won't get duplicated entries */
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
	GtkTreeIter iter;
	gtk_tree_selection_get_selected(selection,&treestore,&iter);
	gtk_tree_model_get(treestore,&iter,0,&text_ip,1,&text_mac,2,&text_name,-1);	

	if ( (strlen(en_ip_t) == 0) && (strlen(en_mac_t) == 0) )
		return;

	if ( (strcmp(en_ip_t, text_ip) == 0) && (strcmp(en_mac_t, text_mac) == 0) && 
							(strcmp(en_name_t, text_name) == 0)) {
		//printf("values are the same, we don't insert them!\n");
		error("Error: values are the same, we don't insert them!");
		return;
	}
	/* now we have to check if the values are ok */

	if ( (strlen(en_ip_t) != 0) && (check_ip_address(en_ip_t) == -1) ) {
		//printf("wrong ip entry in address list\n");
		error("Error: wrong ip entry in address list");
		return;
	}
	
	if ( (strlen(en_mac_t) != 0) && (check_mac_address(en_mac_t) == -1) ) {
		//printf("wrong mac entry in address list\n");
		error("Error: wrong mac entry in address list");
		return;
	}
	
       	tmp[0]= (gchar *)malloc(16*sizeof(gchar));
       	tmp[1]= (gchar *)malloc(18*sizeof(gchar));
       	tmp[2]= (gchar *)malloc(50*sizeof(gchar));
	
	strcpy(tmp[0], en_ip_t);
	strcpy(tmp[1], en_mac_t);
	strcpy(tmp[2], en_name_t);
	
	gtk_tree_store_append(GTK_TREE_STORE(treestore),&toplevel,NULL);
	gtk_tree_store_set(GTK_TREE_STORE(treestore),&toplevel,0,tmp[0],1,tmp[1],2,tmp[2],-1);
	
	free(tmp[0]);
	free(tmp[1]);
	free(tmp[2]);
		
}