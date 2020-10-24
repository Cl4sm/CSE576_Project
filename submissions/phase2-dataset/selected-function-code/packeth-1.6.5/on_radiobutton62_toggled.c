void
on_radiobutton62_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *en1;
	int i;
	char ime[10];

	for(i=0; i<10; i++) {
		sprintf(ime, "entry%d", i+185);
        	en1 = lookup_widget(GTK_WIDGET(togglebutton), ime);
        	gtk_widget_set_sensitive(en1, FALSE);

		sprintf(ime, "entry%d", i+121);
        	en1 = lookup_widget(GTK_WIDGET(togglebutton), ime);
        	gtk_widget_set_sensitive(en1, TRUE);

		sprintf(ime, "entry%d", i+131);
        	en1 = lookup_widget(GTK_WIDGET(togglebutton), ime);
        	gtk_widget_set_sensitive(en1, TRUE);

		sprintf(ime, "entry%d", i+141);
        	en1 = lookup_widget(GTK_WIDGET(togglebutton), ime);
        	gtk_widget_set_sensitive(en1, TRUE);

        	en1 = lookup_widget(GTK_WIDGET(togglebutton), "button87");
        	gtk_widget_set_sensitive(en1, FALSE);

        	en1 = lookup_widget(GTK_WIDGET(togglebutton), "entry152");
        	gtk_widget_set_sensitive(en1, TRUE);
	}

}