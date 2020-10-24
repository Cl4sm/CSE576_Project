void
on_button79_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget *w1, *w2;
	gchar tmp[2];
	bzero(tmp,2);

        w1 = lookup_widget(GTK_WIDGET(button), "radiobutton55");
        w2 = lookup_widget(GTK_WIDGET(button), "radiobutton57");

	if ( (GTK_TOGGLE_BUTTON(w1)->active) && (GTK_TOGGLE_BUTTON(w2)->active) )
				tmp[0] = '3';
	else if ( (GTK_TOGGLE_BUTTON(w1)->active) && !(GTK_TOGGLE_BUTTON(w2)->active) )
				tmp[0] = '2';
	else if ( !(GTK_TOGGLE_BUTTON(w1)->active) && (GTK_TOGGLE_BUTTON(w2)->active) )
				tmp[0] = '1';
	else
				tmp[0] = '0';
	
	gtk_entry_set_max_length(GTK_ENTRY(entry_field_fragment),1);
	gtk_entry_set_text(GTK_ENTRY(entry_field_fragment),tmp);

	gtk_grab_remove(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));

}