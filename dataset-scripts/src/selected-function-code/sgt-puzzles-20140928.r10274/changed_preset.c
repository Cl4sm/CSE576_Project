static void changed_preset(frontend *fe)
{
    int n = midend_which_preset(fe->me);

    fe->preset_threaded = TRUE;
    if (n < 0 && fe->preset_custom) {
	gtk_check_menu_item_set_active(
	    GTK_CHECK_MENU_ITEM(fe->preset_custom),
	    TRUE);
    } else {
	GSList *gs = fe->preset_radio;
	int i = fe->n_preset_menu_items - 1 - n;
	if (fe->preset_custom)
	    gs = gs->next;
	while (i && gs) {
	    i--;
	    gs = gs->next;
	}
	if (gs) {
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gs->data),
					   TRUE);
	} else for (gs = fe->preset_radio; gs; gs = gs->next) {
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gs->data),
					   FALSE);
	}
    }
    fe->preset_threaded = FALSE;

    /*
     * Update the greying on the Copy menu option.
     */
    if (fe->copy_menu_item) {
	int enabled = midend_can_format_as_text_now(fe->me);
	gtk_widget_set_sensitive(fe->copy_menu_item, enabled);
    }
}