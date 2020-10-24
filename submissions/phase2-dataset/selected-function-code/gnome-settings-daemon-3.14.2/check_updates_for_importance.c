static void
check_updates_for_importance (GsdUpdatesManager *manager)
{
        guint i;
        PkPackage *pkg;
        GPtrArray *important_array;

        /* check each package */
        important_array = g_ptr_array_new ();
        for (i = 0; i < manager->priv->update_packages->len; i++) {
                pkg = g_ptr_array_index (manager->priv->update_packages, i);
                if (pk_package_get_info (pkg) == PK_INFO_ENUM_SECURITY ||
                    pk_package_get_info (pkg) == PK_INFO_ENUM_IMPORTANT)
                        g_ptr_array_add (important_array, pkg);
        }
        if (important_array->len > 0) {
                notify_critical_updates (manager,
                                         important_array);
        } else {
                notify_normal_updates_maybe (manager,
                                             manager->priv->update_packages);
        }
        g_ptr_array_unref (important_array);
}