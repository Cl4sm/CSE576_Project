int tcp_header(GtkButton *button, int whocalled) {

	int x, i, j;
	char tmp[4600], tmp2[3], ch;

	if (whocalled==2) {
		protokol = TCP;
		return 1;
	}


	/* for standard header this is minimum length */
	if (remain < 20) {
		error("Can't load packet: Packet length shorter than TCP header length!");
		return -1;
	}

	/* ok, packet is long enough to fill in the standard header, but what is the header length?
	 * we insert this later but need now to see that the packet is long enough */
	x = retint(ptrf+24);
	if ( (x * 4) > remain ) {
		error("Can't load packet:\nPacket length shorter than TCP header length!");
		return -1;
	}
	if ( x < 5 ) {
		error("Can't load packet:\nTCP header length shorter than 20 bytes!");
		return -1;
	}

	/* source port */
	insint(button, "entry46", ptrf, 4);
	
	/* destination port */
	insint(button, "entry47", ptrf, 4);

	/* sequence number */
	insint(button, "entry48", ptrf, 8);

	/* acknowledgement number */
	insint(button, "entry49", ptrf, 8);
	
	/* now we insert value for length */
	snprintf(tmp2, 3, "%d", x*4);
	w1 = lookup_widget(GTK_WIDGET(button), "entry50");
	gtk_entry_set_text(GTK_ENTRY(w1), tmp2);

	/* increase by one for length and for another one for 4 bits that are reserved */
	ptrf = ptrf + 2;

	/* flags; next byte */	
	ch = char2x(ptrf) % 0x0100;

	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton22");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x80) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton23");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x40) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton7");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x20) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton8");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x10) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton9");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x08) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton10");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x04) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton11");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x02) > 0 ? TRUE : FALSE);
	
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton12");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), (ch & 0x01) > 0 ? TRUE : FALSE);
	
	ptrf = ptrf + 2;

	/* window size */
	insint(button, "entry51", ptrf, 4);
	
	/* checksum */
	w1 = lookup_widget(GTK_WIDGET(button), "checkbutton13");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
	//inspar(button, "entry52", ptrf, 4);
	ptrf = ptrf + 4;

	/* window size */
	insint(button, "entry53", ptrf, 4);
	
	/* any options ? */
	/* - 20 for standard header */
	inspar(button, "entry54", ptrf, ( (x*4) - 20) * 2);

	remain = remain - x*4;

	/* get access to buffer of the text field */
	w2 = lookup_widget(GTK_WIDGET(button), "text4");
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w2));

	if (remain > 0) {
		w1 = lookup_widget(GTK_WIDGET(button), "checkbutton14");
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
		w1 = lookup_widget(GTK_WIDGET(button), "checkbutton14");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), FALSE);
	}

	/* since tcp does not have any protocol field we could return destination port value which 
	 * usually describes next layer protocol; currently we return 1 */

	return 1;
}