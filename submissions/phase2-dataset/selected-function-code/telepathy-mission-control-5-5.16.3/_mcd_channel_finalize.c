static void
_mcd_channel_finalize (GObject * object)
{
    McdChannelPrivate *priv = MCD_CHANNEL_PRIV (object);
    GList *list;

    list = priv->satisfied_requests;

    while (list)
    {
        g_object_unref (list->data);
        list = g_list_delete_link (list, list);
    }

    if (priv->error != NULL)
    {
        g_error_free (priv->error);
        priv->error = NULL;
    }

    G_OBJECT_CLASS (mcd_channel_parent_class)->finalize (object);
}