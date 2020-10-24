void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *notbk;
	gint page;
	FILE *file_p;
	gchar *fname /* , *fname2 */;
	char buff[101];

	fname = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(lookup_widget(gtk_widget_get_toplevel(GTK_WIDGET(button)),"fileselection1"))));
	//fname2 = g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_FILE_SELECTION (user_data)->selection_entry)));

	if((file_p = fopen(fname, "r")) == NULL) { 
                //printf("can not open file for reading\n");
                error("Error: can't open file for reading!");
                return;
        }
	
	/* now who called this function */
	switch (load_select_nr) {
		case 1: { /* this is the toolbar load button, we need to know which notebook is open */
			notbk = lookup_widget(GTK_WIDGET(btx), "notebook1");
        		page =  gtk_notebook_get_current_page(GTK_NOTEBOOK(notbk));

        		if (page == 0) { /* so we have the build notebook open */
				
				if (load_data(btx, file_p, 1, 1) == -1) {
					/* calling previous function with last argument =1 means loading for builder */
					fclose(file_p);
			 		return;
				}
				break;
			}
			else if (page == 1) { /* it is the send build generator */

				if (load_gen_b_data(btx, file_p) == -1) {
					error("Error: wrong file format!");
					fclose(file_p);
			 		return;
				}
				break;
			}

			/* page with sequence generator is open */
			else if (page == 2) { /* it is the send build generator */

				if (load_gen_s_data(btx, file_p) == -1) {
					error("Error: wrong file format!");
					fclose(file_p);
			 		return;
				}
				break;
			}
			else if (page == 3) { /* it is the send pcap file generator */

				if (load_data(btx, file_p, 2, MAXNUMLOADPACKETS) == -1) {
					error("Error: wrong file format!");
					fclose(file_p);
			 		return;
				}
				break;
			}
		}
		/* next are the select buttons on the gen-s window */
		case 2: ;
		case 3: ;
		case 4: ;
		case 5: ;
		case 6: ;
		case 7: ;
		case 8: ;
		case 9: ;
		case 10: {
			//if (check_if_file_is_packet(file_p) == -1) {
			if (load_data(btx, file_p, 1, 1) == -1) {
				error("Error: wrong file format!");
				fclose(file_p);
				return;
			}
        		gtk_entry_set_text(GTK_ENTRY(entry_field), fname);
			break;
		}			

	}		

	fclose(file_p);

	snprintf(buff, 100, "  Parameters loaded from file %s", fname);
	statusbar_text(btx, buff);

	gtk_grab_remove(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));

}