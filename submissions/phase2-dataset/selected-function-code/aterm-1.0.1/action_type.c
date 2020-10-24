int
action_type(action_t * action, unsigned char *str)
{
#ifdef MENUBAR
    unsigned int    len;

#if defined (DEBUG_MENU) || defined (DEBUG_MENUARROWS)
    len = strlen(str);
    printf("(len %d) = %s\n", len, str);
#else
    len = Str_escaped((char *) str);
#endif

    if (!len)
	return -1;

/* sort command vs. terminal actions */
    action->type = MenuAction;
    if (str[0] == '\0') {
    /* the functional equivalent: memmove (str, str+1, len); */
	unsigned char  *dst = (str);
	unsigned char  *src = (str + 1);
	unsigned char  *end = (str + len);

	while (src <= end)
	    *dst++ = *src++;

	len--;			/* decrement length */
	if (str[0] != '\0')
	    action->type = MenuTerminalAction;
    }
    action->str = str;
    action->len = len;
#endif

    return 0;
}
