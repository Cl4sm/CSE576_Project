void
on_L_optmenu1_bt_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *option_menu, *menu, *active_item, *opt_value;
	gint active_index;

	option_menu = lookup_widget (GTK_WIDGET (button), "L_optmenu1_bt");
	opt_value = lookup_widget (GTK_WIDGET (button), "L_ethtype");
	menu = GTK_OPTION_MENU (option_menu)->menu;
	active_item = gtk_menu_get_active (GTK_MENU (menu));
	active_index = g_list_index (GTK_MENU_SHELL (menu)->children, active_item);


	if (active_index == 0) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0800");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 1) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "86DD");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 2) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "0806");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	}
	else if (active_index == 3) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), TRUE);
	}
}