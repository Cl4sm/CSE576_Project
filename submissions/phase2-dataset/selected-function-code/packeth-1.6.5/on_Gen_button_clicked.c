void
on_Gen_button_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *ntbk2, *ntbk4, *ntbk7;
	GtkWidget *nt1;
        GtkWidget *text_e;
	GtkWidget *crc_value;

	unsigned long crc32;
	char str_crc32[9];
	char tmp[4600];
	guint i, j, m, page1, page2;
	
	ntbk2 = lookup_widget (GTK_WIDGET (button), "notebook2");
	ntbk4 = lookup_widget (GTK_WIDGET (button), "notebook4");
	ntbk7 = lookup_widget (GTK_WIDGET (button), "notebook7");
	nt1 = lookup_widget(GTK_WIDGET(button), "notebook1");
	crc_value = lookup_widget(GTK_WIDGET (button), "entry164");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt1), 1);
	
        nt1 = lookup_widget(GTK_WIDGET(button), "Load_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Save_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Reset_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "button62");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Interface_button");
        gtk_widget_set_sensitive(nt1, TRUE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Send_button");
        gtk_widget_set_sensitive(nt1, TRUE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Stop_button");
	gtk_widget_set_sensitive(nt1, TRUE);

	statusbar_text(button, "  Gen-b window opened.");

	/* get access to the buffer of text field */
	text_e = lookup_widget(GTK_WIDGET (button), "text5");
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_e)); 

	show_error_dialog = 0;

	if (make_packet(button, user_data) == -1) {
        	//error("Packet contents is not ok!\n");
		snprintf(&tmp[0], 200, "\n\n\n\n                   Packet constructed in Builder is not ok!");
		gtk_entry_set_text(GTK_ENTRY(crc_value), "");
        }
	else {
		/* copy data to tmp field */
		for (i=0, j=0, m=1; j < number; m++, j++) {
			snprintf(&tmp[i], 4600, "%02x", packet[j]);
			i++; i++;
			/* we allow only 16 bytes in each row - looks nicer */
			if ((m % 16) == 0 && (m > 1)) {
				tmp[i]='\n';
				m = 0;
			}
			else
				tmp[i] = ' '; 
			i++;
		}
		tmp[i] = '\0';

		crc32 = get_crc32(packet, number);
		snprintf(str_crc32, 9, "%08lX", crc32);
		gtk_entry_set_text(GTK_ENTRY(crc_value), str_crc32);
	}
      
        /* insert the text in the text field */
	gtk_text_buffer_set_text(buffer,tmp,-1);      

        /* we want to set correct options in send built change while sending option menu */
	if (page1 == 3)
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk7), 2);
	else if (page1 == 1) 
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk7), 0);
	else if (page1 == 2) 
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk7), 0);
	else {
		page2 = gtk_notebook_get_current_page(GTK_NOTEBOOK(ntbk4));
		if (page2 == 0)
			gtk_notebook_set_page(GTK_NOTEBOOK(ntbk7), 4);
		else if (page2 == 1)
			gtk_notebook_set_page(GTK_NOTEBOOK(ntbk7), 3);
		else
			gtk_notebook_set_page(GTK_NOTEBOOK(ntbk7), 1);
	
	}
	
	show_error_dialog = 1;
}