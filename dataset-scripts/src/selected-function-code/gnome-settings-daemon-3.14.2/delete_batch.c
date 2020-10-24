static void
delete_batch (GObject      *source,
              GAsyncResult *res,
              gpointer      user_data)
{
        GFileEnumerator *enumerator = G_FILE_ENUMERATOR (source);
        DeleteData *data = user_data;
        GList *files, *f;
        GFile *child_file;
        DeleteData *child;
        GFileInfo *info;
        GError *error = NULL;

        files = g_file_enumerator_next_files_finish (enumerator, res, &error);

        g_debug ("GsdHousekeeping: purging %d children of %s", g_list_length (files), data->name);

        if (files) {
                for (f = files; f; f = f->next) {
                        if (g_cancellable_is_cancelled (data->cancellable))
                                break;
                        info = f->data;

                        child_file = g_file_get_child (data->file, g_file_info_get_name (info));
                        child = delete_data_new (child_file,
                                                 data->cancellable,
                                                 data->old,
                                                 data->dry_run,
                                                 data->trash,
                                                 data->depth + 1);
                        delete_recursively_by_age (child);
                        delete_data_unref (child);
                        g_object_unref (child_file);
                }
                g_list_free_full (files, g_object_unref);
                if (!g_cancellable_is_cancelled (data->cancellable)) {
                        g_file_enumerator_next_files_async (enumerator, 20,
                                                            0,
                                                            data->cancellable,
                                                            delete_batch,
                                                            data);
                        return;
                }
        }

        g_file_enumerator_close (enumerator, data->cancellable, NULL);
        g_object_unref (enumerator);

        if (data->depth > 0 && !g_cancellable_is_cancelled (data->cancellable)) {
                if ((data->trash && data->depth > 1) ||
                     should_purge_file (data->file, data->cancellable, data->old)) {
                        g_debug ("GsdHousekeeping: purging %s\n", data->name);
                        if (!data->dry_run) {
                                g_file_delete (data->file, data->cancellable, NULL);
                        }
                }
        }
        delete_data_unref (data);
}