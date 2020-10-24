static void
mpris_controller_dispose (GObject *object)
{
  MprisControllerPrivate *priv = MPRIS_CONTROLLER (object)->priv;

  g_clear_object (&priv->cancellable);
  g_clear_object (&priv->mpris_client_proxy);

  if (priv->namespace_watcher_id)
    {
      bus_unwatch_namespace (priv->namespace_watcher_id);
      priv->namespace_watcher_id = 0;
    }

  if (priv->other_players)
    {
      g_slist_free_full (priv->other_players, g_free);
      priv->other_players = NULL;
    }

  G_OBJECT_CLASS (mpris_controller_parent_class)->dispose (object);
}