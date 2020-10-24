int base64_encode(char *source, size_t sourcelen, char *target, size_t targetlen)
{
	/* check if the result will fit in the target buffer */
	if ((sourcelen + 2)/ 3*4 > targetlen - 1)
		return 0;

	/* encode all full triples */
	while (sourcelen >= 3) {
		_base64_encode_triple(source, target);
		sourcelen -= 3;
		source += 3;
		target += 4;
	}

	/* encode the last one or two characters */
	if (sourcelen > 0) {
		unsigned char temp[3];
		memset(temp, 0, sizeof(temp));
		memcpy(temp, source, sourcelen);
		_base64_encode_triple(temp, target);
		target[3] = '=';
		if (sourcelen == 1)
			target[2] = '=';
		target += 4;
	}

	/* terminate the string */
	target[0] = 0;

	return 1;
}