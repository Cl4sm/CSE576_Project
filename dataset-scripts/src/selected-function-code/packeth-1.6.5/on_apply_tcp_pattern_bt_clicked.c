void
on_apply_tcp_pattern_bt_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget *en1, *en2, *text_e;
	long length;
	gchar *en1_t, *en2_t;
	char tmp[4600], ch1, ch2;
	guint i, j;
	
        en1 = lookup_widget(GTK_WIDGET(button), "entry107");
        en2 = lookup_widget(GTK_WIDGET(button), "entry108");
	text_e = lookup_widget(GTK_WIDGET (button), "text4");
	en1_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));
	en2_t = (char *)gtk_entry_get_text(GTK_ENTRY(en2));

	length = strtol(en2_t, (char **)NULL, 10);
	if ( (length > 1460) || (length <= 0) ) {
		//printf("Error: tcp pattern length error\n");
		error("Error: tcp pattern length error");
		return;
	}

	/* we chech the pattern */
        if (char2x(en1_t) == -1) {
                //printf("Error: tcp pattern field\n");
                error("Error: tcp pattern field");
                return;
        }

	/* get access to buffer of the text field */
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_e));

	/* apply the new pattern */
	ch1 = *en1_t;
	en1_t++;
	ch2 = *en1_t;

	/* copy data to tmp field */
	for (i=0, j=1; (i < (length*3) ); i++, j++) {
		tmp[i] = ch1; i++;
		tmp[i] = ch2; i++;
		/* we allow only 16 bytes in each row - looks nicer */
		if ((j % 16) == 0 && (j > 1)) {
			tmp[i]='\n';
			j = 0;
		}
		else
			tmp[i] = ' '; 
	}
	tmp[i] = '\0';
	
	/* insert the text in the text field */
	gtk_text_buffer_set_text(buffer,tmp,-1);	

}