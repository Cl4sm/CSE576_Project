selectent(entry)
	const struct dirent	*entry;
{
	if (*(entry->d_name) == '.')
		return (0);
	if (my_strnicmp((u_char *)entry->d_name, this_arg, my_strlen(this_arg)))
		return (0);
	else
	{
		int len = my_strlen(entry->d_name);
#ifdef ZCAT
		const char *temp;
	/*
	 * Handle major length of filename is case of suffix .Z:
	 * stripping suffix length
	 */
		temp = &(entry->d_name[len - my_strlen(ZSUFFIX)]);
		if (!my_strcmp(temp, ZSUFFIX))
			len -= my_strlen(ZSUFFIX);
#endif /* ZCAT */
		entry_size = (len > entry_size) ? len : entry_size;
		return (1);
	}
}
