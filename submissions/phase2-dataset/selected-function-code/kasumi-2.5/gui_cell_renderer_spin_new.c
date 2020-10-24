GtkCellRenderer *
gui_cell_renderer_spin_new (gdouble  lower,
                            gdouble  upper,
                            gdouble  step_inc,
                            gdouble  page_inc,
                            gdouble  page_size,
                            gdouble  climb_rate,
                            guint    digits)
{
        GtkCellRenderer     *cell;
        GuiCellRendererSpin *spincell;

        cell = g_object_new(GUI_TYPE_CELL_RENDERER_SPIN, NULL);

        spincell = GUI_CELL_RENDERER_SPIN(cell);

        spincell->lower      = lower;
        spincell->upper      = upper;
        spincell->step_inc   = step_inc;
        spincell->page_inc   = page_inc;
        spincell->page_size  = page_size;
        spincell->climb_rate = climb_rate;
        spincell->digits     = digits;

  return cell;
}
