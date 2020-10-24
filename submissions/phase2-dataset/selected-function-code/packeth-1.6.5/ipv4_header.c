int ipv4_header(GtkButton *button, int whocalled, struct clist_hdr *clptr ) {

	char tmp[5];
	int x, header_l, prot;

	if (whocalled==2) {
		protokol = IPv4;
	}

	/* for standard header this is minimum length */
	if (remain < 20) {
		error("Can't load packet: IPv4 header field is not long enough!");
		return -1;
	}

	/* first comes version but we will first check the length and then insert version */
	ptrf++;

	/* check the header length */
	/* we don't need to check the return value here, it is already done when reading from file */
	header_l = retint(ptrf);
	/* header length is the number of 32-bit words in the header, including any options. 
	 * Since this is a 4-bit field, it limits the header to 60 bytes. So the remaining length 
	 * should be at least that long or we exit here */
	if ( (header_l * 4) < 20 ) {
		error("Can't load packet:\nIPv4 header length shorter than 20 bytes!");
		return -1;
	}
	if ( (header_l * 4) > remain ) {
		error("Can't load packet:\nPacket length shorter than IPv4 header length!");
		return -1;
	}
	ptrf--;

	if (whocalled==1) {
		/* insert version */
		inspar(button, "entry26", ptrf, 1);

		/* insert header length */
		inspar(button, "entry27", ptrf, 1);

		/* insert tos */
		inspar(button, "entry28", ptrf, 2);

		/* insert total length */
		w1 = lookup_widget(GTK_WIDGET(button), "checkbutton21");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		//insint(button, "entry29", ptrf, 4);
	}
	else
		ptrf = ptrf+4;

	ptrf = ptrf + 4;

	if (whocalled==1) {
		/* insert identification */
		inspar(button, "entry30", ptrf, 4);
	}
	else
		ptrf = ptrf+4;

	/* insert flags */
	*tmp = 0x30; /* 0x30 == 0 */
	*(tmp+1) = *ptrf;
	x = char2x(tmp);
	x = x >> 1; /* use only first 3 bits */
	
	if (whocalled==1) {
		w1 = lookup_widget(GTK_WIDGET(button), "entry31");
        	snprintf(tmp, 4, "%d", x);
        	gtk_entry_set_text(GTK_ENTRY(w1), tmp);
	}

	/* insert fragment offset */
	*tmp = 0x30; /* 0x30 == 0 */
	*(tmp+1) = *ptrf;
	x = (char2x(tmp)%2); /* need only last bit */
	if (x == 0)
		*tmp = 0x30;
	else
		*tmp = 0x31;
	strncpy(tmp+1, ptrf+1, 3);

	if (whocalled==1) {
		insint(button, "entry32", tmp, 4);
	
		/* insert ttl */
		insint(button, "entry44", ptrf, 2);
	}
	else
		ptrf = ptrf+6;

	prot = char2x(ptrf);

	if (whocalled==1) {
		/* insert protocol */
		insint(button, "entry34", ptrf, 2);

		/* insert header checksum */
		w1 = lookup_widget(GTK_WIDGET(button), "ip_header_cks_cbt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		//inspar(button, "entry35", ptrf, 4);
	}
	else
		ptrf = ptrf+2;

	ptrf = ptrf + 4;

	/*insert source ip */
	ptrt = temp;
	memset(temp, 0, 20);
	if (whocalled==1)
		w1 = lookup_widget(GTK_WIDGET(button), "entry38");
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

	if (whocalled==1)
		gtk_entry_set_text(GTK_ENTRY(w1), temp);
	else
		memcpy(clptr->src, temp, 20);

	/*insert destination ip */
	ptrt = temp;
	memset(temp, 0, 20);
	if (whocalled==1)
		w1 = lookup_widget(GTK_WIDGET(button), "entry37");
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
	if (whocalled==1)
		gtk_entry_set_text(GTK_ENTRY(w1), temp);
	else
		memcpy(clptr->dst, temp, 20);

	/* insert ipv4 options 
	 * header_l * 4 == total header length, - 20 for standard header == options length in bytes*/
	if (whocalled==1)
		inspar(button, "entry39", ptrf, ( (header_l*4) - 20) * 2);

	remain = remain - (header_l * 4);
	
	return prot;
}