static void update_purchase_unit_limit( PurchaseDlg *pdlg, int add )
{
	SDL_Surface *contents = pdlg->ulimit_frame->contents;
	char str[16];
	int y = 7;
	
	if (pdlg->cur_unit_limit != -1) {
		pdlg->cur_unit_limit += add;
		if (pdlg->cur_unit_limit < 0)
			pdlg->cur_unit_limit = 0;
	}
	
	SDL_FillRect( contents, 0, 0x0 );
	gui->font_std->align = ALIGN_X_CENTER | ALIGN_Y_TOP;
	write_line(contents, gui->font_std, tr("Available:"), contents->w/2, &y );
	if (pdlg->cur_unit_limit == -1)
		strcpy( str, tr("None") );
	else
		snprintf(str, 16, "%d %s", pdlg->cur_unit_limit,
			(pdlg->cur_unit_limit==1)?tr("Unit"):tr("Units"));
	write_line( contents, gui->font_std, str, contents->w/2, &y );
	
	frame_apply( pdlg->ulimit_frame );
}