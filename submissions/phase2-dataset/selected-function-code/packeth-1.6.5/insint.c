void insint(GtkButton *button, char *entry, char *from, int length) {

	GtkWidget *widg;
	char tmp[11];
	unsigned long value = 0;
	int i;
	unsigned char x = 0;

	widg = lookup_widget(GTK_WIDGET(button), entry);

	for (i = 0; i < length; i++) {
		if ( (*from >= '0') && (*from <= '9')) 
			x = ((*from) - 48);
		else if ((*from >= 'A') && (*from <= 'F')) 
			x = ((*from) - 55);
		else if ((*from >= 'a') && (*from <= 'f')) 
			x = ((*from) - 87);
		
		value = value + ((int)x) * ((unsigned long)1 << (4*(length-1-i)) );
		from++;
	}

	ptrf = ptrf + length;

	snprintf(tmp, 11, "%lu", value);
	gtk_entry_set_text(GTK_ENTRY(widg), tmp);
} 