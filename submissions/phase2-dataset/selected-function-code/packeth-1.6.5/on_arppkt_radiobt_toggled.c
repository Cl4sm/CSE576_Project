void
on_arppkt_radiobt_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *nt2, *nt4;
	GtkWidget *option_menu, *opt_value;
	GtkWidget *option_menu2, *opt_value2;
	GtkWidget *cbt;
	
	cbt = lookup_widget(GTK_WIDGET(togglebutton), "auto_get_mac_cbt");
	gtk_widget_set_sensitive (cbt, TRUE);

	/* for eth II */
	option_menu = lookup_widget(GTK_WIDGET(togglebutton), "L_optmenu1_bt");
	opt_value = lookup_widget(GTK_WIDGET(togglebutton), "L_ethtype");
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), 2);
	gtk_entry_set_text(GTK_ENTRY(opt_value), "0806");

	/* for eth 802.3 */
	option_menu2 = lookup_widget(GTK_WIDGET(togglebutton), "optionmenu6");
	opt_value2 = lookup_widget(GTK_WIDGET(togglebutton), "L_pid");
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu2), 2);
	gtk_entry_set_text(GTK_ENTRY(opt_value2), "0806");

	/* open arp notebook page and empty notebook page for 4 layer */
	nt2 = lookup_widget(GTK_WIDGET(togglebutton), "notebook2");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt2), 3);
	nt4 = lookup_widget(GTK_WIDGET(togglebutton), "notebook4");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 5);

	gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	gtk_editable_set_editable(GTK_EDITABLE(opt_value2), FALSE);
}