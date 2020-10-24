static void
gconf_editor_window_list_view_popup_menu (GtkWidget *widget, GConfEditorWindow *window)
{
	GtkTreeIter iter;

	GtkTreeSelection *selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (window->list_view));
	/* Select our row */
	if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/EditKey"), 
					  TRUE);
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/UnsetKey"), 
					  window->type != GCONF_EDITOR_WINDOW_TYPE_DEFAULTS);
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/DefaultKey"), 
					  (gconf_util_can_edit_defaults () && window->type != GCONF_EDITOR_WINDOW_TYPE_DEFAULTS) ||
					  (gconf_pk_can_set_default ("/") && window->type == GCONF_EDITOR_WINDOW_TYPE_NORMAL));
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/MandatoryKey"), 
					  (gconf_util_can_edit_mandatory () && window->type != GCONF_EDITOR_WINDOW_TYPE_MANDATORY) ||
					  (gconf_pk_can_set_mandatory ("/") && window->type == GCONF_EDITOR_WINDOW_TYPE_NORMAL));
	}
	else {
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/EditKey"), 
					  FALSE);
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/UnsetKey"), 
					  FALSE);
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/DefaultKey"), 
					  FALSE);
		gtk_widget_set_sensitive (gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu/MandatoryKey"), 
					  FALSE);

	}
	
	gtk_menu_popup (GTK_MENU (window->popup_menu), NULL, NULL,
			(GtkMenuPositionFunc) gconf_editor_window_list_view_popup_position, 
			gtk_widget_get_window (widget),
			0, gtk_get_current_event_time ());
}