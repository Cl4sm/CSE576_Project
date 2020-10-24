int load_data(GtkButton *button, FILE *file_p, int whocalled, int howmanypackets) {
	
	struct pcap_hdr fh;
	struct pcaprec_hdr ph;
	struct clist_hdr clh;
	int j, ji, freads;
	guint32 secu = 0, secb = 0;
	guint32 usecu = 0, usecb = 0;
	double timediff = 0;
	double timebeg= 0;
	char pkt_temp[3100];
	GtkWidget *clis;

	/* first we read the pcap file header */
	freads = fread(pkt_temp, sizeof(fh), 1, file_p);
	/* if EOF, exit */
	if (freads == 0)
		return 1;

	memcpy(&fh, pkt_temp, 24);

	/* if magic number in NOK, exit */
	if (fh.magic != PCAP_MAGIC) 
		return -1;

	/* hm, I  forgot a little bit, but I assume 1 means builder here */
	if (whocalled == 1) {
		
		/* next the  pcap packet header */
		freads = fread(pkt_temp, sizeof(ph), 1, file_p);
		
		/* if EOF, exit */
		if (freads == 0)
			return 1;

		/* copy the 16 bytes into ph structure */
		memcpy(&ph, pkt_temp, 16);

		/* and the packet itself, but only up to the capture length */
		freads = fread(pkt_temp+sizeof(ph), ph.incl_len, 1, file_p);

		/* if EOF, exit */
		if (freads == 0)
			return 1;

		/* convert the packet information from int to hex */
		for(ji=0; ji<(sizeof(ph)+ph.incl_len); ji++)
			c8(&field[2*ji], *(pkt_temp+ji)); 
		field[2*ji+2] = '\0';
		
		load_packet_disector(button, field, 1, &clh, ph.incl_len);			

		return 1; 
	}

	else { 
	
		clis = lookup_widget(GTK_WIDGET (button), "clist2");
		gtk_clist_clear(GTK_CLIST(clis));
	
		for (j=0; j<howmanypackets; j++) {
	
			/* next the  pcap packet header */
			fread(pkt_temp, sizeof(ph), 1, file_p);

			/* if EOF, exit */
			if (freads == 0)
				return 1;

			/* copy the 16 bytes into ph structure */
			memcpy(&ph, pkt_temp, 16);

			/* and the packet itself, but only up to the capture length */
			freads = fread(pkt_temp+sizeof(ph), ph.incl_len, 1, file_p);

			/* if EOF, exit */
			if (freads == 0)
				return 1;

			/* convert the packet information from int to hex */
			for(ji=0; ji<(sizeof(ph)+ph.incl_len); ji++)
				c8(&field[2*ji], *(pkt_temp+ji)); 
			field[2*ji+2] = '\0';
		
			/* we have to dissect the packet to get information for the list */
			load_packet_disector(button, field, 2, &clh, ph.incl_len);			
			//printf("tukaj3 %s\n", field);
			
			/* calculate the time information */
			if (j==0) { 
				timediff = 0;
				timebeg = 0;
				secb = ph.ts_sec;
				usecb = ph.ts_usec;
			}
			else { 
				timediff = ((double)((double)ph.ts_sec - (double)secu)*1000000 + 
								(double)((double)ph.ts_usec - (double)usecu)) / 1000000;
				timebeg = ((double)((double)ph.ts_sec - (double)secb)*1000000 + 
								(double)((double)ph.ts_usec - (double)usecb)) / 1000000;
			}

			//printf("tukaj2 %s\n", field);

			secu = ph.ts_sec;
			usecu = ph.ts_usec;

			/* insert a new row into clist */
			load_gen_p_data(button, clis, field, &ph, j+1, &clh, timediff, timebeg);
		}
		if (j == howmanypackets) 
			error("Only first 1000 packets loaded!\nTo change this modify #define on top of callbacks.c");
	}

	return 1;

}