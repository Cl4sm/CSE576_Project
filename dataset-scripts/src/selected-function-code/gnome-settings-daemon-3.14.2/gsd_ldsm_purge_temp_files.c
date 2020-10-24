void
gsd_ldsm_purge_temp_files (GDateTime *old)
{
        DeleteData *data;
        GFile *file;

        file = g_file_new_for_path (g_get_tmp_dir ());
        data = delete_data_new (file, NULL, old, FALSE, FALSE, 0);
        delete_recursively_by_age (data);
        delete_data_unref (data);
        g_object_unref (file);

        if (g_strcmp0 (g_get_tmp_dir (), "/var/tmp") != 0) {
                file = g_file_new_for_path ("/var/tmp");
                data = delete_data_new (file, NULL, old, FALSE, FALSE, 0);
                delete_recursively_by_age (data);
                delete_data_unref (data);
                g_object_unref (file);
        }

        if (g_strcmp0 (g_get_tmp_dir (), "/tmp") != 0) {
                file = g_file_new_for_path ("/tmp");
                data = delete_data_new (file, NULL, old, FALSE, FALSE, 0);
                delete_recursively_by_age (data);
                delete_data_unref (data);
                g_object_unref (file);
        }
}