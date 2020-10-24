static void header_decode_word(char *word, unistr *ret)
{
	char *my_word;
	char *charset, *encoding, *string, *end;
	char *binary;
	size_t bin_len;


	if ((word[0] != '=') || (word[1] != '?')) {
		unistr_append_usascii(ret, word, strlen(word));
		return;
	}

	my_word = mystrdup(word);

	charset = my_word + 2;

	if ((encoding = strchr(charset, '?')) == NULL) {
		/* missing encoding */
		unistr_append_usascii(ret, "???", 3);
		myfree(my_word);
		return;
	}
	*(encoding++) = '\0';

	if ((string = strchr(encoding, '?')) == NULL) {
		/* missing string */
		unistr_append_usascii(ret, "???", 3);
		myfree(my_word);
		return;
	}
	*(string++) = '\0';

	if ((end = strchr(string, '?')) == NULL) {
		/* missing end */
		unistr_append_usascii(ret, "???", 3);
		myfree(my_word);
		return;
	}
	*(end++) = '\0';
	if ((end[0] != '=') || (end[1] != '\0')) {
		/* broken end */
		unistr_append_usascii(ret, "???", 3);
		myfree(my_word);
		return;
	}

	if (tolower(encoding[0]) == 'q') {
		decode_qp(string, &binary, &bin_len);
	} else if (tolower(encoding[0]) == 'b') {
		decode_base64(string, &binary, &bin_len);
	} else {
		/* unknown encoding */
		unistr_append_usascii(ret, "???", 3);
		myfree(my_word);
		return;
	}

	if (strcasecmp(charset, "us-ascii") == 0) {
		unistr_append_usascii(ret, binary, bin_len);
	} else if (strcasecmp(charset, "utf-8") == 0) {
		unistr_append_utf8(ret, binary, bin_len);
	} else if (strcasecmp(charset, "iso-8859-1") == 0) {
		unistr_append_iso88591(ret, binary, bin_len);
	} else {
		/* unknown charset */
		unistr_append_usascii(ret, "???", 3);
	}

	myfree(my_word);
	myfree(binary);
}