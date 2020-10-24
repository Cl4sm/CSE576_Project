static void
add_filename (GsdUpdatesFirmware *firmware, const gchar *filename_no_path)
{
        gboolean ret;
        gchar *filename_path = NULL;
        gchar *missing_path = NULL;
        gchar *sysfs_path = NULL;
        GsdUpdatesFirmwareRequest *req;
        GPtrArray *array;
        guint i;

        /* this is the file we want to load */
        filename_path = g_build_filename (GSD_UPDATES_FIRMWARE_LOADING_DIR,
                                          filename_no_path, NULL);

        /* file already exists */
        ret = g_file_test (filename_path, G_FILE_TEST_EXISTS);
        if (ret)
                goto out;

        /* this is the file that udev created for us */
        missing_path = g_build_filename (GSD_UPDATES_FIRMWARE_MISSING_DIR,
                                         filename_no_path, NULL);
        g_debug ("filename=%s -> %s", missing_path, filename_path);

        /* get symlink target */
        sysfs_path = get_device (firmware, missing_path);
        if (sysfs_path == NULL)
                goto out;

        /* find any previous requests with this path or firmware */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                if (g_strcmp0 (sysfs_path, req->sysfs_path) == 0) {
                        g_debug ("ignoring previous sysfs request for %s",
                                 sysfs_path);
                        goto out;
                }
                if (g_strcmp0 (filename_path, req->filename) == 0) {
                        g_debug ("ignoring previous filename request for %s",
                                 filename_path);
                        goto out;
                }
        }

        /* create new request object */
        req = request_new (filename_path, sysfs_path);
        g_ptr_array_add (firmware->priv->array_requested, req);
out:
        g_free (missing_path);
        g_free (filename_path);
        g_free (sysfs_path);
}