static GtkWidget *
gconf_key_editor_create_list_type_menu (GConfKeyEditor *editor)
{
	GtkWidget *combo_box;

	combo_box = gtk_combo_box_text_new ();

	/* These have to be ordered so the EDIT_ enum matches the
	 * combobox indices
	 */

	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("Integer"));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("Boolean"));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("String"));

	gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);
	
	g_signal_connect (combo_box, "changed",
			  G_CALLBACK (list_type_menu_changed),
			  editor);

        gtk_widget_show_all (combo_box);
        return combo_box;
}