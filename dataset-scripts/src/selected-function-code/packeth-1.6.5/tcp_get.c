int tcp_get(GtkButton *button, gpointer user_data, guint32 pseudo_header_sum) {
	
	GtkWidget *srcport, *dstport, *sequence_number, *ack_number, *header_length;
	GtkWidget *flag_cwr, *flag_ecn;
	GtkWidget *flag_urg, *flag_ack, *flag_psh, *flag_rst, *flag_syn, *flag_fin;
	GtkWidget *window_size, *checksum, *checksum_bt, *urgent_pointer, *options;
	GtkWidget *payload_bt, *payload;

	gchar *srcport_t, *dstport_t, *sequence_number_t, *ack_number_t, *header_length_t;
	gchar *window_size_t, *checksum_t, *urgent_pointer_t, *options_t, *payload_t;

	int checksum_start, cks_start, cks_stop, i, j, payload_length, odd=0;
	guint32 tcpcksum;
	guint32 seqnr, acknr;
	int flag_value = 0;

	srcport = lookup_widget(GTK_WIDGET(button), "entry46");
	dstport = lookup_widget(GTK_WIDGET(button), "entry47");
	sequence_number = lookup_widget(GTK_WIDGET(button), "entry48");
	ack_number = lookup_widget(GTK_WIDGET(button), "entry49");
	header_length = lookup_widget(GTK_WIDGET(button), "entry50");
	flag_cwr = lookup_widget(GTK_WIDGET(button), "checkbutton22");
	flag_ecn = lookup_widget(GTK_WIDGET(button), "checkbutton23");
	flag_urg = lookup_widget(GTK_WIDGET(button), "checkbutton7");
	flag_ack = lookup_widget(GTK_WIDGET(button), "checkbutton8");
	flag_psh = lookup_widget(GTK_WIDGET(button), "checkbutton9");
	flag_rst = lookup_widget(GTK_WIDGET(button), "checkbutton10");
	flag_syn = lookup_widget(GTK_WIDGET(button), "checkbutton11");
	flag_fin = lookup_widget(GTK_WIDGET(button), "checkbutton12");
	window_size = lookup_widget(GTK_WIDGET(button), "entry51");
	checksum = lookup_widget(GTK_WIDGET(button), "entry52");
	checksum_bt = lookup_widget(GTK_WIDGET(button), "checkbutton13");
	urgent_pointer = lookup_widget(GTK_WIDGET(button), "entry53");
	options = lookup_widget(GTK_WIDGET(button), "entry54");
	payload_bt = lookup_widget(GTK_WIDGET(button), "checkbutton14");
	payload = lookup_widget(GTK_WIDGET(button), "text4");
	
	srcport_t= (char *)gtk_entry_get_text(GTK_ENTRY(srcport));
	dstport_t= (char *)gtk_entry_get_text(GTK_ENTRY(dstport));
	sequence_number_t = (char *)gtk_entry_get_text(GTK_ENTRY(sequence_number));
	ack_number_t = (char *)gtk_entry_get_text(GTK_ENTRY(ack_number));
	header_length_t = (char *)gtk_entry_get_text(GTK_ENTRY(header_length));
	window_size_t = (char *)gtk_entry_get_text(GTK_ENTRY(window_size));
	checksum_t = (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
	urgent_pointer_t = (char *)gtk_entry_get_text(GTK_ENTRY(urgent_pointer));
	options_t = (char *)gtk_entry_get_text(GTK_ENTRY(options));
	
	cks_start = number;
	
	/* we need this one for knowing where the tcp part starts
	 * we need this one when sending the packets out and modifing some values */
	tcp_start = number;	
		
	/* source port */
	if ( (atoi(srcport_t) < 0) || (atoi(srcport_t) > 65535) ) {
		//printf("Error: tcp source port range\n");
		error("Error: tcp source port range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(srcport_t, strlen(srcport_t), "Error: tcp srcport field values") == -1)
                                return -1;

	packet[number] = (char)(atol(srcport_t)/256);
        number++;
        packet[number] = (char)(atol(srcport_t)%256);
        number++;
	
	/* destination port */
	if ( (atoi(dstport_t) < 0) || (atoi(dstport_t) > 65535) ) {
		//printf("Error: tcp destination port range\n");
		error("Error: tcp destination port range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(dstport_t, strlen(dstport_t), "Error: tcp destination port field values") == -1)
                                return -1;

	packet[number] = (char)(atol(dstport_t)/256);
        number++;
        packet[number] = (char)(atol(dstport_t)%256);
        number++;

	/* sequence number */
	if ( strtoull(sequence_number_t, (char **)NULL, 10) > 0xFFFFFFFF ) {
		//printf("Error: tcp sequence number range\n");
		error("Error: tcp sequence number range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(sequence_number_t, strlen(sequence_number_t), 
						"Error: tcp sequence number field values") == -1)
                                return -1;

	seqnr = strtoul(sequence_number_t, (char **)NULL, 10);
	packet[number] = (char)(seqnr/16777216);
        number++;
        packet[number] = (char)(seqnr/65536);
        number++;
        packet[number] = (char)(seqnr/256);
        number++;
        packet[number] = (char)(seqnr%256);
        number++;

	/* acknowledgment number */
	if ( strtoull(ack_number_t, (char **)NULL, 10) > 0xFFFFFFFF) {
		//printf("Error: tcp ack number range\n");
		error("Error: tcp ack number range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(ack_number_t, strlen(ack_number_t), "Error: tcp ack number field values") == -1)
                                return -1;

	acknr = strtoul(ack_number_t, (char **)NULL, 10);
	packet[number] = (char)(acknr/16777216);
        number++;
        packet[number] = (char)(acknr/65536);
        number++;
        packet[number] = (char)(acknr/256);
        number++;
        packet[number] = (char)(acknr%256);
        number++;

	/* header length */	
	if ( (atoi(header_length_t) < 0) || (atoi(header_length_t) > 60) ) {
		//printf("Error: tcp header_length range\n");
		error("Error: tcp header_length range");
                return -1;
	}

	/* since we insert value as int, when dividing it with 4 there must remain 0 */
	if ( atoi(header_length_t) % 4 !=  0) {
		//printf("Error: tcp header_length range\n");
		error("Error: Wrong tcp header length value          \n(length mod 4 must be 0)");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(header_length_t, strlen(header_length_t), 
						"Error: tcp header_length field values") == -1)
                                return -1;

	packet[number] = (char)((atoi(header_length_t)*4));
	number++;	

	/* flags */
	if (GTK_TOGGLE_BUTTON(flag_cwr)->active) {
		flag_value = flag_value + 128;
	}	
	if (GTK_TOGGLE_BUTTON(flag_ecn)->active) {
		flag_value = flag_value + 64;
	}
	if (GTK_TOGGLE_BUTTON(flag_urg)->active) {
		flag_value = flag_value + 32;
	}
	if (GTK_TOGGLE_BUTTON(flag_ack)->active) {
		flag_value = flag_value + 16;
	}
	if (GTK_TOGGLE_BUTTON(flag_psh)->active) {
		flag_value = flag_value + 8;
	}
	if (GTK_TOGGLE_BUTTON(flag_rst)->active) {
		flag_value = flag_value + 4;
	}
	if (GTK_TOGGLE_BUTTON(flag_syn)->active) {
		flag_value = flag_value + 2;
	}
	if (GTK_TOGGLE_BUTTON(flag_fin)->active) {
		flag_value = flag_value + 1;
	}
	packet[number] = (char)flag_value;
	number++;	
	
	/* window size */	
	if ( (atoi(window_size_t) < 0) || (atoi(window_size_t) > 65535) ) {
		//printf("Error: tcp window size range\n");
		error("Error: tcp window size range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(window_size_t, strlen(window_size_t), "Error: tcp window size field values") == -1)
                                return -1;

	packet[number] = (char)(atol(window_size_t)/256);
	number++;	
	packet[number] = (char)(atol(window_size_t)%256);
	number++;	

	/* tcp checksum */
	if (GTK_TOGGLE_BUTTON(checksum_bt)->active) {
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
			//printf("Error: tcp checksum field\n");
			error("Error: tcp checksum field");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(checksum_t);
		checksum_t++; checksum_t++; number++;
		if (char2x(checksum_t) == -1) {
			//printf("Error: tcp checksum field\n");
			error("Error: tcp checksum field");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(checksum_t);
		number++;
	}
		
	/* urgent pointer */	
	if ( (atoi(urgent_pointer_t) < 0) || (atoi(urgent_pointer_t) > 65535) ) {
		//printf("Error: tcp urgent pointer range\n");
		error("Error: tcp urgent pointer range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(urgent_pointer_t, strlen(urgent_pointer_t), 
						"Error: tcp urgent pointer field values") == -1)
                                return -1;

	packet[number] = (char)(atol(urgent_pointer_t)/256);
	number++;	
	packet[number] = (char)(atol(urgent_pointer_t)%256);
	number++;	

	/* tcp options */
	if ( (strlen(options_t)%8) != 0) {
		//printf("Error: Wrong length of tcp options field (length mod 8 must be 0)\n");
		error("Error: Wrong length of tcp options field      \n(length mod 8 must be 0)");
		return -1;
	}
	
	if ( strlen(options_t) > 80) {
		//printf("Error: tcp options field to long\n");
		error("Error: tcp options field to long");
		return -1;
	}
	
	j = strlen(options_t)/2;
	for (i=0; i<j; i++) {
		if (char2x(options_t) == -1) {
			//printf("Error: tcp options field\n");
			error("Error: tcp options field");
       		        return -1;
		}
		packet[number] = (unsigned char)char2x(options_t);
		number++; options_t++; options_t++;
	}

	/* tcp payload */	
	if (GTK_TOGGLE_BUTTON(payload_bt)->active) {
		
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(payload));
                payload_length = gtk_text_buffer_get_char_count(buffer);
		GtkTextIter start,end;
		//gtk_text_buffer_get_start_iter(buffer,start);
		//gtk_text_buffer_get_end_iter(buffer,end);
		gtk_text_buffer_get_bounds(buffer,&start,&end);
		payload_t = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
		//g_free(start);
		//g_free(end);
		//payload_t = (char *) malloc(payload_length + 1);
                //payload_t = gtk_editable_get_chars(GTK_EDITABLE(payload),0,-1);
		
		/* YYY 1514-number is not ok in case we use 802.1q!!! */
		if (get_network_payload(button, user_data, payload_length, 
						1514-number, payload_t) == -1) {
			//printf("Error: Problem with tcp payload\n");
                        g_free(payload_t);
			return -1;
		}
                else
                        g_free(payload_t);

		cks_stop = number;
	}
	else
		cks_stop = number;

	/* it will be possible that we add some other protocols on top of tcp
	 * they will follow here... */
	

	/* and finally compute the tcp checksum if auto was enabled */
	if (checksum_start > 0) {
	       	
		/* this if for length  */
		tcpcksum = (guint32)(cks_stop - cks_start);
		/* pseudo header (ip part) + tcplength + nr of cicles over guint16 */
		tcpcksum = pseudo_header_sum + tcpcksum;
		/* if length is odd we have to add a pad byte */
		if( (cks_stop - cks_start)%2 != 0)
                                odd = 1;
		/* previos value + part from tcp checksum */
		tcpcksum = tcpcksum + get_checksum32(cks_start, cks_stop+odd);
		while (tcpcksum >> 16)
			tcpcksum = (tcpcksum & 0xFFFF) + (tcpcksum >> 16);
		/* the one's complement */
		tcpcksum = (-1) - tcpcksum;

		/* what about if the lenght is odd ??? 
		 * we check this in get_checksum routine */

		//no idea why -6, but the wireshark shows it is the magic number :)
		//it seems it has something to do with protocol nr. since the udp checksum requires -17
		if (ipv6_used == 1)
			tcpcksum = tcpcksum - 6;
		
		/* let's write it */
		packet[checksum_start] = (char)(tcpcksum/256);
		packet[checksum_start+1] =  (char)(tcpcksum%256);
	}
	
	return 1;
}