
   ScLinkComboData *combo_data;
   ScLinkCombo *combo;
   GtkListStore *store;
   GtkTreeIter iter;
   GtkCellRenderer *cell;
   gint i;

   combo_data = (ScLinkComboData *)malloc(sizeof(ScLinkComboData));
   g_return_val_if_fail(combo_data != NULL, NULL);
   combo_data->entries = entries;
   combo_data->value = value;

   combo = g_object_new(sc_link_combo_get_type(), NULL);
   g_return_val_if_fail(combo != NULL, NULL);

   store = gtk_list_store_new(1, G_TYPE_STRING);
   for(i = 0; entries[i] != NULL; ++i) {
      gtk_list_store_insert_with_values(store, &iter, i, 0, (gchar *)entries[i], -1);
   }
   gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
   g_object_unref(G_OBJECT(store));

   cell = gtk_cell_renderer_text_new();
   gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), cell, TRUE);
   gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), cell, "text", 0, NULL);

   gtk_widget_set_size_request(GTK_WIDGET(combo), 130, -1);
   g_object_set_data(G_OBJECT(GTK_COMBO_BOX(combo)), "user_data", combo_data);
   gtk_combo_box_set_active(GTK_COMBO_BOX(combo), *value);

   return(GTK_WIDGET(combo));

}
