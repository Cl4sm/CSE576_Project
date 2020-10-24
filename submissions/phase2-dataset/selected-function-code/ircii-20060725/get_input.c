get_input()
{
	iconv_const char *source = (iconv_const char*)get_input_raw();
	/* The input buffer is UTF-8 encoded. Clients will want irc_encoding instead. */
	static u_char converted_buffer[INPUT_BUFFER_SIZE];

#ifdef HAVE_ICONV_OPEN
	iconv_t conv = iconv_open(CP(irc_encoding), "UTF-8");
	char* dest = (char *)converted_buffer;
	size_t left, space;

	left = my_strlen(source);
	space  = sizeof(converted_buffer);
	while (*source != '\0')
	{
		size_t retval;

		retval = iconv(conv,
		                &source, &left,
		                &dest, &space);
		if (retval == (size_t)-1)
		{
			if (errno == E2BIG)
				break;
			if (errno == EILSEQ)
			{
				/* Ignore silently 1 illegal byte */
				if (left > 0)
				{
					++source;
					--left;
				}
			}
			if (errno == EINVAL)
			{
				/* Input terminated with a partial byte. */
				/* Ignore the error silently. */
				break;
			}
		}
	}
	/* Reset the converter, create a reset-sequence */
	iconv(conv, NULL, &left, &dest, &space);
	
	/* Ensure null-terminators are where they should be */
	converted_buffer[sizeof(converted_buffer)-1] = '\0';
	*dest = '\0';
	
	iconv_close(conv);
#else
	/* Must convert manually - assume output is ISO-8859-1 */
	unsigned dest = 0;

	while (*source != '\0')
	{
		unsigned len = calc_unival_length(source);
		unsigned unival;

		if (!len)
		{
			/* ignore illegal byte (shouldn't happen) */
			++source;
			continue;
		}
		unival = calc_unival(source);
		if (displayable_unival(unival, NULL))
		{
			converted_buffer[dest++] = unival;
			if (dest+1 >= sizeof(converted_buffer))
				break;
		}
		source += len;
	}
	converted_buffer[dest] = '\0';
#endif /* HAVE_ICONV_OPEN */
	
	return converted_buffer;
}
