static void
combo_box_changed (GtkWidget *combo_box,
		   GConfKeyEditor *editor)
{
	int index;

	index = gtk_combo_box_get_active (GTK_COMBO_BOX (combo_box));

	editor->active_type = index;
		
	gtk_window_set_resizable (GTK_WINDOW (editor), FALSE);	
	gtk_widget_hide (editor->int_box);
	gtk_widget_hide (editor->bool_box);
	gtk_widget_hide (editor->string_box);
	gtk_widget_hide (editor->float_box);
	gtk_widget_hide (editor->list_box);
				       
	switch (index) {
		case EDIT_INTEGER:
			gtk_widget_show_all (editor->int_box);
			break;
		case EDIT_BOOLEAN:
			gtk_widget_show_all (editor->bool_box);
			break;
		case EDIT_STRING:
			gtk_widget_show_all (editor->string_box);
			break;
  		case EDIT_FLOAT:
			gtk_widget_show_all (editor->float_box);
			break;
		case EDIT_LIST:
			gtk_window_set_resizable (GTK_WINDOW (editor), TRUE);
			gtk_widget_show_all (editor->list_box);
			break;
		default:
			g_assert_not_reached ();
	}
}