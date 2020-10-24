static const char *tuple_to_str(IN ftree_tuple_t tuple)
{
	static char buffer[FTREE_TUPLE_BUFFERS_NUM][FTREE_TUPLE_BUFF_LEN];
	static uint8_t ind = 0;
	char *ret_buffer;
	uint32_t i;

	if (!tuple_assigned(tuple))
		return "INDEX.NOT.ASSIGNED";

	buffer[ind][0] = '\0';

	for (i = 0; (i < FTREE_TUPLE_LEN) && (tuple[i] != 0xFF); i++) {
		if ((strlen(buffer[ind]) + 10) > FTREE_TUPLE_BUFF_LEN)
			return "INDEX.TOO.LONG";
		if (i != 0)
			strcat(buffer[ind], ".");
		sprintf(&buffer[ind][strlen(buffer[ind])], "%u", tuple[i]);
	}

	ret_buffer = buffer[ind];
	ind = (ind + 1) % FTREE_TUPLE_BUFFERS_NUM;
	return ret_buffer;
}				/* tuple_to_str() */