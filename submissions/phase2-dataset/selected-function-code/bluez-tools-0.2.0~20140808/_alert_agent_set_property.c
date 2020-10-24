{
	AlertAgent *self = ALERT_AGENT(object);
	GError *error = NULL;

	switch (property_id) {
	case PROP_DBUS_OBJECT_PATH:
		self->priv->object_path = g_value_dup_string(value);
		_alert_agent_create_gdbus_proxy(self, ALERT_AGENT_DBUS_SERVICE, self->priv->object_path, &error);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}

	if (error != NULL)
		g_critical("%s", error->message);

	g_assert(error == NULL);
}
