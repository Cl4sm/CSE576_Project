static gchar *
get_device (GsdUpdatesFirmware *firmware, const gchar *filename)
{
        GFile *file;
        GFileInfo *info;
        const gchar *symlink_path;
        gchar *syspath = NULL;
        GError *error = NULL;
        gchar *target = NULL;
        gchar *tmp;

        /* get the file data */
        file = g_file_new_for_path (filename);
        info = g_file_query_info (file,
                                  G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET,
                                  G_FILE_QUERY_INFO_NONE,
                                  NULL,
                                  &error);
        if (info == NULL) {
                g_warning ("Failed to get symlink: %s",
                           error->message);
                g_error_free (error);
                goto out;
        }

        /* /devices/pci0000:00/0000:00:1d.0/usb5/5-2/firmware/5-2 */
        symlink_path = g_file_info_get_symlink_target (info);
        if (symlink_path == NULL) {
                g_warning ("failed to get symlink target");
                goto out;
        }

        /* prepend sys to make '/sys/devices/pci0000:00/0000:00:1d.0/usb5/5-2/firmware/5-2' */
        syspath = g_strconcat ("/sys", symlink_path, NULL);

        /* start with the longest, and try to find a sub-path that exists */
        tmp = &syspath[strlen (syspath)];
        while (tmp != NULL) {
                *tmp = '\0';
                g_debug ("testing %s", target);
                if (g_file_test (syspath, G_FILE_TEST_EXISTS)) {
                        target = g_strdup (syspath);
                        goto out;
                }
                tmp = g_strrstr (syspath, "/");
        }
out:
        if (info != NULL)
                g_object_unref (info);
        g_object_unref (file);
        g_free (syspath);
        return target;
}