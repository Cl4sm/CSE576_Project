static void
gsd_updates_refresh_class_init (GsdUpdatesRefreshClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        object_class->finalize = gsd_updates_refresh_finalize;
        g_type_class_add_private (klass, sizeof (GsdUpdatesRefreshPrivate));
        signals [REFRESH_CACHE] =
                g_signal_new ("refresh-cache",
                              G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST,
                              0, NULL, NULL, g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);
        signals [GET_UPDATES] =
                g_signal_new ("get-updates",
                              G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST,
                              0, NULL, NULL, g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);
        signals [GET_UPGRADES] =
                g_signal_new ("get-upgrades",
                              G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST,
                              0, NULL, NULL, g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);
}