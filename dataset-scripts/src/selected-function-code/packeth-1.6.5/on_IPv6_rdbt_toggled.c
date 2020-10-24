void
on_IPv6_rdbt_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *nt2;
	GtkWidget *nt4;
	GtkWidget *option_menu, *opt_value;
	GtkWidget *rb1, *rb2, *rb3, *rb4;
	
	//cbt = lookup_widget(GTK_WIDGET(togglebutton), "auto_get_mac_cbt");
	//gtk_widget_set_sensitive (cbt, TRUE);
	option_menu = lookup_widget(GTK_WIDGET(togglebutton), "L_optmenu1_bt");
	opt_value = lookup_widget(GTK_WIDGET(togglebutton), "L_ethtype");
	nt2 = lookup_widget(GTK_WIDGET(togglebutton), "notebook2");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt2), 1);
	gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), 1);
	gtk_entry_set_text(GTK_ENTRY(opt_value), "86DD");
	gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);

      /* what is next page */
        rb1 = lookup_widget(GTK_WIDGET(togglebutton), "radiobutton67");
        rb2 = lookup_widget(GTK_WIDGET(togglebutton), "radiobutton68");
        rb3 = lookup_widget(GTK_WIDGET(togglebutton), "radiobutton69");
        rb4 = lookup_widget(GTK_WIDGET(togglebutton), "radiobutton71");
        nt4 = lookup_widget(GTK_WIDGET(togglebutton), "notebook4");

        //gtk_editable_set_editable(GTK_EDITABLE(opt_value), FALSE);
        //gtk_editable_set_editable(GTK_EDITABLE(opt_value2), FALSE);

        if (GTK_TOGGLE_BUTTON(rb1)->active)
                gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 0);
        else if (GTK_TOGGLE_BUTTON(rb2)->active)
                gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 1);
        else if (GTK_TOGGLE_BUTTON(rb3)->active)
                gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 5);
        else
                gtk_notebook_set_page(GTK_NOTEBOOK(nt4), 3);


}