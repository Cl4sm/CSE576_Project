static void
_gsd_smartcard_service_token_schedule_emit_changed (GsdSmartcardServiceTokenSkeleton *skeleton, const _ExtendedGDBusPropertyInfo *info, guint prop_id, const GValue *orig_value)
{
  ChangedProperty *cp;
  GList *l;
  cp = NULL;
  for (l = skeleton->priv->changed_properties; l != NULL; l = l->next)
    {
      ChangedProperty *i_cp = l->data;
      if (i_cp->info == info)
        {
          cp = i_cp;
          break;
        }
    }
  if (cp == NULL)
    {
      cp = g_new0 (ChangedProperty, 1);
      cp->prop_id = prop_id;
      cp->info = info;
      skeleton->priv->changed_properties = g_list_prepend (skeleton->priv->changed_properties, cp);
      g_value_init (&cp->orig_value, G_VALUE_TYPE (orig_value));
      g_value_copy (orig_value, &cp->orig_value);
    }
}