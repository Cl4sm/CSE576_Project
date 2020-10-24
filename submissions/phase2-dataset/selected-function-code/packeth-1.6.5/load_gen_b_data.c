int load_gen_b_data(GtkButton *button, FILE *file_p) {
				
	long int buff4[5];
        char buff[10];
        char buffc[11][200];
	int c, j, k;

	w1 = lookup_widget(GTK_WIDGET (button), "radiobutton35");
	w2 = lookup_widget(GTK_WIDGET (button), "radiobutton34");
	w3 = lookup_widget(GTK_WIDGET (button), "optionmenu9");
	w4 = lookup_widget(GTK_WIDGET (button), "entry109");
	w5 = lookup_widget(GTK_WIDGET (button), "entry110");
	w6 = lookup_widget(GTK_WIDGET(button), "checkbutton35");
	w7 = lookup_widget(GTK_WIDGET(button), "checkbutton37");
	/* we read the file ohh python, where are you... */
	k = 0;
	j = 0;
	/* rules for config files:
	 * - comments start with #
	 * - there can be spaces and newlines
	 * - only digits and - are acceptable characters
	 * - ...
	 */
	/* we have to limit the lines we read paramters from */
	while ( (c = fgetc( file_p )) != EOF ) {
		/* all the comment lines, starting with # , no limit for comment lines*/
		if ( (j==0) && (c == 35)) {
			/* ok, read till the end of line */
			while ( getc(file_p) != 10);
			continue;
		}

		/* let's limit the size */
		if ( (j > 9) || (k > 2) )
			return -1;

		/* ok, it is not a comment line so the info: only digits and minus sign are acceptable */
		if ( (isdigit(c) != 0) || (c == 45) ) { 
			buff[j] = c;
			j++;
			buff[j] = '\0';
		}
		/* no digit is it a newline? */
		else if (c==10) {
			if (j==0)
				continue;
			if (strlen(buff) == 0)
				*buff = 0;
			buff4[k] = strtol(buff, (char **)NULL, 10);
			j = 0;
			strncpy(&buffc[k][j], buff, 9);
			k++;
		}
		/* not, ok this is an error */
		else {
			return -1;
		}
	}

	if (buff4[0] == 1)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w2), 1);
	else if (buff4[0] == 0)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w1), 1);
	else {
		return -1;
	}

	/* adjusting parameters...
	if ( (buff4[1] >= 0) && (buff4[1] <= 4) )
		gtk_option_menu_set_history (GTK_OPTION_MENU (w3), buff4[1]);
	else {
		return -1;
	} */
	
	if (buff4[1] == - 3) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w6), 1);
		gtk_entry_set_text(GTK_ENTRY(w4), "");
		gtk_widget_set_sensitive (w4, FALSE);
	}
	else if ( (buff4[1] > 0) && (buff4[1] <= 9999999) ) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w6), 0);
		gtk_widget_set_sensitive (w4, TRUE);
		gtk_entry_set_text(GTK_ENTRY(w4), &buffc[1][0]);
	}
	else {
		return -1;
	}

	if (buff4[2] == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w7), 1);
		gtk_entry_set_text(GTK_ENTRY(w5), "");
		gtk_widget_set_sensitive (w5, FALSE);
	}
	else if ( (buff4[2] >= 1) && (buff4[2] <= 999999999)  ) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w7), 0);
		gtk_widget_set_sensitive (w5, TRUE);
		gtk_entry_set_text(GTK_ENTRY(w5), &buffc[2][0]);
	}
	else {
		return -1;
	}
	
	return 1;
}