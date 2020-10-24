static void
status_changed (McdSlacker *self,
    GVariant *prop)
{
  gboolean old = self->priv->is_inactive;

  if (g_variant_classify (prop) != G_VARIANT_CLASS_UINT32)
    {
      WARNING ("%s.%s property is of type %s and we expected u",
          SERVICE_INTERFACE, SERVICE_PROP_NAME,
          g_variant_get_type_string (prop));
      return;
    }

  self->priv->is_inactive = (g_variant_get_uint32 (prop) == STATUS_IDLE);

  if (self->priv->is_inactive != old)
    {
      DEBUG ("device became %s",
          self->priv->is_inactive ? "inactive" : "active");
      g_signal_emit (self, signals[SIG_INACTIVITY_CHANGED], 0,
          self->priv->is_inactive);
    }
}