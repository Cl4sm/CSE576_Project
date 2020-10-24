static gboolean
ldsm_mount_has_trash (LdsmMountInfo *mount)
{
        const gchar *user_data_dir;
        gchar *user_data_attr_id_fs;
        gchar *path_attr_id_fs;
        gboolean mount_uses_user_trash = FALSE;
        gchar *trash_files_dir;
        gboolean has_trash = FALSE;
        GDir *dir;
        const gchar *path;

        user_data_dir = g_get_user_data_dir ();
        user_data_attr_id_fs = ldsm_get_fs_id_for_path (user_data_dir);

        path = g_unix_mount_get_mount_path (mount->mount);
        path_attr_id_fs = ldsm_get_fs_id_for_path (path);

        if (g_strcmp0 (user_data_attr_id_fs, path_attr_id_fs) == 0) {
                /* The volume that is low on space is on the same volume as our home
                 * directory. This means the trash is at $XDG_DATA_HOME/Trash,
                 * not at the root of the volume which is full.
                 */
                mount_uses_user_trash = TRUE;
        }

        g_free (user_data_attr_id_fs);
        g_free (path_attr_id_fs);

        /* I can't think of a better way to find out if a volume has any trash. Any suggestions? */
        if (mount_uses_user_trash) {
                trash_files_dir = g_build_filename (g_get_user_data_dir (), "Trash", "files", NULL);
        } else {
                gchar *uid;

                uid = g_strdup_printf ("%d", getuid ());
                trash_files_dir = g_build_filename (path, ".Trash", uid, "files", NULL);
                if (!g_file_test (trash_files_dir, G_FILE_TEST_IS_DIR)) {
                        gchar *trash_dir;

                        g_free (trash_files_dir);
                        trash_dir = g_strdup_printf (".Trash-%s", uid);
                        trash_files_dir = g_build_filename (path, trash_dir, "files", NULL);
                        g_free (trash_dir);
                        if (!g_file_test (trash_files_dir, G_FILE_TEST_IS_DIR)) {
                                g_free (trash_files_dir);
                                g_free (uid);
                                return has_trash;
                        }
                }
                g_free (uid);
        }

        dir = g_dir_open (trash_files_dir, 0, NULL);
        if (dir) {
                if (g_dir_read_name (dir))
                        has_trash = TRUE;
                g_dir_close (dir);
        }

        g_free (trash_files_dir);

        return has_trash;
}