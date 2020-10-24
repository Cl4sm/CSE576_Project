int udp_get(GtkButton *button, gpointer user_data, guint32 pseudo_header_sum) 
{
	
	GtkWidget *srcport, *dstport, *length, *length_bt, *checksum, *checksum_bt;
	GtkWidget *payload_bt, *payload;

	gchar *srcport_t, *dstport_t, *length_t, *checksum_t, *payload_t;

	int length_start, checksum_start, cks_start, cks_stop, payload_length, odd=0;
	guint32 udpcksum;
	
	srcport = lookup_widget(GTK_WIDGET(button), "entry56");
	dstport = lookup_widget(GTK_WIDGET(button), "entry41");
	length = lookup_widget(GTK_WIDGET(button), "entry42");
	length_bt = lookup_widget(GTK_WIDGET(button), "checkbutton3");
	checksum = lookup_widget(GTK_WIDGET(button), "entry43");
	checksum_bt = lookup_widget(GTK_WIDGET(button), "checkbutton4");
	payload = lookup_widget(GTK_WIDGET(button), "text3");
	payload_bt = lookup_widget(GTK_WIDGET(button), "checkbutton5");
	
	srcport_t= (char *)gtk_entry_get_text(GTK_ENTRY(srcport));
	dstport_t= (char *)gtk_entry_get_text(GTK_ENTRY(dstport));
	length_t= (char *)gtk_entry_get_text(GTK_ENTRY(length));
	checksum_t= (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
	
	cks_start = number;
	
	/* source port */
	if ( (atoi(srcport_t) < 0) || (atoi(srcport_t) > 65535) ) {
		//printf("Error: Udp source port range\n");
		error("Error: Udp source port range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(srcport_t, strlen(srcport_t), "Error: Udp srcport field values") == -1)
                                return -1;

	packet[number] = (char)(atol(srcport_t)/256);
	number++;	
	packet[number] = (char)(atol(srcport_t)%256);
	number++;	
	
	/* destination port */
	if ( (atoi(dstport_t) < 0) || (atoi(dstport_t) > 65535) ) {
		//printf("Error: Udp destination port range\n");
		error("Error: Udp destination port range");
                return -1;
	}

        /* there can be rubbish in this field */
        if (check_digit(dstport_t, strlen(dstport_t), "Error: Udp destination port field values") == -1)
                                return -1;

	packet[number] = (char)(atol(dstport_t)/256);
	number++;	
	packet[number] = (char)(atol(dstport_t)%256);
	number++;	
	
	/* udp length */
	if (GTK_TOGGLE_BUTTON(length_bt)->active) {
		length_start = number;
		number++;
		number++;
	}
	else {
		/* if length_start = 0, we leave it in the end */
		length_start = 0;
		if ( (atoi(length_t) < 0) || (atoi(length_t) > 65535) ) {
			//printf("Error: Udp length range\n");
			error("Error: Udp length range");
        	        return -1;
		}
	
	        /* there can be rubbish in this field */
        	if (check_digit(length_t, strlen(length_t), "Error: Udp length field values") == -1)
                                return -1;

		packet[number] = (char)(atol(length_t)/256);
		number++;	
		packet[number] = (char)(atol(length_t)%256);
		number++;	
	}
	
	/* udp checksum */
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
			//printf("Error: udp checksum field\n");
			error("Error: udp checksum field");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(checksum_t);
		checksum_t++; checksum_t++; number++;
		if (char2x(checksum_t) == -1) {
			//printf("Error: udp checksum field\n");
			error("Error: udp checksum field");
        	        return -1;
		}
		packet[number] = (unsigned char)char2x(checksum_t);
		number++;
	}

	/* we need this one for knowing where the udp payload starts
	 * we need this one when sending the packets out and modifing some values */
	udp_start = number;	
		
	/* udp payload */
	/* so do we allow packet's longer than 1518 (1522) bytes or not ? Not.*/
	if (GTK_TOGGLE_BUTTON(payload_bt)->active) {
		
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(payload));
                payload_length = gtk_text_buffer_get_char_count(buffer);
		GtkTextIter start,end;
		gtk_text_buffer_get_bounds(buffer,&start,&end);
		//gtk_text_buffer_get_start_iter(buffer,&start);
		//gtk_text_buffer_get_end_iter(buffer,&end);
		payload_t = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
		//g_free(start);
		//g_free(end);
		//payload_t = (char *) malloc(payload_length + 1);
                //payload_t = gtk_editable_get_chars(GTK_EDITABLE(payload),0,-1);
		
		/* YYY 1514-number is not ok in case we use 802.1q!!! */
		if (get_network_payload(button, user_data, payload_length, 
						1514-number, payload_t) == -1) {
			//printf("Error: Problem with udp payload\n");
                        g_free(payload_t);
			return -1;
		}
                else
                        g_free(payload_t);

		cks_stop = number;
	}
	else
		cks_stop = number;

	/* it will be possible that we add some other protocols on top of udp
	 * they will follow here... */

	
	/* we have to fill the corect udp length if auto was enabled
	 * we add 2 bytes for source port and 2 bytes for dest port 
	 * because length_start points at udp length field  */
	if (length_start > 0) {
		packet[length_start] = (char)((number - length_start + 4)/256);
		packet[length_start+1] = (char)((number - length_start + 4)%256);
	}

	/* and finally compute the udp checksum if auto was enabled */
	if (checksum_start > 0) {
		/* if the user manually inserts the length value what then??? */
		/* we don't care it that means, if you manually insert the length
		 * than the auto checksum button won't help you 
		 * it would be better if the value would be correct either */
	       	
		/* this if for udp length  */
		udpcksum = (guint32)(cks_stop - cks_start);
		/* pseudo header (ip part) + udplength + nr of cicles over guint16 */
		udpcksum = pseudo_header_sum + udpcksum;
		/* if the length is odd we have to add a pad byte */
		if( (cks_stop - cks_start)%2 != 0)
                               odd = 1;
		/* previos value + part from udp checksum */
		udpcksum = udpcksum + get_checksum32(cks_start, cks_stop+odd);
		while (udpcksum >> 16)
			udpcksum = (udpcksum & 0xFFFF)+ (udpcksum >> 16);
		/* the one's complement */
		udpcksum = (-1) - udpcksum;

		//no idea why -17, but the wireshark shows it is the magic number :)
		if (ipv6_used == 1)
			udpcksum = udpcksum - 17;

		/* let's write it */
		packet[checksum_start] = (char)(udpcksum/256);
		packet[checksum_start+1] =  (char)(udpcksum%256);
	}
	return 1;
}