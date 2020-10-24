int get_8023(GtkButton *button)
{
	GtkWidget *ethlength_e, *L8023llc_tbt, *L8023llcsnap_tbt, *Ldsap_e, *Lssap_e;
	GtkWidget *Lctrl_e, *Loui_e, *Lpid_e, *autolength_bt;
	gchar *Ldsap_t, *Lssap_t, *Lctrl_t, *Loui_t, *Lpid_t;
	gchar *ethlength_t;

	/* do we need to calculate the length field or will be suplied manually */
	autolength_bt = lookup_widget(GTK_WIDGET (button), "checkbutton2");
	if (GTK_TOGGLE_BUTTON(autolength_bt)->active) {
                autolength = number;
		packet[number] = 0x0; number++; packet[number] = 0x0; number++;
	}
        else {
                autolength = 0;
		ethlength_e = lookup_widget(GTK_WIDGET (button), "entry5");
		ethlength_t = (char *)gtk_entry_get_text(GTK_ENTRY(ethlength_e));
		if (char2x(ethlength_t) == -1) {
			//printf("Error: 802.3 length field\n");
			error("Error: 802.3 length field");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(ethlength_t);
		ethlength_t++; ethlength_t++; number++;
		if (char2x(ethlength_t) == -1) {
			//printf("Error: 802.3 length field\n");
			error("Error: 802.3 length field");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(ethlength_t);
		number++;
	}
	
	L8023llc_tbt = lookup_widget(GTK_WIDGET (button), "L_8023_llc_tbt");
	L8023llcsnap_tbt = lookup_widget(GTK_WIDGET (button), "L_8023_llcsnap_tbt");
	Ldsap_e = lookup_widget(GTK_WIDGET (button), "L_dsap");
	Lssap_e= lookup_widget(GTK_WIDGET (button), "L_ssap");
	Lctrl_e= lookup_widget(GTK_WIDGET (button), "L_ctrl");

	Ldsap_t = (char *)gtk_entry_get_text(GTK_ENTRY(Ldsap_e));
	if (char2x(Ldsap_t) == -1) {
		//printf("Error: 802.3 ldsap field\n");
		error("Error: 802.3 ldsap field");
	               return -1;
	}
	packet[number] = (unsigned char)char2x(Ldsap_t);
	number++;

	Lssap_t = (char *)gtk_entry_get_text(GTK_ENTRY(Lssap_e));
	if (char2x(Lssap_t) == -1) {
		//printf("Error: 802.3 lssap field\n");
		error("Error: 802.3 lssap field");
	               return -1;
	}
	packet[number] = (unsigned char)char2x(Lssap_t);
	number++;

	Lctrl_t = (char *)gtk_entry_get_text(GTK_ENTRY(Lctrl_e));
	if (char2x(Lctrl_t) == -1) {
		//printf("Error: 802.3 Ctrl field\n");
		error("Error: 802.3 Ctrl field");
	               return -1;
	}
	packet[number] = (unsigned char)char2x(Lctrl_t);
	number++;

	/* do we need snap encapsulation */ 
	if (GTK_TOGGLE_BUTTON(L8023llcsnap_tbt)->active) {
		Loui_e = lookup_widget(GTK_WIDGET (button), "L_oui");
		Lpid_e = lookup_widget(GTK_WIDGET (button), "L_pid");
		
		Loui_t = (char *)gtk_entry_get_text(GTK_ENTRY(Loui_e));
		if (char2x(Loui_t) == -1) {
			//printf("Error: 802.3 oui field\n");
			error("Error: 802.3 oui field");
	               	return -1;
		}
		packet[number] = (unsigned char)char2x(Loui_t);
		number++; Loui_t++, Loui_t++;

		if (char2x(Loui_t) == -1) {
			//printf("Error: 802.3 oui field\n");
			error("Error: 802.3 oui field");
	               	return -1;
		}
		packet[number] = (unsigned char)char2x(Loui_t);
		number++; Loui_t++, Loui_t++;

		if (char2x(Loui_t) == -1) {
			//printf("Error: 802.3 oui field\n");
			error("Error: 802.3 oui field");
	               	return -1;
		}
		packet[number] = (unsigned char)char2x(Loui_t);
		number++; 

		Lpid_t = (char *)gtk_entry_get_text(GTK_ENTRY(Lpid_e));
		if (char2x(Lpid_t) == -1) {
			//printf("Error: 802.3 snap pid field\n");
			error("Error: 802.3 snap pid field");
	               	return -1;
		}
		packet[number] = (unsigned char)char2x(Lpid_t);
		number++; Lpid_t++; Lpid_t++;

		if (char2x(Lpid_t) == -1) {
			//printf("Error: 802.3 snap pid field\n");
			error("Error: 802.3 snap pid field");
	               	return -1;
		}
		packet[number] = (unsigned char)char2x(Lpid_t);
		number++; 

		return 1;
	}
        else 
		return 1;
}