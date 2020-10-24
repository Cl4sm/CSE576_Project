int ipv6_get(GtkButton *button, gpointer user_data) {
	GtkWidget *version, *tos, *flowlabel, *payloadlength, *nextheader, *hoplimit;
	GtkWidget *src6ip, *dst6ip, *src6bt, *dst6bt, *payloadlength_bt;
	GtkWidget *extensionhdr, *exthdrbto;
	GtkWidget *udp_bt, *tcp_bt, *icmp6_bt, *usedef_bt;

	gchar *version_t, *tos_t, *flowlabel_t, *plength_t, *next_t, *hop_t;
	gchar *src_t, *dst_t, *ext_t;
	guint32 pseudo_header_sum;
	int length_start, length_start_field, x_length, dst_length, i;

	gchar tmp[4];
	gchar tmp2[6];
	gchar src_tmp[40];
	gchar dst_tmp[40];

	version = lookup_widget(GTK_WIDGET(button), "entry195");
	tos = lookup_widget(GTK_WIDGET(button), "entry196");
	flowlabel = lookup_widget(GTK_WIDGET(button), "entry197");
	payloadlength = lookup_widget(GTK_WIDGET(button), "entry198");
	payloadlength_bt = lookup_widget(GTK_WIDGET(button), "checkbutton43");
	nextheader = lookup_widget(GTK_WIDGET(button), "entry199");
	hoplimit = lookup_widget(GTK_WIDGET(button), "entry200");
	src6ip = lookup_widget(GTK_WIDGET(button), "entry201");
	dst6ip = lookup_widget(GTK_WIDGET(button), "entry202");
	src6bt = lookup_widget(GTK_WIDGET(button), "button88");
	dst6bt = lookup_widget(GTK_WIDGET(button), "button89");
	extensionhdr = lookup_widget(GTK_WIDGET(button), "entry203");
	exthdrbto = lookup_widget(GTK_WIDGET(button), "button91");
	udp_bt = lookup_widget(GTK_WIDGET(button), "radiobutton67");
	tcp_bt = lookup_widget(GTK_WIDGET(button), "radiobutton68");
	icmp6_bt = lookup_widget(GTK_WIDGET(button), "radiobutton69");
	usedef_bt = lookup_widget(GTK_WIDGET(button), "radiobutton71");

	version_t = (char *)gtk_entry_get_text(GTK_ENTRY(version));
	tos_t = (char *)gtk_entry_get_text(GTK_ENTRY(tos));
	flowlabel_t = (char *)gtk_entry_get_text(GTK_ENTRY(flowlabel));
	plength_t = (char *)gtk_entry_get_text(GTK_ENTRY(payloadlength));
	next_t = (char *)gtk_entry_get_text(GTK_ENTRY(nextheader));
	hop_t = (char *)gtk_entry_get_text(GTK_ENTRY(hoplimit));
	src_t = (char *)gtk_entry_get_text(GTK_ENTRY(src6ip));
	dst_t = (char *)gtk_entry_get_text(GTK_ENTRY(dst6ip));
	ext_t = (char *)gtk_entry_get_text(GTK_ENTRY(extensionhdr));

	ipv6_used = 1;

	/*start parsing the ipv6 header */
	strncpy(&tmp[0], version_t, 1);
	strncpy(&tmp[1], tos_t, 1);
	strncpy(&tmp2[0], (tos_t+1), 1);
	strncpy(&tmp2[1], flowlabel_t, 5);
	
	if (char2x(tmp) == -1) {
		error("Error: ipv6 version or tos field");
                return -1;
	}

	packet[number] = (unsigned char)char2x(tmp);
	number++;
	
	if (char2x(tmp2) == -1) {
		error("Error: ipv6 tos field or flow label");
                return -1;
	}

	packet[number] = (unsigned char)char2x(tmp2);
	number++;

	if (char2x(tmp2+2) == -1) {
		error("Error: flow label");
                return -1;
	}

	packet[number] = (unsigned char)char2x(tmp2+2);
	number++;

	if (char2x(tmp2+4) == -1) {
		error("Error: ipv6 tos field or flow label");
                return -1;
	}

	packet[number] = (unsigned char)char2x(tmp2+4);
	number++;

	/* total length */
	if (GTK_TOGGLE_BUTTON(payloadlength_bt)->active) {
		length_start_field = number;
		number++;
		number++;
	}
	else {
		length_start = 0; /* if length start is 0, then we leave it in the end */
		if ( (atol(plength_t) < 0) || (atol(plength_t) > 65535) ) {
			error("Error: ipv6 total length range");
                	return -1;
		}

		/* there can be rubbish in this field */
		if (check_digit(plength_t, strlen(plength_t), 
					"Error: ipv6 total length field values") == -1)
                                return -1;

		packet[number] = (char)(atol(plength_t)/256);
		number++;	
		packet[number] = (char)(atol(plength_t)%256);
		number++;	
	}

	if (char2x(next_t) == -1) {
		error("Error: ipv6 next header field");
                return -1;
	}

	packet[number] = (unsigned char)char2x(next_t);
	number++;

	/* hop limit */
	if ( (atoi(hop_t) < 0) || (atoi(hop_t) > 255) ) {
		error("Error: ipv6 hop limit range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(hop_t, strlen(hop_t), "Error: ipv6 hop limit field values") == -1)
                                return -1;

	packet[number] = (char)(atoi(hop_t));
	number++;	


	// now the addresses: at the moment, we allow only full uncompressed format of ipv6 address
	x_length = strlen(src_t);
	if (x_length != 39) {
		error("Error: ipv6 source address.\nOnly full uncompressed format allowed!");
		return -1;
	}

	for (i=0; i<8; i++) {
		if (char2x(src_t) == -1) {
			error("Error: ipv6 source address.\nOnly full uncompressed format allowed!");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(src_t);
		number++;
		src_t++; src_t++;

		if (char2x(src_t) == -1) {
			error("Error: ipv6 source address.\nOnly full uncompressed format allowed!");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(src_t);
		number++;
		src_t++; src_t++;

		if (i==7)
			break;

		if (strncmp(src_t, ":", 1) != 0) { 
			error("Error: ipv6 source address.\nOnly full uncompressed format allowed!");
	                return -1;
		}
		src_t++;
	}
		
		
	x_length = strlen(dst_t);
	if (x_length != 39) {
		error("Error: ipv6 source address.\nOnly full uncompressed format allowed!");
		return -1;
	}

	for (i=0; i<8; i++) {
		if (char2x(dst_t) == -1) {
			error("Error: ipv6 destination address.\nOnly full uncompressed format allowed!");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(dst_t);
		number++;
		dst_t++; dst_t++;

		if (char2x(dst_t) == -1) {
			error("Error: ipv6 destination address.\nOnly full uncompressed format allowed!");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(dst_t);
		number++;
		dst_t++; dst_t++;

		if (i==7)
			break;

		if (strncmp(dst_t, ":", 1) != 0) { 
			error("Error: ipv6 destination address.\nOnly full uncompressed format allowed!");
	                return -1;
		}
		dst_t++;
	}
	
	//pseudo header for udp and tcp checksum
	pseudo_header_sum = get_checksum32(number-32, number-1);


	//extension headers 
	x_length = strlen(ext_t);
	if ( (x_length !=0) && (x_length % 16 != 0) ) {
		error("Error: extension header must be n times 64 bytes!");
		return -1;
	}
	
	for (i=0; i< (x_length/2); i++) {
		if (char2x(ext_t) == -1) {
			error("Error: extension header!");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(ext_t);
		number++;
		ext_t++; ext_t++;
	}
		
	//if auto header length button is enabled, this is where the packet length count starts
	length_start = number;

	/* so we came to the end of ip header. what is next? */
	/* tcp, udp, icmp or manually attached payload? */
	if (GTK_TOGGLE_BUTTON(udp_bt)->active) {
		if (udp_get(button, user_data, pseudo_header_sum) == -1) {
			//printf("Error: Problem with UDP information\n");
			return -1;
		}
	}
	
	else if (GTK_TOGGLE_BUTTON(tcp_bt)->active) {
		if (tcp_get(button, user_data, pseudo_header_sum) == -1) {
			//printf("Error: Problem with TCP information\n");
			return -1;
		}
	}	
	
//	else if (GTK_TOGGLE_BUTTON(icmp6_bt)->active) {
//		if (icmp6_get(button, user_data) == -1) {
			//printf("Error: Problem with ICMP information\n");
//			return -1;
//		}
//	}	
	
//	else if (GTK_TOGGLE_BUTTON(usedef_bt)->active) {
			
//		pay_text_e = lookup_widget(GTK_WIDGET (button), "text2");
//		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pay_text_e));
//		pay_length = gtk_text_buffer_get_char_count(buffer);	
//		GtkTextIter start,end;
		//gtk_text_buffer_get_start_iter(buffer,start);
		//gtk_text_buffer_get_end_iter(buffer,end);
//		gtk_text_buffer_get_bounds(buffer,&start,&end);
//		pay_text = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
		//g_free(start);
		//g_free(end);
		//pay_text = (char *) malloc(pay_length + 1);
		//pay_text = gtk_editable_get_chars(GTK_EDITABLE(pay_text_e), 0, -1);

		/* YYY 1514-number is not ok in case we use 802.1q!!! */
//		pay_max = 1514 - number;

//		if (get_network_payload(button, user_data, pay_length, pay_max, pay_text) == -1) {
			//printf("Error: Problem with IPv4 payload\n");
//			g_free(pay_text);
//			return -1;
//		}
  //              else
//			g_free(pay_text);
//	}

	else {
		error("Error: IPv6 zoombie error!!!");
		return -1;
	}
		
	
	/* so we are back again to cumpute the length and checksum. so this is for length */
	if (length_start > 0) {
		packet[length_start_field] = (char)((number - length_start)/256);
		packet[length_start_field+1] = (char)((number - length_start)%256);
	}

	return 1;
}