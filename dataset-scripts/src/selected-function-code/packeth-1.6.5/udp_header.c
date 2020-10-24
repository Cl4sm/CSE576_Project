int udp_header(GtkButton *button, int whocalled) {

	int i, j;
	char tmp[4600];

	if (whocalled==2) {
		protokol = UDP;
		return 1;
	}

	/* for standard header this is minimum length */
	if (remain < 8) {
		error("Can't load packet: Packet length shorter than UDP header length!");
		return -1;
	}

	remain = remain - 8;

	/* source port */
	insint(button, "entry56", ptrf, 4);
	
	/* destination port */
	insint(button, "entry41", ptrf, 4);

	/* length */
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton3");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
	//insint(button, "entry42", ptrf, 4);
	ptrf = ptrf + 4;

	/* checksum */
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton4");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
	//inspar(button, "entry43", "", 4);
	ptrf = ptrf + 4;

	/* get access to buffer of the text field */
	w2 = lookup_widget(GTK_WIDGET(button), "text3");
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w2));

	if (remain > 0) {
		w1 = lookup_widget(GTK_WIDGET(button), "checkbutton5");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
	
		/* copy data to tmp field */
		for (i=0, j=1; (i < (remain * 3) ); i++, j++) {
			tmp[i] = *ptrf++; i++;
			tmp[i] = *ptrf++; i++;
			/* we allow only 16 bytes in each row - looks nicer */
			if ((j % 16) == 0 && (j > 1)) {
				tmp[i]='\n';
				j = 0;
			}
			else
				tmp[i] = ' ';
		}
		tmp[i] = '\0';

		/* insert the text in the text field */
		gtk_text_buffer_set_text(buffer,tmp,-1);
	}
	else {	
		w1 = lookup_widget(GTK_WIDGET(button), "checkbutton5");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), FALSE);
	}

	/* since udp does not have any protocol field we could return destination port value which 
	 * usually describes next layer protocol; currently we return 1 */
	return 1;
}