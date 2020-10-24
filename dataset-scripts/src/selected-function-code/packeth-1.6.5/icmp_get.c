int icmp_get(GtkButton *button, gpointer user_data) {
	
	GtkWidget *type; 
	GtkWidget *code, *checksum, *cks_bt, *identifier, *seq_nr, *unused;
        GtkWidget *data_bt, *data;

	gchar *type_t;
	gchar *code_t, *checksum_t, *identifier_t, *seq_nr_t, *data_t, *unused_t;	
	
	int icmp_start, icmp_stop, checksum_start, payload_length;
	guint32 icmpcksum;
	
	type = lookup_widget(GTK_WIDGET(button), "entry57");
	type_t = (char *)gtk_entry_get_text(GTK_ENTRY(type));
	
	icmp_start = number;
	
	/* type */
	if (char2x(type_t) == -1) {
		//printf("Error: icmp type field\n");
		error("Error: icmp type field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(type_t);
	number++;

	/* YYY hmmm, it would be better to be the type filed inserted as int
	 * not as hex, so we have to calculate it */
	switch (atoi(type_t)) {
		case 0: {
			//printf("ICMP echo reply\n");
			code = lookup_widget(GTK_WIDGET(button), "entry62");
			checksum = lookup_widget(GTK_WIDGET(button), "entry63");
			cks_bt = lookup_widget(GTK_WIDGET(button), "checkbutton16");
			identifier = lookup_widget(GTK_WIDGET(button), "entry64");
			seq_nr = lookup_widget(GTK_WIDGET(button), "entry65");
			data_bt = lookup_widget(GTK_WIDGET(button), "checkbutton17");
			data = lookup_widget(GTK_WIDGET(button), "entry66");

			code_t = (char *)gtk_entry_get_text(GTK_ENTRY(code));
			checksum_t = (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
			identifier_t = (char *)gtk_entry_get_text(GTK_ENTRY(identifier));
			seq_nr_t = (char *)gtk_entry_get_text(GTK_ENTRY(seq_nr));
			data_t = (char *)gtk_entry_get_text(GTK_ENTRY(data));
			
			/* code */
			if (char2x(code_t) == -1) {
				//printf("Error: icmp reply code field\n");
				error("Error: icmp reply code field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(code_t);
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
					//printf("Error: icmp reply checksum field\n");
					error("Error: icmp reply checksum field");
        	        		return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				checksum_t++; checksum_t++; number++;
				if (char2x(checksum_t) == -1) {
					//printf("Error: icmp reply checksum field\n");
					error("Error: icmp reply checksum field");
        		        	return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				number++;
			}
			
			/* identifier */
			if (char2x(identifier_t) == -1) {
				//printf("Error: icmp reply identifier field\n");
				error("Error: icmp reply identifier field");
        		        return -1;
			}
			packet[number] = (unsigned char)char2x(identifier_t);
			identifier_t++; identifier_t++; number++;
			if (char2x(identifier_t) == -1) {
				//printf("Error: icmp reply identifier field\n");
				error("Error: icmp reply identifier field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(identifier_t);
			number++;
			
			/* sequence number */
			if (char2x(seq_nr_t) == -1) {
				//printf("Error: icmp reply identifier field\n");
				error("Error: icmp reply identifier field");
        		        return -1;
			}
			packet[number] = (unsigned char)char2x(seq_nr_t);
			seq_nr_t++; seq_nr_t++; number++;
			if (char2x(seq_nr_t) == -1) {
				//printf("Error: icmp reply identifier field\n");
				error("Error: icmp reply identifier field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(seq_nr_t);
			number++;
			
			/* data */
			if (GTK_TOGGLE_BUTTON(data_bt)->active) {
				
				payload_length = strlen(data_t);
				
				/* YYY 1514-number is not ok in case we use 802.1q!!! */
				if (get_network_payload(button, user_data, payload_length, 
							1514-number, data_t) == -1) {
					//printf("problem with icmp reply payload\n");
					return -1;
				}

				icmp_stop = number;
			}
			else
				icmp_stop = number;

			if (checksum_start > 0) {
				
				icmpcksum =  get_checksum16(icmp_start, icmp_stop); 
				/* the one's complement */
				icmpcksum = (-1) - icmpcksum;

				/* let's write it */
				packet[checksum_start] = (char)(icmpcksum/256);
				packet[checksum_start+1] =  (char)(icmpcksum%256);
			}
			break;
		}
		

		/* icmp echo request */
		case 8: {
			//printf("ICMP echo request\n");
			code = lookup_widget(GTK_WIDGET(button), "entry74");
			checksum = lookup_widget(GTK_WIDGET(button), "entry77");
			cks_bt = lookup_widget(GTK_WIDGET(button), "checkbutton20");
			identifier = lookup_widget(GTK_WIDGET(button), "entry75");
			seq_nr = lookup_widget(GTK_WIDGET(button), "entry78");
			data_bt = lookup_widget(GTK_WIDGET(button), "checkbutton19");
			data = lookup_widget(GTK_WIDGET(button), "entry76");

			code_t = (char *)gtk_entry_get_text(GTK_ENTRY(code));
			checksum_t = (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
			identifier_t = (char *)gtk_entry_get_text(GTK_ENTRY(identifier));
			seq_nr_t = (char *)gtk_entry_get_text(GTK_ENTRY(seq_nr));
			data_t = (char *)gtk_entry_get_text(GTK_ENTRY(data));
			
			/* code */
			if (char2x(code_t) == -1) {
				//printf("Error: icmp request code field\n");
				error("Error: icmp request code field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(code_t);
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
					//printf("Error: icmp request checksum field\n");
					error("Error: icmp request checksum field");
        	        		return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				checksum_t++; checksum_t++; number++;
				if (char2x(checksum_t) == -1) {
					//printf("Error: icmp request checksum field\n");
					error("Error: icmp request checksum field");
        			        return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				number++;
			}
			
			/* identifier */
			if (char2x(identifier_t) == -1) {
				//printf("Error: icmp request identifier field\n");
				error("Error: icmp request identifier field");
        		        return -1;
			}
			packet[number] = (unsigned char)char2x(identifier_t);
			identifier_t++; identifier_t++; number++;
			if (char2x(identifier_t) == -1) {
				//printf("Error: icmp request identifier field\n");
				error("Error: icmp request identifier field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(identifier_t);
			number++;
			
			/* sequence number */
			if (char2x(seq_nr_t) == -1) {
				//printf("Error: icmp request identifier field\n");
				error("Error: icmp request identifier field");
        		        return -1;
			}
			packet[number] = (unsigned char)char2x(seq_nr_t);
			seq_nr_t++; seq_nr_t++; number++;
			if (char2x(seq_nr_t) == -1) {
				//printf("Error: icmp request identifier field\n");
				error("Error: icmp request identifier field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(seq_nr_t);
			number++;
			
			/* data */
			if (GTK_TOGGLE_BUTTON(data_bt)->active) {
				
				payload_length = strlen(data_t);
				
				/* YYY 1514-number is not ok in case we use 802.1q!!! */
				if (get_network_payload(button, user_data, payload_length, 
							1514-number, data_t) == -1) {
					//printf("Error: problem with icmp request payload\n");
					return -1;
				}

				icmp_stop = number;
			}
			else
				icmp_stop = number;

			if (checksum_start > 0) {
				
				icmpcksum =  get_checksum16(icmp_start, icmp_stop); 
				/* the one's complement */
				icmpcksum = (-1) - icmpcksum;

				/* let's write it */
				packet[checksum_start] = (char)(icmpcksum/256);
				packet[checksum_start+1] =  (char)(icmpcksum%256);
			}
			break;
		}
		
			
		/* icmp destination unreacheable */
		case 3: {
			//printf("ICMP destination unreacheable\n");
			code = lookup_widget(GTK_WIDGET(button), "entry58");
			checksum = lookup_widget(GTK_WIDGET(button), "entry59");
			cks_bt = lookup_widget(GTK_WIDGET(button), "checkbutton15");
			unused = lookup_widget(GTK_WIDGET(button), "entry60");
			data_bt = lookup_widget(GTK_WIDGET(button), "checkbutton24");
			data = lookup_widget(GTK_WIDGET(button), "entry61");

			code_t = (char *)gtk_entry_get_text(GTK_ENTRY(code));
			checksum_t = (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
			unused_t = (char *)gtk_entry_get_text(GTK_ENTRY(unused));
			data_t = (char *)gtk_entry_get_text(GTK_ENTRY(data));
			
			/* code */
			if (char2x(code_t) == -1) {
				//printf("Error: icmp destination unreacheable code field\n");
				error("Error: icmp destination unreacheable code field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(code_t);
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
					//printf("Error: icmp destination unreacheable checksum field\n");
					error("Error: icmp destination unreacheable checksum field");
        	        		return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				checksum_t++; checksum_t++; number++;
				if (char2x(checksum_t) == -1) {
					//printf("Error: icmp destination unreacheable checksum field\n");
					error("Error: icmp destination unreacheable checksum field");
        		        	return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				number++;
			}
			
			/* unused field */
			if (char2x(unused_t) == -1) {
				//printf("Error: icmp destination unreacheable unused field\n");
				error("Error: icmp destination unreacheable unused field");
        		        return -1;
			}
			packet[number] = (unsigned char)char2x(unused_t);
			unused_t++; unused_t++; number++;
			if (char2x(unused_t) == -1) {
				//printf("Error: icmp destination unreacheable unused field\n");
				error("Error: icmp destination unreacheable unused field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(unused_t);
			unused_t++; unused_t++; number++;
			if (char2x(unused_t) == -1) {
				//printf("Error: icmp destination unreacheable unused field\n");
				error("Error: icmp destination unreacheable unused field");
        		        return -1;
			}
			packet[number] = (unsigned char)char2x(unused_t);
			unused_t++; unused_t++; number++;
			if (char2x(unused_t) == -1) {
				//printf("Error: icmp destination unreacheable unused field\n");
				error("Error: icmp destination unreacheable unused field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(unused_t);
			number++;
			
			/* data */
			if (GTK_TOGGLE_BUTTON(data_bt)->active) {
				
				payload_length = strlen(data_t);
				
				/* YYY 1514-number is not ok in case we use 802.1q!!! */
				if (get_network_payload(button, user_data, payload_length, 
							1514-number, data_t) == -1) {
					//printf("Error: problem with icmp destination unreacheable payload\n");
					return -1;
				}

				icmp_stop = number;
			}
			else
				icmp_stop = number;

			if (checksum_start > 0) {
				
				icmpcksum =  get_checksum16(icmp_start, icmp_stop); 
				/* the one's complement */
				icmpcksum = (-1) - icmpcksum;

				/* let's write it */
				packet[checksum_start] = (char)(icmpcksum/256);
				packet[checksum_start+1] =  (char)(icmpcksum%256);
			}
			break;
		}
			
		default: {
			//printf("Other type of icmp message\n");
			code = lookup_widget(GTK_WIDGET(button), "entry157");
			checksum = lookup_widget(GTK_WIDGET(button), "entry158");
			cks_bt = lookup_widget(GTK_WIDGET(button), "checkbutton38");
			data = lookup_widget(GTK_WIDGET(button), "entry159");

			code_t = (char *)gtk_entry_get_text(GTK_ENTRY(code));
			checksum_t = (char *)gtk_entry_get_text(GTK_ENTRY(checksum));
			data_t = (char *)gtk_entry_get_text(GTK_ENTRY(data));
			
			/* code */
			if (char2x(code_t) == -1) {
				//printf("Error: icmp other code field\n");
				error("Error: icmp other code field");
		                return -1;
			}
			packet[number] = (unsigned char)char2x(code_t);
			number++;
			
			/* checksum */	
			if (GTK_TOGGLE_BUTTON(cks_bt)->active) {
				checksum_start = number;
				packet[number] = (unsigned char)0;
				number++;
				packet[number] = (unsigned char)0;
				number++;
			}
			/* if checksum_start = 0, we leave it in the end */
			else {
				checksum_start = 0;
	
				if (char2x(checksum_t) == -1) {
					//printf("Error: icmp destination unreacheable checksum field\n");
					error("Error: icmp destination unreacheable checksum field");
        	        		return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				checksum_t++; checksum_t++; number++;
				if (char2x(checksum_t) == -1) {
					//printf("Error: icmp destination unreacheable checksum field\n");
					error("Error: icmp destination unreacheable checksum field");
        		        	return -1;
				}
				packet[number] = (unsigned char)char2x(checksum_t);
				number++;
			}
			
			/* data */
			payload_length = strlen(data_t);
				
			/* YYY 1514-number is not ok in case we use 802.1q!!! */
			if (get_network_payload(button, user_data, payload_length, 
						1514-number, data_t) == -1) {
				//printf("Error: problem with icmp destination unreacheable payload\n");
				return -1;
			}

			icmp_stop = number;

			if (checksum_start > 0) {
				
				icmpcksum =  get_checksum16(icmp_start, icmp_stop); 
				/* the one's complement */
				icmpcksum = (-1) - icmpcksum;

				/* let's write it */
				packet[checksum_start] = (char)(icmpcksum/256);
				packet[checksum_start+1] =  (char)(icmpcksum%256);
			}
		}
	}
	return 1;
}