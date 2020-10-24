static void option_toggled_cb(GtkToggleButton *button, gpointer user_data UNUSED)
{
  gboolean active = gtk_toggle_button_get_active(button);
  GtkWidget *togglebutton = GTK_WIDGET(button);
  
  if (togglebutton == swap_check) {
     Prefs.disable_swap_matrix = !active;
  }
  else if (togglebutton == io_check) {
    Prefs.disable_io_matrix = !active;
  }
  else if (togglebutton == hdlist_check) {
    if (active) {
      Prefs.hdlist_pos = AL_BOTTOM + AL_LEFT;
      gtk_widget_set_sensitive(leds_check, TRUE);
      gtk_widget_set_sensitive(hddtemp_check, TRUE);
    }
    else {
      Prefs.hdlist_pos = AL_NONE;
      gtk_widget_set_sensitive(leds_check, FALSE);
      gtk_widget_set_sensitive(hddtemp_check, FALSE);
    }
  }
  else if (togglebutton == leds_check) {
    Prefs.disable_hd_leds = !active;
  }
  else if (togglebutton == hddtemp_check) {
    Prefs.enable_hddtemp = active;
    app->displayed_hd_changed = 1;
  }
}