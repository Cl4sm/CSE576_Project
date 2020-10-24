void
on_Reset_button_clicked                (GtkButton       *button,
                                         gpointer         user_data)
{ 
	GtkWidget *notbk;
	gint page;
	FILE *file_p;

	statusbar_text(button, "");

	notbk = lookup_widget(GTK_WIDGET(button), "notebook1");
	page =  gtk_notebook_get_current_page(GTK_NOTEBOOK(notbk));

	
	if (page == 0) { /* so we have the build notebook open */
		if((file_p = fopen(".defaultBuilder", "r")) == NULL) { 
			error("Can't open file with default parameters: \".defaultBuilder\"");
			return;
		}
		
		if (load_data(button, file_p, 1, 1) == -1) 
			;//error("Data in file \".defaultBuilder\" has wrong format");
		fclose(file_p);
	}
	
	else if (page == 1) { /* so we have the Gen-b notebook open */
		if((file_p = fopen(".defaultGen-b", "r")) == NULL) { 
			error("Can't open file with default parameters: \".defaultGen-b\"");
			return;
		}
		
		if (load_gen_b_data(button, file_p) == -1) 
			;//error("Data in file \".defaultGen-b\" has wrong format");
		fclose(file_p);
	}
	
	else if (page == 2) { /* so we have the Gen-s notebook open */
		if((file_p = fopen(".defaultGen-s", "r")) == NULL) { 
			error("Can't open file with default parameters: \".defaultGen-s\"");
			return;
		}
		
		if (load_gen_s_data(button, file_p) == -1) 
			//error("Data in file \".defaultGen-s\" has wrong format");
			;
		//fclose(file_p);
		//YYY : the above line causes a crash, I don't know why...
	}
	
	statusbar_text(button, "  Loaded default parameters");

	return;
}