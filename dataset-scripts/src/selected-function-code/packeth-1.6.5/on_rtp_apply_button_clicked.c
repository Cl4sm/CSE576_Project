void
on_rtp_apply_button_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *freq_entry, *alaw_bt, *ulaw_bt, *length_entry, *apply_bt, *payload_entry;
	GtkWidget *menu, *menu_item, *amp;
	gint amp_index;
	gchar *freq_entry_t, *length_entry_t;
	int length, frequency;

	freq_entry = lookup_widget(GTK_WIDGET(button), "entry104");
	length_entry = lookup_widget(GTK_WIDGET(button), "entry106");
	payload_entry = lookup_widget(GTK_WIDGET(button), "entry103");
	alaw_bt= lookup_widget(GTK_WIDGET(button), "radiobutton33");
	ulaw_bt = lookup_widget(GTK_WIDGET(button), "radiobutton32");
	apply_bt = lookup_widget(GTK_WIDGET(button), "rtp_apply_button");
	amp = lookup_widget(GTK_WIDGET(button), "optionmenu12");

	freq_entry_t = (char *)gtk_entry_get_text(GTK_ENTRY(freq_entry));
	length_entry_t = (char *)gtk_entry_get_text(GTK_ENTRY(length_entry));

	
	/* next we need the amplitude */
        menu = GTK_OPTION_MENU(amp)->menu;
        menu_item = gtk_menu_get_active (GTK_MENU (menu));
        amp_index = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);

	/* frequency; there can be rubbish in this field */
        if (check_digit(freq_entry_t, strlen(freq_entry_t),
                                                "Error: apply frequency field values") == -1)
                                return;

        frequency = strtol(freq_entry_t, (char **)NULL, 10);
        if ( (frequency >= 4000 ) || (frequency < 0) ) {
                //printf("Error: RTP frequency range\n");
                error("Error: RTP frequency range ( 0 <= f < 3999 )");
                return;
        }

        /* length */
        if (check_digit(length_entry_t, strlen(length_entry_t),
                                                "Error: apply length field values") == -1)
                                return;

	/* length */
	length = strtol(length_entry_t, (char **)NULL, 10);
	if ( (length >= 1460 ) || (length <= 0) ) {
		//printf("Error: RTP length range\n");
		error("Error: RTP length range ( 0 < length < 1460 )");
		return;
	}

	/* call the function */
	if (GTK_TOGGLE_BUTTON(alaw_bt)->active)  {
		if (insert_frequency(1, frequency, length, payload_entry, amp_index) == 0) {
			//printf("Error: Problem inserting RTP alaw payload frequency\n");
			error("Error: Problem inserting RTP alaw payload frequency");
			return;
		}
	}
	else {
		if (insert_frequency(0, frequency, length, payload_entry, amp_index) == 0) {
			//printf("Error: Problem inserting RTP ulaw payload frequency\n");
			error("Error: Problem inserting RTP ulaw payload frequency");
			return;
		}
	}

}