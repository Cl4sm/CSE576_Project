static gboolean
list_view_button_press_event (GtkTreeView *tree_view, GdkEventButton *event, GConfEditorWindow *window)
{
	GtkTreePath *path;

	if (event->button == 3) {
		gtk_widget_grab_focus (GTK_WIDGET (tree_view));

		/* Select our row */
		if (gtk_tree_view_get_path_at_pos (tree_view, event->x, event->y, &path, NULL, NULL, NULL)) {
			gtk_tree_selection_select_path (gtk_tree_view_get_selection (tree_view), path);

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
			
			gtk_tree_path_free (path);
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
		
		gtk_menu_popup (GTK_MENU (window->popup_menu), NULL, NULL, NULL, NULL,
				event->button, event->time);
		return TRUE;
	}

	return FALSE;
}