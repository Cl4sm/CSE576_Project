int load_gen_s_data(GtkButton *button, FILE *file_p) {

	long int buff4[5];
        char buff[100];
        char buffc[11][200];
	int c, j, k;
	char *ptr = NULL, *ptr2 = NULL;
        long int cc;


	k = 0;
	j = 0;
	/* rules for config files:
	 * - comments start with #
	 * - there can be spaces and newlines
	 * - only digits and - are acceptable characters
	 * - ...
	 */
	while ( (c = fgetc( file_p )) != EOF ) {
		/* all the comment lines, starting with # */
		if ( (j==0) && (c == 35)) {
			while ( getc(file_p) != 10);
			continue;
		}
		/* all blank lines */
		if ( (j==0) && (c == 10))
			continue;
		/* read the whole lines */
		if ((isascii(c) != 0) && (j<200) && (c!=10) && (k<11)) {
			buffc[k][j] = c;
			j++;
			buffc[k][j] = '\0';
		}
		/* , or \n mean end of string */
		else if (c==10) {
			j=0;
			k++;
		}
		else {
			return -1;
		     }
	}

	w1 = lookup_widget(GTK_WIDGET (button), "radiobutton36");
	w2 = lookup_widget(GTK_WIDGET (button), "radiobutton37");
	w4 = lookup_widget(GTK_WIDGET (button), "entry151");
	w5 = lookup_widget(GTK_WIDGET (button), "entry152");
	w6 = lookup_widget(GTK_WIDGET(button), "checkbutton36");

	/* first line should have three parameters */
	/* first is absolute or relative delay, allowed values 0 and 1 */
	if (strncmp(&buffc[0][0], "1", 1) == 0)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w1), 1);
	else if (strncmp(&buffc[0][0], "0", 1) == 0)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w2), 1);
	else {
		return -1;
	}

	/* second is number of packets: -3 means infinite, or 1 till 9999999) */
	if (strncmp(&buffc[0][2], "-3", 2) == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w6), 1);
		gtk_entry_set_text(GTK_ENTRY(w4), "");
		gtk_widget_set_sensitive (w4, FALSE);
		ptr = &buffc[0][4];
	}
	else {
		if ( (ptr = strchr(&buffc[0][2], 44)) == NULL) {
			return -1;
	 	}
		*ptr = '\0';
		buff4[0] = strtol(&buffc[0][2], (char **)NULL, 10);

		if ( (buff4[0] >= 0) && (buff4[0] <= 9999999) ) {
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w6), 0);
			gtk_widget_set_sensitive (w4, TRUE);
			gtk_entry_set_text(GTK_ENTRY(w4), &buffc[0][2]);
		}
		else {
			return -1;
		}
	}

	/* last parameter is delay between sequences */
	buff4[0] = strtol(ptr+1, (char **)NULL, 10);

	if ( (buff4[0] >= 0) && (buff4[0] <= 999999999) ) {
		gtk_entry_set_text(GTK_ENTRY(w5), ptr+1);
	}
	else {
		return -1;
	}

	/* we have to clean everything */
	for (j = 0; j < 10; j++) {
		snprintf(buff, 10, "entry%d", 111+j);
		w2 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w2), "");
		snprintf(buff, 100, "entry%d", 121+j);
		w3 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w3), "");
		snprintf(buff, 100, "entry%d", 131+j);
		w3 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w3), "");
		snprintf(buff, 100, "entry%d", 141+j);
		w3 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w3), "");
		snprintf(buff, 100, "checkbutton%d", 25+j);
		w2 = lookup_widget(GTK_WIDGET(button), buff);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w2), 0);
	}

	/* and now all the rest */
	for (j = 1; j < k; j++) {
		/* first is packet name */
		if ( (ptr2 = strchr(&buffc[j][0], 44)) == NULL)
			continue;
		*ptr2 = '\0';
		if ( (strlen(&buffc[j][0]) > 0 ) && (strlen(&buffc[j][0]) < 70) ) {
			snprintf(buff, 10, "entry%d", 110+j);
			w2 = lookup_widget(GTK_WIDGET (button), buff);
			gtk_entry_set_text(GTK_ENTRY(w2), &buffc[j][0]);
		}
		else {
			return -1;
		}
		/* number of packets */
		ptr = ptr2; ptr++;
		if ( (ptr2 = strchr(ptr, 44)) == NULL) {
			return -1;
		}
		*ptr2 = '\0';
		cc = strtol(ptr, (char **)NULL, 10);
		if ( (cc < 0) || (cc > 9999999) ) {
			return -1;
		}
		snprintf(buff, 100, "entry%d", 120+j);
		w3 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w3), ptr);

		/* delay between packets */
		ptr = ptr2; ptr++;
		if ( (ptr2 = strchr(ptr, 44)) == NULL) {
			return -1;
		}
		*ptr2 = '\0';
		cc = strtol(ptr, (char **)NULL, 10);
		if ( (cc < 0) || (cc > 999999999) ) {
			return -1;
		}
		snprintf(buff, 100, "entry%d", 130+j);
		w3 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w3), ptr);

		/* delay to next */
		ptr = ptr2; ptr++;
		if ( (ptr2 = strchr(ptr, 44)) == NULL) {
			return -1;
		}
		*ptr2 = '\0';
		cc = strtol(ptr, (char **)NULL, 10);
		if ( (cc < 0) || (cc > 999999999) ) {
			return -1;
		}
		snprintf(buff, 100, "entry%d", 140+j);
		w3 = lookup_widget(GTK_WIDGET (button), buff);
		gtk_entry_set_text(GTK_ENTRY(w3), ptr);

		/* enable or disable */
		ptr = ptr2; ptr++;
		snprintf(buff, 100, "checkbutton%d", 24+j);
		w2 = lookup_widget(GTK_WIDGET(button), buff);
		if (*ptr == '1')
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w2), 0);
		else if (*ptr == '0')
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w2), 1);
		else {
		      return -1;
		}

	}

	return 1;

}