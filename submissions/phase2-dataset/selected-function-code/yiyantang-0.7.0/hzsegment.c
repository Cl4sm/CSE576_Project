int hzsegment (unsigned char *buf, int count)
{
	unsigned char *pbuf;
	int i;

	pbuf = buf;
	i = 0;

	/* Ascii 0-127 and not 2nd half big5 */
	if(testascii(pbuf[0]) && 
			(!(ishz2(pbuf[0]) && ishz1(pbuf[1])))) {
		for (i = 0; i < count; i++)
		{
			if (ishz1 (pbuf[i]))
				return i;
		}

	}
	/* Double byte segment. */
	else
	{
		for (i = 1; i < count; i++)
		{
			/* Don't let printable ascii break double byte 
			 * sequence (but we don't want to swallow newline.)
			 * Since for curses program, non-printable chars
			 * are used to move the cursor around.
			 * Then we can have longer hanzi string for more 
			 * accurate encode judgement. 
			 *
			 * Old GB locale might be a bit broken for isgraph(), :).
			 * Update your locale. */
			if(testascii(pbuf[i]) && 
					(!isgraph(pbuf[i])) &&
					(pbuf[i] != 0x20)) /* space is already isgraph? */
			{
				if (!(ishz2 (pbuf[i]) && ishz1 (pbuf[i-1])))
				{
					return i;
				} /* else keep going. */
			}
		} /* for double byte */
	}
	return i;
}
