static char
input_do_check_prompt(int update)
{
	u_char	*prompt;
	
	char changed = 0;

	if (current_screen->promptlist)
		prompt = current_screen->promptlist->prompt;
	else
		prompt = input_prompt;
	if (prompt)
	{
		if (update != NO_UPDATE)
		{
			int free_it = 1;
			unsigned len;
			u_char	*ptr;
			int	args_used;	/* this isn't used here but is
						 * passed to expand_alias()
						 */
			if (is_process(get_target_by_refnum(0)))
			{
				ptr = get_prompt_by_refnum(0);
				free_it = 0;
			}
			else
				ptr = expand_alias((u_char *) 0, prompt, empty_string, &args_used, NULL);

			len = my_strlen(ptr);
			if (my_strncmp(ptr, current_screen->inputdata.buffer.buf, len) || !len)
			{
				input_do_replace_prompt(ptr);
				changed = 1;
			}
			if (free_it)
				new_free(&ptr);
		}
	}
	return changed;
}
