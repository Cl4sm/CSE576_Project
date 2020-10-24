{
	self->priv = obex_agent_get_instance_private(self);
        g_assert(session_conn != NULL);
        self->priv->registration_id = 0;
        self->priv->root_folder = NULL;
        self->priv->agent_released_callback = NULL;
        self->priv->user_data = NULL;
        self->priv->agent_approved_callback = NULL;
        self->priv->approved_user_data = NULL;
        
        GError *error = NULL;
        GDBusInterfaceVTable obex_agent_table;
        memset(&obex_agent_table, 0x0, sizeof(obex_agent_table));
    
        GDBusNodeInfo *obex_agent_node_info = g_dbus_node_info_new_for_xml(_obex_agent_introspect_xml, &error);
        g_assert(error == NULL);
        GDBusInterfaceInfo *obex_agent_interface_info = g_dbus_node_info_lookup_interface(obex_agent_node_info, OBEX_AGENT_DBUS_INTERFACE);
        obex_agent_table.method_call = _obex_agent_method_call_func;
	self->priv->registration_id = g_dbus_connection_register_object(session_conn, OBEX_AGENT_DBUS_PATH, obex_agent_interface_info, &obex_agent_table, self, _obex_agent_g_destroy_notify, &error);
        g_assert(error == NULL);
        g_assert(self->priv->registration_id != 0);
        g_dbus_node_info_unref(obex_agent_node_info);
}
