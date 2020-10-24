void on_PrefsToolbarStyle_activate(GtkComboBox *combobox, gpointer user_data) {
#if HAVE_GTK3 == 0
    #if GTK_CHECK_VERSION(2,6,0)
    gchar *selection = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox));
    if (gconfconnect != NULL){
        if(g_ascii_strcasecmp(selection, _("both")) == 0){
             gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "both", NULL);
        } else if(g_ascii_strcasecmp(selection, _("icon")) == 0){
             gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "icon", NULL);
        } else if(g_ascii_strcasecmp(selection, _("text")) == 0){
             gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "text", NULL);
        } else {
             gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "both", NULL);
        }
    }
    g_free(selection);
    #else
    gint selection = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
    if(gconfconnect!= NULL){
        switch(selection){
        case 0:
            gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "both", NULL); 
            break;
        case 1:
            gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "icon", NULL); 
            break;
        case 2:
            gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "text", NULL); 
            break;
        default:
            gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", "both", NULL); 
            break;
        }        
    }
    #endif
#else
    gchar *selection = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox));
    if (gsettings_connect != NULL){
       
        if(g_ascii_strcasecmp(selection, _("both")) == 0){
             g_settings_set_string(gsettings_connect, "toolbarstyle", "both");
        } else if(g_ascii_strcasecmp(selection, _("icon")) == 0){
             g_settings_set_string(gsettings_connect, "toolbarstyle", "icon");
        } else if(g_ascii_strcasecmp(selection, _("text")) == 0){
             g_settings_set_string(gsettings_connect, "toolbarstyle", "text");
        } else {
             g_settings_set_string(gsettings_connect, "toolbarstyle", "both");
        }
        g_settings_sync();
    }
    g_free(selection);
#endif
} // end on_PrefsToolbarStyle_activate();
