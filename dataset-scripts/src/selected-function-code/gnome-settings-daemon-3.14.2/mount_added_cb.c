static void
mount_added_cb (GVolumeMonitor *volume_monitor,
                GMount *mount,
                GsdUpdatesManager *manager)
{
        gboolean ret = FALSE;
        gchar **filenames = NULL;
        gchar *media_repo_filenames;
        gchar *root_path;
        GFile *root;
        guint i;

        /* check if any installed media is an install disk */
        root = g_mount_get_root (mount);
        root_path = g_file_get_path (root);

        /* use settings */
        media_repo_filenames = g_settings_get_string (manager->priv->settings_gsd,
                                                      GSD_SETTINGS_MEDIA_REPO_FILENAMES);
        if (media_repo_filenames == NULL) {
                g_warning ("failed to get media repo filenames");
                goto out;
        }

        /* search each possible filename */
        filenames = g_strsplit (media_repo_filenames, ",", -1);
        for (i=0; filenames[i] != NULL; i++) {
                ret = file_exists_in_root (root_path, filenames[i]);
                if (ret)
                        break;
        }

        /* do an updates check with the new media */
        if (ret)
                query_updates (manager);
out:
        g_strfreev (filenames);
        g_free (media_repo_filenames);
        g_free (root_path);
        g_object_unref (root);
}