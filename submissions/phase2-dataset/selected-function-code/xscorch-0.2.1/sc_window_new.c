sc_window *sc_window_new(sc_config *c, __libj_unused int argc, __libj_unused char **argv) {

   sc_window_gtk *w;
   GtkWidget *cont;
   GdkColor black;
   gint   fake_argc = 1;
   gchar* fake_argv[] = { "xscorch", NULL };
   char **fake_argv_p;

   /* initialise GTK */
   fake_argv_p = fake_argv;
   gtk_init(&fake_argc, &fake_argv_p);

   /* Initialise w */
   w = g_new0(sc_window_gtk, 1);
   c->window = (sc_window *)w;
   w->exposed = FALSE;
   w->ready = FALSE;
   w->statenabled = TRUE;
   w->chatbox = NULL;
   w->windarrowsize = 0;
   w->state = 0;
   w->c = c;

   /* Load the necessary fonts */
   sc_window_load_fonts(w);

   /* Create the window */
   w->app = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(w->app), "XScorch");
   g_signal_connect(G_OBJECT(w->app), "delete_event",
                    (GCallback)_sc_delete_event_gtk, w);
   gtk_window_set_resizable(GTK_WINDOW(w->app), FALSE);

   /* Create the main (vertical) container */
   cont = gtk_vbox_new(FALSE, 0);
   gtk_container_set_border_width(GTK_CONTAINER(cont), 1);
   gtk_container_add(GTK_CONTAINER(w->app), cont);

   /* Create menus, etc */
   sc_window_create_menus_gtk(w);
   gtk_box_pack_start(GTK_BOX(cont), w->mainmenu, TRUE, TRUE, 0);

   /* Next comes the top statusbar */
   w->status = sc_window_active_console_new(w, 0, 0, 1, 1, CONSOLE_BORDERLESS);
   sc_status_configure_gtk(w);
   sc_status_setup((sc_window *)w);
   gtk_box_pack_start(GTK_BOX(cont), w->status, FALSE, TRUE, 1);

   /* Create drawable screen area */
   /*w->border = sc_display_new(w->c->fieldwidth + 4, w->c->fieldheight + 4);
   gtk_box_pack_start(GTK_BOX(cont), w->border, TRUE, TRUE, 0);*/
   w->screen = sc_display_new(w->c->fieldwidth, w->c->fieldheight);
   g_signal_connect(G_OBJECT(sc_display_get_drawbuf(SC_DISPLAY(w->screen))),
                    "expose_event", (GCallback)_sc_screen_expose_gtk, w);
   /*gtk_fixed_put(GTK_FIXED(w->border), w->screen, 2, 2);*/
   gtk_box_pack_start(GTK_BOX(cont), w->screen, TRUE, TRUE, 0);

   /* Please initialise the colormaps */
   w->colormap = sc_colormap_new_gtk();
   sc_colormap_alloc_colors_gtk(w);

   /* Show everything */
   gtk_widget_show_all(w->app);

   /* Setup display background color */
   gdk_color_black(gtk_widget_get_colormap(w->screen), &black);
   gdk_window_set_background(w->screen->window, &black);

   /* Setup the offscreen land buffer */
   w->landbuffer = gdk_pixmap_new(w->app->window, w->c->fieldwidth, w->c->fieldheight, -1);

   /* Setup explosion cache */
   w->explcache = sc_expl_cache_new_gtk();

   /* Load the images */
   if(!_sc_load_images(w)) return(NULL);
   gdk_window_set_icon(w->app->window, NULL, w->icon, w->icon_m);
   w->ready = TRUE;

   /* connect after loading, just to retain sanity. */
   g_signal_connect(G_OBJECT(w->app), "key_press_event",
                    (GCallback)_sc_window_keypress_gtk, w);

   /* Return the structure */
   return((sc_window *)w);

}
