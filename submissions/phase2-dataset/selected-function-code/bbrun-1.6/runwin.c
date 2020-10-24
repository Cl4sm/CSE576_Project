int runwin()
{
  GtkWidget *window;
  GtkWidget *cancelButton;
  GtkWidget *okButton;
  GtkWidget *table;
  GtkWidget *buttonPanel;
  GList *combo_items = NULL;
  int c = 0;
  int windowRows, windowCols;
  char *pwd;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "Run");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10); 	// Sets the border width of the window


  // Create a text entry area with drop down history list
  combo = gtk_combo_new();
  gtk_widget_set_usize(combo, 300, 30);
  gtk_combo_set_use_arrows_always(GTK_COMBO(combo), TRUE); 	// scroll through the list with the arrow keys.
  gtk_signal_connect(GTK_OBJECT(GTK_COMBO(combo)->entry), "activate", GTK_SIGNAL_FUNC(callback), "ok");
  gtk_window_set_focus(GTK_WINDOW(window), GTK_COMBO(combo)->entry); // focus the entry area so we can type right away.

  // Instead of appending the items in reverse order, we prepend each item.
  // This fixes a problem with the arrow keys cycling through the list,
  // You would only be able to go up the list from oldest item to newest.
  histTOC2 = histTOC;
  for (c = 0; c < historyLength; c++) {
    combo_items = g_list_prepend(combo_items, (char *) *histTOC2);
    histTOC2++;
  }

  combo_items = g_list_prepend(combo_items, "");
  gtk_combo_set_popdown_strings(GTK_COMBO(combo), combo_items);
  g_list_free(combo_items);

  // Create the Cancel button
  cancelButton = gtk_button_new_with_label("Cancel");
  gtk_signal_connect(GTK_OBJECT(cancelButton), "clicked", GTK_SIGNAL_FUNC(callback), (gpointer) "cancel");
  gtk_signal_connect_object(GTK_OBJECT(cancelButton), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(window));

  // Create the Ok button
  okButton = gtk_button_new_with_label("OK");
  gtk_signal_connect(GTK_OBJECT(okButton), "clicked", GTK_SIGNAL_FUNC(callback), (gpointer) "ok");
  gtk_signal_connect_object(GTK_OBJECT(okButton), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(window));

  if (advancedMode) {
    windowRows = ADVANCED_WINDOW_ROWS;
    windowCols = ADVANCED_WINDOW_COLS;
  } else {
    windowRows = SIMPLE_WINDOW_ROWS;
    windowCols = SIMPLE_WINDOW_COLS;
  }

  table = gtk_table_new(windowRows, windowCols, FALSE); // Create a table to pack things in

  // Group the buttons together for ease of placement
  buttonPanel = gtk_hbox_new(FALSE, 0);
  gtk_container_set_border_width(GTK_CONTAINER(buttonPanel), 1);
  gtk_container_add(GTK_CONTAINER(buttonPanel), cancelButton);
  gtk_container_add(GTK_CONTAINER(buttonPanel), okButton);
  gtk_widget_show(okButton);
  gtk_widget_show(cancelButton);

  // Place the buttons and the combo box
  gtk_table_attach_defaults(GTK_TABLE(table), combo, 0, windowCols, 0, 1);
  gtk_widget_show(combo);
  gtk_table_attach_defaults(GTK_TABLE(table), buttonPanel, windowCols - 6, windowCols, windowRows - 1, windowRows);
  gtk_widget_show(buttonPanel);

  if (advancedMode) {
    GtkWidget *workingDirectoryFrame;
    GtkWidget *workingDirectoryBoxLayout;

    // Create and set up the 'working directory' text box
    workingDirectoryFrame = gtk_frame_new(" Working Directory ");
    gtk_table_attach_defaults(GTK_TABLE(table), workingDirectoryFrame, 0, windowCols, 1, 2);
    gtk_widget_show(workingDirectoryFrame);

    workingDirectoryBoxLayout = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(workingDirectoryBoxLayout), 2);
    gtk_container_add(GTK_CONTAINER(workingDirectoryFrame), workingDirectoryBoxLayout);
    gtk_widget_show(workingDirectoryBoxLayout);

    workingDirectoryEntry = gtk_entry_new_with_max_length(MAXPATHLEN);

    if ((pwd = getenv("PWD")))	// In case PWD is not availalbe
      gtk_entry_set_text(GTK_ENTRY(workingDirectoryEntry), pwd);

    gtk_entry_set_editable(GTK_ENTRY(workingDirectoryEntry), TRUE);
    gtk_entry_select_region(GTK_ENTRY(workingDirectoryEntry), 0, GTK_ENTRY(workingDirectoryEntry)->text_length);
    gtk_container_add(GTK_CONTAINER(workingDirectoryBoxLayout), workingDirectoryEntry);
    gtk_widget_show(workingDirectoryEntry);
  }

  gtk_container_add(GTK_CONTAINER(window), table);	// Put the table in the main window
  gtk_widget_show(table);
  gtk_widget_show(window);

  g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(key_pressed), NULL);
  g_signal_connect (GTK_OBJECT(window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

  GTK_WIDGET_SET_FLAGS (okButton, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (okButton);

  gtk_main();					// Rest in gtk_main and wait for the fun to begin!

  return 0;
}
