void
on_optionmenu5_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *option_menu, *menu, *active_item, *opt_value;
	gint active_index;

	option_menu = lookup_widget (GTK_WIDGET (button), "optionmenu5");
	opt_value = lookup_widget (GTK_WIDGET (button), "entry58");
	menu = GTK_OPTION_MENU (option_menu)->menu;
	active_item = gtk_menu_get_active (GTK_MENU (menu));
	active_index = g_list_index (GTK_MENU_SHELL (menu)->children, active_item);
	
	if (active_index == 0) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "00");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 1) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "01");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 2) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "02");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 3) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "03");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 4) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "04");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 5) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "05");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 6) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "06");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 7) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "07");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 8) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "08");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 9) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "09");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 10) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0a");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 11) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0b");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 12) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0c");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 13) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0d");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 14) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0e");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 15) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0f");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 16) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), TRUE);
	}

}