static GtkWidget *
gconf_key_editor_create_combo_box (GConfKeyEditor *editor)
{
	GtkWidget *combo_box;

	combo_box = gtk_combo_box_text_new ();

	/* These have to be ordered so the EDIT_ enum matches the
	 * menu indices
	 */

	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("Integer"));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("Boolean"));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("String"));
	/* Translators: this refers to "Floating point":
	 * see http://en.wikipedia.org/wiki/Floating_point
	 */
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("Float"));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), _("List"));

	gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);

	g_signal_connect (combo_box, "changed",
			  G_CALLBACK (combo_box_changed),
			  editor);
	
	gtk_widget_show_all (combo_box);
	return combo_box;
}