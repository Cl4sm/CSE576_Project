int arp_header(GtkButton *button, int whocalled) {

	char tmp[5];
	int x;

	if (whocalled==2) {
		protokol = ARP;
		return 1;
	}

	/* arp header length == 28; but packet can be longer, f.e. to satisfy the min packet length */
	if (remain < 28) {
		error("Can't load packet: Packet length shorter than ARP header length!");
		return -1;
	}

	remain = remain - 28;

	/* hardware type */
	inspar(button, "A_hwtype", ptrf, 4);

	/* protocol type */
	inspar(button, "A_prottype", ptrf, 4);

	/* hardware size */
	inspar(button, "A_hwsize", ptrf, 2);

	/* protocol size */
	inspar(button, "A_protsize", ptrf, 2);

	/* opcode is next */
	if ( (*ptrf == '0') && (*(ptrf+1) == '0') && (*(ptrf+2) == '0') && (*(ptrf+3) == '1') ) {
		w1 = lookup_widget(GTK_WIDGET(button), "radiobutton10");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		ptrf = ptrf + 4;
	}
	else if ( (*ptrf == '0') && (*(ptrf+1) == '0') && (*(ptrf+2) == '0') && (*(ptrf+3) == '2') ) {
		w1 = lookup_widget(GTK_WIDGET(button), "radiobutton11");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		ptrf = ptrf + 4;
	}
	else {
		w1 = lookup_widget(GTK_WIDGET(button), "radiobutton17");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		inspar(button, "entry81", ptrf, 4);
	}

	/* sender mac */
	ptrt = temp;
	w1 = lookup_widget(GTK_WIDGET(button), "A_sendermac");
	for (i=1; i<=18; i++, ptrt++) {
		if (i%3 == 0) 
			*ptrt = ':';
		else {
			*ptrt = *ptrf++;
		}
	}
	*ptrt = '\0';
	gtk_entry_set_text(GTK_ENTRY(w1), temp);

	/* sender ip */
	ptrt = temp;
	memset(temp, 0, 20);
	w1 = lookup_widget(GTK_WIDGET(button), "A_senderip");
	for (i=1; i<=12; i++, ptrt++) {
		if (i%3 == 0) {			
			x = char2x(tmp);
			if (i==12)
				snprintf(tmp, 4, "%d", x);		
			else
				snprintf(tmp, 5, "%d.", x);		
			strcat(temp, tmp);
		}
		else {
			tmp[(i-1)%3] = *ptrf++;
		}
	}
	gtk_entry_set_text(GTK_ENTRY(w1), temp);

	/* target mac */
	ptrt = temp;
	w1 = lookup_widget(GTK_WIDGET(button), "A_targetmac");
	for (i=1; i<=18; i++, ptrt++) {
		if (i%3 == 0) 
			*ptrt = ':';
		else {
			*ptrt = *ptrf++;
		}
	}
	*ptrt = '\0';
	gtk_entry_set_text(GTK_ENTRY(w1), temp);

	/* target ip */
	ptrt = temp;
	memset(temp, 0, 20);
	w1 = lookup_widget(GTK_WIDGET(button), "A_targetip");
	for (i=1; i<=12; i++, ptrt++) {
		if (i%3 == 0) {			
			x = char2x(tmp);
			if (i==12)
				snprintf(tmp, 4, "%d", x);		
			else
				snprintf(tmp, 5, "%d.", x);		
			strcat(temp, tmp);
		}
		else {
			tmp[(i-1)%3] = *ptrf++;
		}
	}
	gtk_entry_set_text(GTK_ENTRY(w1), temp);

	return 1;

}