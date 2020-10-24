void
on_button62_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *notbk;
	gint page;
	FILE *file_p;

	statusbar_text(button, "");

	notbk = lookup_widget(GTK_WIDGET(button), "notebook1");
	page =  gtk_notebook_get_current_page(GTK_NOTEBOOK(notbk));
	
	if (page == 0) { /* we have the build notebook open */
		if((file_p = fopen(".defaultBuilder", "w")) == NULL) {
			error("Can't save parameters in file: \".defaultBuilder\"");
			return;
		}

		if (save_packet(button, user_data, file_p) == -1) {
                        fclose(file_p);
                        return;
                }
	}
	
	else if (page == 1) { /* we have the Gen-b notebook open */
		if((file_p = fopen(".defaultGen-b", "w")) == NULL) {
			error("Can't save parameters in file: \".defaultGen-b\"");
			return;
		}

		if (save_gen_b(button, file_p) == -1) {
                        fclose(file_p);
                        return;
                }
	}
	
	else if (page == 2) { /* we have the Gen-s notebook open */
		if((file_p = fopen(".defaultGen-s", "w")) == NULL) {
			error("Can't save parameters in file: \".defaultGen-s\"");
			return;
		}

		if (save_gen_s(button, file_p) == -1) {
                        fclose(file_p);
                        return;
                }
	}
	
	else
		return;

	fclose(file_p);

	statusbar_text(button, "  Parameters set as default parameters");

	return;

}