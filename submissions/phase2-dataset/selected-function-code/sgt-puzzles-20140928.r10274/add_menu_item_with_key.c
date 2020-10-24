static GtkWidget *add_menu_item_with_key(frontend *fe, GtkContainer *cont,
                                         char *text, int key)
{
    GtkWidget *menuitem = gtk_menu_item_new_with_label(text);
    int keyqual;
    gtk_container_add(cont, menuitem);
    gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
                        GINT_TO_POINTER(key));
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_key_event), fe);
    switch (key & ~0x1F) {
      case 0x00:
	key += 0x60;
	keyqual = GDK_CONTROL_MASK;
	break;
      case 0x40:
	key += 0x20;
	keyqual = GDK_SHIFT_MASK;
	break;
      default:
	keyqual = 0;
	break;
    }
    gtk_widget_add_accelerator(menuitem,
			       "activate", fe->accelgroup,
			       key, keyqual,
			       GTK_ACCEL_VISIBLE);
    gtk_widget_show(menuitem);
    return menuitem;
}