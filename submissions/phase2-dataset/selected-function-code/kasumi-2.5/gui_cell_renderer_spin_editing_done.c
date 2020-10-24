gui_cell_renderer_spin_editing_done (GtkCellEditable *spinbutton,
                                     gpointer         data)
{
  const gchar         *path;
  const gchar         *new_text;
  GCRSpinInfo         *info;

  info = g_object_get_data (G_OBJECT (data), GUI_CELL_RENDERER_SPIN_INFO);

        if (info->focus_out_id > 0)
        {
                g_signal_handler_disconnect (spinbutton, info->focus_out_id);
                info->focus_out_id = 0;
        }

  if (GTK_ENTRY(spinbutton)->editing_canceled)
    return;

  path = g_object_get_data (G_OBJECT (spinbutton), GUI_CELL_RENDERER_SPIN_PATH);
  new_text = gtk_entry_get_text (GTK_ENTRY(spinbutton));

  g_signal_emit_by_name(data, "edited", path, new_text);
}
