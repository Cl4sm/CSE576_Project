static void
gsd_ldsm_dialog_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
        GsdLdsmDialog *self;
	
        g_return_if_fail (GSD_IS_LDSM_DIALOG (object));
	
        self = GSD_LDSM_DIALOG (object);

        switch (prop_id)
        {
        case PROP_OTHER_USABLE_PARTITIONS:
                self->priv->other_usable_partitions = g_value_get_boolean (value);
                break;
        case PROP_OTHER_PARTITIONS:
                self->priv->other_partitions = g_value_get_boolean (value);
                break;
        case PROP_HAS_TRASH:
                self->priv->has_trash = g_value_get_boolean (value);
                break;
        case PROP_SPACE_REMAINING:
                self->priv->space_remaining = g_value_get_int64 (value);
                break;
        case PROP_PARTITION_NAME:
                self->priv->partition_name = g_value_dup_string (value);
                break;
        case PROP_MOUNT_PATH:
                self->priv->mount_path = g_value_dup_string (value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}