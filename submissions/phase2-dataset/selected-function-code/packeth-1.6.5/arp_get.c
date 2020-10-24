int arp_get(GtkButton *button, gpointer user_data)
{
	GtkWidget *hwtype, *prottype, *hwsize, *protsize;
	GtkWidget *rbt10, *rbt11, *rbt17, *en81;
	GtkWidget *sendermac, *senderip, *targetmac, *targetip;
	gchar *hwtype_t, *prottype_t, *hwsize_t, *protsize_t, *en81_t;
	gchar *sendermac_t, *senderip_t, *targetmac_t, *targetip_t;
	int i, j;
	gchar tmp[4];

	hwtype = lookup_widget(GTK_WIDGET(button), "A_hwtype");	
	prottype = lookup_widget(GTK_WIDGET(button), "A_prottype");	
	hwsize = lookup_widget(GTK_WIDGET(button), "A_hwsize");	
	protsize = lookup_widget(GTK_WIDGET(button), "A_protsize");	
	
        rbt10 = lookup_widget(GTK_WIDGET(button), "radiobutton10");	
	rbt11 = lookup_widget(GTK_WIDGET(button), "radiobutton11");	
	rbt17 = lookup_widget(GTK_WIDGET(button), "radiobutton17");	
	en81 = lookup_widget(GTK_WIDGET(button), "entry81");	

	sendermac = lookup_widget(GTK_WIDGET(button), "A_sendermac");	
	senderip = lookup_widget(GTK_WIDGET(button), "A_senderip");	
	targetmac = lookup_widget(GTK_WIDGET(button), "A_targetmac");	
	targetip = lookup_widget(GTK_WIDGET(button), "A_targetip");	

	hwtype_t = (char *)gtk_entry_get_text(GTK_ENTRY(hwtype));
	prottype_t = (char *)gtk_entry_get_text(GTK_ENTRY(prottype));
	hwsize_t = (char *)gtk_entry_get_text(GTK_ENTRY(hwsize));
	protsize_t = (char *)gtk_entry_get_text(GTK_ENTRY(protsize));
	sendermac_t = (char *)gtk_entry_get_text(GTK_ENTRY(sendermac));
	senderip_t = (char *)gtk_entry_get_text(GTK_ENTRY(senderip));
	targetmac_t = (char *)gtk_entry_get_text(GTK_ENTRY(targetmac));
	targetip_t = (char *)gtk_entry_get_text(GTK_ENTRY(targetip));
	en81_t = (char *)gtk_entry_get_text(GTK_ENTRY(en81));
	
	/* now we have all the widget and we start parsing the info: we start with the hardware type */
	if (char2x(hwtype_t) == -1) {
		//printf("Error: hwtype field\n");
		error("Error: hwtype field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(hwtype_t);
	hwtype_t++; hwtype_t++; number++;
	if (char2x(hwtype_t) == -1) {
		//printf("Error: hwtype field\n");
		error("Error: hwtype field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(hwtype_t);
	number++;

	/* prottype */
	if (char2x(prottype_t) == -1) {
		//printf("Error: prottype field\n");
		error("Error: prottype field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(prottype_t);
	prottype_t++; prottype_t++; number++;
	if (char2x(prottype_t) == -1) {
		//printf("Error: prottype field\n");
		error("Error: prottype field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(prottype_t);
	number++;

	/* hwsize */
	if (char2x(hwsize_t) == -1) {
		//printf("Error: hwsize field\n");
		error("Error: hwsize field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(hwsize_t);
	number++;

	/* protsize */
	if (char2x(protsize_t) == -1) {
		//printf("Error: protsize field\n");
		error("Error: protsize field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(protsize_t);
	number++;

	/* which opcode */
	if (GTK_TOGGLE_BUTTON(rbt10)->active) {
		packet[number] = 0x00;
		number++;
		packet[number] = 0x01;
		number++;
		
	}
	else if (GTK_TOGGLE_BUTTON(rbt11)->active) { 
		packet[number] = 0x00;
		number++;
		packet[number] = 0x02;
		number++;
	}
	else if (GTK_TOGGLE_BUTTON(rbt17)->active) { 
		if (char2x(en81_t) == -1) {
			//printf("Error: entry arp opcode\n");
			error("Error: entry arp opcode");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(en81_t);
		en81_t++; en81_t++; number++;
		if (char2x(en81_t) == -1) {
			//printf("Error: entry arp opcode\n");
			error("Error: entry arp opcode");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(en81_t);
		number++;
	}
	else {
		//printf("Error: Something is wrong with the arp opcode\n");
		error("Error: Something is wrong with the arp opcode");
		return -1;
	}

	
	/* and now the ip&mac values: check if addresses are ok */
	eth_start = number;
	
	if (check_mac_address(sendermac_t) == -1) {
		//printf("Error: Wrong mac entry in arp sender field, can't copy it\n");
		error("Error: Wrong mac entry in arp sender field, can't copy it");
		return -1;
	}
	if (check_mac_address(targetmac_t) == -1) {
		//printf("Error: Wrong mac entry in arp target field, can't copy it\n");
		error("Error: Wrong mac entry in arp target field, can't copy it");
		return -1;
	}
	if (check_ip_address(senderip_t) == -1) {
		//printf("Error: Wrong ip entry in arp sender field, can't copy it\n");
		error("Error: Wrong ip entry in arp sender field, can't copy it");
		return -1;
	}
	if (check_ip_address(targetip_t) == -1) {
		//printf("Error: Wrong ip entry in arp target field, can't copy it\n");
		error("Error: Wrong ip entry in arp target field, can't copy it");
		return -1;
	}
	
	/* if all addresses are ok, we copy them into packet: first sender mac */
	for(i=0; i<6; i++) {
		packet[number] = (unsigned char)char2x(sendermac_t);
		sendermac_t = sendermac_t + 3; number++;
	}
	
	/* sender ip */
	for (i=0; i<4; i++) {
		for(j=0; j<4 && (*senderip_t != '\0'); j++) {
			if ( ((int)*senderip_t == '.') && (i<3) && (j>0) ) {
				senderip_t++;
				break;
			}
			tmp[j] = *senderip_t;
			senderip_t++;
		}
		tmp[j] = '\0';
		packet[number] = (unsigned char)(atoi(tmp));
		number++;		
	}
	
	/* target mac */
	for(i=0; i<6; i++) {
		packet[number] = (unsigned char)char2x(targetmac_t);
		targetmac_t = targetmac_t + 3; number++;
	}
	
	/* target ip */
	for (i=0; i<4; i++) {
		for(j=0; j<4 && (*targetip_t != '\0'); j++) {
			if ( ((int)*targetip_t == '.') && (i<3) && (j>0) ) {
				targetip_t++;
				break;
			}
			tmp[j] = *targetip_t;
			targetip_t++;
		}
		tmp[j] = '\0';
		packet[number] = (unsigned char)(atoi(tmp));
		number++;		
	}
	
	return 1;
}