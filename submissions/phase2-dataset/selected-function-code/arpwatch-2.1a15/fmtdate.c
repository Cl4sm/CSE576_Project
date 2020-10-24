static char *
fmtdate(time_t t)
{
	register struct tm *tm;
	register int32_t mw;
	register char ch;
	static int init = 0;
	static char zone[32], buf[132];

	if (t == 0)
		return("<no date>");

	if (!init) {
		mw = gmt2local() / 60;
		if (mw < 0) {
			ch = '-';
			mw = -mw;
		} else {
			ch = '+';
		}
		(void)sprintf(zone, "%c%02d%02d", ch, mw / 60, mw % 60);
	}

	tm = localtime(&t);
	(void)sprintf(buf, "%s, %s %d, %d %d:%02d:%02d %s",
	    DOW(tm->tm_wday),
	    MOY(tm->tm_mon),
	    tm->tm_mday,
	    tm->tm_year + 1900,
	    tm->tm_hour,
	    tm->tm_min,
	    tm->tm_sec,
	    zone);
	return(buf);
}