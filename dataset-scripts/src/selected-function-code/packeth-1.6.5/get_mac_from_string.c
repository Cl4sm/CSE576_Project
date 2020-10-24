int get_mac_from_string(GtkButton *button)
{
	GtkWidget *dstmac_e, *srcmac_e;
	gchar *dstmac_t, *srcmac_t;
	int dst_length, src_length, i;

	dstmac_e = lookup_widget(GTK_WIDGET (button), "L_dst_mac");
	srcmac_e = lookup_widget(GTK_WIDGET (button), "L_src_mac");
	dstmac_t = (char *)gtk_entry_get_text(GTK_ENTRY(dstmac_e));
	srcmac_t = (char *)gtk_entry_get_text(GTK_ENTRY(srcmac_e));
	dst_length = strlen(dstmac_t);
	src_length = strlen(srcmac_t);

	/* mac naslov mora viti v formatu xx:xx:xx:xx:xx:xx to pomeni 17 znakov skupaj! */
	if ((src_length != 17) || (dst_length != 17))
		return -1;
	
	/* first we store destination address into packet[] */
	for(i=0; i<6; i++) {
		if (char2x(dstmac_t) == -1)
			return -1;
		packet[i] = (unsigned char)char2x(dstmac_t);
		dstmac_t = dstmac_t + 2;
		if ((i<5) && (*dstmac_t != ':'))
			return -1;
		else if (i == 5)
			;
		else
			dstmac_t++;
	}
		 
	/* source address into packet[] */
	for(i=6; i<12; i++) {
		if (char2x(srcmac_t) == -1)
			return -1;
		packet[i] = (unsigned char)char2x(srcmac_t);
		srcmac_t = srcmac_t + 2;
		if ((i<5) && (*srcmac_t != ':'))
			return -1;
		else if (i == 5)
			;
		else
			srcmac_t++;
	}
		 
	return 1;
}