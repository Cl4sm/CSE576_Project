static frontend *new_window(char *arg, int argtype, char **error)
{
    frontend *fe;
    GtkBox *vbox, *hbox;
    GtkWidget *menu, *menuitem;
    GdkPixmap *iconpm;
    GList *iconlist;
    int x, y, n;
    char errbuf[1024];
    extern char *const *const xpm_icons[];
    extern const int n_xpm_icons;

    fe = snew(frontend);

    fe->timer_active = FALSE;
    fe->timer_id = -1;

    fe->me = midend_new(fe, &thegame, &gtk_drawing, fe);

    if (arg) {
	char *err;
	FILE *fp;

	errbuf[0] = '\0';

	switch (argtype) {
	  case ARG_ID:
	    err = midend_game_id(fe->me, arg);
	    if (!err)
		midend_new_game(fe->me);
	    else
		sprintf(errbuf, "Invalid game ID: %.800s", err);
	    break;
	  case ARG_SAVE:
	    fp = fopen(arg, "r");
	    if (!fp) {
		sprintf(errbuf, "Error opening file: %.800s", strerror(errno));
	    } else {
		err = midend_deserialise(fe->me, savefile_read, fp);
                if (err)
                    sprintf(errbuf, "Invalid save file: %.800s", err);
                fclose(fp);
	    }
	    break;
	  default /*case ARG_EITHER*/:
	    /*
	     * First try treating the argument as a game ID.
	     */
	    err = midend_game_id(fe->me, arg);
	    if (!err) {
		/*
		 * It's a valid game ID.
		 */
		midend_new_game(fe->me);
	    } else {
		FILE *fp = fopen(arg, "r");
		if (!fp) {
		    sprintf(errbuf, "Supplied argument is neither a game ID (%.400s)"
			    " nor a save file (%.400s)", err, strerror(errno));
		} else {
		    err = midend_deserialise(fe->me, savefile_read, fp);
		    if (err)
			sprintf(errbuf, "%.800s", err);
		    fclose(fp);
		}
	    }
	    break;
	}
	if (*errbuf) {
	    *error = dupstr(errbuf);
	    midend_free(fe->me);
	    sfree(fe);
	    return NULL;
	}

    } else {
	midend_new_game(fe->me);
    }

    fe->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(fe->window), thegame.name);

    vbox = GTK_BOX(gtk_vbox_new(FALSE, 0));
    gtk_container_add(GTK_CONTAINER(fe->window), GTK_WIDGET(vbox));
    gtk_widget_show(GTK_WIDGET(vbox));

    fe->accelgroup = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(fe->window), fe->accelgroup);

    hbox = GTK_BOX(gtk_hbox_new(FALSE, 0));
    gtk_box_pack_start(vbox, GTK_WIDGET(hbox), FALSE, FALSE, 0);
    gtk_widget_show(GTK_WIDGET(hbox));

    fe->menubar = gtk_menu_bar_new();
    gtk_box_pack_start(hbox, fe->menubar, TRUE, TRUE, 0);
    gtk_widget_show(fe->menubar);

    menuitem = gtk_menu_item_new_with_mnemonic("_Game");
    gtk_container_add(GTK_CONTAINER(fe->menubar), menuitem);
    gtk_widget_show(menuitem);

    menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);

    add_menu_item_with_key(fe, GTK_CONTAINER(menu), "New", 'n');

    menuitem = gtk_menu_item_new_with_label("Restart");
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_restart_event), fe);
    gtk_widget_show(menuitem);

    menuitem = gtk_menu_item_new_with_label("Specific...");
    gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
			GINT_TO_POINTER(CFG_DESC));
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_config_event), fe);
    gtk_widget_show(menuitem);

    menuitem = gtk_menu_item_new_with_label("Random Seed...");
    gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
			GINT_TO_POINTER(CFG_SEED));
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_config_event), fe);
    gtk_widget_show(menuitem);

    fe->preset_radio = NULL;
    fe->preset_custom = NULL;
    fe->n_preset_menu_items = 0;
    fe->preset_threaded = FALSE;
    if ((n = midend_num_presets(fe->me)) > 0 || thegame.can_configure) {
        GtkWidget *submenu;
        int i;

        menuitem = gtk_menu_item_new_with_mnemonic("_Type");
        gtk_container_add(GTK_CONTAINER(fe->menubar), menuitem);
        gtk_widget_show(menuitem);

        submenu = gtk_menu_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);

        for (i = 0; i < n; i++) {
            char *name;
            game_params *params;

            midend_fetch_preset(fe->me, i, &name, &params);

	    menuitem =
		gtk_radio_menu_item_new_with_label(fe->preset_radio, name);
	    fe->preset_radio =
		gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM(menuitem));
	    fe->n_preset_menu_items++;
            gtk_container_add(GTK_CONTAINER(submenu), menuitem);
            gtk_object_set_data(GTK_OBJECT(menuitem), "user-data", params);
            gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                               GTK_SIGNAL_FUNC(menu_preset_event), fe);
            gtk_widget_show(menuitem);
        }

	if (thegame.can_configure) {
	    menuitem = fe->preset_custom =
		gtk_radio_menu_item_new_with_label(fe->preset_radio,
						   "Custom...");
	    fe->preset_radio =
		gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM(menuitem));
            gtk_container_add(GTK_CONTAINER(submenu), menuitem);
            gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
				GPOINTER_TO_INT(CFG_SETTINGS));
            gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                               GTK_SIGNAL_FUNC(menu_config_event), fe);
            gtk_widget_show(menuitem);
	}

    }

    add_menu_separator(GTK_CONTAINER(menu));
    menuitem = gtk_menu_item_new_with_label("Load...");
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_load_event), fe);
    gtk_widget_show(menuitem);
    menuitem = gtk_menu_item_new_with_label("Save...");
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_save_event), fe);
    gtk_widget_show(menuitem);
#ifndef STYLUS_BASED
    add_menu_separator(GTK_CONTAINER(menu));
    add_menu_item_with_key(fe, GTK_CONTAINER(menu), "Undo", 'u');
    add_menu_item_with_key(fe, GTK_CONTAINER(menu), "Redo", 'r');
#endif
    if (thegame.can_format_as_text_ever) {
	add_menu_separator(GTK_CONTAINER(menu));
	menuitem = gtk_menu_item_new_with_label("Copy");
	gtk_container_add(GTK_CONTAINER(menu), menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
			   GTK_SIGNAL_FUNC(menu_copy_event), fe);
	gtk_widget_show(menuitem);
	fe->copy_menu_item = menuitem;
    } else {
	fe->copy_menu_item = NULL;
    }
    if (thegame.can_solve) {
	add_menu_separator(GTK_CONTAINER(menu));
	menuitem = gtk_menu_item_new_with_label("Solve");
	gtk_container_add(GTK_CONTAINER(menu), menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
			   GTK_SIGNAL_FUNC(menu_solve_event), fe);
	gtk_widget_show(menuitem);
    }
    add_menu_separator(GTK_CONTAINER(menu));
    add_menu_item_with_key(fe, GTK_CONTAINER(menu), "Exit", 'q');

    menuitem = gtk_menu_item_new_with_mnemonic("_Help");
    gtk_container_add(GTK_CONTAINER(fe->menubar), menuitem);
    gtk_widget_show(menuitem);

    menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);

    menuitem = gtk_menu_item_new_with_label("Contents");
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_help_contents_event), fe);
    gtk_widget_show(menuitem);

    if (thegame.htmlhelp_topic) {
	char *item;
	assert(thegame.name);
	item = snewn(9+strlen(thegame.name), char); /*ick*/
	sprintf(item, "Help on %s", thegame.name);
	menuitem = gtk_menu_item_new_with_label(item);
	sfree(item);
	gtk_container_add(GTK_CONTAINER(menu), menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
			   GTK_SIGNAL_FUNC(menu_help_specific_event), fe);
	gtk_widget_show(menuitem);
    }

    menuitem = gtk_menu_item_new_with_label("About");
    gtk_container_add(GTK_CONTAINER(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(menu_about_event), fe);
    gtk_widget_show(menuitem);

#ifdef STYLUS_BASED
    menuitem=gtk_button_new_with_mnemonic("_Redo");
    gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
			GINT_TO_POINTER((int)('r')));
    gtk_signal_connect(GTK_OBJECT(menuitem), "clicked",
		       GTK_SIGNAL_FUNC(menu_key_event), fe);
    gtk_box_pack_end(hbox, menuitem, FALSE, FALSE, 0);
    gtk_widget_show(menuitem);

    menuitem=gtk_button_new_with_mnemonic("_Undo");
    gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
			GINT_TO_POINTER((int)('u')));
    gtk_signal_connect(GTK_OBJECT(menuitem), "clicked",
		       GTK_SIGNAL_FUNC(menu_key_event), fe);
    gtk_box_pack_end(hbox, menuitem, FALSE, FALSE, 0);
    gtk_widget_show(menuitem);

    if (thegame.flags & REQUIRE_NUMPAD) {
	hbox = GTK_BOX(gtk_hbox_new(FALSE, 0));
	gtk_box_pack_start(vbox, GTK_WIDGET(hbox), FALSE, FALSE, 0);
	gtk_widget_show(GTK_WIDGET(hbox));

	*((int*)errbuf)=0;
	errbuf[1]='\0';
	for(errbuf[0]='0';errbuf[0]<='9';errbuf[0]++) {
	    menuitem=gtk_button_new_with_label(errbuf);
	    gtk_object_set_data(GTK_OBJECT(menuitem), "user-data",
				GINT_TO_POINTER((int)(errbuf[0])));
	    gtk_signal_connect(GTK_OBJECT(menuitem), "clicked",
			       GTK_SIGNAL_FUNC(menu_key_event), fe);
	    gtk_box_pack_start(hbox, menuitem, TRUE, TRUE, 0);
	    gtk_widget_show(menuitem);
	}
    }
#endif /* STYLUS_BASED */

    changed_preset(fe);

    snaffle_colours(fe);

    if (midend_wants_statusbar(fe->me)) {
	GtkWidget *viewport;
	GtkRequisition req;

	viewport = gtk_viewport_new(NULL, NULL);
	gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);
	fe->statusbar = gtk_statusbar_new();
	gtk_container_add(GTK_CONTAINER(viewport), fe->statusbar);
	gtk_widget_show(viewport);
	gtk_box_pack_end(vbox, viewport, FALSE, FALSE, 0);
	gtk_widget_show(fe->statusbar);
	fe->statusctx = gtk_statusbar_get_context_id
	    (GTK_STATUSBAR(fe->statusbar), "game");
	gtk_statusbar_push(GTK_STATUSBAR(fe->statusbar), fe->statusctx,
			   "test");
	gtk_widget_size_request(fe->statusbar, &req);
#if 0
	/* For GTK 2.0, should we be using gtk_widget_set_size_request? */
#endif
	gtk_widget_set_usize(viewport, -1, req.height);
    } else
	fe->statusbar = NULL;

    fe->area = gtk_drawing_area_new();
#if GTK_CHECK_VERSION(2,0,0)
    GTK_WIDGET_UNSET_FLAGS(fe->area, GTK_DOUBLE_BUFFERED);
#endif
    get_size(fe, &x, &y);
    fe->drawing_area_shrink_pending = FALSE;
    gtk_drawing_area_size(GTK_DRAWING_AREA(fe->area), x, y);
    fe->w = x;
    fe->h = y;

    gtk_box_pack_end(vbox, fe->area, TRUE, TRUE, 0);

    clear_backing_store(fe);
    fe->fonts = NULL;
    fe->nfonts = fe->fontsize = 0;

    fe->paste_data = NULL;
    fe->paste_data_len = 0;

    gtk_signal_connect(GTK_OBJECT(fe->window), "destroy",
		       GTK_SIGNAL_FUNC(destroy), fe);
    gtk_signal_connect(GTK_OBJECT(fe->window), "key_press_event",
		       GTK_SIGNAL_FUNC(key_event), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "button_press_event",
		       GTK_SIGNAL_FUNC(button_event), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "button_release_event",
		       GTK_SIGNAL_FUNC(button_event), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "motion_notify_event",
		       GTK_SIGNAL_FUNC(motion_event), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "selection_get",
		       GTK_SIGNAL_FUNC(selection_get), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "selection_clear_event",
		       GTK_SIGNAL_FUNC(selection_clear), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "expose_event",
		       GTK_SIGNAL_FUNC(expose_area), fe);
    gtk_signal_connect(GTK_OBJECT(fe->window), "map_event",
		       GTK_SIGNAL_FUNC(map_window), fe);
    gtk_signal_connect(GTK_OBJECT(fe->area), "configure_event",
		       GTK_SIGNAL_FUNC(configure_area), fe);
    gtk_signal_connect(GTK_OBJECT(fe->window), "configure_event",
		       GTK_SIGNAL_FUNC(configure_window), fe);

    gtk_widget_add_events(GTK_WIDGET(fe->area),
                          GDK_BUTTON_PRESS_MASK |
                          GDK_BUTTON_RELEASE_MASK |
			  GDK_BUTTON_MOTION_MASK |
			  GDK_POINTER_MOTION_HINT_MASK);

    if (n_xpm_icons) {
	gtk_widget_realize(fe->window);
	iconpm = gdk_pixmap_create_from_xpm_d(fe->window->window, NULL,
					      NULL, (gchar **)xpm_icons[0]);
	gdk_window_set_icon(fe->window->window, NULL, iconpm, NULL);
	iconlist = NULL;
	for (n = 0; n < n_xpm_icons; n++) {
	    iconlist =
		g_list_append(iconlist,
			      gdk_pixbuf_new_from_xpm_data((const gchar **)
							   xpm_icons[n]));
	}
	gdk_window_set_icon_list(fe->window->window, iconlist);
    }

    gtk_widget_show(fe->area);
    gtk_widget_show(fe->window);

    fe->drawing_area_shrink_pending = TRUE;
    try_shrink_drawing_area(fe);
    set_window_background(fe, 0);

    return fe;
}