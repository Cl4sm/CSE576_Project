void
on_optionmenu14_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *option_menu, *menu, *active_item;
	gint active_index;
	GtkWidget *en;

	option_menu = lookup_widget (GTK_WIDGET (button), "optionmenu14");
	en = lookup_widget (GTK_WIDGET (button), "entry161");
	
	menu = GTK_OPTION_MENU (option_menu)->menu;
	active_item = gtk_menu_get_active (GTK_MENU (menu));
	active_index = g_list_index (GTK_MENU_SHELL (menu)->children, active_item);

	if ((active_index == 0) || (active_index == 3) || (active_index == 4) || (active_index == 5))
		gtk_widget_set_sensitive (en, FALSE);
	else 
		gtk_widget_set_sensitive (en, TRUE);

}