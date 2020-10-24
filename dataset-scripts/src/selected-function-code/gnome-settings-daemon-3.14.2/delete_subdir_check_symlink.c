static void
delete_subdir_check_symlink (GObject      *source,
                             GAsyncResult *res,
                             gpointer      user_data)
{
        GFile *file = G_FILE (source);
        DeleteData *data = user_data;
        GFileInfo *info;
        GFileType type;

        info = g_file_query_info_finish (file, res, NULL);
        if (!info) {
                delete_data_unref (data);
                return;
        }

        type = g_file_info_get_file_type (info);
        g_object_unref (info);

        if (type == G_FILE_TYPE_SYMBOLIC_LINK) {
                if (should_purge_file (data->file, data->cancellable, data->old)) {
                        g_debug ("Purging %s leaf node", data->name);
                        if (!data->dry_run) {
                                g_file_delete (data->file, data->cancellable, NULL);
                        }
                }
        } else {
                g_file_enumerate_children_async (data->file,
                                                 G_FILE_ATTRIBUTE_STANDARD_NAME ","
                                                 G_FILE_ATTRIBUTE_STANDARD_TYPE,
                                                 G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
                                                 0,
                                                 data->cancellable,
                                                 delete_subdir,
                                                 delete_data_ref (data));
        }
        delete_data_unref (data);
}