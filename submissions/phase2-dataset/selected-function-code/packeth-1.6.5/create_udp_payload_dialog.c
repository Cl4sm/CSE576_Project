GtkWidget*
create_udp_payload_dialog (void)
{
  GtkWidget *udp_payload_dialog;
  GtkWidget *dialog_vbox4;
  GtkWidget *notebook6;
  GtkWidget *frame26;
  GtkWidget *vbox54;
  GtkWidget *table2;
  GtkWidget *label210;
  GtkWidget *label211;
  GtkWidget *label212;
  GtkWidget *label213;
  GtkWidget *label202;
  GtkWidget *label203;
  GtkWidget *label204;
  GtkWidget *label205;
  GtkWidget *label206;
  GtkWidget *label214;
  GtkWidget *radiobutton26;
  GSList *radiobutton26_group = NULL;
  GtkWidget *radiobutton24;
  GSList *radiobutton24_group = NULL;
  GtkWidget *radiobutton27;
  GSList *radiobutton27_group = NULL;
  GtkWidget *radiobutton23;
  GtkWidget *label199;
  GtkWidget *label198;
  GtkWidget *label197;
  GtkWidget *label196;
  GtkWidget *label195;
  GtkWidget *radiobutton28;
  GtkWidget *label217;
  GtkWidget *label218;
  GtkWidget *radiobutton25;
  GtkWidget *entry98;
  GtkWidget *entry99;
  GtkWidget *hbox84;
  GtkWidget *entry101;
  GtkWidget *label220;
  GtkWidget *hbox85;
  GtkWidget *entry92;
  GtkWidget *label221;
  GtkWidget *hbox86;
  GtkWidget *entry91;
  GtkWidget *label222;
  GtkWidget *hbox87;
  GtkWidget *entry102;
  GtkWidget *optionmenu7;
  GtkWidget *convertwidget122;
  GtkWidget *convertwidget123;
  GtkWidget *convertwidget124;
  GtkWidget *convertwidget125;
  GtkWidget *convertwidget126;
  GtkWidget *convertwidget127;
  GtkWidget *hbox93;
  GtkWidget *entry97;
  GtkWidget *label232;
  GtkWidget *hbox94;
  GtkWidget *entry96;
  GtkWidget *label233;
  GtkWidget *hseparator4;
  GtkWidget *label216;
  GtkWidget *frame27;
  GtkWidget *hbox95;
  GtkWidget *label234;
  GtkWidget *entry103;
  GtkWidget *frame38;
  GtkWidget *vbox61;
  GtkWidget *hbox88;
  GtkWidget *label223;
  GtkWidget *entry104;
  GtkWidget *label227;
  GtkWidget *label225;
  GtkWidget *label226;
  GtkWidget *entry106;
  GtkWidget *label250;
  GtkWidget *hbox100;
  GtkWidget *radiobutton33;
  GSList *radiobutton33_group = NULL;
  GtkWidget *radiobutton32;
  GtkWidget *optionmenu12;
  GtkWidget *convertwidget128;
  GtkWidget *convertwidget129;
  GtkWidget *convertwidget130;
  GtkWidget *convertwidget131;
  GtkWidget *rtp_apply_button;
  GtkWidget *label180;
  GtkWidget *dialog_action_area4;
  GtkWidget *hbox77;
  GtkWidget *cancel_rtp_bt;
  GtkWidget *rtp_ok_bt;

  udp_payload_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (udp_payload_dialog), _("Select UDP payload"));
  gtk_window_set_type_hint (GTK_WINDOW (udp_payload_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox4 = GTK_DIALOG (udp_payload_dialog)->vbox;
  gtk_widget_show (dialog_vbox4);

  notebook6 = gtk_notebook_new ();
  gtk_widget_show (notebook6);
  gtk_box_pack_start (GTK_BOX (dialog_vbox4), notebook6, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (notebook6), 10);

  frame26 = gtk_frame_new (NULL);
  gtk_widget_show (frame26);
  gtk_container_add (GTK_CONTAINER (notebook6), frame26);
  gtk_container_set_border_width (GTK_CONTAINER (frame26), 10);

  vbox54 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox54);
  gtk_container_add (GTK_CONTAINER (frame26), vbox54);

  table2 = gtk_table_new (7, 6, FALSE);
  gtk_widget_show (table2);
  gtk_box_pack_start (GTK_BOX (vbox54), table2, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (table2), 10);
  gtk_table_set_row_spacings (GTK_TABLE (table2), 2);
  gtk_table_set_col_spacings (GTK_TABLE (table2), 5);

  label210 = gtk_label_new (_(" "));
  gtk_widget_show (label210);
  gtk_table_attach (GTK_TABLE (table2), label210, 5, 6, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label210), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label210), 0, 0.5);

  label211 = gtk_label_new (_(" "));
  gtk_widget_show (label211);
  gtk_table_attach (GTK_TABLE (table2), label211, 5, 6, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label211), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label211), 0, 0.5);

  label212 = gtk_label_new (_(" "));
  gtk_widget_show (label212);
  gtk_table_attach (GTK_TABLE (table2), label212, 5, 6, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label212), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label212), 0, 0.5);

  label213 = gtk_label_new (_(" "));
  gtk_widget_show (label213);
  gtk_table_attach (GTK_TABLE (table2), label213, 5, 6, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label213), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label213), 0, 0.5);

  label202 = gtk_label_new (_("               Payload type   "));
  gtk_widget_show (label202);
  gtk_table_attach (GTK_TABLE (table2), label202, 3, 4, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label202), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label202), 0, 0.5);

  label203 = gtk_label_new (_("            Sequence nr "));
  gtk_widget_show (label203);
  gtk_table_attach (GTK_TABLE (table2), label203, 3, 4, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label203), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label203), 0, 0.5);

  label204 = gtk_label_new (_("              Timestamp       "));
  gtk_widget_show (label204);
  gtk_table_attach (GTK_TABLE (table2), label204, 3, 4, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label204), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label204), 0, 0.5);

  label205 = gtk_label_new (_("             SSRC            "));
  gtk_widget_show (label205);
  gtk_table_attach (GTK_TABLE (table2), label205, 3, 4, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label205), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label205), 0, 0.5);

  label206 = gtk_label_new (_(" "));
  gtk_widget_show (label206);
  gtk_table_attach (GTK_TABLE (table2), label206, 3, 4, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label206), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label206), 0, 0.5);

  label214 = gtk_label_new (_(" "));
  gtk_widget_show (label214);
  gtk_table_attach (GTK_TABLE (table2), label214, 4, 5, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label214), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label214), 0, 0.5);

  radiobutton26 = gtk_radio_button_new_with_mnemonic (NULL, _("Flase"));
  gtk_widget_show (radiobutton26);
  gtk_table_attach (GTK_TABLE (table2), radiobutton26, 2, 3, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton26), radiobutton26_group);
  radiobutton26_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton26));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton26), TRUE);

  radiobutton24 = gtk_radio_button_new_with_mnemonic (NULL, _("no"));
  gtk_widget_show (radiobutton24);
  gtk_table_attach (GTK_TABLE (table2), radiobutton24, 2, 3, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton24), radiobutton24_group);
  radiobutton24_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton24));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton24), TRUE);

  radiobutton27 = gtk_radio_button_new_with_mnemonic (NULL, _("yes"));
  gtk_widget_show (radiobutton27);
  gtk_table_attach (GTK_TABLE (table2), radiobutton27, 1, 2, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton27), radiobutton27_group);
  radiobutton27_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton27));

  radiobutton23 = gtk_radio_button_new_with_mnemonic (NULL, _("yes"));
  gtk_widget_show (radiobutton23);
  gtk_table_attach (GTK_TABLE (table2), radiobutton23, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton23), radiobutton24_group);
  radiobutton24_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton23));

  label199 = gtk_label_new (_("Marker        "));
  gtk_widget_show (label199);
  gtk_table_attach (GTK_TABLE (table2), label199, 0, 1, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label199), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label199), 0, 0.5);

  label198 = gtk_label_new (_("CSRC          "));
  gtk_widget_show (label198);
  gtk_table_attach (GTK_TABLE (table2), label198, 0, 1, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label198), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label198), 0, 0.5);

  label197 = gtk_label_new (_("Extension     "));
  gtk_widget_show (label197);
  gtk_table_attach (GTK_TABLE (table2), label197, 0, 1, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label197), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label197), 0, 0.5);

  label196 = gtk_label_new (_("Padding       "));
  gtk_widget_show (label196);
  gtk_table_attach (GTK_TABLE (table2), label196, 0, 1, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label196), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label196), 0, 0.5);

  label195 = gtk_label_new (_("Version       "));
  gtk_widget_show (label195);
  gtk_table_attach (GTK_TABLE (table2), label195, 0, 1, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label195), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label195), 0, 0.5);

  radiobutton28 = gtk_radio_button_new_with_mnemonic (NULL, _("no"));
  gtk_widget_show (radiobutton28);
  gtk_table_attach (GTK_TABLE (table2), radiobutton28, 2, 3, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton28), radiobutton27_group);
  radiobutton27_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton28));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton28), TRUE);

  label217 = gtk_label_new (_("CSRC      0x"));
  gtk_widget_show (label217);
  gtk_table_attach (GTK_TABLE (table2), label217, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label217), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label217), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label217), 0, 25);

  label218 = gtk_label_new (_("               Extension    0x"));
  gtk_widget_show (label218);
  gtk_table_attach (GTK_TABLE (table2), label218, 3, 4, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label218), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label218), 0, 0.5);

  radiobutton25 = gtk_radio_button_new_with_mnemonic (NULL, _("yes"));
  gtk_widget_show (radiobutton25);
  gtk_table_attach (GTK_TABLE (table2), radiobutton25, 1, 2, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton25), radiobutton26_group);
  radiobutton26_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton25));

  entry98 = gtk_entry_new ();
  gtk_widget_show (entry98);
  gtk_table_attach (GTK_TABLE (table2), entry98, 1, 3, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry98), 32);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry98), 8226);

  entry99 = gtk_entry_new ();
  gtk_widget_show (entry99);
  gtk_table_attach (GTK_TABLE (table2), entry99, 4, 6, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry99), 32);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry99), 8226);

  hbox84 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox84);
  gtk_table_attach (GTK_TABLE (table2), hbox84, 4, 5, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry101 = gtk_entry_new ();
  gtk_widget_show (entry101);
  gtk_box_pack_start (GTK_BOX (hbox84), entry101, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry101), 5);
  gtk_entry_set_text (GTK_ENTRY (entry101), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry101), 8226);

  label220 = gtk_label_new (_(" "));
  gtk_widget_show (label220);
  gtk_box_pack_start (GTK_BOX (hbox84), label220, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label220), GTK_JUSTIFY_CENTER);

  hbox85 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox85);
  gtk_table_attach (GTK_TABLE (table2), hbox85, 1, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry92 = gtk_entry_new ();
  gtk_widget_show (entry92);
  gtk_box_pack_start (GTK_BOX (hbox85), entry92, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry92), 2);
  gtk_entry_set_text (GTK_ENTRY (entry92), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry92), 8226);

  label221 = gtk_label_new ("");
  gtk_widget_show (label221);
  gtk_box_pack_start (GTK_BOX (hbox85), label221, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label221), GTK_JUSTIFY_CENTER);

  hbox86 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox86);
  gtk_table_attach (GTK_TABLE (table2), hbox86, 1, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry91 = gtk_entry_new ();
  gtk_widget_show (entry91);
  gtk_box_pack_start (GTK_BOX (hbox86), entry91, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry91), 1);
  gtk_entry_set_text (GTK_ENTRY (entry91), _("2"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry91), 8226);

  label222 = gtk_label_new ("");
  gtk_widget_show (label222);
  gtk_box_pack_start (GTK_BOX (hbox86), label222, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label222), GTK_JUSTIFY_CENTER);

  hbox87 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox87);
  gtk_table_attach (GTK_TABLE (table2), hbox87, 4, 6, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry102 = gtk_entry_new ();
  gtk_widget_show (entry102);
  gtk_box_pack_start (GTK_BOX (hbox87), entry102, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry102), 3);
  gtk_entry_set_text (GTK_ENTRY (entry102), _("8"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry102), 8226);

  optionmenu7 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu7);
  gtk_box_pack_start (GTK_BOX (hbox87), optionmenu7, FALSE, FALSE, 10);

  convertwidget122 = gtk_menu_new ();

  convertwidget123 = gtk_menu_item_new_with_mnemonic (_("G.711 Alaw"));
  gtk_widget_show (convertwidget123);
  gtk_container_add (GTK_CONTAINER (convertwidget122), convertwidget123);

  convertwidget124 = gtk_menu_item_new_with_mnemonic (_("G.711 ulaw"));
  gtk_widget_show (convertwidget124);
  gtk_container_add (GTK_CONTAINER (convertwidget122), convertwidget124);

  convertwidget125 = gtk_menu_item_new_with_mnemonic (_("G.723.1"));
  gtk_widget_show (convertwidget125);
  gtk_container_add (GTK_CONTAINER (convertwidget122), convertwidget125);

  convertwidget126 = gtk_menu_item_new_with_mnemonic (_("G.729"));
  gtk_widget_show (convertwidget126);
  gtk_container_add (GTK_CONTAINER (convertwidget122), convertwidget126);

  convertwidget127 = gtk_menu_item_new_with_mnemonic (_("Other"));
  gtk_widget_show (convertwidget127);
  gtk_container_add (GTK_CONTAINER (convertwidget122), convertwidget127);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu7), convertwidget122);

  hbox93 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox93);
  gtk_table_attach (GTK_TABLE (table2), hbox93, 4, 5, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry97 = gtk_entry_new ();
  gtk_widget_show (entry97);
  gtk_box_pack_start (GTK_BOX (hbox93), entry97, TRUE, TRUE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry97), 10);
  gtk_entry_set_text (GTK_ENTRY (entry97), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry97), 8226);

  label232 = gtk_label_new (_("         "));
  gtk_widget_show (label232);
  gtk_box_pack_start (GTK_BOX (hbox93), label232, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label232), GTK_JUSTIFY_CENTER);

  hbox94 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox94);
  gtk_table_attach (GTK_TABLE (table2), hbox94, 4, 5, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry96 = gtk_entry_new ();
  gtk_widget_show (entry96);
  gtk_box_pack_start (GTK_BOX (hbox94), entry96, TRUE, TRUE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry96), 10);
  gtk_entry_set_text (GTK_ENTRY (entry96), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry96), 8226);

  label233 = gtk_label_new (_("         "));
  gtk_widget_show (label233);
  gtk_box_pack_start (GTK_BOX (hbox94), label233, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label233), GTK_JUSTIFY_CENTER);

  hseparator4 = gtk_hseparator_new ();
  gtk_widget_show (hseparator4);
  gtk_box_pack_start (GTK_BOX (vbox54), hseparator4, FALSE, FALSE, 0);

  label216 = gtk_label_new (_("RTP PAYLOAD  (put every byte as two hex characters without spaces)"));
  gtk_widget_show (label216);
  gtk_box_pack_start (GTK_BOX (vbox54), label216, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label216), GTK_JUSTIFY_CENTER);

  frame27 = gtk_frame_new (NULL);
  gtk_widget_show (frame27);
  gtk_box_pack_start (GTK_BOX (vbox54), frame27, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame27), 5);

  hbox95 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox95);
  gtk_container_add (GTK_CONTAINER (frame27), hbox95);

  label234 = gtk_label_new (_(" 0x  "));
  gtk_widget_show (label234);
  gtk_box_pack_start (GTK_BOX (hbox95), label234, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label234), GTK_JUSTIFY_CENTER);

  entry103 = gtk_entry_new ();
  gtk_widget_show (entry103);
  gtk_box_pack_start (GTK_BOX (hbox95), entry103, TRUE, TRUE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry103), 1460);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry103), 8226);

  frame38 = gtk_frame_new (NULL);
  gtk_widget_show (frame38);
  gtk_box_pack_start (GTK_BOX (vbox54), frame38, FALSE, FALSE, 7);
  gtk_container_set_border_width (GTK_CONTAINER (frame38), 10);

  vbox61 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox61);
  gtk_container_add (GTK_CONTAINER (frame38), vbox61);

  hbox88 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox88);
  gtk_box_pack_start (GTK_BOX (vbox61), hbox88, TRUE, TRUE, 5);

  label223 = gtk_label_new (_("   G.711:  Sin frequency ( 0 - 4000 Hz):   "));
  gtk_widget_show (label223);
  gtk_box_pack_start (GTK_BOX (hbox88), label223, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label223), GTK_JUSTIFY_CENTER);

  entry104 = gtk_entry_new ();
  gtk_widget_show (entry104);
  gtk_box_pack_start (GTK_BOX (hbox88), entry104, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry104), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry104), 8226);

  label227 = gtk_label_new (_("        "));
  gtk_widget_show (label227);
  gtk_box_pack_start (GTK_BOX (hbox88), label227, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label227), GTK_JUSTIFY_CENTER);

  label225 = gtk_label_new (_("  "));
  gtk_widget_show (label225);
  gtk_box_pack_start (GTK_BOX (hbox88), label225, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label225), GTK_JUSTIFY_CENTER);

  label226 = gtk_label_new (_("Length:  "));
  gtk_widget_show (label226);
  gtk_box_pack_start (GTK_BOX (hbox88), label226, FALSE, FALSE, 3);
  gtk_label_set_justify (GTK_LABEL (label226), GTK_JUSTIFY_CENTER);

  entry106 = gtk_entry_new ();
  gtk_widget_show (entry106);
  gtk_box_pack_start (GTK_BOX (hbox88), entry106, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry106), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry106), 8226);

  label250 = gtk_label_new (_("  "));
  gtk_widget_show (label250);
  gtk_box_pack_start (GTK_BOX (hbox88), label250, TRUE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label250), GTK_JUSTIFY_CENTER);

  hbox100 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox100);
  gtk_box_pack_start (GTK_BOX (vbox61), hbox100, TRUE, TRUE, 5);

  radiobutton33 = gtk_radio_button_new_with_mnemonic (NULL, _("Alaw"));
  gtk_widget_show (radiobutton33);
  gtk_box_pack_start (GTK_BOX (hbox100), radiobutton33, FALSE, FALSE, 47);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton33), radiobutton33_group);
  radiobutton33_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton33));

  radiobutton32 = gtk_radio_button_new_with_mnemonic (NULL, _("ulaw"));
  gtk_widget_show (radiobutton32);
  gtk_box_pack_start (GTK_BOX (hbox100), radiobutton32, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton32), radiobutton33_group);
  radiobutton33_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton32));

  optionmenu12 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu12);
  gtk_box_pack_start (GTK_BOX (hbox100), optionmenu12, FALSE, FALSE, 35);

  convertwidget128 = gtk_menu_new ();

  convertwidget129 = gtk_menu_item_new_with_mnemonic (_("Amplitude level low"));
  gtk_widget_show (convertwidget129);
  gtk_container_add (GTK_CONTAINER (convertwidget128), convertwidget129);

  convertwidget130 = gtk_menu_item_new_with_mnemonic (_("Amplitude level mid"));
  gtk_widget_show (convertwidget130);
  gtk_container_add (GTK_CONTAINER (convertwidget128), convertwidget130);

  convertwidget131 = gtk_menu_item_new_with_mnemonic (_("Amplitude level max"));
  gtk_widget_show (convertwidget131);
  gtk_container_add (GTK_CONTAINER (convertwidget128), convertwidget131);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu12), convertwidget128);

  rtp_apply_button = gtk_button_new_with_mnemonic (_("Apply into payload"));
  gtk_widget_show (rtp_apply_button);
  gtk_box_pack_start (GTK_BOX (hbox100), rtp_apply_button, FALSE, FALSE, 10);

  label180 = gtk_label_new (_("    RTP    "));
  gtk_widget_show (label180);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook6), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook6), 0), label180);
  gtk_label_set_justify (GTK_LABEL (label180), GTK_JUSTIFY_CENTER);

  dialog_action_area4 = GTK_DIALOG (udp_payload_dialog)->action_area;
  gtk_widget_show (dialog_action_area4);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area4), GTK_BUTTONBOX_END);

  hbox77 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox77);
  gtk_container_add (GTK_CONTAINER (dialog_action_area4), hbox77);

  cancel_rtp_bt = gtk_button_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (cancel_rtp_bt);
  gtk_box_pack_start (GTK_BOX (hbox77), cancel_rtp_bt, TRUE, FALSE, 0);

  rtp_ok_bt = gtk_button_new_with_mnemonic (_("OK"));
  gtk_widget_show (rtp_ok_bt);
  gtk_box_pack_start (GTK_BOX (hbox77), rtp_ok_bt, TRUE, FALSE, 0);

  g_signal_connect ((gpointer) udp_payload_dialog, "destroy",
                    G_CALLBACK (on_udp_payload_dialog_destroy),
                    NULL);
  g_signal_connect ((gpointer) rtp_apply_button, "clicked",
                    G_CALLBACK (on_rtp_apply_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) cancel_rtp_bt, "clicked",
                    G_CALLBACK (on_cancel_rtp_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) rtp_ok_bt, "clicked",
                    G_CALLBACK (on_rtp_ok_bt_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (udp_payload_dialog, udp_payload_dialog, "udp_payload_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (udp_payload_dialog, dialog_vbox4, "dialog_vbox4");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, notebook6, "notebook6");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, frame26, "frame26");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, vbox54, "vbox54");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, table2, "table2");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label210, "label210");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label211, "label211");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label212, "label212");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label213, "label213");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label202, "label202");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label203, "label203");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label204, "label204");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label205, "label205");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label206, "label206");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label214, "label214");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton26, "radiobutton26");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton24, "radiobutton24");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton27, "radiobutton27");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton23, "radiobutton23");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label199, "label199");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label198, "label198");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label197, "label197");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label196, "label196");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label195, "label195");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton28, "radiobutton28");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label217, "label217");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label218, "label218");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton25, "radiobutton25");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry98, "entry98");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry99, "entry99");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox84, "hbox84");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry101, "entry101");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label220, "label220");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox85, "hbox85");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry92, "entry92");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label221, "label221");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox86, "hbox86");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry91, "entry91");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label222, "label222");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox87, "hbox87");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry102, "entry102");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, optionmenu7, "optionmenu7");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget122, "convertwidget122");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget123, "convertwidget123");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget124, "convertwidget124");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget125, "convertwidget125");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget126, "convertwidget126");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget127, "convertwidget127");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox93, "hbox93");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry97, "entry97");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label232, "label232");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox94, "hbox94");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry96, "entry96");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label233, "label233");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hseparator4, "hseparator4");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label216, "label216");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, frame27, "frame27");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox95, "hbox95");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label234, "label234");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry103, "entry103");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, frame38, "frame38");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, vbox61, "vbox61");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox88, "hbox88");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label223, "label223");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry104, "entry104");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label227, "label227");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label225, "label225");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label226, "label226");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, entry106, "entry106");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label250, "label250");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox100, "hbox100");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton33, "radiobutton33");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, radiobutton32, "radiobutton32");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, optionmenu12, "optionmenu12");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget128, "convertwidget128");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget129, "convertwidget129");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget130, "convertwidget130");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, convertwidget131, "convertwidget131");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, rtp_apply_button, "rtp_apply_button");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, label180, "label180");
  GLADE_HOOKUP_OBJECT_NO_REF (udp_payload_dialog, dialog_action_area4, "dialog_action_area4");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, hbox77, "hbox77");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, cancel_rtp_bt, "cancel_rtp_bt");
  GLADE_HOOKUP_OBJECT (udp_payload_dialog, rtp_ok_bt, "rtp_ok_bt");

{
GtkWidget *option_menu7;
option_menu7 = lookup_widget (udp_payload_dialog, "optionmenu7");
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu7)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu7_clicked), NULL);
}

  return udp_payload_dialog;
}