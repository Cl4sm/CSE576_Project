int igmp_get(GtkButton *button, gpointer user_data) {
	
	GtkWidget *type, *menux, *menu, *menu_item; 

	GtkWidget *maxresptime, *checksum, *cks_bt, *groupaddress, *resv, *nosf, *sourceaddresses;

	gchar *type_t;
	gchar *maxresptime_t, *checksum_t, *groupaddress_t;	
	gchar *resv_t, *nosf_t, *sourceaddresses_t;	
	
	int igmp_start, igmp_stop, checksum_start, payload_length;
	guint16 igmpcksum;
	gchar tmp[4];
	int i, j, menu_index;
	
	type = lookup_widget(GTK_WIDGET(button), "entry166");
	type_t = (char *)gtk_entry_get_text(GTK_ENTRY(type));
	
	igmp_start = number;
	
	/* type */
	if (char2x(type_t) == -1) {
		//printf("Error: igmp type field\n");
		error("Error: igmp type field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(type_t);
	number++;

	maxresptime = lookup_widget(GTK_WIDGET(button), "entry167");
	menux = lookup_widget(GTK_WIDGET(button), "optionmenu20");
	checksum = lookup_widget(GTK_WIDGET(button), "entry168");
	cks_bt = lookup_widget(GTK_WIDGET(button), "checkbutton41");

	maxresptime_t = (char *)gtk_entry_get_text(GTK_ENTRY(maxresptime));
	checksum_t = (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
	/*gcc warning otherwise*/
	groupaddress = lookup_widget(GTK_WIDGET(button), "entry169");

	/* igmp max response time */
	if (char2x(maxresptime_t) == -1) {
		//printf("Error: igmp max response time\n");
		error("Error: igmp max response time");
		return -1;
	}
	packet[number] = (unsigned char)char2x(maxresptime_t);
	number++;
			
	/* checksum */	
	if (GTK_TOGGLE_BUTTON(cks_bt)->active) {
		checksum_start = number;
		packet[number] = (unsigned char)0;
		number++;
		packet[number] = (unsigned char)0;
		number++;
	}
	else {
		/* if checksum_start = 0, we leave it in the end */
		checksum_start = 0;

		if (char2x(checksum_t) == -1) {
			//printf("Error: igmp reply checksum field\n");
			error("Error: igmp checksum field");
			return -1;
		}
		packet[number] = (unsigned char)char2x(checksum_t);
		checksum_t++; checksum_t++; number++;
		if (char2x(checksum_t) == -1) {
			//printf("Error: igmp reply checksum field\n");
			error("Error: igmp checksum field");
			return -1;
		}
		packet[number] = (unsigned char)char2x(checksum_t);
		number++;
	}
			
	menux = lookup_widget(GTK_WIDGET(button), "optionmenu20");

	menu = GTK_OPTION_MENU(menux)->menu;
        menu_item = gtk_menu_get_active (GTK_MENU (menu));
        menu_index = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);

	/* IGMP V3 query */
	if (menu_index == 1) {
		/* group address */
		groupaddress = lookup_widget(GTK_WIDGET(button), "entry169");
		groupaddress_t = (char *)gtk_entry_get_text(GTK_ENTRY(groupaddress));
		if (check_ip_address(groupaddress_t) == -1) {
			//printf("Error: Wrong igmp address\n");
			error("Error: Wrong igmp group address");
			return -1;
		}
		
		for (i=0; i<4; i++) {
			for(j=0; j<4 && (*groupaddress_t != '\0'); j++) {
				if ( ((int)*groupaddress_t == '.') && (i<3) && (j>0) ) {
					groupaddress_t++;
					break;
				}
				tmp[j] = *groupaddress_t;
				groupaddress_t++;
			}
			tmp[j] = '\0';
			packet[number] = (unsigned char)(atoi(tmp));
			number++;		
		}
			
		resv = lookup_widget(GTK_WIDGET(button), "entry171");
		nosf = lookup_widget(GTK_WIDGET(button), "entry172");
		sourceaddresses = lookup_widget(GTK_WIDGET(button), "entry173");

		resv_t = (char *)gtk_entry_get_text(GTK_ENTRY(resv));
		nosf_t = (char *)gtk_entry_get_text(GTK_ENTRY(nosf));
		sourceaddresses_t = (char *)gtk_entry_get_text(GTK_ENTRY(sourceaddresses));

		/* Resv, S, QRV, QQIC IGMP V3 values */
		if (char2x(resv_t) == -1) {
			//printf("Error: Resv, S, QRV, QQIC IGMP V3 values\n");
			error("Error: Resv, S, QRV, QQIC IGMP V3 values");
			return -1;
		}
		packet[number] = (unsigned char)char2x(resv_t);
		resv_t++; resv_t++; number++;
		if (char2x(resv_t) == -1) {
			//printf("Error: Resv, S, QRV, QQIC IGMP V3 values\n");
			error("Error: Resv, S, QRV, QQIC IGMP V3 values");
			return -1;
		}
		packet[number] = (unsigned char)char2x(resv_t);
		number++;
		
		/* number of sources */
		if (char2x(nosf_t) == -1) {
			//printf("Error: IGMP V3 number of sources\n");
			error("Error: IGMP V3 number of sources");
			return -1;
		}
		packet[number] = (unsigned char)char2x(nosf_t);
		nosf_t++; nosf_t++; number++;
		if (char2x(nosf_t) == -1) {
			//printf("Error: IGMP V3 number of sources\n");
			error("Error: IGMP V3 number of sources");
			return -1;
		}
		packet[number] = (unsigned char)char2x(nosf_t);
		number++;
		
		/* source addresses */
		payload_length = strlen(sourceaddresses_t);
		
		/* YYY 1514-number is not ok in case we use 802.1q!!! */
		if (get_network_payload(button, user_data, payload_length, 
					1514-number, sourceaddresses_t) == -1) {
			//printf("problem with igmp reply payload\n");
			return -1;
		}
		
		igmp_stop = number;

		if (checksum_start > 0) {
			
			igmpcksum =  get_checksum16(igmp_start, igmp_stop); 
			/* the one's complement */
			igmpcksum = (-1) - igmpcksum;

			/* let's write it */
			packet[checksum_start] = (char)(igmpcksum/256);
			packet[checksum_start+1] =  (char)(igmpcksum%256);
		}
	}

	/* IGMP V3 report */
	else if (menu_index == 4) {
		resv = lookup_widget(GTK_WIDGET(button), "entry176");
		nosf = lookup_widget(GTK_WIDGET(button), "entry177");
		sourceaddresses = lookup_widget(GTK_WIDGET(button), "entry178");

		resv_t = (char *)gtk_entry_get_text(GTK_ENTRY(resv));
		nosf_t = (char *)gtk_entry_get_text(GTK_ENTRY(nosf));
		sourceaddresses_t = (char *)gtk_entry_get_text(GTK_ENTRY(sourceaddresses));

		/* Resv values */
		if (char2x(resv_t) == -1) {
			//printf("Error: Resv, S, QRV, QQIC IGMP V3 values\n");
			error("Error: Reserved IGMP V3 report values");
			return -1;
		}
		packet[number] = (unsigned char)char2x(resv_t);
		resv_t++; resv_t++; number++;
		if (char2x(resv_t) == -1) {
			//printf("Error: Resv, S, QRV, QQIC IGMP V3 values\n");
			error("Error: Reserved IGMP V3 report values");
			return -1;
		}
		packet[number] = (unsigned char)char2x(resv_t);
		number++;
		
		/* number of group records */
		if (char2x(nosf_t) == -1) {
			//printf("Error: IGMP V3 number of sources\n");
			error("Error: IGMP V3 report number of group records");
			return -1;
		}
		packet[number] = (unsigned char)char2x(nosf_t);
		nosf_t++; nosf_t++; number++;
		if (char2x(nosf_t) == -1) {
			//printf("Error: IGMP V3 number of sources\n");
			error("Error: IGMP V3 report number of group records");
			return -1;
		}
		packet[number] = (unsigned char)char2x(nosf_t);
		number++;
		
		/* group records */
		payload_length = strlen(sourceaddresses_t);
		
		/* YYY 1514-number is not ok in case we use 802.1q!!! */
		if (get_network_payload(button, user_data, payload_length, 
					1514-number, sourceaddresses_t) == -1) {
			//printf("problem with igmp reply payload\n");
			return -1;
		}
		
		igmp_stop = number;

		if (checksum_start > 0) {
			
			igmpcksum =  get_checksum16(igmp_start, igmp_stop); 
			/* the one's complement */
			igmpcksum = (-1) - igmpcksum;

			/* let's write it */
			packet[checksum_start] = (char)(igmpcksum/256);
			packet[checksum_start+1] =  (char)(igmpcksum%256);
		}


	}
	/* for all the other types */
	else	{
		/* group address */
		groupaddress = lookup_widget(GTK_WIDGET(button), "entry175");
		groupaddress_t = (char *)gtk_entry_get_text(GTK_ENTRY(groupaddress));
		if (check_ip_address(groupaddress_t) == -1) {
			//printf("Error: Wrong igmp address\n");
			error("Error: Wrong igmp group address");
			return -1;
		}
		
		for (i=0; i<4; i++) {
			for(j=0; j<4 && (*groupaddress_t != '\0'); j++) {
				if ( ((int)*groupaddress_t == '.') && (i<3) && (j>0) ) {
					groupaddress_t++;
					break;
				}
				tmp[j] = *groupaddress_t;
				groupaddress_t++;
			}
			tmp[j] = '\0';
			packet[number] = (unsigned char)(atoi(tmp));
			number++;		
		}

		igmp_stop = number;

		if (checksum_start > 0) {
			
			igmpcksum =  get_checksum16(igmp_start, igmp_stop); 
			/* the one's complement */
			igmpcksum = (-1) - igmpcksum;

			/* let's write it */
			packet[checksum_start] = (char)(igmpcksum/256);
			packet[checksum_start+1] =  (char)(igmpcksum%256);
		}
	}	
	
	return 1;
}