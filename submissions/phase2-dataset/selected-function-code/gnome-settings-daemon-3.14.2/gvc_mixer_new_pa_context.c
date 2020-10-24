static void
gvc_mixer_new_pa_context (GvcMixerControl *self)
{
        pa_proplist     *proplist;

        g_return_if_fail (self);
        g_return_if_fail (!self->priv->pa_context);

        proplist = pa_proplist_new ();
        pa_proplist_sets (proplist,
                          PA_PROP_APPLICATION_NAME,
                          self->priv->name);
        pa_proplist_sets (proplist,
                          PA_PROP_APPLICATION_ID,
                          "org.gnome.VolumeControl");
        pa_proplist_sets (proplist,
                          PA_PROP_APPLICATION_ICON_NAME,
                          "multimedia-volume-control");
        pa_proplist_sets (proplist,
                          PA_PROP_APPLICATION_VERSION,
                          PACKAGE_VERSION);

        self->priv->pa_context = pa_context_new_with_proplist (self->priv->pa_api, NULL, proplist);

        pa_proplist_free (proplist);
        g_assert (self->priv->pa_context);
}