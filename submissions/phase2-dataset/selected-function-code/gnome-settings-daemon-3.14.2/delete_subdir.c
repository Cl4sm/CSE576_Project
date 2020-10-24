static void
delete_subdir (GObject      *source,
               GAsyncResult *res,
               gpointer      user_data)
{
        GFile *file = G_FILE (source);
        DeleteData *data = user_data;
        GFileEnumerator *enumerator;
        GError *error = NULL;

        g_debug ("GsdHousekeeping: purging %s in %s\n",
                 data->trash ? "trash" : "temporary files", data->name);

        enumerator = g_file_enumerate_children_finish (file, res, &error);
        if (error) {
                if (!g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_DIRECTORY))
                        g_warning ("Failed to enumerate children of %s: %s\n", data->name, error->message);
        }
        if (enumerator) {
                g_file_enumerator_next_files_async (enumerator, 20,
                                                    0,
                                                    data->cancellable,
                                                    delete_batch,
                                                    delete_data_ref (data));
        } else if (data->depth > 0 && g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_DIRECTORY)) {
                if ((data->trash && data->depth > 1) ||
                     should_purge_file (data->file, data->cancellable, data->old)) {
                        g_debug ("Purging %s leaf node", data->name);
                        if (!data->dry_run) {
                                g_file_delete (data->file, data->cancellable, NULL);
                        }
                }
        }

        if (error)
                g_error_free (error);
        delete_data_unref (data);
}