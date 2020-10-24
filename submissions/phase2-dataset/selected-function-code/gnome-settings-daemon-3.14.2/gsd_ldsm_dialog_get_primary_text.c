static gchar*
gsd_ldsm_dialog_get_primary_text (GsdLdsmDialog *dialog)
{
        gchar *primary_text, *free_space;
	
        g_return_val_if_fail (GSD_IS_LDSM_DIALOG (dialog), NULL);
	
        free_space = g_format_size (dialog->priv->space_remaining);
	
        if (dialog->priv->other_partitions) {
                primary_text = g_strdup_printf (_("The volume \"%s\" has only %s disk space remaining."),
                                                dialog->priv->partition_name, free_space);
        } else {
                primary_text = g_strdup_printf (_("This computer has only %s disk space remaining."),
                                                free_space);
        }
	
        g_free (free_space);
	
        return primary_text;	
}