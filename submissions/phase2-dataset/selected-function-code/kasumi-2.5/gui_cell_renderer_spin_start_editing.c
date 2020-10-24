static GtkCellEditable *
gui_cell_renderer_spin_start_editing (GtkCellRenderer      *cell,
                                      GdkEvent             *event,
                                      GtkWidget            *widget,
                                      const gchar          *path,
                                      GdkRectangle         *background_area,
                                      GdkRectangle         *cell_area,
                                      GtkCellRendererState  flags)
{
  GtkCellRendererText *celltext;
  GuiCellRendererSpin *spincell;
        GtkAdjustment       *adj;
  GtkWidget           *spinbutton;
        GCRSpinInfo         *info;
        gdouble              curval = 0.0;

  celltext = GTK_CELL_RENDERER_TEXT(cell);
        spincell = GUI_CELL_RENDERER_SPIN(cell);

  /* If the cell isn't editable we return NULL. */
  if (celltext->editable == FALSE)
    return NULL;

  spinbutton = g_object_new (GTK_TYPE_SPIN_BUTTON, "has_frame", FALSE, "numeric", TRUE, NULL);

        /* dirty */
  if (celltext->text)
                curval = atof(celltext->text);

        adj = GTK_ADJUSTMENT(gtk_adjustment_new(curval,
                                                spincell->lower,
                                                spincell->upper,
                                                spincell->step_inc,
                                                spincell->page_inc,
                                                spincell->page_size));

        gtk_spin_button_configure(GTK_SPIN_BUTTON(spinbutton), adj, spincell->climb_rate, spincell->digits);

  g_object_set_data_full (G_OBJECT(spinbutton), GUI_CELL_RENDERER_SPIN_PATH, g_strdup (path), g_free);

  gtk_editable_select_region (GTK_EDITABLE (spinbutton), 0, -1);

  gtk_widget_show (spinbutton);

  g_signal_connect (spinbutton, "editing_done",
                          G_CALLBACK (gui_cell_renderer_spin_editing_done),
                          celltext);

        /* hack trying to catch the quite annoying effect
         *  a double click has while editing */

  g_signal_connect (spinbutton, "button_press_event",
                          G_CALLBACK (onButtonPress),
                          NULL);

        info = g_new0(GCRSpinInfo, 1);

  info->focus_out_id = g_signal_connect (spinbutton, "focus_out_event",
                             G_CALLBACK (gui_cell_renderer_spin_focus_out_event),
                             celltext);

  g_object_set_data_full (G_OBJECT (cell), GUI_CELL_RENDERER_SPIN_INFO, info, g_free);

  return GTK_CELL_EDITABLE (spinbutton);
}
