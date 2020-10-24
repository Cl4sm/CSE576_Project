void
on_sel1_ok_bt_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *clist; // *en_ip, *en_mac;
	gchar *en_ip_t, *en_mac_t;
	gchar *text_ip, *text_mac, *text_name;
	gchar temp[100];
	FILE *fp;
	int i;
	
	/* so we want to insert the choosen values into the entry mac field.
	 * we have to check again, that the values are correct
	 * if they are not a warning should pop up and the window should stay open
	 * if they are ok, then the value is inserted and we try to write in the file */
	GtkTreeModel *treestore;
        //GtkTreeIter toplevel;

        clist = lookup_widget(GTK_WIDGET (selection1_dialog), "clist1");
        treestore = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
	
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
        GtkTreeIter iter;
        gtk_tree_selection_get_selected(selection,&treestore,&iter);
        gtk_tree_model_get(treestore,&iter,0,&en_ip_t,1,&en_mac_t,-1);	
	
	/* we need to access the L_dst_mac or L_src_mac entry in the main window! 
	 * that is why we putted it static global before */
	if (MAC_yes == TRUE) {
		if (check_mac_address(en_mac_t) == -1) {
			//printf("wrong mac entry in address list\n");
			error("Error: wrong mac entry in address list");
			return;
		}
		else
			gtk_entry_set_text(GTK_ENTRY(entry_field), en_mac_t);
	}

	/* in case we need to insert the IP value as well
	 * this is in case of an arp packet or ip packet, so we check the IP_yes value */
	if (IP_yes == TRUE) {
		if (check_ip_address(en_ip_t) == -1) { // ---
			//printf("wrong ip entry in address list\n");
			error("Error: wrong ip entry in address list");
			return;
		}
		else
			gtk_entry_set_text(GTK_ENTRY(entry_field_ip), en_ip_t);
	}

	/* we need to reopen the file with datebase and overwrite it with
	 * the values in the clist field */
	if((fp = fopen(address_filename, "w")) == NULL) { /* could be also some other failure??? */
		//printf("file %s with database can't be opened!\n", address_filename);
		snprintf(temp, 100, "file %s with database can't be opened", address_filename);
		error(temp);
		/* YYY we could call a function where we could select the database file */
		return;
	}
	//GtkTreeIter iter2;
	gtk_tree_model_get_iter_first(treestore,&iter);
	for(i=0; ; i++) {
		gtk_tree_model_get(treestore,&iter,0,&text_ip,1,&text_mac,2,&text_name,-1);
		fputs(text_ip, fp);
		fputc(44, fp);
		fputs(text_mac, fp);
		fputc(44, fp);
		fputs(text_name, fp);
		fputc(10, fp);
		if (gtk_tree_model_iter_next(treestore,&iter) == FALSE)
			break;
	}
	
	fclose(fp);
		
	gtk_grab_remove(GTK_WIDGET(selection1_dialog));
	gtk_widget_destroy(GTK_WIDGET(selection1_dialog));
	
}