static int encode_percent_hack(const char *name, char **key, unsigned int use_class)
{
	unsigned int len = 0;

	if (!key)
		return -1;

	if (use_class)
		len = get_encoded_len_escaping_sequences(name);
	else
		len = get_encoded_len_escaping_every_cap(name);

	/* If there is no escaping to be done, return 0 */
	if (len == strlen(name))
		return 0;

	*key = malloc(len + 1);
	if (!*key)
		return -1;

	if (use_class)
		encode_sequence(name, *key, len);
	else
		encode_individual(name, *key, len);

	if (strlen(*key) != len)
		crit(LOGOPT_ANY, MODPREFIX "encoded key length mismatch: key "
		     "%s len %d strlen %d", *key, len, strlen(*key));

	return strlen(*key);
}