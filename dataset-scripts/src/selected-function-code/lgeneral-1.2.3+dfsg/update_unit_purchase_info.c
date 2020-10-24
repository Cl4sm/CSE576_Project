static void update_unit_purchase_info( PurchaseDlg *pdlg )
{
	int total_cost = 0;
	SDL_Surface *contents = pdlg->main_group->frame->contents;
	Unit_Lib_Entry *unit_entry = NULL, *trsp_entry = NULL;
	Unit *reinf_unit = NULL;
	
	SDL_FillRect( contents, 0, 0x0 );
	
	/* get selected objects */
	reinf_unit = lbox_get_selected_item( pdlg->reinf_lbox );
	if (reinf_unit) {
		unit_entry = &reinf_unit->prop;
		if (reinf_unit->trsp_prop.id)
			trsp_entry = &reinf_unit->trsp_prop;
	} else {
		unit_entry = lbox_get_selected_item( pdlg->unit_lbox );
		trsp_entry = lbox_get_selected_item( pdlg->trsp_lbox );
	}
	
	/* unit info */
	if (unit_entry) {
		render_unit_lib_entry_info( unit_entry, contents, 10, 10 );
		total_cost += unit_entry->cost;
		
		/* if no transporter allowed clear transporter list. if allowed
		 * but list empty, fill it. */
		if (reinf_unit)
			; /* unit/trsp has been cleared already */
		else if(!(unit_entry->flags & GROUND_TRSP_OK)) {
			lbox_clear_items(pdlg->trsp_lbox);
			trsp_entry = NULL;
		} else if (lbox_is_empty(pdlg->trsp_lbox) && pdlg->trsp_uclass)
			lbox_set_items(pdlg->trsp_lbox,
					get_purchasable_unit_lib_entries(
					pdlg->cur_nation->id,
					pdlg->trsp_uclass->id,
					(const Date*)&scen_info->start_date));
	}
	
	/* transporter info */
	if (trsp_entry) {
		render_unit_lib_entry_info( trsp_entry, contents, 10, 155 );
		total_cost += trsp_entry->cost;
	}
	
	/* show cost if any selection; mark red if not purchasable */
	if ( total_cost > 0 ) {
		char str[128];
		Font *font = gui->font_std;
		int y = group_get_height( pdlg->main_group ) - 10;
		
		snprintf( str, 128, tr("Total Cost: %d"), total_cost );
		if (!reinf_unit && !player_can_purchase_unit(cur_player, 
						unit_entry, trsp_entry)) {
			font = gui->font_error;
			total_cost = 0; /* indicates "not purchasable" here */
		}
		font->align = ALIGN_X_LEFT | ALIGN_Y_BOTTOM;
		write_text( font, contents, 10, y, str, 255 );
		
		if (total_cost > 0) {
			font->align = ALIGN_X_RIGHT | ALIGN_Y_BOTTOM;
			write_text( font, contents, 
				group_get_width( pdlg->main_group ) - 62, y,
				reinf_unit?tr("REFUND?"):tr("REQUEST?"), 255 );
		}
	}
	
	/* if total cost is set, purchase is possible */
	if (total_cost > 0)
		group_set_active( pdlg->main_group, ID_PURCHASE_OK, 1 );
	else
		group_set_active( pdlg->main_group, ID_PURCHASE_OK, 0 );

	/* apply rendered contents */
	frame_apply( pdlg->main_group->frame );
}