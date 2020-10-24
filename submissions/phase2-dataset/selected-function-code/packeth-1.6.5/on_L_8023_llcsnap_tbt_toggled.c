void
on_L_8023_llcsnap_tbt_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *l_oui, *l_pid, *lbl_oui, *lbl_pid, *opt, *menu, *menu_item;
	gint index;

	l_oui = lookup_widget(GTK_WIDGET(togglebutton), "L_oui");
	l_pid = lookup_widget(GTK_WIDGET(togglebutton), "L_pid");
	lbl_oui = lookup_widget(GTK_WIDGET(togglebutton), "label_oui");
	lbl_pid = lookup_widget(GTK_WIDGET(togglebutton), "label_pid");
	opt = lookup_widget(GTK_WIDGET(togglebutton), "optionmenu6");

	gtk_widget_set_sensitive(l_oui, TRUE);
	gtk_widget_set_sensitive(l_pid, TRUE);
	gtk_widget_set_sensitive(lbl_oui, TRUE);
	gtk_widget_set_sensitive(lbl_pid, TRUE);
	gtk_entry_set_text(GTK_ENTRY(l_oui), "000000");
	menu = GTK_OPTION_MENU(opt)->menu;
        menu_item = gtk_menu_get_active (GTK_MENU (menu));
        index = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
	switch (index) {
		case 1: {
			gtk_entry_set_text(GTK_ENTRY(l_pid), "86DD");
			break;
			}
		case 2: {
			gtk_entry_set_text(GTK_ENTRY(l_pid), "0806");
			break;
			}
		case 3: {
			gtk_entry_set_text(GTK_ENTRY(l_pid), "");
			break;
			}
		default:
			gtk_entry_set_text(GTK_ENTRY(l_pid), "0800");
	}

	gtk_widget_set_sensitive(opt, TRUE);

}