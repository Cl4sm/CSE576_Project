input_check_resized(void)
{
	ScreenInputData* inputdata = &current_screen->inputdata;

	if (inputdata->old_li == current_screen->li
	    && inputdata->old_co == current_screen->co)
		return 0;

	/* resized?  Keep it simple and reset everything */
	inputdata->cursor_x = 0;
	inputdata->cursor_y = current_screen->li - 1;
	inputdata->old_li   = current_screen->li;
	inputdata->old_co   = current_screen->co;
	
	inputdata->zone     = current_screen->co;
	if (inputdata->zone > WIDTH)
		inputdata->zone -= WIDTH;
	if (inputdata->zone > WIDTH)
		inputdata->zone -= WIDTH;
	return 1;
}
