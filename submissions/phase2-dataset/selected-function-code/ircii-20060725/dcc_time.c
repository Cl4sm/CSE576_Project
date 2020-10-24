static	u_char	*
dcc_time(the_time)
	time_t	the_time;
{
	struct	tm	*btime;
	u_char	*buf;
	static	char	*months[] = 
	{
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	btime = localtime(&the_time);
	buf = (u_char *) malloc(22);
	if (snprintf(CP(buf), 22, "%-2.2d:%-2.2d:%-2.2d %s %-2.2d %d",
			btime->tm_hour, btime->tm_min, btime->tm_sec,
			months[btime->tm_mon], btime->tm_mday,
			btime->tm_year + 1900))
		return buf;
	else
		return empty_string;
}
