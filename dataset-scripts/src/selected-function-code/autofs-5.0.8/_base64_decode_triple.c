static int _base64_decode_triple(char quadruple[4], char *result)
{
	int i, triple_value, bytes_to_decode = 3, only_equals_yet = 1;
	int char_value[4];

	for (i=0; i<4; i++)
		char_value[i] = _base64_char_value(quadruple[i]);

	/* check if the characters are valid */
	for (i=3; i>=0; i--) {
		if (char_value[i]<0) {
			if (only_equals_yet && quadruple[i]=='=') {
				/* we will ignore this character anyway, make it
				 * something that does not break our calculations */
				char_value[i]=0;
				bytes_to_decode--;
				continue;
			}
			return 0;
		}
		/* after we got a real character, no other '=' are allowed anymore */
		only_equals_yet = 0;
	}

	/* if we got "====" as input, bytes_to_decode is -1 */
	if (bytes_to_decode < 0)
		bytes_to_decode = 0;

	/* make one big value out of the partial values */
	triple_value = char_value[0];
	triple_value *= 64;
	triple_value += char_value[1];
	triple_value *= 64;
	triple_value += char_value[2];
	triple_value *= 64;
	triple_value += char_value[3];

	/* break the big value into bytes */
	for (i=bytes_to_decode; i<3; i++)
		triple_value /= 256;
	for (i=bytes_to_decode-1; i>=0; i--) {
		result[i] = triple_value%256;
		triple_value /= 256;
	}

	return bytes_to_decode;
}