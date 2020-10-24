void
on_rtp_ok_bt_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	char tmp[3000], tmp2[4600];
	int ij = 0, i, j;
 	GtkWidget *version, *csrc_nr, *payload_type, *seq_nr, *timestamp, *ssrc, *csrc, *extension;
	GtkWidget *padding_true, *padding_false, *extension_true, *extension_false;
	GtkWidget *marker_true, *marker_false, *menu1, *rtp_payload;
	gchar *vers_t, *csrc_nr_t, *payload_type_t, *seq_nr_t, *timestamp_t, *ssrc_t;
	gchar *csrc_t, *extension_t, *rtp_payload_t;
	int sum, intversion, intpadding, intmarker, intextension, intcsrc, inttype;
	guint16 intseq;
	guint32 inttimestamp, intssrc; 
	
        version = lookup_widget(GTK_WIDGET(button), "entry91");
        csrc_nr = lookup_widget(GTK_WIDGET(button), "entry92");
        payload_type = lookup_widget(GTK_WIDGET(button), "entry102");
        seq_nr = lookup_widget(GTK_WIDGET(button), "entry101");
        timestamp = lookup_widget(GTK_WIDGET(button), "entry97");
        ssrc = lookup_widget(GTK_WIDGET(button), "entry96");
        csrc = lookup_widget(GTK_WIDGET(button), "entry98");
        extension = lookup_widget(GTK_WIDGET(button), "entry99");
        padding_true = lookup_widget(GTK_WIDGET(button), "radiobutton23");
        padding_false = lookup_widget(GTK_WIDGET(button), "radiobutton24");
        extension_true = lookup_widget(GTK_WIDGET(button), "radiobutton25");
        extension_false = lookup_widget(GTK_WIDGET(button), "radiobutton26");
        marker_true = lookup_widget(GTK_WIDGET(button), "radiobutton27");
        marker_false = lookup_widget(GTK_WIDGET(button), "radiobutton28");
        menu1 = lookup_widget(GTK_WIDGET(button), "optionmenu7");
	rtp_payload = lookup_widget(GTK_WIDGET(button), "entry103");

	vers_t = (char *)gtk_entry_get_text(GTK_ENTRY(version));
	csrc_nr_t = (char *)gtk_entry_get_text(GTK_ENTRY(csrc_nr));
	payload_type_t = (char *)gtk_entry_get_text(GTK_ENTRY(payload_type));
	seq_nr_t = (char *)gtk_entry_get_text(GTK_ENTRY(seq_nr));
	timestamp_t = (char *)gtk_entry_get_text(GTK_ENTRY(timestamp));
	ssrc_t = (char *)gtk_entry_get_text(GTK_ENTRY(ssrc));
	csrc_t = (char *)gtk_entry_get_text(GTK_ENTRY(csrc));
	extension_t = (char *)gtk_entry_get_text(GTK_ENTRY(extension));
	rtp_payload_t = (char *)gtk_entry_get_text(GTK_ENTRY(rtp_payload));

	/* what numbers should we allow? only 2? */
	intversion = strtol(vers_t, (char **)NULL, 10);
	if ( (intversion > 3) || (intversion < 0) ) {
		//printf("Error: RTP version type\n");
		error("Error: RTP version type");
		return;
	}

	/* there can be rubbish in this field */
        if (check_digit(vers_t, strlen(vers_t), "Error: RTP version value") == -1)
                                return;
	
	/* should we add paddind automaticaly or not? no we do not do it */
	if (GTK_TOGGLE_BUTTON(padding_true)->active) 
		intpadding = 1;
	else
		intpadding = 0;

	if (GTK_TOGGLE_BUTTON(extension_true)->active) 
		intextension = 1;
	else
		intextension = 0;

	/* what numbers should we allow? between 0 and 15? */
	intcsrc = strtol(csrc_nr_t, (char **)NULL, 10);
	if ( (intcsrc > 15) || (intcsrc < 0) ) {
		//printf("Error: RTP csrc count field\n");
		error("Error: RTP csrc count field");
		return;
	}

	/* there can be rubbish in this field */
        if (check_digit(csrc_nr_t, strlen(csrc_nr_t), "Error: rtp csrc value") == -1)
                                return;
	
	/* first byte is version + padding + extension + csrc */
	tmp[ij] = c4((intversion*4 + intpadding*2 + intextension)); ij++;
 	tmp[ij] = c4(intcsrc); ij++;

	/* next byte */	
	if (GTK_TOGGLE_BUTTON(marker_true)->active) 
		intmarker = 1;
	else
		intmarker = 0;
	
	inttype = strtol(payload_type_t, (char **)NULL, 10);
	if ( (inttype > 127) || (inttype < 0) ) {
		//printf("Error: RTP payload type number\n");
		error("Error: RTP payload type number");
		return;
	}

	/* there can be rubbish in this field */
        if (check_digit(payload_type_t, strlen(payload_type_t),
                                                "Error: rtp payload type value") == -1)
                                return;

	snprintf(&(tmp[ij]), 3, "%02x", (intmarker*128) + inttype);
	ij = ij + 2;
				
	/* next 2 bytes sequence number */	
	intseq = strtoul(seq_nr_t, (char **)NULL, 10);

	/* there can be rubbish in this field */
        if (check_digit(seq_nr_t, strlen(seq_nr_t), "Error: rtp sequence number value") == -1)
                                return;

	if ( atol(seq_nr_t) > 65535) {
                //printf("Error: rtp sequence number value\n");
                error("Error: rtp sequence number value");
                return ;
        }

	snprintf(&(tmp[ij]), 5, "%04x", intseq);
	ij = ij + 4;

	/* next 4 bytes timestamp */
	inttimestamp = strtoul(timestamp_t, (char **)NULL, 10);

	/* there can be rubbish in this field */
        if (check_digit(timestamp_t, strlen(timestamp_t), "Error: rtp timestamp value") == -1)
                                return;

	if ( atoll(timestamp_t) > 0xFFFFFFFF) {
		//printf("Error: rtp timestamp value\n");
		error("Error: rtp timestamp value");
		return ;
	}

	snprintf(&(tmp[ij]), 9, "%08x", inttimestamp);
	ij = ij + 8;

	/* next 4 bytes ssrc */
	intssrc = strtoul(ssrc_t, (char **)NULL, 10);

	/* there can be rubbish in this field */
        if (check_digit(ssrc_t, strlen(ssrc_t), "Error: rtp ssrc value") == -1)
                                return;

	if ( atoll(timestamp_t) > 0xFFFFFFFF) {
                //printf("Error: rtp ssrc value\n");
                error("Error: rtp ssrc value");
                return ;
        }

	snprintf(&(tmp[ij]), 9, "%08x", intssrc);
	ij = ij + 8;

	/* csrc identifiers */
	if ( (strlen(csrc_t)%8) != 0) {
		//printf("Error: rtp csrc identifiers field (length mod 8 should equal 0)\n");
		error("Error: rtp csrc identifiers field      \n(length mod 8 should equal 0)");
		return;
	}

	/* there can be rubbish in this field */
	if ( (strlen(csrc_t) == 0))
		;
        else if (check_hex(csrc_t, strlen(csrc_t), "Error: rtp crsc identifiers field") == -1)
                                return;

	strncpy(&tmp[ij], csrc_t, strlen(csrc_t));
	ij = ij + strlen(csrc_t);
	
	/* extension field */
	if ( (strlen(extension_t)%8) != 0) {
		//printf("Error: RTP extension field (length mod 8 should equal 0)\n");
		error("Error: rtp extension value      \n(length mod 8 should equal 0)");
		return;
	}

	/* there can be rubbish in this field */
	if ( (strlen(extension_t) == 0) )
		;
	else if (check_hex(extension_t, strlen(extension_t), "Error: rtp extension value") == -1)
                                return;

	strncpy(&tmp[ij], extension_t, strlen(extension_t));
	ij = ij + strlen(extension_t);
	
	if ( (strlen(rtp_payload_t)> 1460) || (strlen(rtp_payload_t)%2 != 0) ) {
		//printf("Error: RTP payload length\n");
		error("Error: RTP payload length");
		return;
	}
	strncpy(&tmp[ij], rtp_payload_t, strlen(rtp_payload_t));
	ij = ij + strlen(rtp_payload_t);
	
	/* copy data to tmp2 field */ 
	for (i=0, j=1, sum = 0; (sum < ij ); sum++, i++, j++) {
		tmp2[i] = tmp[sum]; i++; sum++;
		tmp2[i] = tmp[sum]; i++; 
		if ((j % 16) == 0 && (j > 1)) {
			tmp2[i]='\n';
			j = 0;
		}
		else
			tmp2[i] = ' '; 
	}
	tmp2[i] = '\0';
	
	/* get access to buffer of the text field */
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry_field_udp));

	/* insert the text in the text field */
	gtk_text_buffer_set_text(buffer,tmp2,-1);
	
	gtk_grab_remove(gtk_widget_get_toplevel(GTK_WIDGET(button)));
        gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));

}