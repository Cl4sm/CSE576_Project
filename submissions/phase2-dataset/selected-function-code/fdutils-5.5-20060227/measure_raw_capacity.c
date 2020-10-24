			int cylinder, int warmup, int verbosity)
{
	int cap, min, ctr;

	if(warmup)
		/* first formatting, so that we have sth to read during the
		 * warmup cycles */
		m_format_track(fd, dn, rate, cylinder);

	m_format_track(fd, dn, rate, cylinder);
	min = cap = read_track(fd, dn, rate, cylinder);
	ctr = 0;
	while(ctr < 10) {
		m_format_track(fd, dn, rate, cylinder);
		cap = read_track(fd, dn, rate, cylinder);
		if(cap < min) {
			min = cap;
			ctr = 0;
		} else
			ctr++;
		if(verbosity)
			fprintf(stderr,"warmup cycle: %3d %6d %6d\r", ctr, 
				cap, min);

	}
	if(verbosity)
		fprintf(stderr,"                                \r");
	return min;
}
