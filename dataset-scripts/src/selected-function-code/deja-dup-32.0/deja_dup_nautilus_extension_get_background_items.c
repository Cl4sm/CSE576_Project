static GList *
deja_dup_nautilus_extension_get_background_items(NautilusMenuProvider *provider,
                                                 GtkWidget *window,
                                                 NautilusFileInfo *file)
{
  NautilusMenuItem *item;
  guint length;
  GList *file_copies;
  gchar *path;

  if (file == NULL)
    return NULL;

  path = g_find_program_in_path("deja-dup");
  if (!path)
    return NULL;
  g_free(path);

  if (!is_dir_included(nautilus_file_info_get_location(file)))
    return NULL;

  item = nautilus_menu_item_new("DejaDupNautilusExtension::restore_missing_item",
                                dgettext(GETTEXT_PACKAGE, "Restore Missing Files…"),
                                dgettext(GETTEXT_PACKAGE, "Restore deleted files from backup"),
                                "deja-dup");

  g_signal_connect(item, "activate", G_CALLBACK(restore_missing_files_callback), NULL);
  g_object_set_data_full (G_OBJECT(item), "deja_dup_extension_file",
                          g_object_ref(file),
                          (GDestroyNotify)g_object_unref);

  return g_list_append(NULL, item);
}