int ipv4_get(GtkButton *button, gpointer user_data) {
	GtkWidget *version, *header_length, *tos, *total_length, *identification, *flags;
	GtkWidget *frag_offset, *ttl, *protocol, *header_cks, *header_cks_bt;
	GtkWidget *src_ip, *dst_ip, *options, *total_length_bt;
	GtkWidget *udp_bt, *tcp_bt, *icmp_bt, *igmp_bt, *usedef_bt, *pay_text_e;
	gchar *version_t, *header_length_t, *tos_t, *total_length_t, *identification_t, *flags_t;
	gchar *frag_offset_t, *ttl_t, *protocol_t, *header_cks_t;
	gchar *src_ip_t, *dst_ip_t, *options_t, *pay_text;
	int length_start, header_cks_start, cks_start, cks_stop;
	gchar tmp[4];
	int i, j, pay_length, pay_max;
	guint16 value, ipcksum;
	guint32 pseudo_header_sum;
	
	version = lookup_widget(GTK_WIDGET(button), "entry26");
	header_length = lookup_widget(GTK_WIDGET(button), "entry27");
	tos = lookup_widget(GTK_WIDGET(button), "entry28");
	total_length = lookup_widget(GTK_WIDGET(button), "entry29");
	total_length_bt = lookup_widget(GTK_WIDGET(button), "checkbutton21");
	identification = lookup_widget(GTK_WIDGET(button), "entry30");
	flags = lookup_widget(GTK_WIDGET(button), "entry31");
	frag_offset = lookup_widget(GTK_WIDGET(button), "entry32");
	ttl = lookup_widget(GTK_WIDGET(button), "entry44");
	protocol = lookup_widget(GTK_WIDGET(button), "entry34");
	header_cks = lookup_widget(GTK_WIDGET(button), "entry35");
	header_cks_bt = lookup_widget(GTK_WIDGET(button), "ip_header_cks_cbt");
	src_ip = lookup_widget(GTK_WIDGET(button), "entry38");
	dst_ip = lookup_widget(GTK_WIDGET(button), "entry37");
	options = lookup_widget(GTK_WIDGET(button), "entry39");
	udp_bt = lookup_widget(GTK_WIDGET(button), "udp_bt");
	tcp_bt = lookup_widget(GTK_WIDGET(button), "tcp_bt");
	icmp_bt = lookup_widget(GTK_WIDGET(button), "icmp_bt");
	igmp_bt = lookup_widget(GTK_WIDGET(button), "igmp_bt");
	usedef_bt = lookup_widget(GTK_WIDGET(button), "ip_user_data_bt");
	
	version_t = (char *)gtk_entry_get_text(GTK_ENTRY(version));
	header_length_t = (char *)gtk_entry_get_text(GTK_ENTRY(header_length));
	tos_t = (char *)gtk_entry_get_text(GTK_ENTRY(tos));
	total_length_t = (char *)gtk_entry_get_text(GTK_ENTRY(total_length));
	identification_t = (char *)gtk_entry_get_text(GTK_ENTRY(identification));
	flags_t = (char *)gtk_entry_get_text(GTK_ENTRY(flags));
	frag_offset_t = (char *)gtk_entry_get_text(GTK_ENTRY(frag_offset));
	ttl_t = (char *)gtk_entry_get_text(GTK_ENTRY(ttl));
	protocol_t = (char *)gtk_entry_get_text(GTK_ENTRY(protocol));
	header_cks_t = (char *)gtk_entry_get_text(GTK_ENTRY(header_cks));
	src_ip_t = (char *)gtk_entry_get_text(GTK_ENTRY(src_ip));
	dst_ip_t = (char *)gtk_entry_get_text(GTK_ENTRY(dst_ip));
	options_t = (char *)gtk_entry_get_text(GTK_ENTRY(options));

	ipv6_used = 0;
	
	/* we want to know where the ip header starts, to calculate the checksum later */
	cks_start = number;

	/* now we have all the widgets, so let start parsing them */
	/* starting with version */
	strncpy(&tmp[0], version_t, 1);
	strncpy(&tmp[1], header_length_t, 1);
	
	if (char2x(tmp) == -1) {
		//printf("Error: ipv4 version or header length field\n");
		error("Error: ipv4 version or header length field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(tmp);
	number++;
	
	/* tos field */
	if (char2x(tos_t) == -1) {
		//printf("Error: ipv4 tos field\n");
		error("Error: ipv4 tos field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(tos_t);
	number++;

	/* total length */
	/* if auto is on then we have to calculate this, but we can do this
	 * at the end, when we have the whole ip packet together. so if auto 
	 * is enabled we set the marking and recaltulate it in the end */
	if (GTK_TOGGLE_BUTTON(total_length_bt)->active) {
		length_start = number;
		number++;
		number++;
	}
	else {
		length_start = 0; /* if length start is 0, then we leave it in the end */
		if ( (atol(total_length_t) < 0) || (atol(total_length_t) > 65535) ) {
			//printf("Error: ipv4 total length range\n");
			error("Error: ipv4 total length range");
                	return -1;
		}

		/* there can be rubbish in this field */
		if (check_digit(total_length_t, strlen(total_length_t), 
					"Error: ipv4 total length field values") == -1)
                                return -1;

		packet[number] = (char)(atol(total_length_t)/256);
		number++;	
		packet[number] = (char)(atol(total_length_t)%256);
		number++;	
	}
	
	/* identification */
	if (char2x(identification_t) == -1) {
		//printf("Error: ipv4 identification field\n");
		error("Error: ipv4 identification field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(identification_t);
	number++;
	identification_t++; identification_t++;
	if (char2x(identification_t) == -1) {
		//printf("Error: ipv4 identification field\n");
		error("Error: ipv4 identification field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(identification_t);
	number++;

	/* flags and fragment offset */
	if ( (atoi(flags_t) > 7) || (atoi(flags_t) < 0) ) {
		//printf("Error: ipv4 flags field: the value can be beetwen 0 and 7\n");
		error("Error: ipv4 flags field: the value can be beetwen 0 and 7");
		return -1;
	}
		
	if ( (atoi(frag_offset_t) > 8191) || (atoi(frag_offset_t) < 0) ) {
		//printf("Error: ipv4 fragmentation offset field: (0 - 8191)\n");
		error("Error: ipv4 fragmentation offset field: (0 - 8191)");
		return -1;
	}

	/* there can be rubbish in this field */
	if (check_digit(flags_t, strlen(flags_t), "Error: ipv4 flags values") == -1)
                                return -1;

	/* there can be rubbish in this field */
        if (check_digit(frag_offset_t, strlen(frag_offset_t), 
					"Error: ipv4 fragmentation offset field values ") == -1)
                                return -1;

	/* this is the correct int value now 
	 * we need to store it as 2 byte hex value */
	value = (atoi(flags_t)<<13   & 0xE000) | 
		(atoi(frag_offset_t) & 0x1FFF) ;
	
	/* YYY what about big endian computers - hope it works */
	value = htons(value);
	memcpy(&packet[number], &value, 2);
	number++;
	number++;
		
	/* ttl value */
	if ( (atoi(ttl_t) < 0) || (atoi(ttl_t) > 255) ) {
		//printf("Error: ipv4 ttl range\n");
		error("Error: ipv4 ttl range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(ttl_t, strlen(ttl_t), "Error: ipv4 ttl field values") == -1)
                                return -1;

	packet[number] = (char)(atoi(ttl_t));
	number++;	

	/* protocol field */
	if ( (atoi(protocol_t) < 0) || (atoi(protocol_t) > 255) ) {
		//printf("Error: ipv4 protocol range\n");
		error("Error: ipv4 protocol range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(protocol_t, strlen(protocol_t), "Error: ipv4 protocol field values") == -1)
                                return -1;

	packet[number] = (char)(atoi(protocol_t));
	number++;	

	pseudo_header_sum = (guint32)(packet[number-1]);
	
	/* header checksum */
	/* if auto is on then we have to calculate this, but we can do this
	 * at the end and recaltulate it for now we store the current number into
	 * another variable. we will calculate length in the end */
	if (GTK_TOGGLE_BUTTON(header_cks_bt)->active) {
		header_cks_start = number;
		packet[number] = (unsigned char)0;
		number++;
		packet[number] = (unsigned char)0;
		number++;
	}
	else {
		/* if header_cks_start = 0, we leave it in the end */
		header_cks_start = 0;
		if (char2x(header_cks_t) == -1) {
			//printf("Error: ipv4 header checksum field\n");
			error("Error: ipv4 header checksum field");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(header_cks_t);
		header_cks_t++; header_cks_t++; number++;
		if (char2x(header_cks_t) == -1) {
			//printf("Error: ipv4 header checksum field\n");
			error("Error: ipv4 header checksum field");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(header_cks_t);
		number++;
	}
	
	/* source ip address */
	ip_start = number;

	if (check_ip_address(src_ip_t) == -1) {
		//printf("Error: Wrong source ipv4 address\n");
		error("Error: Wrong source ipv4 address");
		return -1;
	}
	
	for (i=0; i<4; i++) {
		for(j=0; j<4 && (*src_ip_t != '\0'); j++) {
			if ( ((int)*src_ip_t == '.') && (i<3) && (j>0) ) {
				src_ip_t++;
				break;
			}
			tmp[j] = *src_ip_t;
			src_ip_t++;
		}
		tmp[j] = '\0';
		packet[number] = (unsigned char)(atoi(tmp));
		number++;		
	}

	/* destination ip address */
	if (check_ip_address(dst_ip_t) == -1) {
		//printf("Error: Wrong destination ipv4 address\n");
		error("Error: Wrong destination ipv4 address");
		return -1;
	}
	
	for (i=0; i<4; i++) {
		for(j=0; j<4 && (*dst_ip_t != '\0'); j++) {
			if ( ((int)*dst_ip_t == '.') && (i<3) && (j>0) ) {
				dst_ip_t++;
				break;
			}
			tmp[j] = *dst_ip_t;
			dst_ip_t++;
		}
		tmp[j] = '\0';
		packet[number] = (unsigned char)(atoi(tmp));
		number++;		
	}

	/* this is checksum for protocol field plus IP source and destination 
	 * we need this later when we want to calculate the TCP or UDP checksum
	 * there we need this values so we pass them when calling the routine*/
	pseudo_header_sum = pseudo_header_sum + get_checksum32(number-8, number-1);
	
	/* options? do allow then and how long can they be??? 
	 * ok we allow them and limit them to 40 bytes and the user should 
	 * care that the options length is always a multiple of 32 bits (4 bytes) */
	if ( (strlen(options_t)%8) != 0) {
		//printf("Error: Wrong ipv4 length of options field (length mod 8 must be 0)\n");
		error("Error: Wrong ipv4 length of options field      \n(length mod 8 must be 0)");
		return -1;
	}
	
	if ( strlen(options_t) > 80) {
		//printf("Error: ipv4 options field to long\n");
		error("Error: ipv4 options field to long");
		return -1;
	}
	
	j = strlen(options_t)/2;
	for (i=0; i<j; i++) {
		if (char2x(options_t) == -1) {
			//printf("Error: ipv4 options field\n");
			error("Error: ipv4 options field");
       		        return -1;
		}
		packet[number] = (unsigned char)char2x(options_t);
		number++; options_t++; options_t++;
	}

	cks_stop = number;

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
	
	else if (GTK_TOGGLE_BUTTON(icmp_bt)->active) {
		if (icmp_get(button, user_data) == -1) {
			//printf("Error: Problem with ICMP information\n");
			return -1;
		}
	}	
	
	else if (GTK_TOGGLE_BUTTON(igmp_bt)->active) {
		if (igmp_get(button, user_data) == -1) {
			//printf("Error: Problem with IGMP information\n");
			return -1;
		}
	}	

	else if (GTK_TOGGLE_BUTTON(usedef_bt)->active) {
			
		pay_text_e = lookup_widget(GTK_WIDGET (button), "text2");
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pay_text_e));
		pay_length = gtk_text_buffer_get_char_count(buffer);	
		GtkTextIter start,end;
		//gtk_text_buffer_get_start_iter(buffer,start);
		//gtk_text_buffer_get_end_iter(buffer,end);
		gtk_text_buffer_get_bounds(buffer,&start,&end);
		pay_text = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
		//g_free(start);
		//g_free(end);
		//pay_text = (char *) malloc(pay_length + 1);
		//pay_text = gtk_editable_get_chars(GTK_EDITABLE(pay_text_e), 0, -1);

		/* YYY 1514-number is not ok in case we use 802.1q!!! */
		pay_max = 1514 - number;

		if (get_network_payload(button, user_data, pay_length, pay_max, pay_text) == -1) {
			//printf("Error: Problem with IPv4 payload\n");
			g_free(pay_text);
			return -1;
		}
                else
			g_free(pay_text);
	}

	else {
		//printf("Error: IPv4 zoombie error!!!\n");
		error("Error: IPv4 zoombie error!!!");
		return -1;
	}
		
	
	/* so we are back again to cumpute the length and checksum. so this is for length */
	if (length_start > 0) {
		packet[length_start] = (char)((number - length_start + 2)/256);
		packet[length_start+1] = (char)((number - length_start + 2)%256);
	}

	/* and this for checksum */
	if (header_cks_start > 0) {
		ipcksum = ((-1) - get_checksum16(cks_start, cks_stop) % 0x10000);
		packet[header_cks_start] = (char)(ipcksum/256);
		packet[header_cks_start+1] =  (char)(ipcksum%256);
	}
	
	return 1;
}