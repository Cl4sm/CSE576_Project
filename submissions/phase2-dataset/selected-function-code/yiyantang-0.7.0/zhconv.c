{
	int fromcode, tocode;
	static int lastfrom = 0;	/* From code from the last iteration. */
	fromcode = curconfig.fromcode;
	tocode = curconfig.tocode;
	

	if (fromcode == -2 || tocode == fromcode)
	{
		return *count;
	}
	if (fromcode == 0)
	{
		if ((fromcode = j_code (buf, *count)) == tocode)
			return *count;
	}

	/* hz_search will reset half-hanzi everytime. 
	 * We don't want that at the buffer boundary if encoding
	 * is not changed.
	 */
	if(!(curconfig.isnewbuf == TRUE && fromcode == lastfrom))
	{
		lastfrom = fromcode;
		if(! hz_search(fromcode, tocode, 8))
		{
			/* failed to find a valid conversion path. */
			return *count;
		}
	}
	
	hz_convert(buf, count, 0);
	return *count;

}
