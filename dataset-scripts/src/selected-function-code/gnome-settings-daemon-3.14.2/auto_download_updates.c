static void
auto_download_updates (GsdUpdatesManager *manager)
{
        gchar **package_ids;
        guint i;
        PkPackage *pkg;

        /* download each package */
        package_ids = g_new0 (gchar *, manager->priv->update_packages->len + 1);
        for (i = 0; i < manager->priv->update_packages->len; i++) {
                pkg = g_ptr_array_index (manager->priv->update_packages, i);
                package_ids[i] = g_strdup (pk_package_get_id (pkg));
        }

        /* we've set only-download in PkTask */
        pk_task_update_packages_async (manager->priv->task,
                                       package_ids,
                                       manager->priv->cancellable,
                                       NULL, NULL,
                                       (GAsyncReadyCallback) package_download_finished_cb,
                                       manager);
        g_strfreev (package_ids);
}