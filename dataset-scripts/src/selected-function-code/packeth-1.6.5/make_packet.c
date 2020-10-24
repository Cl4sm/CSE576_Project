int make_packet(GtkButton *button, gpointer user_data)
{
	GtkWidget *ipv4, *ipv6, *arp, *usedef;
	GtkWidget *auto_bt, *text_e;
	int max, length;
	gchar *text;

	/* first we fill packet field with 0 */
	memset(packet, 0x00, 1522);

	/* YYY what about auto selection for link layer is on?
	 * in case of saving packet we don't get here and it is ok
	 * in case of user defined payload we automatically disable this feature and it is ok
	 * so what about arp, ipv4 and ipv6?
	 * in case of an arp packet we accept the auto get mac option and it means that 
	 * we take the source and destination mac address from the arp protokol field
	 * in case of an ipv4 packet this means that we don't open the raw socket but
	 * do all the sending on ip socket which helps us getting the mac address */
	
	auto_bt = lookup_widget(GTK_WIDGET (button), "auto_get_mac_cbt");
	ipv4 = lookup_widget(GTK_WIDGET (button), "ippkt_radibt");
	ipv6 = lookup_widget(GTK_WIDGET (button), "IPv6_rdbt");
	arp = lookup_widget(GTK_WIDGET (button), "arppkt_radiobt");
	usedef = lookup_widget(GTK_WIDGET (button), "usedef2_radibt");

	/* what about next layer: ipv4, ipv6, arp or manually attached payload? */
	if (GTK_TOGGLE_BUTTON(ipv4)->active) {
		/* YYY - to be done */
		if (GTK_TOGGLE_BUTTON(auto_bt)->active) {
				error("Not yet implemented");
				//printf("Not yet implemented\n");
				return -1;
		}
		
		/* now we get the link layer info */
		else if (link_level_get(button, user_data) == -1) {
			//printf("Error: problem on link layer with IPv4 packet\n");
			return -1;
		}

		/* call the function that gets the ipv4 protocol information */
		if (ipv4_get(button, user_data) == -1) {
			//printf("Error: problem with IPv4 information\n");
			return -1;
		}
		
		/* grrr, oh you could think on this earlier!!! */
		if (autolength > 0) {
			//printf("tole je auto %d tole pa number %d\n", autolength, number);
			packet[autolength] = (unsigned char)((number - (autolength + 2))/256);
			packet[autolength+1] = (unsigned char)((number - (autolength + 2))%256);
		}	
		
		return 1;
	}
	else if (GTK_TOGGLE_BUTTON(ipv6)->active) {
		/* now we get the link layer info */
		if (link_level_get(button, user_data) == -1) {
			return -1;
		}

		/* call the function that gets the ipv6 protocol information */
		if (ipv6_get(button, user_data) == -1) {
			//printf("Error: problem with IPv6 information\n");
			return -1;
		}
		return 1;
	}
	else if (GTK_TOGGLE_BUTTON(arp)->active) {
		/* we need to check the auto button, if it is on we copy the values from the 
		 * arp filed entries in to the link layer source and destination mac entries 
		 * the good thing about this is that we add a feature that can help
		 * (or confuse?) the bad thing about this is that we can't do it once
		 * with the link layer and take whatever the next layer is*/
		if (GTK_TOGGLE_BUTTON(auto_bt)->active) {
			GtkWidget *en_dst_mac, *en_src_mac, *L_dst, *L_src;
			//gchar tmp_dst[18], tmp_src[18];
			gchar *en_dst_t, *en_src_t;

			en_dst_mac = lookup_widget(GTK_WIDGET(button), "A_targetmac");
			en_src_mac = lookup_widget(GTK_WIDGET(button), "A_sendermac");
			L_dst = lookup_widget(GTK_WIDGET(button), "L_dst_mac");
			L_src = lookup_widget(GTK_WIDGET(button), "L_src_mac");

			en_dst_t = (char *)gtk_entry_get_text(GTK_ENTRY(en_dst_mac));
			en_src_t = (char *)gtk_entry_get_text(GTK_ENTRY(en_src_mac));
			
			/*chech if both addresses are ok */
			if (check_mac_address(en_dst_t) == -1) {
				//printf("Error: wrong mac entry in arp destination field, can't copy it\n");
				error("Error: wrong mac entry in arp destination field, can't copy it");
				return -1;
			}
			 
			/*chech if both addresses are ok */
			if (check_mac_address(en_src_t) == -1) {
				//printf("Error: wrong mac entry in arp source field, can't copy it\n");
				error("Error: wrong mac entry in arp source field, can't copy it");
				return -1;
			}

			/*insert it in the link layer field */
			gtk_entry_set_text(GTK_ENTRY(L_dst), en_dst_t);
			gtk_entry_set_text(GTK_ENTRY(L_src), en_src_t);

		}
	 
		/* now we get the link layer info */
		if (link_level_get(button, user_data) == -1) {
			//printf("Error: problem on link layer with arp packet\n");
			return -1;
		}

		/* call the function that gets the arp protocol information */
		if (arp_get(button, user_data) == -1) {
			//printf("Error: problem with arp information\n");
			return -1;
		}
			
		if (autolength > 0) {
			//printf("tole je auto %d tole pa number %d\n", autolength, number);
			packet[autolength] = (unsigned char)((number - (autolength + 2))/256);
			packet[autolength+1] = (unsigned char)((number - (autolength + 2))%256);
		}	
		return 1;
	}
	
	else if (GTK_TOGGLE_BUTTON(usedef)->active) {
		/* if usedef is active we will manually get the link layer info */
		if (link_level_get(button, user_data) == -1) {
			//printf("Error: problem on link layer\n");
			return -1;
		}
		
		if((number == 14) || (number == 18))
			max = 1500;
		else if ((number == 17) || (number == 21))
			max = 1497;
		else if ((number == 22) || (number == 26))
			max = 1492;
		else {
			//printf("Error: failure with number in layer 2 user defined field\n");
			error("Error: failure with number in layer 2 user defined payload");
			return -1;
		}
 
		text_e = lookup_widget(GTK_WIDGET (button), "text1");
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_e));
		length = gtk_text_buffer_get_char_count(buffer);
		//text = (char *) malloc(length + 1);
		GtkTextIter start,end;
		//gtk_text_buffer_get_start_iter(buffer,start);
		//gtk_text_buffer_get_end_iter(buffer,end);
		gtk_text_buffer_get_bounds(buffer,&start,&end);
		text = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);	
		//text = gtk_editable_get_chars(GTK_EDITABLE(text_e), 0, -1);
		//g_free(start);
		//g_free(end);
	
		if (get_network_payload(button, user_data, length, max, text) == -1) {
			//printf("Error: problem with payload on network layer\n");
			g_free(text);
			return -1;
		}
		else
			g_free(text);

		if (autolength > 0) {
			//printf("tole je auto %d tole pa number %d\n", autolength, number);
			packet[autolength] = (unsigned char)((number - (autolength + 2))/256);
			packet[autolength+1] = (unsigned char)((number - (autolength + 2))%256);
		}	
	}
	else {  /* none of above -> something is wrong! */
		//printf("Error: problem with network layer button\n");
		error("Error: problem with network layer button");
		return -1;
	}
		
	return 1;
}