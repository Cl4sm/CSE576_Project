void
edit_char(ikey)
	u_int ikey;
{
	void	(*func)(u_int, u_char *);
	u_char	*str;
	u_char	extended_key, key = (u_char)ikey;
	WaitPrompt *oldprompt;

	if (current_screen->promptlist &&
			current_screen->promptlist->type == WAIT_PROMPT_KEY)
	{
		oldprompt = current_screen->promptlist;
		(*oldprompt->func)(oldprompt->data, (u_char *)&key);
		set_input(empty_string);
		current_screen->promptlist = oldprompt->next;
		new_free(&oldprompt->data);
		new_free(&oldprompt->prompt);
		new_free(&oldprompt);
		change_input_prompt(-1);
		return;
	}
	if (!get_int_var(EIGHT_BIT_CHARACTERS_VAR))
		key &= 0x7F;			/* strip to 7-bitness */

	extended_key = key;

	if (current_screen->meta1_hit)
	{
		func = key_names[meta1_keys[key].index].func;
		str = meta1_keys[key].stuff;
		current_screen->meta1_hit = 0;
	}
	else if (current_screen->meta2_hit)
	{
		func = key_names[meta2_keys[key].index].func;
		str = meta2_keys[key].stuff;
		current_screen->meta2_hit = 0;
	}
	else if (current_screen->meta3_hit)
	{
		func = key_names[meta3_keys[key].index].func;
		str = meta3_keys[key].stuff;
		current_screen->meta3_hit = 0;
	}
	else if (current_screen->meta4_hit)
	{
		func = key_names[meta4_keys[key].index].func;
		str = meta4_keys[key].stuff;
	}
	else if (current_screen->meta5_hit)
	{
		func = key_names[meta5_keys[key].index].func;
		str = meta5_keys[key].stuff;
		current_screen->meta5_hit = 0;
	}
	else if (current_screen->meta6_hit)
	{
		func = key_names[meta6_keys[key].index].func;
		str = meta6_keys[key].stuff;
		current_screen->meta6_hit = 0;
	}
	else if (current_screen->meta7_hit)
	{
		func = key_names[meta7_keys[key].index].func;
		str = meta7_keys[key].stuff;
		current_screen->meta7_hit = 0;
	}
	else if (current_screen->meta8_hit)
	{
		func = key_names[meta8_keys[key].index].func;
		str = meta8_keys[key].stuff;
		current_screen->meta8_hit = 0;
	}
	else
	{
		func = key_names[keys[key].index].func;
		str = keys[key].stuff;
	}
	if (!current_screen->meta1_hit && !current_screen->meta2_hit &&
	    !current_screen->meta3_hit && !current_screen->meta5_hit &&
	    !current_screen->meta6_hit && !current_screen->meta7_hit &&
	    !current_screen->meta8_hit)
	{
		if (current_screen->inside_menu == 1)
			menu_key((u_int)key);
		else if (current_screen->digraph_hit)
		{
			if (extended_key == 0x08 || extended_key == 0x7f)
				current_screen->digraph_hit = 0;
			else if (current_screen->digraph_hit == 1)
			{
				current_screen->digraph_first = extended_key;
				current_screen->digraph_hit = 2;
			}
			else if (current_screen->digraph_hit == 2)
			{
				if ((extended_key =
				    get_digraph((u_int)extended_key)) != '\0')
					input_add_character((u_int)extended_key,
					    (u_char *) 0);
				else
					term_beep();
			}
		}
		else if (current_screen->quote_hit)
		{
			current_screen->quote_hit = 0;
			input_add_character((u_int)extended_key, (u_char *) 0);
		}
		else if (func)
			func(extended_key, str ? str : empty_string);
	}
	else
		term_beep();
}
