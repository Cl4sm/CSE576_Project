void
on_Gen_p_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *nt1;
	
	nt1 = lookup_widget(GTK_WIDGET(button), "notebook1");
	gtk_notebook_set_page(GTK_NOTEBOOK(nt1), 3);

        nt1 = lookup_widget(GTK_WIDGET(button), "Load_button");
        gtk_widget_set_sensitive(nt1, TRUE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Save_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Reset_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "button62");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Interface_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Send_button");
        gtk_widget_set_sensitive(nt1, FALSE);
        nt1 = lookup_widget(GTK_WIDGET(button), "Stop_button");
	gtk_widget_set_sensitive(nt1, FALSE);

	statusbar_text(button, "  Open a Pcap file. Selected packet will be shown in Builder!");

}