int usedef_insert(GtkButton *button, char *entry, int whocalled) {

	int i, j;
	char tmp[4600];

	if (whocalled == 1)
		return 1;

	/* get access to buffer of the text field */
	w2 = lookup_widget(GTK_WIDGET(button), entry);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w2));

	/* copy data to tmp field */
	for (i=0, j=1; (i < (remain * 3) ); i++, j++) {
		tmp[i] = *ptrf++; i++;
		tmp[i] = *ptrf++; i++;
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

	return 1;


}