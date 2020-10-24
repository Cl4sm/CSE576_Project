McdChannel *
mcd_channel_new_from_path (TpConnection *connection, const gchar *object_path,
                           const gchar *type, guint handle,
                           TpHandleType handle_type)
{
    GHashTable *props;
    GValue v_type = G_VALUE_INIT;
    GValue v_handle = G_VALUE_INIT;
    GValue v_handle_type = G_VALUE_INIT;
    McdChannel *channel;

    props = g_hash_table_new (g_str_hash, g_str_equal);

    g_value_init (&v_type, G_TYPE_STRING);
    g_value_set_static_string (&v_type, type);
    g_hash_table_insert (props, TP_IFACE_CHANNEL ".ChannelType", &v_type);

    g_value_init (&v_handle, G_TYPE_UINT);
    g_value_set_uint (&v_handle, handle);
    g_hash_table_insert (props, TP_IFACE_CHANNEL ".TargetHandle", &v_handle);

    g_value_init (&v_handle_type, G_TYPE_UINT);
    g_value_set_uint (&v_handle_type, handle_type);
    g_hash_table_insert (props, TP_IFACE_CHANNEL ".TargetHandleType",
                         &v_handle_type);

    channel = mcd_channel_new_from_properties (connection, object_path, props);

    g_hash_table_unref (props);
    return channel;
}