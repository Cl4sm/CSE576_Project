void
on_button87_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

	GtkWidget *en1;
	FILE *file_p;
        gchar *mbps_t;
	double bw[10];
	long pkts[10], pktnr[10];
	int i;
	long totalpkt=0, usdelta, minpkts=1500000, pktlen;
	char ime[10];

	/* dodat moraš če je slučajno prazen, al pa disablan */
	for(i=0; i<10; i++) {
		bw[i]= -1; pkts[i]= -1; pktnr[i]= -1;
		sprintf(ime, "entry%d", i+185);
        	en1 = lookup_widget(GTK_WIDGET(button), ime);
        	mbps_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));

		/* if there is nothing inside Mbit/s field, just skip */
		if(strlen(mbps_t)==0)
			continue;

		/* if the entry is wrong, skip */
		//if (check_digit(mbps_t, strlen(mbps_t), 
		//	"Error: Mbit/s values\nAllowed format: 10,22") == -1) return;

		/* remember bw */
		bw[i] = strtod((mbps_t), NULL);

		sprintf(ime, "entry%d", i+111);
        	en1 = lookup_widget(GTK_WIDGET(button), ime);
        	mbps_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));

		/* if the file is not ready for opening, return error */
		if((file_p = fopen(mbps_t, "r")) == NULL) { 
        	        error("Error: can't open file for reading!");
        	        return;
        	}
		fseek(file_p, 0 , SEEK_END);
		pktlen = ftell(file_p)-40; /* 40 bytes is the pcap header, not nice I know, but... */
		close(file_p);

		/* number of packets per second */
		pkts[i]=bw[i]*1000000/(pktlen*8);

		/* remember the min value of packets per second */
		if (pkts[i] < minpkts )
			minpkts = pkts[i];

		totalpkt = totalpkt + pkts[i];

	}

	usdelta = 1000000/totalpkt;

	for(i=0; i<10; i++) {
		if (bw[i] > -1 ) {
			pktnr[i] = pkts[i] / minpkts;	

			sprintf(ime, "entry%d", i+121);
        		en1 = lookup_widget(GTK_WIDGET(button), ime);
			snprintf(ime, 9, "%ld", pktnr[i]);
        		gtk_entry_set_text(GTK_ENTRY(en1), ime);

			sprintf(ime, "entry%d", i+131);
        		en1 = lookup_widget(GTK_WIDGET(button), ime);
			snprintf(ime, 9, "%ld", usdelta);
        		gtk_entry_set_text(GTK_ENTRY(en1), ime);

			sprintf(ime, "entry%d", i+141);
        		en1 = lookup_widget(GTK_WIDGET(button), ime);
			snprintf(ime, 9, "%ld", usdelta);
        		gtk_entry_set_text(GTK_ENTRY(en1), ime);
		}
	}

	//en1 = lookup_widget(GTK_WIDGET(button), "entry152");
	//snprintf(ime, 9, "%ld", usdelta);
        //gtk_entry_set_text(GTK_ENTRY(en1), ime);

}