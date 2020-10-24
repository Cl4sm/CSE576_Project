int load_gen_p_data(GtkButton *button, GtkWidget *clis, char *fieldek, struct pcaprec_hdr *ph2, 
					int pkt_nr, struct clist_hdr *clptr, double timediff, double timebeg) {

	gchar *datap[8];
	gchar fieldp[7][41];
	gchar field_p[1][3100];

	datap[0]=&fieldp[0][0];
	datap[1]=&fieldp[1][0];
	datap[2]=&fieldp[2][0];
	datap[3]=&fieldp[3][0];
	datap[4]=&fieldp[4][0];
	datap[5]=&fieldp[5][0];
	datap[6]=&fieldp[6][0];
	datap[7]=&field_p[0][0];

	//printf("TUKAJ1:%s\n", fieldek);
	g_snprintf(fieldp[0], 20, "%d", pkt_nr);
	g_snprintf(fieldp[1], 20, "%f", timebeg);
	g_snprintf(fieldp[2], 20, "%f", timediff);

	if ( (*ph2).incl_len == (*ph2).orig_len )
		g_snprintf(fieldp[3], 20, "%d", (*ph2).incl_len);
	else
		g_snprintf(fieldp[3], 20, "%d !", (*ph2).incl_len);

	g_snprintf(fieldp[4], 40, "%s", clptr->src);
	g_snprintf(fieldp[5], 40, "%s", clptr->dst);

	switch (protokol) {
		case ETH_II: {
			g_snprintf(fieldp[6], 20, "Ethernet II");
			break;
		}
		case ETH_802_3: {
			g_snprintf(fieldp[6], 20, "Ethernet 802.3");
			break;
		}
		case ARP: {
			g_snprintf(fieldp[6], 20, "ARP");
			break;
		}
		case IPv4: {
			g_snprintf(fieldp[6], 20, "IPv4");
			break;
		}
		case IPv6: {
			g_snprintf(fieldp[6], 20, "IPv6");
			break;
		}
		case TCP: {
			g_snprintf(fieldp[6], 20, "TCP");
			break;
		}
		case UDP: {
			g_snprintf(fieldp[6], 20, "UDP");
			break;
		}
		case IGMP: {
			g_snprintf(fieldp[6], 20, "IGMP");
			break;
		}
		case ICMP: {
			g_snprintf(fieldp[6], 20, "ICMP");
			break;
		}
	}

	g_snprintf(field_p[0], 2*(32+(*ph2).incl_len), "%s", fieldek);
		
	gtk_clist_append(GTK_CLIST(clis), datap);
	
	return 1;

}