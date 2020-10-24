void
change_input_prompt(direction)
	int	direction;
{
	if (!current_screen->promptlist)
	{
		/* Restore stuff */
		memcpy(&current_screen->inputdata.saved_buffer,
		       &current_screen->inputdata.buffer,
		       sizeof(current_screen->inputdata.buffer));
	}
	else if (direction == -1)
	{
		/* Just update whatever should be on screen */
	}
	else if (!current_screen->promptlist->next)
	{
		/* Save stuff */
		memcpy(&current_screen->inputdata.buffer,
		       &current_screen->inputdata.saved_buffer,
		       sizeof(current_screen->inputdata.buffer));
		
		/* Erase input line */
		*current_screen->inputdata.buffer.buf = '\0';
		current_screen->inputdata.buffer.pos =
				current_screen->inputdata.buffer.minpos = 0;
	}
	update_input(UPDATE_ALL);
}
