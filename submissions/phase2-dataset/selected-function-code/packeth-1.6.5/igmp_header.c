int igmp_header(GtkButton *button, int whocalled) {

	int x, x1;
	char tmp[5];

	if (whocalled==2) {
		protokol = IGMP;
		return 1;
	}

	/* well normal igmp type should have at least 8 bytes, so this is min for us */
	if (remain < 8) {
		error("Can't load packet: Packet length shorter than IGMP header length!");
		return -1;
	}

	remain = remain -8;

	/* igmp type */
	x = char2x(ptrf);
	/* insert version */
	inspar(button, "entry166", ptrf, 2);

	w1 = lookup_widget(GTK_WIDGET(button), "optionmenu20");
	w2 = lookup_widget(GTK_WIDGET(button), "notebook8");
	if (x == 17) {
		if (remain > 4) {
	        	gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 1);
			gtk_notebook_set_page(GTK_NOTEBOOK(w2), 1);
			}
		else	{
	        	gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 0);
			gtk_notebook_set_page(GTK_NOTEBOOK(w2), 0);
			}
	}
	else if (x == 18) {
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 2);
		gtk_notebook_set_page(GTK_NOTEBOOK(w2), 0);
		}
	else if (x == 22) {
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 3);
		gtk_notebook_set_page(GTK_NOTEBOOK(w2), 0);
		}
	else if (x == 34) {
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 4);
		gtk_notebook_set_page(GTK_NOTEBOOK(w2), 2);
		}
	else if (x == 23) {
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 5);
		gtk_notebook_set_page(GTK_NOTEBOOK(w2), 0);
		}
	else	{
	        gtk_option_menu_set_history (GTK_OPTION_MENU (w1), 6);
		gtk_notebook_set_page(GTK_NOTEBOOK(w2), 0);
		}

	inspar(button, "entry167", ptrf, 2);

	/* set checksum button on auto */
	w2 = lookup_widget(GTK_WIDGET(button), "checkbutton41");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w2), TRUE);
	ptrf = ptrf + 4;


	if ( (x == 17) && (remain>4) ) { /* IGMP V3 query */
		/*insert group ip */
		ptrt = temp;
		memset(temp, 0, 20);
		w1 = lookup_widget(GTK_WIDGET(button), "entry169");
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

		inspar(button, "entry171", ptrf, 4);
		x1 = (int)retint2(ptrf, 4);
		inspar(button, "entry172", ptrf, 4);
		/*#inspar(button, "entry173", ptrf, x1);*/
		inspar(button, "entry173", ptrf, remain);
		
	}
	else if (x==22) { /*IGMP V3 report */
		inspar(button, "entry176", ptrf, 4);
		x1 = (int)retint2(ptrf, 4);
		inspar(button, "entry177", ptrf, 4);
		inspar(button, "entry178", ptrf, x1);
		
	}
	else { /*all the other versions */
		/*insert group ip */
		ptrt = temp;
		memset(temp, 0, 20);
		w1 = lookup_widget(GTK_WIDGET(button), "entry175");
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
			
	
	}	

	return 1;
}