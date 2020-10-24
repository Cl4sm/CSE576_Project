static gboolean
ldsm_check_all_mounts (gpointer data)
{
        GList *mounts;
        GList *l;
        GList *check_mounts = NULL;
        GList *full_mounts = NULL;
        guint number_of_mounts;
        guint number_of_full_mounts;
        gboolean multiple_volumes = FALSE;
        gboolean other_usable_volumes = FALSE;

        /* We iterate through the static mounts in /etc/fstab first, seeing if
         * they're mounted by checking if the GUnixMountPoint has a corresponding GUnixMountEntry.
         * Iterating through the static mounts means we automatically ignore dynamically mounted media.
         */
        mounts = g_unix_mount_points_get (time_read);

        for (l = mounts; l != NULL; l = l->next) {
                GUnixMountPoint *mount_point = l->data;
                GUnixMountEntry *mount;
                LdsmMountInfo *mount_info;
                const gchar *path;

                path = g_unix_mount_point_get_mount_path (mount_point);
                mount = g_unix_mount_at (path, time_read);
                g_unix_mount_point_free (mount_point);
                if (mount == NULL) {
                        /* The GUnixMountPoint is not mounted */
                        continue;
                }

                mount_info = g_new0 (LdsmMountInfo, 1);
                mount_info->mount = mount;

                path = g_unix_mount_get_mount_path (mount);

                if (g_unix_mount_is_readonly (mount)) {
                        ldsm_free_mount_info (mount_info);
                        continue;
                }

                if (ldsm_mount_is_user_ignore (g_unix_mount_get_mount_path (mount))) {
                        ldsm_free_mount_info (mount_info);
                        continue;
                }

                if (gsd_should_ignore_unix_mount (mount)) {
                        ldsm_free_mount_info (mount_info);
                        continue;
                }

                if (statvfs (path, &mount_info->buf) != 0) {
                        ldsm_free_mount_info (mount_info);
                        continue;
                }

                if (ldsm_mount_is_virtual (mount_info)) {
                        ldsm_free_mount_info (mount_info);
                        continue;
                }

                check_mounts = g_list_prepend (check_mounts, mount_info);
        }

        g_list_free (mounts);

        number_of_mounts = g_list_length (check_mounts);
        if (number_of_mounts > 1)
                multiple_volumes = TRUE;

        for (l = check_mounts; l != NULL; l = l->next) {
                LdsmMountInfo *mount_info = l->data;

                if (!ldsm_mount_has_space (mount_info)) {
                        full_mounts = g_list_prepend (full_mounts, mount_info);
                } else {
                        g_hash_table_remove (ldsm_notified_hash, g_unix_mount_get_mount_path (mount_info->mount));
                        ldsm_free_mount_info (mount_info);
                }
        }

        number_of_full_mounts = g_list_length (full_mounts);
        if (number_of_mounts > number_of_full_mounts)
                other_usable_volumes = TRUE;

        ldsm_maybe_warn_mounts (full_mounts, multiple_volumes,
                                other_usable_volumes);

        g_list_free (check_mounts);
        g_list_free (full_mounts);

        return TRUE;
}