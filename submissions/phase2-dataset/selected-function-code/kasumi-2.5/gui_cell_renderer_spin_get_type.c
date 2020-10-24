GType
gui_cell_renderer_spin_get_type (void)
{
  static GType cell_spin_type = 0;

  if (cell_spin_type)
    return cell_spin_type;

  if (1)
  {
    static const GTypeInfo cell_spin_info =
    {
      sizeof (GuiCellRendererSpinClass),
      NULL,                                                     /* base_init */
      NULL,                                                     /* base_finalize */
      (GClassInitFunc) gui_cell_renderer_spin_class_init,
      NULL,                                                     /* class_finalize */
      NULL,                                                     /* class_data */
      sizeof (GuiCellRendererSpin),
      0,                                                        /* n_preallocs */
      (GInstanceInitFunc) gui_cell_renderer_spin_init,
    };

    /* Derive from GtkCellRenderer */
    cell_spin_type = g_type_register_static (GTK_TYPE_CELL_RENDERER_TEXT,
                                                 "GuiCellRendererSpin",
                                                  &cell_spin_info,
                                                  0);
  }

  return cell_spin_type;
}
