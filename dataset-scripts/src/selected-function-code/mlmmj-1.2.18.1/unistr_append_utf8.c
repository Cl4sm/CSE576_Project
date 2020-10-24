void unistr_append_utf8(unistr *str, const char *binary, size_t bin_len)
{
	unsigned int i, j;
	unistr_char ch;
	unsigned char *bin = (unsigned char *)binary;

	for (i=0; i<bin_len; i++) {
		if (bin[i] <= 0x7F) {  /* 1 */
			unistr_append_char(str, bin[i]);
		} else {
			if ((bin[i] & 224) == 192) {  /* 2 */
				ch = bin[i] & 31;
				j = 1;
			} else if ((bin[i] & 240) == 224) {  /* 3 */
				ch = bin[i] & 15;
				j = 2;
			} else if ((bin[i] & 248) == 240) {  /* 4 */
				ch = bin[i] & 7;
				j = 3;
			} else if ((bin[i] & 252) == 248) {  /* 5 */
				ch = bin[i] & 3;
				j = 4;
			} else if ((bin[i] & 254) == 252) {  /* 6 */
				ch = bin[i] & 1;
				j = 5;
			} else {
				/* invalid byte sequence */
				unistr_append_char(str, '?');
				continue;
			}
			if (ch == 0) {
				/* invalid encoding, no data bits set in first byte */
				unistr_append_char(str, '?');
				continue;
			}
			for (;j>0; j--) {
				i++;
				ch <<= 6;
				if ((bin[i] & 192) != 128) {
					/* invalid byte sequence */
					ch = '?';
					break;
				}
				ch |= bin[i] & 63;
			}
			unistr_append_char(str, ch);
		}
	}
}