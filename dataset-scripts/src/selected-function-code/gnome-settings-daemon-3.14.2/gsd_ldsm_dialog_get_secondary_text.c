static const gchar*
gsd_ldsm_dialog_get_secondary_text (GsdLdsmDialog *dialog)
{
        g_return_val_if_fail (GSD_IS_LDSM_DIALOG (dialog), NULL);
	
        if (dialog->priv->other_usable_partitions) {
                if (dialog->priv->has_trash) {
                        return _("You can free up disk space by emptying the Trash, removing " \
                                 "unused programs or files, or moving files to another disk or partition.");
                } else {
                        return _("You can free up disk space by removing unused programs or files, " \
                                 "or by moving files to another disk or partition.");
                }
        } else {
                if (dialog->priv->has_trash) {
                        return _("You can free up disk space by emptying the Trash, removing unused " \
                                 "programs or files, or moving files to an external disk.");
                } else {
                        return _("You can free up disk space by removing unused programs or files, " \
                                 "or by moving files to an external disk.");
                }
        }
}