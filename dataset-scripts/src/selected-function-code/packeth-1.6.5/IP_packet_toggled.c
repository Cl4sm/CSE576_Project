void
IP_packet_toggled                      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *nt2;
	GtkWidget *nt4;
	GtkWidget *rb1, *rb2, *rb3, *rb4, *rb5;
	GtkWidget *option_menu, *opt_value;
	GtkWidget *option_menu2, *opt_value2;
	GtkWidget *cbt;
	
	cbt = lookup_widget(GTK_WIDGET(togglebutton), "auto_get_mac_cbt");
	gtk_widget_set_sensitive (cbt, TRUE);

	/* eth II */
	option_menu = lookup_widget(GTK_WIDGET(togglebutton), "L_optmenu1_bt");
	opt_value = lookup_widget(GTK_WIDGET(togglebutton), "L_ethtype");
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), 0);
	gtk_entry_set_text(GTK_ENTRY(opt_value), "0800");

	/* eth 802.3 */
	option_menu2 = lookup_widget(GTK_WIDGET(togglebutton), "optionmenu6");
	opt_value2 = lookup_widget(GTK_WIDGET(togglebutton), "L_pid");
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu2), 0);
	gtk_entry_set_text(GTK_ENTRY(opt_value2), "0800");

	/* open ipv4 page */
	nt2 = lookup_widget(GTK_WIDGET(togglebutton), "notebook2");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt2), 0);

	/* what is next page */
	rb1 = lookup_widget(GTK_WIDGET(togglebutton), "tcp_bt");
	rb2 = lookup_widget(GTK_WIDGET(togglebutton), "udp_bt");
	rb3 = lookup_widget(GTK_WIDGET(togglebutton), "icmp_bt");
	rb4 = lookup_widget(GTK_WIDGET(togglebutton), "ip_user_data_bt");
	rb5 = lookup_widget(GTK_WIDGET(togglebutton), "igmp_bt");
	nt4 = lookup_widget(GTK_WIDGET(togglebutton), "notebook4");

	gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
	gtk_editable_set_editable(GTK_EDITABLE(opt_value2), FALSE);

	if (GTK_TOGGLE_BUTTON(rb1)->active) 
		gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 1);
	else if (GTK_TOGGLE_BUTTON(rb2)->active) 
		gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 0);
	else if (GTK_TOGGLE_BUTTON(rb3)->active) 
		gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 2);
	else if (GTK_TOGGLE_BUTTON(rb5)->active) 
		gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 4);
	else 
		gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 3);

}