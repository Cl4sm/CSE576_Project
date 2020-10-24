static void
mpris_player_vanished (GDBusConnection *connection,
                       const gchar     *name,
                       gpointer         user_data)
{
  MprisController *self = user_data;
  MprisControllerPrivate *priv = MPRIS_CONTROLLER (self)->priv;

  if (priv->mpris_client_proxy &&
      g_strcmp0 (name, g_dbus_proxy_get_name (priv->mpris_client_proxy)) == 0)
    {
      g_clear_object (&priv->mpris_client_proxy);

      /* take the next one if there's one */
      if (self->priv->other_players && !priv->connecting)
        {
          GSList *first;
          gchar *name;

          first = self->priv->other_players;
          name = first->data;

          start_mpris_proxy (self, name);

          self->priv->other_players = self->priv->other_players->next;
          g_free (name);
          g_slist_free_1 (first);
        }
    }
}