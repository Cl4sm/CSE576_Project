GHashTable *
_mcd_channel_get_satisfied_requests (McdChannel *channel,
                                     gint64 *get_latest_time)
{
    GList *l;
    GHashTable *result;
    const gchar *path;

    g_return_val_if_fail (MCD_IS_CHANNEL (channel), NULL);

    if (get_latest_time != NULL)
        *get_latest_time = channel->priv->latest_request_time;

    result = g_hash_table_new_full (g_str_hash, g_str_equal,
        g_free, g_object_unref);

    for (l = channel->priv->satisfied_requests; l != NULL; l = g_list_next (l))
    {
        path = _mcd_request_get_object_path (l->data);
        g_assert (path != NULL);
        g_hash_table_insert (result, g_strdup (path), g_object_ref (l->data));
    }

    return result;
}