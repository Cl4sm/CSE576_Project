void
on_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *notbk;
	gint page;
	gchar *fname;
	FILE *file_p;
	char buff4[101];

	/* hm, first we should decide, what are we going to save: a packet, send_built parameters,
	 * send_sequence parameters or send kernel parameters. I last three cases we only save
	 * the values and not also the packets themself (only the names of the packet) 
	 * so let's check which notebook is open */

	notbk = lookup_widget(GTK_WIDGET(btx), "notebook1");
        page =  gtk_notebook_get_current_page(GTK_NOTEBOOK(notbk));

	fname = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(lookup_widget(gtk_widget_get_toplevel(GTK_WIDGET(button)),"fileselection2"))));
	
	/* lets check if the file exists and we don't allow to overwrite it
	 * is there any way to do this in a more elegant manner? */
	//if((file_p = fopen(fname, "r")) != NULL) { /* could be also some other failure??? */
	//	error("Error: wrong file name, file already exists!");
	//	return;
	//}
	
	if((file_p = fopen(fname, "w")) == NULL) {
		error("Error: can not open file for saving");
		return;
	}


        if (page == 0) { /* so we have the build notebook open, it means we save the packet */
		/* YYY ok, this is not yet implemented */
		/* you could also add possibility to save even with this button on??? */
        	//bt1 = lookup_widget(GTK_WIDGET(btx), "auto_get_mac_cbt");
	        //if (GTK_TOGGLE_BUTTON(bt1)->active) {
        	        //printf("Error: you can't save in a packet if auto get link layer is on!\n");
        	//        error("Error: you can't save in a packet if auto get link layer is on!");
        	//        return;
		//}

	        if (save_packet(btx, user_data, file_p) == -1) {
	                fclose(file_p);
			return;
	        }
	}

	else if (page == 1) { /* it is the send_built page */

	        if (save_gen_b(btx, file_p) == -1) {
	                fclose(file_p);
			return;
	        }
	}

	else if (page == 2) {

	        if (save_gen_s(btx, file_p) == -1) {
	                fclose(file_p);
			return;
	        }
	}

	else if (page == 3) {

	        if (save_gen_pcap(btx, file_p) == -1) {
	                fclose(file_p);
			return;
	        }
	}

	else
		return;

	fclose(file_p);

	snprintf(buff4, 100, "  Parameters saved in file %s", fname);
	statusbar_text(btx, buff4);

	gtk_grab_remove(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));

}