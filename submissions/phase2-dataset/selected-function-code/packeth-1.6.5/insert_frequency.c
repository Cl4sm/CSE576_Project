int insert_frequency(int codec, int frequency, int length, GtkWidget *payload_entry, gint amp_index) 
{
	double  fs = 8000;      /* vzorcna frekvenca */
   	double amp;      /* amplituda */
	double  ph = 0;         /* zacetna faza */
	double delta_ph;
	double sample;       /* 16 bit variable */
	gchar entry_t[2*length+1];
	gchar *ptr;

	ptr = entry_t;
	delta_ph = 2* M_PI *frequency/fs;

	/* the amp values are: low - 5000, mid - 15000, max - 30000 */
	amp = 5000 + amp_index * 7500 + amp_index * amp_index * 2500;

	while(length) {
        	sample = amp*sin(ph);
        	ph = ph + delta_ph;
        	while (ph > (2*M_PI)) {
        	    ph = ph - (2*M_PI);
        	}
	
		if (codec == 1) 
			c8(ptr, linear2alaw((gint16)sample));
		else
			c8(ptr, linear2ulaw((gint16)sample));
		ptr++;
		ptr++;

        	length--;
    	}

	*ptr = '\0';

	gtk_entry_set_text(GTK_ENTRY(payload_entry), entry_t);

	return 1;
}