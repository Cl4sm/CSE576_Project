void
on_entry162_changed                    (GtkEditable     *editable,
                                        gpointer         user_data)
{
	GtkWidget *en5, *en6, *text_e;
	gchar *en5_t, *en6_t;
	int length, i, j, m, value, xvalue;
	char tmp[4600];

	en5 = lookup_widget(GTK_WIDGET (editable), "entry162");
        en5_t = (char *)gtk_entry_get_text(GTK_ENTRY(en5));
	length = strlen(en5_t);

	for(i=0; i<length; i++) {
		if (isdigit(*(en5_t+i)) == 0) {
                	error("Error: Wrong byte y entry!");
                	return;
		}
	}
        value =  strtol(en5_t, (char **)NULL, 10);

        if (number < value) {
                error("Error: Wrong byte y offset!");
                return;
        }

	text_e = lookup_widget(GTK_WIDGET (editable), "text5");

	en6 = lookup_widget(GTK_WIDGET (editable), "entry160");
        en6_t = (char *)gtk_entry_get_text(GTK_ENTRY(en6));
	xvalue = strtol(en6_t, (char **)NULL, 10);

	/* copy data to tmp field */
	for (i=0, j=0, m=1; j < number; m++, j++) {
		snprintf(&tmp[i], 4600, "%02x", packet[j]);
		i++; i++;
		/* we allow only 16 bytes in each row - looks nicer */
		if ((m % 16) == 0 && (m > 1)) {
			tmp[i]='\n';
			m = 0;
		}
		else
			tmp[i] = ' '; 
		i++;
	}
	tmp[i] = '\0';

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_e));
	gtk_text_buffer_set_text(buffer,tmp,-1);

	if (value == 0)
		;
	else {
		gtk_text_buffer_set_text(buffer,"??",2);
	}

        if ( (number < xvalue) || (xvalue == 0) ) 
                return;
        else {
		gtk_text_buffer_set_text(buffer,"??",2);
	}

}