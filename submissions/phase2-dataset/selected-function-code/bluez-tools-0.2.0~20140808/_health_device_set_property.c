static void _health_device_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	HealthDevice *self = HEALTH_DEVICE(object);
	GError *error = NULL;

	switch (property_id) {
	case PROP_DBUS_OBJECT_PATH:
		self->priv->object_path = g_value_dup_string(value);
		_health_device_create_gdbus_proxy(self, HEALTH_DEVICE_DBUS_SERVICE, self->priv->object_path, &error);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}

	if (error != NULL)
		g_critical("%s", error->message);

	g_assert(error == NULL);
}
