int get_network_payload(GtkButton *button, gpointer user_data, int length, int max, gchar *entry)
{
	int i, stevec = 0;
	gchar *ptr;

	/* firs we check if total length without spaces is an even number */
	ptr = entry;
	for (i=0; i < length; i++, ptr++) {
		if (isspace(*ptr) != 0) { /* prazne znake ne upostevam */
			continue;
		}	
		stevec++;
	}

	if ( stevec % 2 != 0) {
		//printf("Error: Payload lengtht must be an even number\n");
		error("Error: Payload lengtht must be an even number");
		return -1;
	}
	
	stevec = 1;

	for (i=0; i < length ; ) {
		if (isspace(*entry) != 0) { /* prazne znake ne upostevam */
			entry++;
			i++;
			continue;
		}	
		if (stevec > max) {
			//printf("Error: Network layer payload lengtht to long\n");
			error("Error: Network layer payload lengtht to long");
			return -1;
		}
		if (char2x(entry) == -1) {
			//printf("Error: network layer payload\n");
			error("Error: network layer payload");
			return -1;
		}
		packet[number] = (unsigned char)char2x(entry);
		number++; i++; i++; entry++; entry++; stevec++;; 	
	}
	return 1;	
}