char *find_regex(char * string, char * regex)
{
	int err;
	regex_t preg;
	err = regcomp(&preg, regex, REG_EXTENDED);

	if (err == 0) {
		int match;
		size_t nmatch = 0;
		regmatch_t *pmatch = NULL;
		nmatch = preg.re_nsub;
		pmatch = malloc(sizeof(*pmatch) * nmatch);

		if (pmatch) {
			match = regexec(&preg, string, nmatch, pmatch, 0);
			regfree(&preg);

			if (match == 0) {
				char *result = NULL;
				int start = pmatch[0].rm_so;
				int end = pmatch[0].rm_eo;
				size_t size = end - start;
				result = malloc (sizeof(*result) * (size + 1));

				if (result) {
					strncpy(result, &string[start], size);
					result[size] = '\0';
					return result;
				}
			}
			else return NULL;
		}
	}
	return NULL;
}
