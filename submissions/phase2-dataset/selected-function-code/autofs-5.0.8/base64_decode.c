size_t base64_decode(char *source, char *target, size_t targetlen)
{
	char *src, *tmpptr;
	char quadruple[4], tmpresult[3];
	int i, tmplen = 3;
	size_t converted = 0;

	/* concatinate '===' to the source to handle unpadded base64 data */
	src = malloc(strlen(source)+5);
	if (src == NULL)
		return -1;
	strcpy(src, source);
	strcat(src, "====");
	tmpptr = src;

	memset(target, 0, targetlen);

	/* convert as long as we get a full result */
	while (tmplen == 3) {
		/* get 4 characters to convert */
		for (i = 0; i < 4; i++) {
			/* skip invalid characters - we won't reach the end */
			while (*tmpptr != '=' && _base64_char_value(*tmpptr) < 0)
				tmpptr++;
			quadruple[i] = *(tmpptr++);
		}

		/* convert the characters */
		tmplen = _base64_decode_triple(quadruple, tmpresult);

		/* check if the fit in the result buffer */
		if (targetlen < tmplen) {
			free(src);
			return -1;
		}

		/* put the partial result in the result buffer */
		memcpy(target, tmpresult, tmplen);
		target += tmplen;
		targetlen -= tmplen;
		converted += tmplen;
	}

	free(src);
	return converted;
}