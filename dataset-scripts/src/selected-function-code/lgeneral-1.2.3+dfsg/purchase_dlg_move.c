void purchase_dlg_move( PurchaseDlg *pdlg, int px, int py)
{
	int sx = px, sy = py;
	int ulimit_offset = (group_get_height( pdlg->main_group ) -
			frame_get_height( pdlg->ulimit_frame ) -
			lbox_get_height( pdlg->nation_lbox ) -
			lbox_get_height( pdlg->uclass_lbox )) / 2;
	int reinf_offset = (group_get_height( pdlg->main_group ) -
			lbox_get_height( pdlg->reinf_lbox )) / 2;
	
	frame_move( pdlg->ulimit_frame, sx, sy + ulimit_offset);
	sy += frame_get_height( pdlg->ulimit_frame );
	lbox_move(pdlg->nation_lbox, sx, sy + ulimit_offset);
	sy += lbox_get_height( pdlg->nation_lbox );
	lbox_move(pdlg->uclass_lbox, sx, sy + ulimit_offset);
	sx += lbox_get_width( pdlg->nation_lbox );
	sy = py;
	lbox_move(pdlg->unit_lbox, sx, sy );
	sy += lbox_get_height( pdlg->unit_lbox );
	lbox_move(pdlg->trsp_lbox, sx, sy );
	sx += lbox_get_width( pdlg->unit_lbox );
	sy = py;
	group_move(pdlg->main_group, sx, sy);
	sx += group_get_width( pdlg->main_group );
	lbox_move(pdlg->reinf_lbox, sx, sy + reinf_offset );
}