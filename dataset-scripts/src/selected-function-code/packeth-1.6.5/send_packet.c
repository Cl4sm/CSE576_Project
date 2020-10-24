int send_packet(GtkButton *button, gpointer user_data)
{
	GtkWidget *statusbar, *notebk, *abstime, *reltime, *en5, *en6, *ntbk7;
	GtkWidget *en1, *en2, *en3, *en4, *ckbt1, *ckbt2, *xoptm, *yoptm, *xmenu_item, *ymenu_item; 
	GtkWidget *optm1, *optm2, *optm3, *menu, *xmenu, *ymenu, *menu_item, *stopbt, *toolbar;
	GtkWidget *button1, *button2, *button3, *button4, *button5, *button6;

	int c, i, m, length;
	gchar *en1_t, *en2_t, *en3_t, *en4_t, *en5_t, *en6_t;
	gint context_id, page, cpage;
	char buff[100], buf2[80];
	struct tm *ptr;
	struct timeb tp;
	time_t now;
	pthread_t thread_id;

	stop_flag = 0;
	
	statusbar = lookup_widget(GTK_WIDGET (button), "statusbar1");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "Statusbar example");

 	notebk = lookup_widget(GTK_WIDGET (button), "notebook1");

	/* now we have to decide what happens when the send button is pressed */
	page =  gtk_notebook_get_current_page(GTK_NOTEBOOK(notebk));
	
	if ( page == 0 ) { /* so we have the build notebook open, it means we send only one packet */	

		if (make_packet(button, user_data) == -1) {
			//printf("problems with making packet!\n");
			snprintf(buff, 100, "  Problems with making packet!");
			gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(context_id), buff);
			return -1;
		}
		
		/* YYY if the built packet is shorter then 60 bytes, we add padding zero bytes 
		 * to fill up the length till 60 (min ethrenet frame length). This bytes will be 
		 * added anyway by the device driver, so we do this just to prevent misunderstanding: 
		 * till now if your packet  was 20 bytes long, then it was also said -> 20 bytes 
		 * sent on eth0... but actually 60 bytes (+CRC) were sent */
		if (number < 60) {
			memset(&packet[number], 0x00, ( 60 - number ) );
			number = 60;
		}

		/* thats how the packet looks like */
		//for (i = 0; i < number; i++)
		//	printf("%x ", packet[i]);
		//printf("\nnumber je %d\n", number);

		/* let's send the packet */
		c = packet_go_on_the_link(packet, number);
		
		if ( c == -2) {
			//printf("problems with sending\n");
			snprintf(buff, 100, "  Problems with sending!");
			gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(context_id), buff);
			return -1;
		}
		else {
			ftime(&tp);
			now=tp.time;
			ptr=localtime(&now);
			strftime(buf2,80, "%H:%M:%S", ptr);
			snprintf(buff, 100, " %s  -----> %d bytes sent on %s", buf2, c, iftext);
			gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(context_id), buff);
		}	

		return 1;
	}
	
	/* is it the generator that sends the build packets? */
	else if (page == 1) { 
		if (make_packet(button, user_data) == -1) {
			//printf("problems with making packet!\n");
			snprintf(buff, 100, "  Problems with making packet!");
			gtk_statusbar_push(GTK_STATUSBAR(statusbar), 
							GPOINTER_TO_INT(context_id), buff);
			return -1;
		}

		abstime = lookup_widget(GTK_WIDGET (button), "radiobutton34");
		reltime = lookup_widget(GTK_WIDGET (button), "radiobutton35");
		button1 = lookup_widget(GTK_WIDGET (button), "Build_button");
		button2 = lookup_widget(GTK_WIDGET (button), "Gen_button");
		button3 = lookup_widget(GTK_WIDGET (button), "Genp");
		button4 = lookup_widget(GTK_WIDGET (button), "Interface_button");
		button5 = lookup_widget(GTK_WIDGET (button), "Send_button");
		button6 = lookup_widget(GTK_WIDGET (button), "Gensbt");
		stopbt = lookup_widget(GTK_WIDGET (button), "Stop_button");
		en1 = lookup_widget(GTK_WIDGET (button), "entry109");
		en2 = lookup_widget(GTK_WIDGET (button), "entry110");
		ckbt1 = lookup_widget(GTK_WIDGET(button), "checkbutton35");
		ckbt2 = lookup_widget(GTK_WIDGET(button), "checkbutton37");
		ntbk7 = lookup_widget(GTK_WIDGET(button), "notebook7");

		en1_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));
		en2_t = (char *)gtk_entry_get_text(GTK_ENTRY(en2));

		/* do we have to adjust any parameters while sending? */

		cpage = gtk_notebook_get_current_page(GTK_NOTEBOOK(ntbk7));

		/* depending on what "packet type" is currently open we take some action */
		switch (cpage) {
			/* ethernet only */
			case 0: {
				optm1 = lookup_widget(GTK_WIDGET (button), "optionmenu9");
				menu = GTK_OPTION_MENU(optm1)->menu;
				menu_item = gtk_menu_get_active (GTK_MENU (menu));
				params1.inc = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
				params1.type = 0;
				break;
			}
			/* ip frame with user def option or icmp option */
			case 1: {
				optm1 = lookup_widget(GTK_WIDGET (button), "optionmenu16");
				menu = GTK_OPTION_MENU(optm1)->menu;
				menu_item = gtk_menu_get_active (GTK_MENU (menu));
				params1.inc = 10 + g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
				params1.type = 2;
				break;
			}
			/* arp frame */
			case 2: {
				optm1 = lookup_widget(GTK_WIDGET (button), "optionmenu17");
				menu = GTK_OPTION_MENU(optm1)->menu;
				menu_item = gtk_menu_get_active (GTK_MENU (menu));
				params1.inc = 20 + g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
				params1.type = 1;
				break;
			}
			/* tcp frame */
			case 3: {
				optm1 = lookup_widget(GTK_WIDGET (button), "optionmenu18");
				menu = GTK_OPTION_MENU(optm1)->menu;
				menu_item = gtk_menu_get_active (GTK_MENU (menu));
				params1.inc = 30 + g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
				params1.type = 3;
				break;
			}
			/* udp frame */
			case 4: {
				optm1 = lookup_widget(GTK_WIDGET (button), "optionmenu19");
				menu = GTK_OPTION_MENU(optm1)->menu;
				menu_item = gtk_menu_get_active (GTK_MENU (menu));
				params1.inc = 40 + g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
				params1.type = 4;
				break;
			}
		}

		/* changing mac address */
		if ((params1.inc%10) == 1) {
			if (number < 14) {		
				error("Error: Packets is not long enough to change MAC address");
				return -1;	
			}
			params1.inc = 1;
		}
		/* changing ip source address */
		else if ((params1.inc == 12) || (params1.inc == 32) || (params1.inc == 42)) {
			if (number < ip_start + 1 + 4) {
				error("Error: Packet is not long enough to change IP values");
				return -1;
			}
			params1.inc = 2;
		}
		/* changing ip and mac source address */
		else if ((params1.inc == 13) || (params1.inc == 33) || (params1.inc == 43)) {
			if (number < ip_start + 1 + 4) {
				error("Error: Packet is not long enough to change MAC & IP values");
				return -1;
			}
			params1.inc = 3;
		}
		/* arp values */
		else if (params1.inc == 22) {
			if (number < eth_start + 1 + 6 + 4) {
				error("Error: Packet is not long enough to change ARP values");
				return -1;
			}
			params1.inc = 4;
		}
		/* ip source and tcp port values */
		else if (params1.inc == 34) {
			if (number < tcp_start + 2) {		
				error("Error: Packet isn't long enough to change TCP port");
				return -1;	
			}
			params1.inc = 5;
		}
		/* increase udp payload by one */
		else if (params1.inc == 44) {
			if (number < udp_start + 1) {		
				error("Error: Packet is not long enough to increase UDP payload");
				return -1;	
			}
			params1.inc = 6;
		}
		/* rtp values */
		else if ( (params1.inc >= 45) && (params1.inc <=47) ) {
			if (number < udp_start + 1 + 12) {
				error("Error: Packet is not long enough to increase RTP values");
				return -1;
			}
			params1.inc = params1.inc - 38;
		}
		/* changing byte x */
		/* YYY this part of code is a mess (it's friday 15.30 now))*/
		else if ((params1.inc == 2) || (params1.inc == 14) || (params1.inc == 23) ||
				(params1.inc == 35) || (params1.inc == 48)) {

			/* offset x field, is it ok */
 			en5 = lookup_widget(GTK_WIDGET (button), "entry160");
			en5_t = (char *)gtk_entry_get_text(GTK_ENTRY(en5));
			if ( (strtol(en5_t, (char **)NULL, 10) == 0) || 
					(number < strtol(en5_t, (char **)NULL, 10)) ) {
				error("Error: Wrong byte x offset!");
				return -1;	
			}
			length = strlen(en5_t);
			for(m=0; m<length; m++) {
		                if (isdigit(*(en5_t+m)) == 0) {
        	                error("Error: Wrong byte x entry!");
        	                return -1;
        	        	}
       			}
			params1.xbyte = strtol(en5_t, (char **)NULL, 10);

			/* option menu button for x byte */
 			xoptm = lookup_widget(GTK_WIDGET (button), "optionmenu14");
			xmenu = GTK_OPTION_MENU(xoptm)->menu;
			xmenu_item = gtk_menu_get_active (GTK_MENU (xmenu));
			params1.xchange = g_list_index (GTK_MENU_SHELL (xmenu)->children, xmenu_item);
			memcpy(params1.xstart, &packet[params1.xbyte-1], 4);

			if ((params1.xchange==1) || (params1.xchange==2)) {
				/* range button for x byte */
				en5 = lookup_widget(GTK_WIDGET (button), "entry161");
				en5_t = (char *)gtk_entry_get_text(GTK_ENTRY(en5));
				if ( (strtol(en5_t, (char **)NULL, 10) == 0) ) { 
						// || (4294967294 < strtol(en5_t, (char **)NULL, 10)) ) {
					error("Error: Wrong byte x range!");
					return -1;	
				}
				length = strlen(en5_t);
				for(m=0; m<length; m++) {
					if (isdigit(*(en5_t+m)) == 0) {
					error("Error: Wrong byte x range entry!");
					return -1;
					}
				}
				params1.xrange = strtol(en5_t, (char **)NULL, 10);
			}

			params1.inc = 10;
		}
		/* changing byte x and y */
		else if ((params1.inc == 3) || (params1.inc == 15) || (params1.inc == 24) ||
				(params1.inc == 36) || (params1.inc == 49)) {
 			en5 = lookup_widget(GTK_WIDGET (button), "entry160");
 			en6 = lookup_widget(GTK_WIDGET (button), "entry162");
			en5_t = (char *)gtk_entry_get_text(GTK_ENTRY(en5));
			en6_t = (char *)gtk_entry_get_text(GTK_ENTRY(en6));
			if ( (number < strtol(en5_t, (char **)NULL, 10)) || 
						(number < strtol(en6_t, (char **)NULL, 10)) ) {
				error("Error: Packets is not long enough to change specified byte");
				return -1;	
			}
			length = strlen(en5_t);
			for(m=0; m<length; m++) {
		                if (isdigit(*(en5_t+m)) == 0) {
        	                error("Error: Wrong byte x entry!");
        	                return -1;
        	        	}
       			}
			length = strlen(en6_t);
			for(m=0; m<length; m++) {
		                if (isdigit(*(en6_t+m)) == 0) {
        	                error("Error: Wrong byte y entry!");
        	                return -1;
        	        	}
       			}
			params1.xbyte = strtol(en5_t, (char **)NULL, 10);
 			xoptm = lookup_widget(GTK_WIDGET (button), "optionmenu14");
			xmenu = GTK_OPTION_MENU(xoptm)->menu;
			xmenu_item = gtk_menu_get_active (GTK_MENU (xmenu));
			memcpy(params1.xstart, &packet[params1.xbyte-1], 4);
			params1.xchange = g_list_index (GTK_MENU_SHELL (xmenu)->children, xmenu_item);
			params1.ybyte = strtol(en6_t, (char **)NULL, 10);
 			yoptm = lookup_widget(GTK_WIDGET (button), "optionmenu15");
			ymenu = GTK_OPTION_MENU(yoptm)->menu;
			ymenu_item = gtk_menu_get_active (GTK_MENU (ymenu));
			memcpy(params1.ystart, &packet[params1.ybyte-1], 4);
			params1.ychange = g_list_index (GTK_MENU_SHELL (ymenu)->children, ymenu_item);

			if ((params1.xchange==1) || (params1.xchange==2)) {
				en5 = lookup_widget(GTK_WIDGET (button), "entry161");
				en5_t = (char *)gtk_entry_get_text(GTK_ENTRY(en5));
				if ( (strtol(en5_t, (char **)NULL, 10) == 0) ) { 
						// || (4294967295 < strtol(en5_t, (char **)NULL, 10)) ) {
					error("Error: Wrong byte x range!");
					return -1;	
				}
				length = strlen(en5_t);
				for(m=0; m<length; m++) {
					if (isdigit(*(en5_t+m)) == 0) {
					error("Error: Wrong byte x range entry!");
					return -1;
					}
				}
				params1.xrange = strtol(en5_t, (char **)NULL, 10);
			}

			if ((params1.ychange==1) || (params1.ychange==2)) {
				en5 = lookup_widget(GTK_WIDGET (button), "entry163");
				en5_t = (char *)gtk_entry_get_text(GTK_ENTRY(en5));
				if ( (strtol(en5_t, (char **)NULL, 10) == 0) ) {
						// || (4294967295 < strtol(en5_t, (char **)NULL, 10)) ) {
					error("Error: Wrong byte y range!");
					return -1;	
				}
				length = strlen(en5_t);
				for(m=0; m<length; m++) {
					if (isdigit(*(en5_t+m)) == 0) {
					error("Error: Wrong byte y range entry!");
					return -1;
					}
				}
				params1.yrange = strtol(en5_t, (char **)NULL, 10);
			}

			params1.inc = 11;
		}

		/* if the togglebutton is active, just keep on sending till stop is pressed */
		if (GTK_TOGGLE_BUTTON(ckbt1)->active) {
			params1.count = -3;
		}
		else {
			/* there can be rubbish in this field */
			if (check_digit(en1_t, strlen(en1_t), "Error: Number of packets to send field") == -1)
        	                        return -1;

			params1.count = strtol(en1_t, (char **)NULL, 10);
			/* we allow to send 9999999 max */
			if ( (params1.count > 9999999) || (params1.count < 1) ) {
				//printf("Error: Packets send number value\n");
				error("Error: Packets send number value (1-9999999)");
				return -1;
			}
		}

		if (GTK_TOGGLE_BUTTON(ckbt2)->active) {
			params1.del = 1;
		}
		else {
			/* there can be rubbish in this field */
			if (check_digit(en2_t, strlen(en2_t), "Error: Delay between packets field") == -1)
					return -1;

			params1.del = strtol(en2_t, (char **)NULL, 10);
			/* max delay 999,999999 s */
			if ( (params1.del > 999999999) || (params1.del < 1) ) {
				//printf("Error: Delay between packets value\n");
				error("Error: Delay between packets value (1-999999999)");
				return -1;
			}
		}

		/* YYY if the built packet is shorter then 60 bytes, we add padding zero bytes 
		 * to fill up the length till 60 (min ethrenet frame length). This bytes will be 
		 * added anyway by the device driver, so we do this just to prevent misunderstanding: 
		 * till now if your packet  was 20 bytes long, then it was also said -> 20 bytes 
		 * sent on eth0... but actually 60 bytes (+CRC) were sent */
		if (number < 60) {
			memset(&packet[number], 0x00, ( 60 - number ) );
			/* there were problems with sendbuilt function, if the packet was shorter then
			 * 60 bytes. Checksum was wrong calculated */
			/* number = 60; */
		}

		params1.button = statusbar;			
		params1.context_id = context_id;
		params1.button1 =  button1;
		params1.button2 =  button2;
		params1.button3 =  button3;
		params1.button4 =  button4;
		params1.button5 =  button5;
		params1.button6 =  button6;
		params1.stopbt = stopbt;
		params1.udpstart = udp_start;
		params1.tcpstart = tcp_start;
		params1.ipstart = ip_start;
		params1.ethstart = eth_start;

		if (GTK_TOGGLE_BUTTON(reltime)->active) 
			params1.timeflag = 1;
		else
			params1.timeflag = 0;

		//gtk_widget_set_sensitive (button1, FALSE);
		//gtk_widget_set_sensitive (button2, FALSE);
		//gtk_widget_set_sensitive (button3, FALSE);
		//gtk_widget_set_sensitive (button4, FALSE);
		//gtk_widget_set_sensitive (button5, FALSE);
		//gtk_widget_set_sensitive (stopbt, TRUE);

		snprintf(buff, 100, "  Starting generator...");
		gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(context_id), buff);

		pthread_create(&thread_id, NULL, &sendbuilt, &params1);
		return 1;
	}

	/* is it the generator that sends different sequences? */
	else if (page == 2) { 
		char buff4[101];
		FILE *file_p;
		int j = 0, sum = 0, sum1 = 0;
		
		abstime = lookup_widget(GTK_WIDGET (button), "radiobutton36");
		reltime = lookup_widget(GTK_WIDGET (button), "radiobutton37");
		toolbar = lookup_widget(GTK_WIDGET (button), "toolbar1");
		stopbt = lookup_widget(GTK_WIDGET (button), "Stop_button");
		optm1 = lookup_widget(GTK_WIDGET (button), "checkbutton36");
		optm2 = lookup_widget(GTK_WIDGET (button), "entry151");
		optm3 = lookup_widget(GTK_WIDGET (button), "entry152");
		button1 = lookup_widget(GTK_WIDGET (button), "Build_button");
		button2 = lookup_widget(GTK_WIDGET (button), "Gen_button");
		button3 = lookup_widget(GTK_WIDGET (button), "Genp");
		button4 = lookup_widget(GTK_WIDGET (button), "Interface_button");
		button5 = lookup_widget(GTK_WIDGET (button), "Send_button");
		button6 = lookup_widget(GTK_WIDGET (button), "Gensbt");

		en1_t = (char *)gtk_entry_get_text(GTK_ENTRY(optm2));
		en2_t = (char *)gtk_entry_get_text(GTK_ENTRY(optm3));

		if (GTK_TOGGLE_BUTTON(reltime)->active) 
			params1.timeflag = 1;
		else
			params1.timeflag = 0;

		/* if the togglebutton is active, just keep on sending till stop is pressed */
		if (GTK_TOGGLE_BUTTON(optm1)->active) {
			params1.count = -3;
		}
		else {
			/* there can be rubbish in this field */
			if (check_digit(en1_t, strlen(en1_t), 
							"Error: Number of sequences to send field") == -1)
        	                        return -1;

			params1.count = strtol(en1_t, (char **)NULL, 10);
			/* we allow to send 9999999 max */
			if ( (params1.count > 9999999) || (params1.count < 1) ) {
				//printf("Error: Number of sequences to send field\n");
				error("Error: Number of sequences to send field (1 - 9999999)");
				return -1;
			}
		}

		/* if the above number is 1, we don't need to check for this delay */
		if (params1.count == 1) 
			params1.del = 0;
		else {
			/* there can be rubbish in this field */
			if (check_digit(en2_t, strlen(en2_t), "Error: Delay between sequences field") == -1)
					return -1;

			params1.del = strtol(en2_t, (char **)NULL, 10);
			/* max delay 999,999999 s */
			if ( (params1.del > 999999999) || (params1.del < 0) ) {
				//printf("Error: Delay between sequences field\n");
				error("Error: Delay between sequences field (0 - 999999999)");
				return -1;
			}
		}

		/* we fill in a table with the parameters */
		for (i=0; i<10; i++) {

			/* name of the packet and packet contents */
			snprintf(buff4, 100, "entry%d", 111+i);
			en1 = lookup_widget(GTK_WIDGET (button), buff4);
			en1_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));

			/* enable or disable */
			snprintf(buff4, 100, "checkbutton%d", 25+i);
			ckbt1 = lookup_widget(GTK_WIDGET(button), buff4);
			if (GTK_TOGGLE_BUTTON(ckbt1)->active) {
				params1.partable[i][5] = 0;
				continue;
			}
			else 
				params1.partable[i][5] = 1;

			/* if there is no name, skip it */
			if ( strlen(en1_t) == 0 )  {
				params1.partable[i][0] = 0;
				continue;
			}
			else
				params1.partable[i][0] = 1;

			/* open file for reading */
			if ( (file_p = fopen(en1_t, "r")) == NULL) {
                                snprintf(buff4, 100, "Error: Can not open file for reading:%s", en1_t);
				//printf("Error: Can not open file for reading %s\n", en1_t);
                                error(buff4);
				return -1;
			}

			/* we have to read the packet contents stored in a file */
			{
			struct pcap_hdr fh;
			struct pcaprec_hdr ph;
        		char pkt_temp[100];
			int freads;

		        /* first we read the pcap file header */
        		freads = fread(pkt_temp, sizeof(fh), 1, file_p);
        		/* if EOF, exit */
        		if (freads == 0)
                		return 1;

       			 memcpy(&fh, pkt_temp, 24);

       			 /* if magic number in NOK, exit */
       			 if (fh.magic != PCAP_MAGIC)
        		        return -1;

                	/* next the  pcap packet header */
                	freads = fread(pkt_temp, sizeof(ph), 1, file_p);

                	/* if EOF, exit */
                	if (freads == 0)
                	        return 1;

                	/* copy the 16 bytes into ph structure */
                	memcpy(&ph, pkt_temp, 16);

                	/* and the packet itself, but only up to the capture length */
                	freads = fread(&params1.pkttable[i][0], ph.incl_len, 1, file_p);

                	/* if EOF, exit */
                	if (freads == 0)
                	        return 1;

			fclose(file_p);
			params1.partable[i][1] = ph.incl_len;
			}

			/* number of packets to send */
			snprintf(buff4, 100, "entry%d", 121+i);
			en2 = lookup_widget(GTK_WIDGET (button), buff4);
			en2_t = (char *)gtk_entry_get_text(GTK_ENTRY(en2));
                        snprintf(buff4, 100, "Error: Number of packets field in row %d", i+1);
                        if (check_digit(en2_t,strlen(en2_t), buff4) == -1)
                                        return -1;

                        params1.partable[i][2] = strtol(en2_t, (char **)NULL, 10);
                        /* we allow to send 9999999 max */
                        if ( (params1.partable[i][2] > 9999999) || (params1.partable[i][2] < 0) ) {
                                snprintf(buff4, 100, "Error: number of packets value in row %d", i+1);
                                //printf("Error: number of packets value in row %d\n", i+1);
                                error(buff4);
                        	return -1;
                        }

			/* delay between packets */
			snprintf(buff4, 100, "entry%d", 131+i);
			en3 = lookup_widget(GTK_WIDGET (button), buff4);
			en3_t = (char *)gtk_entry_get_text(GTK_ENTRY(en3));
                        snprintf(buff4, 100, "Error: Delay between packets field in row %d", i+1);
                        if (check_digit(en3_t,strlen(en3_t), buff4) == -1)
                                        return -1;

			params1.partable[i][3] = strtol(en3_t, (char **)NULL, 10);
			/* max delay 999,999999 s */
			if ( (params1.partable[i][3] > 999999999) || (params1.partable[i][3] < 0) ) {
                                snprintf(buff4, 100, "Error: delay between value in row %d", i+1);
                                //printf("Error: delay between value in row %d\n", i+1);
                                error(buff4);
				return -1;
			}

			/* delay to next sequence */
			snprintf(buff4, 100, "entry%d", 141+i);
			en4 = lookup_widget(GTK_WIDGET (button), buff4);
			en4_t = (char *)gtk_entry_get_text(GTK_ENTRY(en4));
                        snprintf(buff4, 100, "Error: Delay to next value in row %d", i+1);
                        if (check_digit(en4_t,strlen(en4_t), buff4) == -1)
                                        return -1;

			params1.partable[i][4] = strtol(en4_t, (char **)NULL, 10);
			/* max delay 999,999999 s */
			if ( (params1.partable[i][4] > 999999999) || (params1.partable[i][4] < 0) ) {
                                snprintf(buff4, 100, "Error: delay to next value in row %d", i+1);
                                //printf("Error: delay to next value in row %d\n", i+1);
                                error(buff4);
				return -1;
			}
			
		}	

		/* if all the fields are empty or disabled we return immediattely */
		for (j=0; j<10; j++) {
			sum = sum + params1.partable[j][0];
			sum1 = sum1 + params1.partable[j][5];
		}

		if ( (sum ==0 ) || (sum1 == 0) ) {
			snprintf(buff, 100, "  Nothing to send...");
			gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(context_id), buff);
			return 1;
		}

		params1.button = statusbar;			
		params1.context_id = context_id;
		params1.toolbar = toolbar;
		params1.stopbt = stopbt;
		params1.button1 =  button1;
		params1.button2 =  button2;
		params1.button3 =  button3;
		params1.button4 =  button4;
		params1.button5 =  button5;
		params1.button6 =  button6;
	
		snprintf(buff, 100, "  Starting sequence generator...");
		gtk_statusbar_push(GTK_STATUSBAR(statusbar), GPOINTER_TO_INT(context_id), buff);

		pthread_create(&thread_id, NULL, &sendsequence, &params1);
		return 1;
	}

	/* is it the generator that uses the kernel module? */
	else if (page == 3) { 
		;
	}

	return 1;
}