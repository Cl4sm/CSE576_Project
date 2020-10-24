find_key (const char * str)
{
	int i;
	int len, klen;
	struct key * kw = NULL;
	struct key * foundkw = NULL;

	len = strlen(str);

	vector_foreach_slot (keys, kw, i) {
		if (strncmp(kw->str, str, len))
			continue;
		klen = strlen(kw->str);
		if (len == klen)
			return kw; /* exact match */
		if (len < klen) {
			if (!foundkw)
				foundkw = kw; /* shortcut match */
			else
				return NULL; /* ambiguous word */
		}
	}
	return foundkw;
}
