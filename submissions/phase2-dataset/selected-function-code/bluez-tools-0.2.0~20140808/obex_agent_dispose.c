static void obex_agent_dispose(GObject *gobject)
{
	ObexAgent *self = OBEX_AGENT(gobject);

        if(self->priv->registration_id)
            g_dbus_connection_unregister_object(session_conn, self->priv->registration_id);
	/* Root folder free */
	g_free(self->priv->root_folder);
        /* callback free */
        if(self->priv->agent_released_callback)
            self->priv->agent_released_callback = NULL;
        /* user data free */
        if(self->priv->user_data)
            self->priv->user_data = NULL;
        /* callback free */
        if(self->priv->agent_approved_callback)
            self->priv->agent_approved_callback = NULL;
        /* user data free */
        if(self->priv->approved_user_data)
            self->priv->approved_user_data = NULL;
	/* Chain up to the parent class */
	G_OBJECT_CLASS(obex_agent_parent_class)->dispose(gobject);
}
