static void
collect_satisfied_requests (McdChannel *channel,
    GPtrArray **paths_out,
    GHashTable **props_out)
{
    GHashTableIter it;
    gpointer path, value;
    GPtrArray *satisfied_requests;
    GHashTable *request_properties;
    GHashTable *reqs;

    reqs = _mcd_channel_get_satisfied_requests (channel, NULL);

    satisfied_requests = g_ptr_array_sized_new (g_hash_table_size (reqs));
    g_ptr_array_set_free_func (satisfied_requests, g_free);

    request_properties = g_hash_table_new_full (g_str_hash, g_str_equal,
        g_free, (GDestroyNotify) g_hash_table_unref);

    g_hash_table_iter_init (&it, reqs);

    while (g_hash_table_iter_next (&it, &path, &value))
    {
        GHashTable *props;

        g_ptr_array_add (satisfied_requests, g_strdup (path));
        props = _mcd_request_dup_immutable_properties (value);
        g_assert (props != NULL);
        g_hash_table_insert (request_properties, g_strdup (path), props);
    }

    g_hash_table_unref (reqs);

    if (paths_out != NULL)
        *paths_out = satisfied_requests;
    else
        g_ptr_array_unref (satisfied_requests);

    if (props_out != NULL)
        *props_out = request_properties;
    else
        g_hash_table_unref (request_properties);
}