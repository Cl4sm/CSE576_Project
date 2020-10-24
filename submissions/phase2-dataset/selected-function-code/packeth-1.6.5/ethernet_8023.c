int ethernet_8023(GtkButton *button, int whocalled) {

	int dsap, lsap, ctrl;
	long pid;

	if (whocalled==2) {
		protokol = ETH_802_3;
		return 1;
	}

	if (remain < 6) {
		error("Can't load packet: Ethernet 802.3 LLC field is not long enough!");
		return -1;
	}
	remain = remain - 3;

	w1 = lookup_widget(GTK_WIDGET(button), "bt_8023");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);

	//w2 = lookup_widget(GTK_WIDGET(button), "frame7");
	//gtk_widget_set_sensitive (w2, TRUE);
	//gtk_notebook_set_page(GTK_NOTEBOOK(w3), 1);

	w1 = lookup_widget(GTK_WIDGET(button), "entry5");
	//inspar(button, "entry5", ptrf, 4);
	ptrf = ptrf + 4;
	gtk_widget_set_sensitive (w1, FALSE);

	w2 = lookup_widget(GTK_WIDGET(button), "checkbutton2");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w2), TRUE);
	
	/*now the LLC / LLC-SNAP part */
	/* we decode only RFC 1042 format, that means the following value:
		dsap == ssap == 0xAA
		ctrl == 0x03
		OUI  == 0x000000 
	*/
	dsap = char2x(ptrf);	
	inspar(button, "L_dsap", ptrf, 2);
	lsap = char2x(ptrf);	
	inspar(button, "L_ssap", ptrf, 2);
	ctrl = char2x(ptrf);	
	inspar(button, "L_ctrl", ptrf, 2);

	/* in case dsap != ssap != 0xAA or ctrl != 0x03 or remain length < 5 bytes, we have only 
	 * LLC without SNAP and we return value for user defined next layer */
	if ( (dsap != 170 ) || (lsap != 170) || (ctrl != 3) || (remain < 5) ) {
		w1 = lookup_widget(GTK_WIDGET(button), "L_8023_llc_tbt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		w1 = lookup_widget(GTK_WIDGET(button), "L_oui");
		w2 = lookup_widget(GTK_WIDGET(button), "L_pid");
		gtk_widget_set_sensitive (w1, FALSE);
		gtk_widget_set_sensitive (w2, FALSE);
		/* this means we insert all the data as user defined field */
		return -2;
	}
	/* in this case everything is ok but oui in not 0 */
	/*	   <--------------this is oui--------------------->   */	
	else if ( (char2x(ptrf) + char2x(ptrf+2) + char2x(ptrf+4) != 0 ) ) {
		w1 = lookup_widget(GTK_WIDGET(button), "L_8023_llc_tbt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		w1 = lookup_widget(GTK_WIDGET(button), "L_oui");
		w2 = lookup_widget(GTK_WIDGET(button), "L_pid");
		gtk_widget_set_sensitive (w1, FALSE);
		gtk_widget_set_sensitive (w2, FALSE);
		/* this means we insert all the data as user defined field */
		return -2;
	}
	
	/* substract 3 for oui and 2 for pid */
	remain = remain - 5;

	/* ok, so we have dsap and ssap == 0xAA, Ctlr == 0x03, OUI == 0x0 and length is long enough */
	/* set llc-snap button */
	w1 = lookup_widget(GTK_WIDGET(button), "L_8023_llcsnap_tbt");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);

	/* insert 0x00 into oui field */
	inspar(button, "L_oui", ptrf, 6);
	pid = char2x(ptrf)*256 + char2x(ptrf+2);

	ptrf = ptrf + 4;

	return pid;
}