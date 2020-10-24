void
on_usedef2_radibt_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *nt2, *nt4;
	GtkWidget *option_menu, *opt_value;
	//GtkWidget *cbt;
	
	//cbt = lookup_widget(GTK_WIDGET(togglebutton), "auto_get_mac_cbt");
	//if (GTK_TOGGLE_BUTTON(cbt)->active) {
	//	on_auto_get_mac_cbt_toggled(togglebutton, user_data);
	//	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cbt), FALSE);
	//	gtk_widget_set_sensitive (cbt, FALSE);
	//}
	//else 
	//	gtk_widget_set_sensitive (cbt, FALSE);
		
	/* for eth II */
	option_menu = lookup_widget(GTK_WIDGET(togglebutton), "L_optmenu1_bt");
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), 3);
	opt_value = lookup_widget(GTK_WIDGET(togglebutton), "L_ethtype");
	gtk_editable_set_editable(GTK_EDITABLE(opt_value), TRUE);
	gtk_entry_set_text(GTK_ENTRY(opt_value), "");

	/* for eth 802.3 */
	option_menu = lookup_widget(GTK_WIDGET(togglebutton), "optionmenu6");
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), 3);
	opt_value = lookup_widget(GTK_WIDGET(togglebutton), "L_pid");
	gtk_editable_set_editable(GTK_EDITABLE(opt_value), TRUE);
	gtk_entry_set_text(GTK_ENTRY(opt_value), "");

	/* set the correct notebooks */
	nt2 = lookup_widget(GTK_WIDGET(togglebutton), "notebook2");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt2), 2);
	nt4 = lookup_widget(GTK_WIDGET(togglebutton), "notebook4");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 5);

}