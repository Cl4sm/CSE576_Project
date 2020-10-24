char *midend_rewrite_statusbar(midend *me, char *text)
{
    /*
     * An important special case is that we are occasionally called
     * with our own laststatus, to update the timer.
     */
    if (me->laststatus != text) {
	sfree(me->laststatus);
	me->laststatus = dupstr(text);
    }

    if (me->ourgame->is_timed) {
	char timebuf[100], *ret;
	int min, sec;

	sec = (int)me->elapsed;
	min = sec / 60;
	sec %= 60;
	sprintf(timebuf, "[%d:%02d] ", min, sec);

	ret = snewn(strlen(timebuf) + strlen(text) + 1, char);
	strcpy(ret, timebuf);
	strcat(ret, text);
	return ret;

    } else {
	return dupstr(text);
    }
}