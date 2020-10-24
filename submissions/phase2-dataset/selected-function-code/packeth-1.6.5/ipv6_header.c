int ipv6_header(GtkButton *button, int whocalled, struct clist_hdr *clptr ) {

        char tmp[5];
        int x, prot, header_l;

        if (whocalled==2) {
                protokol = IPv6;
        }

        /* for standard header this is minimum length */
        if (remain < 40) {
                error("Can't load packet: IPv6 header field is not long enough!");
                return -1;
        }

        if (whocalled==1) {
                /* insert version */
                inspar(button, "entry195", ptrf, 1);

                /* insert traffic class */
                inspar(button, "entry196", ptrf, 2);

                /* insert tos */
                inspar(button, "entry197", ptrf, 5);

                /* insert total length */
                w1 = lookup_widget(GTK_WIDGET(button), "checkbutton43");
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
                //insint(button, "entry29", ptrf, 4);
        }
        else
        	ptrf = ptrf + 8;

       	ptrf = ptrf + 4;


	prot = char2x(ptrf);

        if (whocalled==1) {
                /* insert next header */
                inspar(button, "entry199", ptrf, 2);
        }
        else
                ptrf = ptrf+2;

	 if (whocalled==1) {
                /* insert hop limit */
                insint(button, "entry200", ptrf, 2);
	}
        else
                ptrf = ptrf+2;

	/*insert source ip */
        ptrt = temp6;
        memset(temp6, 0, 40);

        if (whocalled==1)
                w1 = lookup_widget(GTK_WIDGET(button), "entry201");

        for (i=1; i<8; i++) {
		strncpy(ptrt, ptrf, 4);
		ptrt = ptrt + 4;
                strcat(ptrt, ":");
		ptrt++;
		ptrf = ptrf + 4;
        }
	strncpy(ptrt, ptrf, 4);
	ptrf = ptrf + 4;

        if (whocalled==1)
                gtk_entry_set_text(GTK_ENTRY(w1), temp6);
        else
                memcpy(clptr->src, temp6, 40);

	/*insert destination ip */
        ptrt = temp6;
        memset(temp6, 0, 40);
        if (whocalled==1)
                w1 = lookup_widget(GTK_WIDGET(button), "entry202");

        for (i=1; i<8; i++) {
		strncpy(ptrt, ptrf, 4);
		ptrt = ptrt + 4;
                strcat(ptrt, ":");
		ptrt++;
		ptrf = ptrf + 4;
        }
	strncpy(ptrt, ptrf, 4);
	ptrf = ptrf + 4;

        if (whocalled==1)
                gtk_entry_set_text(GTK_ENTRY(w1), temp6);
        else
                memcpy(clptr->dst, temp6, 40);

	//extension header
	while ( (prot==0) || (prot==43) || (prot==44) || (prot==51) || (prot==50) || (prot==60) ) {
		prot = char2x(ptrf);
		header_l = retint2(ptrf+2, 2);
		inspar(button, "entry203", ptrf, header_l);

	}

        return prot;
}