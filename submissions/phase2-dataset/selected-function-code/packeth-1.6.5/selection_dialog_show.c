void
selection_dialog_show			(GtkButton	*button,
					gpointer user_data)
{
	
	FILE *fp;
	gchar *row[3], tmp[84] /*, temp[100]*/;
	int i=0, ch, first = 0, second = 0;
	GtkTreeStore *treestore;
	GtkTreeIter toplevel;

	treestore = gtk_tree_store_new(3,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

	/* if there is a dialog already open, reopen it */
	if (selection1_dialog != NULL){
		gdk_window_show(selection1_dialog->window);
		gdk_window_raise(selection1_dialog->window);
		return;
	}
	/* if we can't open file then we raise an error dialog. user has to change the 
	 * address_filename variable using the File-> Select database option */
	if((fp = fopen(address_filename, "r")) == NULL) { 
		//printf("file %s with database can't be opened!\n", address_filename);
                //snprintf(temp,100,"File %s with database can't be opened!\nClick on File->Select database",
										// address_filename);
                //error(temp);
		error("Error: Can't open selected address database!\n\n"
				"Click on File -> Select database\n"
				"to choose another one or create a new one\n\n"
				"File format: <IP address>,<MAC address>,<Name>");
		return;
	}
	   
	/* create this dialog */
	selection1_dialog = create_sel1_dialog();

	/* resrve place for ip address, mac address and name */
       	row[0]= (gchar *)malloc(16*sizeof(gchar));
       	row[1]= (gchar *)malloc(18*sizeof(gchar));
       	row[2]= (gchar *)malloc(50*sizeof(gchar));

	/* now we have to append the saved values: */
	/* we make following presumption: all entries must be in following format:
	 * xxx.xxx.xxx.xxx,xx:xx:xx:xx:xx:xx,name
	 * that means first there is IP in dot format and then mac address with : and name in the end 
	 * there can be an entry only with either ip or mac address and without name. 
	 *  new lines and comments starting with # are allowed */
	
	for(;;) {
		/* we read the whole line and remember where the comma is 
		 * first is the place of the comma between ip and mac address
		 * second is the variable holding the position between mac address and name 
		 * the line can have max 83 characters: 15 for ip , 18 for mac and 50 for name 
		 * commas are obligatoty */
		for (i=1, first = 0, second = 0; (ch=getc(fp)) != '\n' && i<84 && ch != EOF ;i++ ) {
			if (ch == ',')  {
				if (first == 0)
					first = i;
				else if (second == 0)
					second = i;
			}
			tmp[i-1] = ch;
		}
		tmp[i-1] = '\0';
	
		/* if first and second are == 0 and ch== '\n' - new line, ok, skip this */  
		if ( (first==0) && (second==0) && (ch == '\n') && (i==1) ) 
			continue;
		
		/* we also allow comments lines starting with # */
		if ( (i>1) && (tmp[0] == '#') ) 
			continue;
		
		/* first > 16 - ip address can not be longer then 15 char including points
		 * second - first (mac address) exactly 18 char including : or 1 if without it
		 * second - i > 50 - name can only have max 50 chars 
		 * if first and second are == 0 and i>0 there was no commas -> error 
		 * if first is 1 and seconf is 2, then there were no ip nor mac address -> error */
		if ( (first>16) || (((second-first) > 18) && ((second-first) != 1)) || ((i-second) > 50) ||
			((first==0) && (second==0) && (i>1)) || ((first==1) && (second==2) && (i>1)) ) {
			error("Error: Selected address database has wrong format!\n\n"
				"Click on File -> Select database\n"
				"to choose another one or create a new one\n\n"
				"File format: <IP address>,<MAC address>,<Name>");
			free(row[0]);
			free(row[1]);
			free(row[2]);
			fclose(fp);
			selection1_dialog = NULL;
			return;
		}
		
		if (ch == EOF)
			break;
		
		tmp[first-1] = '\0';
		tmp[second-1] = '\0';
	
		strncpy(row[0], tmp, first );
		strncpy(row[1], &tmp[first], (second-first));
		strncpy(row[2], &tmp[second], (i-second));

		if ( (check_ip_address(row[0]) == -1) && (strlen(row[0]) !=0) ) {
			//printf("wrong ip entry in address list\n");
			error("Error: Wrong IP entry in address list!\n\n"
				"Click on File -> Select database\n"
				"to choose another one or create a new one\n\n"
				"File format: <IP address>,<MAC address>,<Name>");
			free(row[0]);
			free(row[1]);
			free(row[2]);
			fclose(fp);
			selection1_dialog = NULL;
			return;
		}
		
		if ( (check_mac_address(row[1]) == -1) && (strlen(row[1]) !=0) ) {
			//printf("wrong mac entry in address list\n");
			error("Error: Wrong MAC entry in address list\n\n"
				"Click on File -> Select database\n"
				"to choose another one or create a new one\n\n"
				"File format: <IP address>,<MAC address>,<Name>");
			free(row[0]);
			free(row[1]);
			free(row[2]);
			fclose(fp);
			selection1_dialog = NULL;
			return;
		}
		gtk_tree_store_append(treestore,&toplevel,NULL);
		gtk_tree_store_set(treestore,&toplevel,0,row[0],1,row[1],2,row[2],-1);				

	}	
		
	free(row[0]);
	free(row[1]);
	free(row[2]);

	gtk_widget_show(selection1_dialog);

	// GtkTreeView ...

	GtkTreeViewColumn *stolpec;
	GtkCellRenderer *renderer;

	stolpec = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(stolpec,"IP address");
	gtk_tree_view_append_column(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1")),stolpec);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(stolpec,renderer,TRUE);
	gtk_tree_view_column_add_attribute(stolpec,renderer,"text",0);

	stolpec = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(stolpec,"MAC value");
	gtk_tree_view_append_column(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1")),stolpec);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(stolpec,renderer,TRUE);
	gtk_tree_view_column_add_attribute(stolpec,renderer,"text",1);

	stolpec = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(stolpec,"Name");
	gtk_tree_view_append_column(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1")),stolpec);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(stolpec,renderer,TRUE);
	gtk_tree_view_column_add_attribute(stolpec,renderer,"text",2);

	GtkTreeModel *model = GTK_TREE_MODEL(treestore);
	gtk_tree_view_set_model(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1")),model);
	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(model,&iter);
	g_object_unref(model);
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1"))),GTK_SELECTION_SINGLE);
	gtk_tree_selection_select_iter(gtk_tree_view_get_selection(GTK_TREE_VIEW(lookup_widget(GTK_WIDGET (selection1_dialog), "clist1"))),&iter);
	// ...
	
	fclose(fp);

}