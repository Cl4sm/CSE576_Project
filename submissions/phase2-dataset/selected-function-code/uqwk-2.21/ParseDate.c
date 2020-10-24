char *c;
struct qwk_hdr *hp;
{
	char s[PATH_LEN];
	int day, mon, year, hour, minute;
	char month[4];

	/* Skip white space */
	while ( (*c == ' ') || (*c == 9) ) c++;

	/* Dates come in two flavors:  with the weekday, and without.
	   we simply look for the comma which follows the weekday */
	if (c[3] == ',') {
	        sscanf (&c[4], "%d %s %d %d:%d", &day, month, &year,
	                                        &hour, &minute);
	} else {
	        sscanf (c, "%d %s %d %d:%d", &day, month, &year,
	                                        &hour, &minute);
	}

	/* Convert alphabetic month name to integer */
	mon = 0;
	if (!strncmp (month, "Jan", 3)) mon = 1;
	if (!strncmp (month, "Feb", 3)) mon = 2;
	if (!strncmp (month, "Mar", 3)) mon = 3;
	if (!strncmp (month, "Apr", 3)) mon = 4;
	if (!strncmp (month, "May", 3)) mon = 5;
	if (!strncmp (month, "Jun", 3)) mon = 6;
	if (!strncmp (month, "Jul", 3)) mon = 7;
	if (!strncmp (month, "Aug", 3)) mon = 8;
	if (!strncmp (month, "Sep", 3)) mon = 9;
	if (!strncmp (month, "Oct", 3)) mon = 10;
	if (!strncmp (month, "Nov", 3)) mon = 11;
	if (!strncmp (month, "Dec", 3)) mon = 12;

	/* Convert date */
	sprintf (s, "%02d-%02d-%02d", mon, day, year%100);
	PadString (s, hp->date, 8);

	/* Time */
	sprintf (s, "%02d:%02d", hour, minute);
	PadString (s, hp->time, 5);
}
