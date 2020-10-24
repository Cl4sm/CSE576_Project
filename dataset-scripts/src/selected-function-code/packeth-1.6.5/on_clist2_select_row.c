void
on_clist2_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	gchar *text;
	gchar tmp[5];
	int length;

	/* set \0 inside the tmp field, get the length, remove the ! if present
	 * and convert to int, and pass to load_packet_disector() */
	memset(tmp, 0, 5);
	gtk_clist_get_text(GTK_CLIST(clist), row, 3, &text);
	memccpy(tmp, text, 32, 4);
	length = strtol(tmp, (char **)NULL, 10);

	/* Just prints some information about the selected row */
	//g_print("You selected row %d column %d, \ntext is %s\n", row, 7, text);

	gtk_clist_get_text(GTK_CLIST(clist), row, 7, &text);
	load_packet_disector(btx, text, 1, NULL, length);

	return;
}