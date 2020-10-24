int
parse_keysym(char *str, char *arg)
{
    char           *key_string;
    int             n, sym;

    if (arg == NULL) {
	if ((n = Str_match(str, "keysym.")) == 0)
	    return 0;
	str += n;		/* skip `keysym.' */
    }
/* some scanf() have trouble with a 0x prefix */
    if (isdigit(str[0])) {
	if (str[0] == '0' && toupper(str[1]) == 'X')
	    str += 2;
	if (arg) {
	    if (sscanf(str, (strchr(str, ':') ? "%x:" : "%x"), &sym) != 1)
		return -1;
	} else {
	    if (sscanf(str, "%x:", &sym) != 1)
		return -1;

	/* cue to ':', it's there since sscanf() worked */
	    str = strchr(str, ':');
	    str++;
	    arg = Str_trim(str);
	    if (arg == NULL)
		return -1;
	}
    } else {
    /*
     * convert keysym name to keysym number
     */
	if (arg == NULL) {
	    arg = str;

	    arg = strchr(str, ':');
	    if (arg == NULL)
		return -1;

	    *arg++ = '\0';
	    arg = Str_trim(arg);
	    if (arg == NULL)
		return -1;
	}
	sym = XStringToKeysym(str);

	if (sym == None)
	    return -1;
    }

    if (sym < 0xFF00 || sym > 0xFFFF)	/* we only do extended keys */
	return -1;
    sym -= 0xFF00;

    if (KeySym_map[sym] != NULL)	/* already set ? */
	return -1;

    if ((n = strlen(arg)) == 0)
	return -1;

    key_string = MALLOC((n + 2) * sizeof(char));

    STRCPY(key_string + 1, arg);

    n = Str_escaped(key_string + 1);
    if (n) {
	key_string[0] = min(n, 255);
	KeySym_map[sym] = (unsigned char *) key_string;
    } else {
	FREE(key_string);
	return -1;
    }

    return 1;
}
