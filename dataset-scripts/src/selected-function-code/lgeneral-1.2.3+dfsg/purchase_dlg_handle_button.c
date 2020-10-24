int purchase_dlg_handle_button( PurchaseDlg *pdlg, int bid, int cx, int cy,
		Button **pbtn )
{
	void *item = NULL;
	
	if (group_handle_button(pdlg->main_group,bid,cx,cy,pbtn)) {
		/* catch and handle purchase button internally */
		if ((*pbtn)->id == ID_PURCHASE_OK) {
			handle_purchase_button( pdlg );
			return 0;
		}
		return 1;
	}
	if (lbox_handle_button(pdlg->nation_lbox,bid,cx,cy,pbtn,&item)) {
		if ( item && item != pdlg->cur_nation ) {
			pdlg->cur_nation = (Nation*)item;
			update_purchasable_units(pdlg);
		}
		return 0;
	}
	if (lbox_handle_button(pdlg->uclass_lbox,bid,cx,cy,pbtn,&item)) {
		if ( item && item != pdlg->cur_uclass ) {
			pdlg->cur_uclass = (Unit_Class*)item;
			update_purchasable_units(pdlg);
		}
		return 0;
	}
	if (lbox_handle_button(pdlg->unit_lbox,bid,cx,cy,pbtn,&item)) {
		if (item) {
			/* clear reinf selection since we share info window */
			lbox_clear_selected_item( pdlg->reinf_lbox );
			update_unit_purchase_info( pdlg );
		}
		return 0;
	}
	if (lbox_handle_button(pdlg->trsp_lbox,bid,cx,cy,pbtn,&item)) {
		if (item) {
			if (bid == BUTTON_RIGHT) /* deselect entry */
				lbox_clear_selected_item( pdlg->trsp_lbox );
			/* clear reinf selection since we share info window */
			lbox_clear_selected_item( pdlg->reinf_lbox );
			update_unit_purchase_info( pdlg );
		}
		return 0;
	}
	if (lbox_handle_button(pdlg->reinf_lbox,bid,cx,cy,pbtn,&item)) {
		if (item) {
			/* clear unit selection since we share info window */
			lbox_clear_selected_item( pdlg->unit_lbox );
			lbox_clear_items( pdlg->trsp_lbox );
			update_unit_purchase_info( pdlg );
		}
		return 0;
	}
	return 0;
}