int icmp_header(GtkButton *button, int whocalled) {

	int x;

	if (whocalled==2) {
		protokol = ICMP;
		return 1;
	}

	/* well normal icmp type should have at least 8 bytes, so this is min for us */
	if (remain < 8) {
		error("Can't load packet: Packet length shorter than ICMP header length!");
		return -1;
	}

	remain = remain -8;

	/* icmp type */
	x = char2x(ptrf);
	/* insert version */
	inspar(button, "entry57", ptrf, 2);

	w1 = lookup_widget(GTK_WIDGET(button), "optionmenu4");
	if (x == 0)
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 0);
	else if (x == 3) 
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 1);
	else if (x == 8) 
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 2);
	else
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 3);


	if (x == 0) { /* echo reply */
		/* insert code, checksum, identifier and seq number and data if there is some */
		w1 = lookup_widget(GTK_WIDGET(button), "notebook5");
		gtk_notebook_set_page(GTK_NOTEBOOK(w1), 0);
		inspar(button, "entry62", ptrf, 2);
		//inspar(button, "entry63", ptrf, 4);
		ptrf = ptrf + 4;
		inspar(button, "entry64", ptrf, 4);
		inspar(button, "entry65", ptrf, 4);
		/* set checksum button on auto */
		w2 = lookup_widget(GTK_WIDGET(button), "checkbutton16");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w2), TRUE);

		inspar(button, "entry66", ptrf, remain * 2);
		if (remain > 0) {
			w1 = lookup_widget(GTK_WIDGET(button), "checkbutton17");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		}
		else {
			w1 = lookup_widget(GTK_WIDGET(button), "checkbutton17");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), FALSE);
		}
		
	}
	else if (x == 3) { /* destination unreacheable */
		w1 = lookup_widget(GTK_WIDGET(button), "notebook5");
		gtk_notebook_set_page(GTK_NOTEBOOK(w1), 2);
		/* which code? */
		x = char2x(ptrf);
		/* insert code */
		inspar(button, "entry58", ptrf, 2);

		w1 = lookup_widget(GTK_WIDGET(button), "optionmenu5");
		if ( (x >= 0) && (x <= 15) )
			gtk_option_menu_set_history (GTK_OPTION_MENU (w1), x);
		else
			gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 16);
		
		/* insert code, checksum, identifier and seq number and data if there is some */
		//inspar(button, "entry59", ptrf, 4);
		ptrf = ptrf + 4;
		inspar(button, "entry60", ptrf, 8);
		/* set checksum button on auto */
		w2 = lookup_widget(GTK_WIDGET(button), "checkbutton15");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w2), TRUE);

		inspar(button, "entry61", ptrf, remain * 2);
		if (remain > 0) {
			w1 = lookup_widget(GTK_WIDGET(button), "checkbutton24");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		}
		else {
			w1 = lookup_widget(GTK_WIDGET(button), "checkbutton24");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), FALSE);
		}
	}
	else if (x == 8) { /* echo request */
		w1 = lookup_widget(GTK_WIDGET(button), "notebook5");
		gtk_notebook_set_page(GTK_NOTEBOOK(w1), 5);
		/* insert code, checksum, identifier and seq number and data if there is some */
		inspar(button, "entry74", ptrf, 2);
		//inspar(button, "entry77", ptrf, 4);
		ptrf = ptrf + 4;
		inspar(button, "entry75", ptrf, 4);
		inspar(button, "entry78", ptrf, 4);
		/* set checksum button on auto */
		w2 = lookup_widget(GTK_WIDGET(button), "checkbutton20");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w2), TRUE);

		inspar(button, "entry76", ptrf, remain * 2);
		if (remain > 0) {
			w1 = lookup_widget(GTK_WIDGET(button), "checkbutton19");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		}
		else {
			w1 = lookup_widget(GTK_WIDGET(button), "checkbutton19");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), FALSE);
		}

	}
	else { /* all the rest */
		w1 = lookup_widget(GTK_WIDGET(button), "notebook5");
		gtk_notebook_set_page(GTK_NOTEBOOK(w1), 1);
		/* insert code, checksum and data if there is some */
		inspar(button, "entry157", ptrf, 2);
		//inspar(button, "entry158", ptrf, 4);
		ptrf = ptrf + 4;
		/* set checksum button on auto */
		w2 = lookup_widget(GTK_WIDGET(button), "checkbutton38");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w2), TRUE);

		inspar(button, "entry159", ptrf, (remain + 4) * 2);

	}

	return 1;
}