static void
gconf_editor_popup_window_set_as_default (GtkAction *action, GtkWidget *callback_data)
{
  GConfEditorWindow *gconfwindow = GCONF_EDITOR_WINDOW (callback_data);

  GtkTreeIter iter;
  GConfValue *value;
  char *path = NULL;
  GtkWindow *window = GTK_WINDOW (callback_data);
  gboolean can_use_pk;
  
  can_use_pk = (gconfwindow->type == GCONF_EDITOR_WINDOW_TYPE_NORMAL);
  gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (gconfwindow->list_view)),
                                   NULL, &iter);
  gtk_tree_model_get (gconfwindow->sorted_list_model, &iter,
                      GCONF_LIST_MODEL_KEY_PATH_COLUMN, &path,
                      GCONF_LIST_MODEL_VALUE_COLUMN, &value,
                      -1);

  if (gconf_util_can_edit_defaults ()) {
    GConfClient *client;

    client = gconf_editor_window_get_client (GCONF_EDITOR_WINDOW_TYPE_DEFAULTS);
    if (client != NULL) {
      GError *error = NULL;

      gconf_client_set (client, path, value, NULL);
      gconf_client_suggest_sync (client, &error);
      g_object_unref (client);

      if (!error)
        return;
      if (!can_use_pk)
        gconf_editor_window_popup_error_dialog (window, _("Could not sync value. Error was:\n%s"), error);
    }
  }
  if (can_use_pk) {
    gconf_client_suggest_sync (gconfwindow->client, NULL);
    gconf_pk_set_default_async (path,
                                (GFunc) gconf_editor_popup_policykit_callback,
                                g_object_ref (window), g_object_unref);
  }
}