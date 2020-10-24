do_hook(int which, char *format, ...)
{
	va_list vl;
	Hook	*tmp, **list;
	u_char	*name = NULL;
	int	RetVal = 1;
	unsigned int	display;
	int	i,
		old_in_on_who;
	Hook	*hook_array[2048];
	int	hook_num = 0;
	static	int hook_level = 0;
	size_t	len;
	u_char	*foo;
	PUTBUF_INIT
#ifdef NEED_PUTBUF_DECLARED
	/* make this buffer *much* bigger than needed */
	u_char	putbuf[2*BIG_BUFFER_SIZE];

	putbuf[0] ='\0';
#endif

	hook_level++;

	va_start(vl, format);
	PUTBUF_SPRINTF(format, vl);
	va_end(vl);
	if (which < 0)
	{
		NumericList *hook;
		u_char	buf[4];

		snprintf(CP(buf), sizeof buf, "%3.3u", -which);
		if ((hook = (NumericList *) find_in_list((List **)(void *)&numeric_list, buf, 0))
				!= NULL)
		{
			name = hook->name;
			list = &hook->list;
		}
		else
			list = (Hook **) 0;
	}
	else
	{
		if (hook_functions[which].mark && (hook_functions[which].flags & HF_NORECURSE))
			list = (Hook **) 0;
		else
		{
			list = &(hook_functions[which].list);
			name = hook_functions[which].name;
		}
	}
	if (!list)
	{
		RetVal = 1;
		goto out;
	}

	if (which >= 0)
		hook_functions[which].mark++;
			/* not attached, so dont "fix" it */
	{
		int currser = 0, oldser = 0;
		int currmatch = 0, oldmatch = 0;
		Hook *bestmatch = (Hook *) 0;
		int nomorethisserial = 0;

		for (tmp = *list;tmp;tmp = tmp->next)
		{
			currser = tmp->sernum;
			if (currser != oldser)      /* new serial number */
			{
                        	oldser = currser;
				currmatch = oldmatch = nomorethisserial = 0;
				if (bestmatch)
					hook_array[hook_num++] = bestmatch;
				bestmatch = (Hook *) 0;
			}

			if (nomorethisserial) 
				continue;
					/* if there is a specific server
			   		hook and it doesnt match, then
			   		we make sure nothing from
			   		this serial number gets hooked */
			if ((tmp->server != -1) &&
		   	    (tmp->server & HS_NOGENERIC) &&
		   	    (tmp->server != (from_server & HS_NOGENERIC)))
			{
				nomorethisserial = 1;
				bestmatch = (Hook *) 0;
				continue;
			}
			currmatch = wild_match(tmp->nick, putbuf);
			if (currmatch > oldmatch)
			{
				oldmatch = currmatch;
				bestmatch = tmp;
			}
		}
		if (bestmatch)
			hook_array[hook_num++] = bestmatch;
	}

	for (i = 0; i < hook_num; i++)
	{
		tmp = hook_array[i];
		if (!tmp)
		{
			if (which >= 0)
				hook_functions[which].mark--;
			goto out;
		}
		if (tmp->not)
			continue;
		send_text_flag = which;
		if (tmp->noisy > QUIET)
			say("%s activated by \"%s\"", name, putbuf);
		display = window_display;
		if (tmp->noisy < NOISY)
			window_display = 0;

		save_message_from();
		old_in_on_who = in_on_who;
		if (which == WHO_LIST || (which <= -311 && which >= -318))
			in_on_who = 1;
		len = my_strlen(tmp->stuff) + 1; 
		foo = new_malloc(len);
		bcopy(tmp->stuff, foo, len);
		parse_line((u_char *) 0, foo, putbuf, 0, 0, 1);
		new_free(&foo);
		in_on_who = old_in_on_who;
		window_display = display;
		send_text_flag = -1;
		restore_message_from();
		if (!tmp->noisy && !tmp->sernum)
			RetVal = 0;
	}
	if (which >= 0)
		hook_functions[which].mark--;
out:
	PUTBUF_END
	return really_free(--hook_level), RetVal;
}
