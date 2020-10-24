void
on_optionmenu4_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *option_menu, *menu, *active_item, *opt_value, *ntbk5;
	gint active_index;

	option_menu = lookup_widget (GTK_WIDGET (button), "optionmenu4");
	opt_value = lookup_widget (GTK_WIDGET (button), "entry57");
	ntbk5 = lookup_widget (GTK_WIDGET (button), "notebook5");
	menu = GTK_OPTION_MENU (option_menu)->menu;
	active_item = gtk_menu_get_active (GTK_MENU (menu));
	active_index = g_list_index (GTK_MENU_SHELL (menu)->children, active_item);

	if (active_index == 0) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "00");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk5), 0);
	}
	else if (active_index == 1) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "03");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk5), 2);
	}
	else if (active_index == 2) {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "08");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk5), 5);
	}
	else  {
		gtk_entry_set_text(GTK_ENTRY(opt_value), "");
		gtk_editable_set_editable(GTK_EDITABLE(opt_value), TRUE);
		gtk_notebook_set_page(GTK_NOTEBOOK(ntbk5), 1);
	}

}