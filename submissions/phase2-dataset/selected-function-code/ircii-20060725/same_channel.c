static	int
same_channel(ChannelList *chan, u_char	*channel)
{
	size_t	len, len2;

	/* take the easy way out */
	if (*chan->channel != '!' && *channel != '!')
		return (!my_stricmp(chan->channel, channel));

	/*
	 * OK, so what we have is chan->channel = "!!foo" and channel = "!JUNKfoo".
	 */
	len = my_strlen(chan->channel);	
	len2 = my_strlen(channel);	

	/* bail out on known stuff */
	if (len > len2)
		return (0);
	if (len == len2)
		return (!my_stricmp(chan->channel, channel));

	/*
	 * replace the channel name if we are the same!
	 */
	if (!my_stricmp(chan->channel + 2, channel + 2 + (len2 - len)))
	{
		malloc_strcpy(&chan->channel, channel);
		return 1;
	}
	return 0;
}
