GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *file1;
  GtkWidget *file1_menu;
  GtkWidget *select_database1;
  GtkWidget *exit1;
  GtkWidget *help1;
  GtkWidget *help1_menu;
  GtkWidget *about1;
  GtkWidget *hbox101;
  GtkWidget *toolbar1;
  GtkIconSize tmp_toolbar_icon_size;
  GtkWidget *toolitem1;
  GtkWidget *Build_button;
  GtkWidget *vbox34;
  GtkWidget *pixmap15;
  GtkWidget *label166;
  GtkWidget *toolitem2;
  GtkWidget *Gen_button;
  GtkWidget *vbox35;
  GtkWidget *pixmap3;
  GtkWidget *label167;
  GtkWidget *toolitem3;
  GtkWidget *Gensbt;
  GtkWidget *vbox36;
  GtkWidget *pixmap4;
  GtkWidget *label168;
  GtkWidget *toolitem13;
  GtkWidget *Genp;
  GtkWidget *vbox76;
  GtkWidget *image1;
  GtkWidget *label325;
  GtkWidget *toolitem5;
  GtkWidget *label353;
  GtkWidget *toolitem6;
  GtkWidget *Load_button;
  GtkWidget *vbox37;
  GtkWidget *pixmap5;
  GtkWidget *label169;
  GtkWidget *toolitem7;
  GtkWidget *Save_button;
  GtkWidget *vbox38;
  GtkWidget *pixmap6;
  GtkWidget *label170;
  GtkWidget *toolitem8;
  GtkWidget *Reset_button;
  GtkWidget *vbox39;
  GtkWidget *pixmap16;
  GtkWidget *label171;
  GtkWidget *toolitem9;
  GtkWidget *button62;
  GtkWidget *vbox43;
  GtkWidget *pixmap11;
  GtkWidget *label175;
  GtkWidget *toolitem12;
  GtkWidget *label354;
  GtkWidget *toolitem16;
  GtkWidget *Interface_button;
  GtkWidget *vbox40;
  GtkWidget *pixmap8;
  GtkWidget *label172;
  GtkWidget *toolitem17;
  GtkWidget *label355;
  GtkWidget *toolitem18;
  GtkWidget *Send_button;
  GtkWidget *vbox41;
  GtkWidget *pixmap9;
  GtkWidget *label173;
  GtkWidget *toolitem19;
  GtkWidget *Stop_button;
  GtkWidget *vbox42;
  GtkWidget *pixmap10;
  GtkWidget *label174;
  GtkWidget *notebook1;
  GtkWidget *vbox_13;
  GtkWidget *label160;
  GtkWidget *scrolledwindow1;
  GtkWidget *viewport1;
  GtkWidget *vbox2;
  GtkWidget *frame463;
  GtkWidget *alignment6;
  GtkWidget *hbox143;
  GtkWidget *label344;
  GtkWidget *entry179;
  GtkWidget *label346;
  GtkWidget *entry180;
  GtkWidget *label347;
  GtkWidget *entry181;
  GtkWidget *label348;
  GtkWidget *entry182;
  GtkWidget *label343;
  GtkWidget *hbox72;
  GtkWidget *frame1;
  GtkWidget *vbox17;
  GtkWidget *hbox2;
  GtkWidget *vbox3;
  GtkWidget *auto_get_mac_cbt;
  GtkWidget *bt_ver2;
  GSList *bt_ver2_group = NULL;
  GtkWidget *bt_8023;
  GtkWidget *bt_8021q;
  GtkWidget *hbox4;
  GtkWidget *frame5;
  GtkWidget *vbox8;
  GtkWidget *hbox5;
  GtkWidget *label7;
  GtkWidget *L_dst_mac;
  GtkWidget *L_dst_select_bt;
  GtkWidget *hbox6;
  GtkWidget *label8;
  GtkWidget *L_src_mac;
  GtkWidget *L_src_select_bt;
  GtkWidget *notebook_ethtype;
  GtkWidget *hbox8;
  GtkWidget *label11;
  GtkWidget *L_ethtype;
  GtkWidget *L_optmenu1_bt;
  GtkWidget *convertwidget3;
  GtkWidget *convertwidget4;
  GtkWidget *convertwidget5;
  GtkWidget *convertwidget6;
  GtkWidget *convertwidget7;
  GtkWidget *label9;
  GtkWidget *hbox9;
  GtkWidget *label12;
  GtkWidget *label143;
  GtkWidget *entry5;
  GtkWidget *checkbutton2;
  GtkWidget *L_length;
  GtkWidget *label180;
  GtkWidget *frame6;
  GtkWidget *vbox73;
  GtkWidget *hseparator7;
  GtkWidget *hbox136;
  GtkWidget *checkbutton40;
  GtkWidget *optionmenu21;
  GtkWidget *menu4;
  GtkWidget *_0x1;
  GtkWidget *_0x2;
  GtkWidget *_0x3;
  GtkWidget *_0x4;
  GtkWidget *label304;
  GtkWidget *entry165;
  GtkWidget *hseparator6;
  GtkWidget *hbox49;
  GtkWidget *label137;
  GtkWidget *L_tag_id;
  GtkWidget *label120;
  GtkWidget *hbox134;
  GtkWidget *label14;
  GtkWidget *L_optmenu2_bt;
  GtkWidget *convertwidget8;
  GtkWidget *convertwidget9;
  GtkWidget *convertwidget10;
  GtkWidget *convertwidget11;
  GtkWidget *convertwidget12;
  GtkWidget *convertwidget13;
  GtkWidget *convertwidget14;
  GtkWidget *convertwidget15;
  GtkWidget *convertwidget16;
  GtkWidget *hbox135;
  GtkWidget *checkbutton39;
  GtkWidget *label16;
  GtkWidget *L_vlan_id;
  GtkWidget *label181;
  GtkWidget *frame7;
  GtkWidget *vbox74;
  GtkWidget *hbox137;
  GtkWidget *label20;
  GtkWidget *L_8023_llc_tbt;
  GSList *L_8023_llc_tbt_group = NULL;
  GtkWidget *L_8023_llcsnap_tbt;
  GtkWidget *hbox138;
  GtkWidget *label21;
  GtkWidget *L_dsap;
  GtkWidget *label24;
  GtkWidget *L_ssap;
  GtkWidget *hbox139;
  GtkWidget *label22;
  GtkWidget *L_ctrl;
  GtkWidget *label_oui;
  GtkWidget *L_oui;
  GtkWidget *hbox140;
  GtkWidget *label_pid;
  GtkWidget *L_pid;
  GtkWidget *optionmenu6;
  GtkWidget *convertwidget17;
  GtkWidget *convertwidget18;
  GtkWidget *convertwidget19;
  GtkWidget *convertwidget20;
  GtkWidget *convertwidget21;
  GtkWidget *neki1;
  GtkWidget *hbox68;
  GtkWidget *frame14;
  GtkWidget *hbox50;
  GtkWidget *label63;
  GtkWidget *ippkt_radibt;
  GSList *ippkt_radibt_group = NULL;
  GtkWidget *IPv6_rdbt;
  GtkWidget *arppkt_radiobt;
  GtkWidget *usedef2_radibt;
  GtkWidget *label179;
  GtkWidget *hbox71;
  GtkWidget *notebook2;
  GtkWidget *frame2;
  GtkWidget *vbox16;
  GtkWidget *frame15;
  GtkWidget *vbox26;
  GtkWidget *hbox23;
  GtkWidget *label50;
  GtkWidget *entry26;
  GtkWidget *label51;
  GtkWidget *entry27;
  GtkWidget *label52;
  GtkWidget *entry28;
  GtkWidget *button37;
  GtkWidget *label53;
  GtkWidget *entry29;
  GtkWidget *checkbutton21;
  GtkWidget *label54;
  GtkWidget *entry30;
  GtkWidget *hbox24;
  GtkWidget *label55;
  GtkWidget *entry31;
  GtkWidget *button78;
  GtkWidget *label56;
  GtkWidget *entry32;
  GtkWidget *label57;
  GtkWidget *entry44;
  GtkWidget *label58;
  GtkWidget *entry34;
  GtkWidget *optionmenu3;
  GtkWidget *convertwidget22;
  GtkWidget *convertwidget23;
  GtkWidget *convertwidget24;
  GtkWidget *convertwidget25;
  GtkWidget *convertwidget26;
  GtkWidget *convertwidget27;
  GtkWidget *convertwidget28;
  GtkWidget *label59;
  GtkWidget *entry35;
  GtkWidget *ip_header_cks_cbt;
  GtkWidget *frame16;
  GtkWidget *hbox25;
  GtkWidget *label60;
  GtkWidget *entry38;
  GtkWidget *button24;
  GtkWidget *label61;
  GtkWidget *entry37;
  GtkWidget *button25;
  GtkWidget *label62;
  GtkWidget *entry39;
  GtkWidget *hbox69;
  GtkWidget *frame17;
  GtkWidget *hbox52;
  GtkWidget *label64;
  GtkWidget *udp_bt;
  GSList *udp_bt_group = NULL;
  GtkWidget *tcp_bt;
  GtkWidget *icmp_bt;
  GtkWidget *igmp_bt;
  GtkWidget *ip_user_data_bt;
  GtkWidget *neki2;
  GtkWidget *label4;
  GtkWidget *frame13;
  GtkWidget *vbox79;
  GtkWidget *frame466;
  GtkWidget *frame467;
  GtkWidget *alignment8;
  GtkWidget *vbox80;
  GtkWidget *hbox149;
  GtkWidget *label363;
  GtkWidget *entry195;
  GtkWidget *label364;
  GtkWidget *entry196;
  GtkWidget *button90;
  GtkWidget *label365;
  GtkWidget *entry197;
  GtkWidget *label366;
  GtkWidget *entry198;
  GtkWidget *checkbutton43;
  GtkWidget *label367;
  GtkWidget *entry199;
  GtkWidget *label368;
  GtkWidget *entry200;
  GtkWidget *frame468;
  GtkWidget *alignment9;
  GtkWidget *hbox152;
  GtkWidget *label369;
  GtkWidget *entry201;
  GtkWidget *button88;
  GtkWidget *label370;
  GtkWidget *entry202;
  GtkWidget *button89;
  GtkWidget *frame469;
  GtkWidget *alignment10;
  GtkWidget *hbox153;
  GtkWidget *label371;
  GtkWidget *entry203;
  GtkWidget *button91;
  GtkWidget *hbox146;
  GtkWidget *frame465;
  GtkWidget *hbox147;
  GtkWidget *label375;
  GtkWidget *radiobutton67;
  GSList *radiobutton67_group = NULL;
  GtkWidget *radiobutton68;
  GtkWidget *radiobutton69;
  GtkWidget *radiobutton71;
  GtkWidget *neki3;
  GtkWidget *label117;
  GtkWidget *frame4;
  GtkWidget *hbox56;
  GtkWidget *frame18;
  GtkWidget *scrolledwindow5;
  GtkWidget *text1;
  GtkWidget *hbox58;
  GtkWidget *vbox31;
  GtkWidget *frame19;
  GtkWidget *vbox31_1;
  GtkWidget *hbox59;
  GtkWidget *label144;
  GtkWidget *entry79;
  GtkWidget *hbox60;
  GtkWidget *label145;
  GtkWidget *entry80;
  GtkWidget *N_apply_pattern;
  GtkWidget *N_select_payload;
  GtkWidget *label146;
  GtkWidget *neki4;
  GtkWidget *label6;
  GtkWidget *vbox47;
  GtkWidget *frame3;
  GtkWidget *table1;
  GtkWidget *label27;
  GtkWidget *label28;
  GtkWidget *label29;
  GtkWidget *label30;
  GtkWidget *label31;
  GtkWidget *label32;
  GtkWidget *A_prottype;
  GtkWidget *A_hwsize;
  GtkWidget *A_protsize;
  GtkWidget *A_sendermac;
  GtkWidget *A_senderip;
  GtkWidget *A_hwtype;
  GtkWidget *label118;
  GtkWidget *label36;
  GtkWidget *label37;
  GtkWidget *label38;
  GtkWidget *button33;
  GtkWidget *button34;
  GtkWidget *label147;
  GtkWidget *label148;
  GtkWidget *label149;
  GtkWidget *label150;
  GtkWidget *label151;
  GtkWidget *label152;
  GtkWidget *label153;
  GtkWidget *label154;
  GtkWidget *hseparator1;
  GtkWidget *label34;
  GtkWidget *label33;
  GtkWidget *hseparator2;
  GtkWidget *A_targetip;
  GtkWidget *A_targetmac;
  GtkWidget *hseparator3;
  GtkWidget *button36;
  GtkWidget *button35;
  GtkWidget *radiobutton11;
  GSList *radiobutton11_group = NULL;
  GtkWidget *radiobutton10;
  GtkWidget *label155;
  GtkWidget *hbox61;
  GtkWidget *radiobutton17;
  GtkWidget *entry81;
  GtkWidget *neki5;
  GtkWidget *label_zapolni_arp_spodaj;
  GtkWidget *label5;
  GtkWidget *hbox70;
  GtkWidget *notebook4;
  GtkWidget *frame9;
  GtkWidget *vbox19;
  GtkWidget *frame22;
  GtkWidget *hbox27;
  GtkWidget *label70;
  GtkWidget *entry56;
  GtkWidget *label71;
  GtkWidget *label73;
  GtkWidget *entry41;
  GtkWidget *label72;
  GtkWidget *entry42;
  GtkWidget *checkbutton3;
  GtkWidget *label74;
  GtkWidget *entry43;
  GtkWidget *checkbutton4;
  GtkWidget *frame23;
  GtkWidget *hbox28;
  GtkWidget *checkbutton5;
  GtkWidget *frame24;
  GtkWidget *scrolledwindow7;
  GtkWidget *text3;
  GtkWidget *label192;
  GtkWidget *vbox52;
  GtkWidget *frame25;
  GtkWidget *vbox53;
  GtkWidget *hbox81;
  GtkWidget *label193;
  GtkWidget *entry89;
  GtkWidget *hbox82;
  GtkWidget *label194;
  GtkWidget *entry90;
  GtkWidget *udp_apply_pattern_button;
  GtkWidget *udp_select_payload_button;
  GtkWidget *label191;
  GtkWidget *neki9;
  GtkWidget *label65;
  GtkWidget *vbox29;
  GtkWidget *frame8;
  GtkWidget *vbox55;
  GtkWidget *frame29;
  GtkWidget *vbox18;
  GtkWidget *hbox29;
  GtkWidget *label75;
  GtkWidget *entry46;
  GtkWidget *label76;
  GtkWidget *entry47;
  GtkWidget *label77;
  GtkWidget *entry48;
  GtkWidget *label78;
  GtkWidget *entry49;
  GtkWidget *hbox30;
  GtkWidget *label79;
  GtkWidget *entry50;
  GtkWidget *label80;
  GtkWidget *checkbutton22;
  GtkWidget *checkbutton23;
  GtkWidget *checkbutton7;
  GtkWidget *checkbutton8;
  GtkWidget *checkbutton9;
  GtkWidget *checkbutton10;
  GtkWidget *checkbutton11;
  GtkWidget *checkbutton12;
  GtkWidget *hbox31;
  GtkWidget *label81;
  GtkWidget *entry51;
  GtkWidget *label82;
  GtkWidget *entry52;
  GtkWidget *checkbutton13;
  GtkWidget *label83;
  GtkWidget *entry53;
  GtkWidget *label84;
  GtkWidget *entry54;
  GtkWidget *frame30;
  GtkWidget *hbox90;
  GtkWidget *checkbutton14;
  GtkWidget *frame31;
  GtkWidget *scrolledwindow8;
  GtkWidget *text4;
  GtkWidget *label228;
  GtkWidget *vbox56;
  GtkWidget *frame32;
  GtkWidget *vbox57;
  GtkWidget *hbox91;
  GtkWidget *label230;
  GtkWidget *entry107;
  GtkWidget *hbox92;
  GtkWidget *label231;
  GtkWidget *entry108;
  GtkWidget *apply_tcp_pattern_bt;
  GtkWidget *select_tpc_payload_bt;
  GtkWidget *label229;
  GtkWidget *neki6;
  GtkWidget *label66;
  GtkWidget *frame10;
  GtkWidget *vbox20;
  GtkWidget *hbox33;
  GtkWidget *label85;
  GtkWidget *entry57;
  GtkWidget *optionmenu4;
  GtkWidget *convertwidget35;
  GtkWidget *convertwidget36;
  GtkWidget *convertwidget37;
  GtkWidget *convertwidget38;
  GtkWidget *convertwidget39;
  GtkWidget *frame12;
  GtkWidget *notebook5;
  GtkWidget *vbox22;
  GtkWidget *hbox38;
  GtkWidget *label105;
  GtkWidget *entry62;
  GtkWidget *label106;
  GtkWidget *entry63;
  GtkWidget *checkbutton16;
  GtkWidget *hbox40;
  GtkWidget *label107;
  GtkWidget *entry64;
  GtkWidget *label108;
  GtkWidget *entry65;
  GtkWidget *hbox42;
  GtkWidget *checkbutton17;
  GtkWidget *entry66;
  GtkWidget *label104;
  GtkWidget *vbox67;
  GtkWidget *hbox112;
  GtkWidget *label284;
  GtkWidget *entry157;
  GtkWidget *label285;
  GtkWidget *entry158;
  GtkWidget *checkbutton38;
  GtkWidget *hbox113;
  GtkWidget *label286;
  GtkWidget *entry159;
  GtkWidget *label283;
  GtkWidget *vbox21;
  GtkWidget *hbox34;
  GtkWidget *label86;
  GtkWidget *entry58;
  GtkWidget *optionmenu5;
  GtkWidget *convertwidget40;
  GtkWidget *convertwidget41;
  GtkWidget *convertwidget42;
  GtkWidget *convertwidget43;
  GtkWidget *convertwidget44;
  GtkWidget *convertwidget45;
  GtkWidget *convertwidget46;
  GtkWidget *convertwidget47;
  GtkWidget *convertwidget48;
  GtkWidget *convertwidget49;
  GtkWidget *convertwidget50;
  GtkWidget *convertwidget51;
  GtkWidget *convertwidget52;
  GtkWidget *convertwidget53;
  GtkWidget *convertwidget54;
  GtkWidget *convertwidget55;
  GtkWidget *convertwidget56;
  GtkWidget *convertwidget57;
  GtkWidget *hbox35;
  GtkWidget *label87;
  GtkWidget *entry59;
  GtkWidget *checkbutton15;
  GtkWidget *label88;
  GtkWidget *entry60;
  GtkWidget *hbox37;
  GtkWidget *checkbutton24;
  GtkWidget *entry61;
  GtkWidget *label89_2;
  GtkWidget *vbox25;
  GtkWidget *hbox43;
  GtkWidget *label112;
  GtkWidget *entry70;
  GtkWidget *hbox44;
  GtkWidget *label113;
  GtkWidget *entry71;
  GtkWidget *checkbutton18;
  GtkWidget *label114;
  GtkWidget *entry72;
  GtkWidget *hbox46;
  GtkWidget *label115;
  GtkWidget *entry73;
  GtkWidget *button31;
  GtkWidget *label90;
  GtkWidget *label121;
  GtkWidget *label91;
  GtkWidget *vbox30;
  GtkWidget *hbox53;
  GtkWidget *label133;
  GtkWidget *entry74;
  GtkWidget *label135;
  GtkWidget *entry77;
  GtkWidget *checkbutton20;
  GtkWidget *hbox54;
  GtkWidget *label134;
  GtkWidget *entry75;
  GtkWidget *label136;
  GtkWidget *entry78;
  GtkWidget *hbox55;
  GtkWidget *checkbutton19;
  GtkWidget *entry76;
  GtkWidget *label92;
  GtkWidget *label123;
  GtkWidget *label93;
  GtkWidget *label124;
  GtkWidget *label116;
  GtkWidget *label125;
  GtkWidget *label94;
  GtkWidget *label126;
  GtkWidget *label95;
  GtkWidget *label127;
  GtkWidget *label96;
  GtkWidget *label128;
  GtkWidget *label97;
  GtkWidget *label129;
  GtkWidget *label98;
  GtkWidget *label130;
  GtkWidget *label99;
  GtkWidget *label131;
  GtkWidget *label100;
  GtkWidget *label132;
  GtkWidget *label101;
  GtkWidget *label159;
  GtkWidget *label158;
  GtkWidget *neki7;
  GtkWidget *label67;
  GtkWidget *frame11;
  GtkWidget *vbox58;
  GtkWidget *hbox62;
  GtkWidget *frame20;
  GtkWidget *scrolledwindow6;
  GtkWidget *text2;
  GtkWidget *vbox32;
  GtkWidget *frame21;
  GtkWidget *vbox33;
  GtkWidget *hbox63;
  GtkWidget *label156;
  GtkWidget *entry82;
  GtkWidget *hbox64;
  GtkWidget *label157;
  GtkWidget *entry83;
  GtkWidget *button39;
  GtkWidget *button38;
  GtkWidget *napis_pod_ip_payload;
  GtkWidget *neki8;
  GtkWidget *label68;
  GtkWidget *frame45;
  GtkWidget *vbox69;
  GtkWidget *hbox122;
  GtkWidget *label305;
  GtkWidget *entry166;
  GtkWidget *optionmenu20;
  GtkWidget *convertwidget60;
  GtkWidget *convertwidget61;
  GtkWidget *convertwidget62;
  GtkWidget *convertwidget63;
  GtkWidget *convertwidget64;
  GtkWidget *convertwidget65;
  GtkWidget *convertwidget66;
  GtkWidget *convertwidget67;
  GtkWidget *hbox124;
  GtkWidget *label306;
  GtkWidget *entry167;
  GtkWidget *label307;
  GtkWidget *entry168;
  GtkWidget *checkbutton41;
  GtkWidget *notebook8;
  GtkWidget *vbox71;
  GtkWidget *hbox130;
  GtkWidget *label315;
  GtkWidget *entry175;
  GtkWidget *label309;
  GtkWidget *vbox70;
  GtkWidget *hbox125;
  GtkWidget *label308;
  GtkWidget *entry169;
  GtkWidget *hbox127;
  GtkWidget *label311;
  GtkWidget *entry171;
  GtkWidget *label312;
  GtkWidget *entry172;
  GtkWidget *hbox128;
  GtkWidget *label313;
  GtkWidget *entry173;
  GtkWidget *label310;
  GtkWidget *vbox72;
  GtkWidget *hbox131;
  GtkWidget *label316;
  GtkWidget *entry176;
  GtkWidget *label317;
  GtkWidget *entry177;
  GtkWidget *hbox132;
  GtkWidget *label318;
  GtkWidget *entry178;
  GtkWidget *label314;
  GtkWidget *frame461;
  GtkWidget *hbox133;
  GtkWidget *label320;
  GtkWidget *frame46;
  GtkWidget *button81;
  GtkWidget *neki0;
  GtkWidget *label69;
  GtkWidget *label376;
  GtkWidget *label319;
  GtkWidget *label1;
  GtkWidget *vbox59;
  GtkWidget *frame34;
  GtkWidget *label241;
  GtkWidget *vertbox60;
  GtkWidget *hbox97;
  GtkWidget *frame36;
  GtkWidget *vbox60;
  GtkWidget *label243;
  GtkWidget *hbox98;
  GtkWidget *entry109;
  GtkWidget *checkbutton35;
  GtkWidget *label244;
  GtkWidget *hbox99;
  GtkWidget *entry110;
  GtkWidget *checkbutton37;
  GtkWidget *label287;
  GtkWidget *hbox114;
  GtkWidget *radiobutton34;
  GSList *radiobutton34_group = NULL;
  GtkWidget *radiobutton35;
  GtkWidget *frame37;
  GtkWidget *table3;
  GtkWidget *notebook7;
  GtkWidget *optionmenu9;
  GtkWidget *convertwidget68;
  GtkWidget *convertwidget69;
  GtkWidget *convertwidget70;
  GtkWidget *convertwidget71;
  GtkWidget *convertwidget72;
  GtkWidget *label290;
  GtkWidget *optionmenu16;
  GtkWidget *convertwidget73;
  GtkWidget *convertwidget74;
  GtkWidget *convertwidget75;
  GtkWidget *convertwidget76;
  GtkWidget *convertwidget77;
  GtkWidget *convertwidget78;
  GtkWidget *convertwidget79;
  GtkWidget *label291;
  GtkWidget *optionmenu17;
  GtkWidget *convertwidget80;
  GtkWidget *convertwidget81;
  GtkWidget *convertwidget82;
  GtkWidget *convertwidget83;
  GtkWidget *convertwidget84;
  GtkWidget *convertwidget85;
  GtkWidget *label292;
  GtkWidget *optionmenu18;
  GtkWidget *convertwidget86;
  GtkWidget *convertwidget87;
  GtkWidget *convertwidget88;
  GtkWidget *convertwidget89;
  GtkWidget *convertwidget90;
  GtkWidget *convertwidget91;
  GtkWidget *convertwidget92;
  GtkWidget *convertwidget93;
  GtkWidget *label293;
  GtkWidget *optionmenu19;
  GtkWidget *convertwidget94;
  GtkWidget *convertwidget95;
  GtkWidget *convertwidget96;
  GtkWidget *convertwidget97;
  GtkWidget *convertwidget98;
  GtkWidget *convertwidget99;
  GtkWidget *convertwidget100;
  GtkWidget *convertwidget101;
  GtkWidget *convertwidget102;
  GtkWidget *convertwidget103;
  GtkWidget *convertwidget104;
  GtkWidget *label294;
  GtkWidget *hbox117;
  GtkWidget *label295;
  GtkWidget *entry160;
  GtkWidget *label296;
  GtkWidget *optionmenu14;
  GtkWidget *convertwidget105;
  GtkWidget *convertwidget106;
  GtkWidget *convertwidget107;
  GtkWidget *convertwidget108;
  GtkWidget *random_2_bytes;
  GtkWidget *random_3_bytes;
  GtkWidget *random_4_bytes;
  GtkWidget *label297;
  GtkWidget *entry161;
  GtkWidget *label358;
  GtkWidget *hbox118;
  GtkWidget *label298;
  GtkWidget *entry162;
  GtkWidget *label299;
  GtkWidget *optionmenu15;
  GtkWidget *convertwidget109;
  GtkWidget *convertwidget110;
  GtkWidget *convertwidget111;
  GtkWidget *convertwidget112;
  GtkWidget *random_2_bytes2;
  GtkWidget *random_3_bytes2;
  GtkWidget *random_4_bytes2;
  GtkWidget *label300;
  GtkWidget *entry163;
  GtkWidget *label359;
  GtkWidget *hbox120;
  GtkWidget *hbox121;
  GtkWidget *scrolledwindow10;
  GtkWidget *text5;
  GtkWidget *vbox68;
  GtkWidget *label303;
  GtkWidget *entry164;
  GtkWidget *label288;
  GtkWidget *label245;
  GtkWidget *label2;
  GtkWidget *vbox62;
  GtkWidget *frame39;
  GtkWidget *label252;
  GtkWidget *frame40;
  GtkWidget *hbox102;
  GtkWidget *label253;
  GtkWidget *radiobutton36;
  GSList *radiobutton36_group = NULL;
  GtkWidget *radiobutton37;
  GtkWidget *label360;
  GtkWidget *radiobutton61;
  GtkWidget *radiobutton62;
  GtkWidget *frame41;
  GtkWidget *scrolledwindow9;
  GtkWidget *viewport2;
  GtkWidget *table4;
  GtkWidget *entry122;
  GtkWidget *entry123;
  GtkWidget *entry124;
  GtkWidget *entry125;
  GtkWidget *entry126;
  GtkWidget *entry127;
  GtkWidget *entry128;
  GtkWidget *entry129;
  GtkWidget *entry130;
  GtkWidget *label255;
  GtkWidget *label256;
  GtkWidget *label257;
  GtkWidget *label254;
  GtkWidget *entry131;
  GtkWidget *entry132;
  GtkWidget *entry133;
  GtkWidget *entry134;
  GtkWidget *entry135;
  GtkWidget *entry136;
  GtkWidget *entry137;
  GtkWidget *entry138;
  GtkWidget *entry141;
  GtkWidget *entry142;
  GtkWidget *entry143;
  GtkWidget *entry144;
  GtkWidget *entry145;
  GtkWidget *entry146;
  GtkWidget *entry147;
  GtkWidget *entry148;
  GtkWidget *entry149;
  GtkWidget *entry150;
  GtkWidget *entry112;
  GtkWidget *entry113;
  GtkWidget *entry114;
  GtkWidget *entry115;
  GtkWidget *entry116;
  GtkWidget *entry117;
  GtkWidget *entry118;
  GtkWidget *entry119;
  GtkWidget *entry120;
  GtkWidget *entry139;
  GtkWidget *entry140;
  GtkWidget *label268;
  GtkWidget *entry121;
  GtkWidget *entry151;
  GtkWidget *checkbutton36;
  GtkWidget *label271;
  GtkWidget *label270;
  GtkWidget *label272;
  GtkWidget *entry111;
  GtkWidget *button65;
  GtkWidget *button66;
  GtkWidget *button67;
  GtkWidget *button68;
  GtkWidget *button69;
  GtkWidget *button70;
  GtkWidget *button71;
  GtkWidget *button72;
  GtkWidget *button73;
  GtkWidget *button74;
  GtkWidget *checkbutton25;
  GtkWidget *checkbutton26;
  GtkWidget *checkbutton27;
  GtkWidget *checkbutton28;
  GtkWidget *checkbutton29;
  GtkWidget *checkbutton30;
  GtkWidget *checkbutton31;
  GtkWidget *checkbutton32;
  GtkWidget *checkbutton33;
  GtkWidget *checkbutton34;
  GtkWidget *label258;
  GtkWidget *label361;
  GtkWidget *entry186;
  GtkWidget *entry187;
  GtkWidget *entry188;
  GtkWidget *entry189;
  GtkWidget *entry190;
  GtkWidget *entry191;
  GtkWidget *entry192;
  GtkWidget *entry193;
  GtkWidget *entry194;
  GtkWidget *entry185;
  GtkWidget *entry152;
  GtkWidget *button87;
  GtkWidget *hseparator5;
  GtkWidget *label3;
  GtkWidget *vbox77;
  GtkWidget *frame464;
  GtkWidget *alignment7;
  GtkWidget *vbox78;
  GtkWidget *hbox145;
  GtkWidget *label351;
  GtkWidget *checkbutton42;
  GtkWidget *entry183;
  GtkWidget *label352;
  GtkWidget *entry184;
  GtkWidget *hbox144;
  GtkWidget *label350;
  GtkWidget *radiobutton59;
  GSList *radiobutton59_group = NULL;
  GtkWidget *radiobutton60;
  GtkWidget *label349;
  GtkWidget *frame462;
  GtkWidget *alignment5;
  GtkWidget *scrolledwindow12;
  GtkWidget *clist2;
  GtkWidget *label334;
  GtkWidget *label335;
  GtkWidget *label336;
  GtkWidget *label337;
  GtkWidget *label338;
  GtkWidget *label339;
  GtkWidget *label341;
  GtkWidget *label342;
  GtkWidget *label333;
  GtkWidget *button86;
  GtkWidget *label251;
  GtkWidget *statusbar1;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), _("PackETH - ethernet packet generator"));
  gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (window1), 950, 750);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  file1 = gtk_menu_item_new_with_mnemonic (_("File"));
  gtk_widget_show (file1);
  gtk_container_add (GTK_CONTAINER (menubar1), file1);

  file1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (file1), file1_menu);

  select_database1 = gtk_menu_item_new_with_mnemonic (_("Select database"));
  gtk_widget_show (select_database1);
  gtk_container_add (GTK_CONTAINER (file1_menu), select_database1);

  exit1 = gtk_menu_item_new_with_mnemonic (_("Exit"));
  gtk_widget_show (exit1);
  gtk_container_add (GTK_CONTAINER (file1_menu), exit1);

  help1 = gtk_menu_item_new_with_mnemonic (_("Help"));
  gtk_widget_show (help1);
  gtk_container_add (GTK_CONTAINER (menubar1), help1);

  help1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (help1), help1_menu);

  about1 = gtk_menu_item_new_with_mnemonic (_("About"));
  gtk_widget_show (about1);
  gtk_container_add (GTK_CONTAINER (help1_menu), about1);

  hbox101 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox101);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox101, FALSE, FALSE, 0);

  toolbar1 = gtk_toolbar_new ();
  gtk_widget_show (toolbar1);
  gtk_box_pack_start (GTK_BOX (hbox101), toolbar1, TRUE, TRUE, 0);
  gtk_widget_set_size_request (toolbar1, 820, -1);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_BOTH);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1));

  toolitem1 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem1);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem1);

  Build_button = gtk_button_new ();
  gtk_widget_show (Build_button);
  gtk_container_add (GTK_CONTAINER (toolitem1), Build_button);

  vbox34 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox34);
  gtk_container_add (GTK_CONTAINER (Build_button), vbox34);

  pixmap15 = create_pixmap (window1, "Build.xpm");
  gtk_widget_show (pixmap15);
  gtk_box_pack_start (GTK_BOX (vbox34), pixmap15, TRUE, TRUE, 0);

  label166 = gtk_label_new (_("Builder"));
  gtk_widget_show (label166);
  gtk_box_pack_start (GTK_BOX (vbox34), label166, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label166), GTK_JUSTIFY_CENTER);

  toolitem2 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem2);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem2);

  Gen_button = gtk_button_new ();
  gtk_widget_show (Gen_button);
  gtk_container_add (GTK_CONTAINER (toolitem2), Gen_button);

  vbox35 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox35);
  gtk_container_add (GTK_CONTAINER (Gen_button), vbox35);

  pixmap3 = create_pixmap (window1, "preference.xpm");
  gtk_widget_show (pixmap3);
  gtk_box_pack_start (GTK_BOX (vbox35), pixmap3, TRUE, TRUE, 0);

  label167 = gtk_label_new (_("Gen-b"));
  gtk_widget_show (label167);
  gtk_box_pack_start (GTK_BOX (vbox35), label167, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label167), GTK_JUSTIFY_CENTER);

  toolitem3 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem3);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem3);

  Gensbt = gtk_button_new ();
  gtk_widget_show (Gensbt);
  gtk_container_add (GTK_CONTAINER (toolitem3), Gensbt);

  vbox36 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox36);
  gtk_container_add (GTK_CONTAINER (Gensbt), vbox36);

  pixmap4 = create_pixmap (window1, "preference.xpm");
  gtk_widget_show (pixmap4);
  gtk_box_pack_start (GTK_BOX (vbox36), pixmap4, TRUE, TRUE, 0);

  label168 = gtk_label_new (_("Gen-s"));
  gtk_widget_show (label168);
  gtk_box_pack_start (GTK_BOX (vbox36), label168, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label168), GTK_JUSTIFY_CENTER);

  toolitem13 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem13);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem13);

  Genp = gtk_button_new ();
  gtk_widget_show (Genp);
  gtk_container_add (GTK_CONTAINER (toolitem13), Genp);

  vbox76 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox76);
  gtk_container_add (GTK_CONTAINER (Genp), vbox76);

  image1 = create_pixmap (window1, "preference.xpm");
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (vbox76), image1, TRUE, TRUE, 0);

  label325 = gtk_label_new (_("Pcap"));
  gtk_widget_show (label325);
  gtk_box_pack_start (GTK_BOX (vbox76), label325, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label325), GTK_JUSTIFY_CENTER);

  toolitem5 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem5);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem5);

  label353 = gtk_label_new (_("     "));
  gtk_widget_show (label353);
  gtk_container_add (GTK_CONTAINER (toolitem5), label353);

  toolitem6 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem6);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem6);

  Load_button = gtk_button_new ();
  gtk_widget_show (Load_button);
  gtk_container_add (GTK_CONTAINER (toolitem6), Load_button);

  vbox37 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox37);
  gtk_container_add (GTK_CONTAINER (Load_button), vbox37);

  pixmap5 = create_pixmap (window1, "load.xpm");
  gtk_widget_show (pixmap5);
  gtk_box_pack_start (GTK_BOX (vbox37), pixmap5, TRUE, TRUE, 0);

  label169 = gtk_label_new (_("Load"));
  gtk_widget_show (label169);
  gtk_box_pack_start (GTK_BOX (vbox37), label169, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label169), GTK_JUSTIFY_CENTER);

  toolitem7 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem7);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem7);

  Save_button = gtk_button_new ();
  gtk_widget_show (Save_button);
  gtk_container_add (GTK_CONTAINER (toolitem7), Save_button);

  vbox38 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox38);
  gtk_container_add (GTK_CONTAINER (Save_button), vbox38);

  pixmap6 = create_pixmap (window1, "save.xpm");
  gtk_widget_show (pixmap6);
  gtk_box_pack_start (GTK_BOX (vbox38), pixmap6, TRUE, TRUE, 0);

  label170 = gtk_label_new (_("Save"));
  gtk_widget_show (label170);
  gtk_box_pack_start (GTK_BOX (vbox38), label170, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label170), GTK_JUSTIFY_CENTER);

  toolitem8 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem8);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem8);

  Reset_button = gtk_button_new ();
  gtk_widget_show (Reset_button);
  gtk_container_add (GTK_CONTAINER (toolitem8), Reset_button);

  vbox39 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox39);
  gtk_container_add (GTK_CONTAINER (Reset_button), vbox39);

  pixmap16 = create_pixmap (window1, "reset.xpm");
  gtk_widget_show (pixmap16);
  gtk_box_pack_start (GTK_BOX (vbox39), pixmap16, TRUE, TRUE, 0);

  label171 = gtk_label_new (_("Default"));
  gtk_widget_show (label171);
  gtk_box_pack_start (GTK_BOX (vbox39), label171, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label171), GTK_JUSTIFY_CENTER);

  toolitem9 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem9);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem9);

  button62 = gtk_button_new ();
  gtk_widget_show (button62);
  gtk_container_add (GTK_CONTAINER (toolitem9), button62);

  vbox43 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox43);
  gtk_container_add (GTK_CONTAINER (button62), vbox43);

  pixmap11 = create_pixmap (window1, "save.xpm");
  gtk_widget_show (pixmap11);
  gtk_box_pack_start (GTK_BOX (vbox43), pixmap11, TRUE, TRUE, 0);

  label175 = gtk_label_new (_("Default"));
  gtk_widget_show (label175);
  gtk_box_pack_start (GTK_BOX (vbox43), label175, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label175), GTK_JUSTIFY_CENTER);

  toolitem12 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem12);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem12);

  label354 = gtk_label_new (_("     "));
  gtk_widget_show (label354);
  gtk_container_add (GTK_CONTAINER (toolitem12), label354);

  toolitem16 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem16);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem16);

  Interface_button = gtk_button_new ();
  gtk_widget_show (Interface_button);
  gtk_container_add (GTK_CONTAINER (toolitem16), Interface_button);

  vbox40 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox40);
  gtk_container_add (GTK_CONTAINER (Interface_button), vbox40);

  pixmap8 = create_pixmap (window1, "interface.xpm");
  gtk_widget_show (pixmap8);
  gtk_box_pack_start (GTK_BOX (vbox40), pixmap8, TRUE, TRUE, 0);

  label172 = gtk_label_new (_("Interface"));
  gtk_widget_show (label172);
  gtk_box_pack_start (GTK_BOX (vbox40), label172, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label172), GTK_JUSTIFY_CENTER);

  toolitem17 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem17);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem17);

  label355 = gtk_label_new (_("     "));
  gtk_widget_show (label355);
  gtk_container_add (GTK_CONTAINER (toolitem17), label355);

  toolitem18 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem18);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem18);

  Send_button = gtk_button_new ();
  gtk_widget_show (Send_button);
  gtk_container_add (GTK_CONTAINER (toolitem18), Send_button);

  vbox41 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox41);
  gtk_container_add (GTK_CONTAINER (Send_button), vbox41);

  pixmap9 = create_pixmap (window1, "send.xpm");
  gtk_widget_show (pixmap9);
  gtk_box_pack_start (GTK_BOX (vbox41), pixmap9, TRUE, TRUE, 0);

  label173 = gtk_label_new (_("Send"));
  gtk_widget_show (label173);
  gtk_box_pack_start (GTK_BOX (vbox41), label173, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label173), GTK_JUSTIFY_CENTER);

  toolitem19 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem19);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem19);

  Stop_button = gtk_button_new ();
  gtk_widget_show (Stop_button);
  gtk_container_add (GTK_CONTAINER (toolitem19), Stop_button);

  vbox42 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox42);
  gtk_container_add (GTK_CONTAINER (Stop_button), vbox42);

  pixmap10 = create_pixmap (window1, "X.xpm");
  gtk_widget_show (pixmap10);
  gtk_box_pack_start (GTK_BOX (vbox42), pixmap10, TRUE, TRUE, 0);

  label174 = gtk_label_new (_("Stop"));
  gtk_widget_show (label174);
  gtk_box_pack_start (GTK_BOX (vbox42), label174, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label174), GTK_JUSTIFY_CENTER);

  notebook1 = gtk_notebook_new ();
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);
  GTK_WIDGET_UNSET_FLAGS (notebook1, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook1), FALSE);

  vbox_13 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox_13);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox_13);

  label160 = gtk_label_new ("");
  gtk_widget_show (label160);
  gtk_box_pack_start (GTK_BOX (vbox_13), label160, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label160), GTK_JUSTIFY_CENTER);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox_13), scrolledwindow1, TRUE, TRUE, 0);
  gtk_widget_set_size_request (scrolledwindow1, 859, -1);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow1, GTK_CAN_FOCUS);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), viewport1);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (viewport1), vbox2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox2), 10);

  frame463 = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (vbox2), frame463, FALSE, FALSE, 0);
  gtk_widget_set_size_request (frame463, 893, -1);
  gtk_container_set_border_width (GTK_CONTAINER (frame463), 10);

  alignment6 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment6);
  gtk_container_add (GTK_CONTAINER (frame463), alignment6);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment6), 0, 0, 12, 0);

  hbox143 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox143);
  gtk_container_add (GTK_CONTAINER (alignment6), hbox143);
  gtk_widget_set_size_request (hbox143, 863, -1);

  label344 = gtk_label_new (_("Timestamp sec"));
  gtk_widget_show (label344);
  gtk_box_pack_start (GTK_BOX (hbox143), label344, FALSE, FALSE, 10);

  entry179 = gtk_entry_new ();
  gtk_widget_show (entry179);
  gtk_box_pack_start (GTK_BOX (hbox143), entry179, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry179, 100, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry179), 10);

  label346 = gtk_label_new (_("usec"));
  gtk_widget_show (label346);
  gtk_box_pack_start (GTK_BOX (hbox143), label346, FALSE, FALSE, 10);

  entry180 = gtk_entry_new ();
  gtk_widget_show (entry180);
  gtk_box_pack_start (GTK_BOX (hbox143), entry180, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry180, 100, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry180), 10);

  label347 = gtk_label_new (_("       Capture length"));
  gtk_widget_show (label347);
  gtk_box_pack_start (GTK_BOX (hbox143), label347, FALSE, FALSE, 10);

  entry181 = gtk_entry_new ();
  gtk_widget_show (entry181);
  gtk_box_pack_start (GTK_BOX (hbox143), entry181, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry181, 100, -1);

  label348 = gtk_label_new (_("Original length"));
  gtk_widget_show (label348);
  gtk_box_pack_start (GTK_BOX (hbox143), label348, FALSE, FALSE, 10);

  entry182 = gtk_entry_new ();
  gtk_widget_show (entry182);
  gtk_box_pack_start (GTK_BOX (hbox143), entry182, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry182, 100, -1);

  label343 = gtk_label_new (_("Pcap header"));
  gtk_widget_show (label343);
  gtk_frame_set_label_widget (GTK_FRAME (frame463), label343);
  gtk_label_set_use_markup (GTK_LABEL (label343), TRUE);

  hbox72 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox72);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox72, FALSE, FALSE, 0);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (hbox72), frame1, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame1), 10);

  vbox17 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox17);
  gtk_container_add (GTK_CONTAINER (frame1), vbox17);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox17), hbox2, FALSE, FALSE, 0);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (hbox2), vbox3, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox3), 5);

  auto_get_mac_cbt = gtk_toggle_button_new_with_mnemonic (_("Auto"));
  gtk_box_pack_start (GTK_BOX (vbox3), auto_get_mac_cbt, TRUE, FALSE, 0);

  bt_ver2 = gtk_radio_button_new_with_mnemonic (NULL, _("ver II"));
  gtk_widget_show (bt_ver2);
  gtk_box_pack_start (GTK_BOX (vbox3), bt_ver2, TRUE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (bt_ver2), bt_ver2_group);
  bt_ver2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (bt_ver2));

  bt_8023 = gtk_radio_button_new_with_mnemonic (NULL, _("802.3"));
  gtk_widget_show (bt_8023);
  gtk_box_pack_start (GTK_BOX (vbox3), bt_8023, TRUE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (bt_8023), bt_ver2_group);
  bt_ver2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (bt_8023));

  bt_8021q = gtk_check_button_new_with_mnemonic (_("802.1q"));
  gtk_widget_show (bt_8021q);
  gtk_box_pack_start (GTK_BOX (vbox3), bt_8021q, TRUE, FALSE, 0);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (hbox2), hbox4, FALSE, FALSE, 0);

  frame5 = gtk_frame_new (NULL);
  gtk_widget_show (frame5);
  gtk_box_pack_start (GTK_BOX (hbox4), frame5, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame5), 5);

  vbox8 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox8);
  gtk_container_add (GTK_CONTAINER (frame5), vbox8);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (vbox8), hbox5, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox5), 10);

  label7 = gtk_label_new (_("Destination "));
  gtk_widget_show (label7);
  gtk_box_pack_start (GTK_BOX (hbox5), label7, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_CENTER);

  L_dst_mac = gtk_entry_new ();
  gtk_widget_show (L_dst_mac);
  gtk_box_pack_start (GTK_BOX (hbox5), L_dst_mac, FALSE, FALSE, 0);
  gtk_widget_set_size_request (L_dst_mac, 140, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_dst_mac), 17);
  gtk_entry_set_invisible_char (GTK_ENTRY (L_dst_mac), 8226);

  L_dst_select_bt = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (L_dst_select_bt);
  gtk_box_pack_start (GTK_BOX (hbox5), L_dst_select_bt, FALSE, FALSE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (L_dst_select_bt), 1);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox6);
  gtk_box_pack_start (GTK_BOX (vbox8), hbox6, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox6), 10);

  label8 = gtk_label_new (_("Source       "));
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox6), label8, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_CENTER);

  L_src_mac = gtk_entry_new ();
  gtk_widget_show (L_src_mac);
  gtk_box_pack_start (GTK_BOX (hbox6), L_src_mac, FALSE, FALSE, 1);
  gtk_widget_set_size_request (L_src_mac, 140, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_src_mac), 17);
  gtk_entry_set_invisible_char (GTK_ENTRY (L_src_mac), 8226);

  L_src_select_bt = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (L_src_select_bt);
  gtk_box_pack_start (GTK_BOX (hbox6), L_src_select_bt, FALSE, FALSE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (L_src_select_bt), 1);

  notebook_ethtype = gtk_notebook_new ();
  gtk_widget_show (notebook_ethtype);
  gtk_box_pack_start (GTK_BOX (vbox8), notebook_ethtype, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (notebook_ethtype), 10);
  GTK_WIDGET_UNSET_FLAGS (notebook_ethtype, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook_ethtype), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook_ethtype), FALSE);

  hbox8 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox8);
  gtk_container_add (GTK_CONTAINER (notebook_ethtype), hbox8);
  gtk_container_set_border_width (GTK_CONTAINER (hbox8), 5);

  label11 = gtk_label_new (_("Ethertype   0x "));
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox8), label11, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label11), GTK_JUSTIFY_CENTER);

  L_ethtype = gtk_entry_new ();
  gtk_widget_show (L_ethtype);
  gtk_box_pack_start (GTK_BOX (hbox8), L_ethtype, FALSE, FALSE, 0);
  gtk_widget_set_size_request (L_ethtype, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_ethtype), 4);
  gtk_editable_set_editable (GTK_EDITABLE (L_ethtype), FALSE);
  gtk_entry_set_text (GTK_ENTRY (L_ethtype), _("0800"));
  gtk_entry_set_invisible_char (GTK_ENTRY (L_ethtype), 8226);

  L_optmenu1_bt = gtk_option_menu_new ();
  gtk_widget_show (L_optmenu1_bt);
  gtk_box_pack_start (GTK_BOX (hbox8), L_optmenu1_bt, FALSE, FALSE, 5);

  convertwidget3 = gtk_menu_new ();

  convertwidget4 = gtk_menu_item_new_with_mnemonic (_("     IPv4"));
  gtk_widget_show (convertwidget4);
  gtk_container_add (GTK_CONTAINER (convertwidget3), convertwidget4);

  convertwidget5 = gtk_menu_item_new_with_mnemonic (_("     IPv6"));
  gtk_widget_show (convertwidget5);
  gtk_container_add (GTK_CONTAINER (convertwidget3), convertwidget5);

  convertwidget6 = gtk_menu_item_new_with_mnemonic (_("     ARP"));
  gtk_widget_show (convertwidget6);
  gtk_container_add (GTK_CONTAINER (convertwidget3), convertwidget6);

  convertwidget7 = gtk_menu_item_new_with_mnemonic (_("User defined"));
  gtk_widget_show (convertwidget7);
  gtk_container_add (GTK_CONTAINER (convertwidget3), convertwidget7);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (L_optmenu1_bt), convertwidget3);

  label9 = gtk_label_new (_("label9"));
  gtk_widget_show (label9);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_ethtype), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_ethtype), 0), label9);
  gtk_label_set_justify (GTK_LABEL (label9), GTK_JUSTIFY_CENTER);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox9);
  gtk_container_add (GTK_CONTAINER (notebook_ethtype), hbox9);

  label12 = gtk_label_new (_("Length         "));
  gtk_widget_show (label12);
  gtk_box_pack_start (GTK_BOX (hbox9), label12, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_CENTER);

  label143 = gtk_label_new (_("0x "));
  gtk_widget_show (label143);
  gtk_box_pack_start (GTK_BOX (hbox9), label143, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label143), GTK_JUSTIFY_CENTER);

  entry5 = gtk_entry_new ();
  gtk_widget_show (entry5);
  gtk_box_pack_start (GTK_BOX (hbox9), entry5, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry5, 40, -1);
  gtk_widget_set_sensitive (entry5, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry5), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry5), 8226);

  checkbutton2 = gtk_check_button_new_with_mnemonic (_("auto"));
  gtk_widget_show (checkbutton2);
  gtk_box_pack_start (GTK_BOX (hbox9), checkbutton2, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton2), TRUE);

  L_length = gtk_label_new (_("label10"));
  gtk_widget_show (L_length);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_ethtype), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_ethtype), 1), L_length);
  gtk_label_set_justify (GTK_LABEL (L_length), GTK_JUSTIFY_CENTER);

  label180 = gtk_label_new (_("MAC Header"));
  gtk_widget_show (label180);
  gtk_frame_set_label_widget (GTK_FRAME (frame5), label180);

  frame6 = gtk_frame_new (NULL);
  gtk_widget_show (frame6);
  gtk_box_pack_start (GTK_BOX (hbox4), frame6, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame6), 5);
  gtk_widget_set_sensitive (frame6, FALSE);

  vbox73 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox73);
  gtk_container_add (GTK_CONTAINER (frame6), vbox73);

  hseparator7 = gtk_hseparator_new ();
  gtk_widget_show (hseparator7);
  gtk_box_pack_start (GTK_BOX (vbox73), hseparator7, TRUE, TRUE, 0);

  hbox136 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox136);
  gtk_box_pack_start (GTK_BOX (vbox73), hbox136, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox136), 3);

  checkbutton40 = gtk_check_button_new_with_mnemonic (_("QinQ"));
  gtk_widget_show (checkbutton40);
  gtk_box_pack_start (GTK_BOX (hbox136), checkbutton40, FALSE, FALSE, 0);

  optionmenu21 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu21);
  gtk_box_pack_start (GTK_BOX (hbox136), optionmenu21, FALSE, FALSE, 5);

  menu4 = gtk_menu_new ();

  _0x1 = gtk_menu_item_new_with_mnemonic (_("0x8100"));
  gtk_widget_show (_0x1);
  gtk_container_add (GTK_CONTAINER (menu4), _0x1);

  _0x2 = gtk_menu_item_new_with_mnemonic (_("0x9100"));
  gtk_widget_show (_0x2);
  gtk_container_add (GTK_CONTAINER (menu4), _0x2);

  _0x3 = gtk_menu_item_new_with_mnemonic (_("0x9200"));
  gtk_widget_show (_0x3);
  gtk_container_add (GTK_CONTAINER (menu4), _0x3);

  _0x4 = gtk_menu_item_new_with_mnemonic (_("0x88a8"));
  gtk_widget_show (_0x4);
  gtk_container_add (GTK_CONTAINER (menu4), _0x4);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu21), menu4);

  label304 = gtk_label_new (_(" 0x "));
  gtk_widget_show (label304);
  gtk_box_pack_start (GTK_BOX (hbox136), label304, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label304), GTK_JUSTIFY_CENTER);

  entry165 = gtk_entry_new ();
  gtk_widget_show (entry165);
  gtk_box_pack_start (GTK_BOX (hbox136), entry165, TRUE, TRUE, 0);
  gtk_widget_set_size_request (entry165, 50, -1);
  gtk_widget_set_sensitive (entry165, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry165), 4);
  gtk_entry_set_text (GTK_ENTRY (entry165), _("0000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry165), 8226);

  hseparator6 = gtk_hseparator_new ();
  gtk_widget_show (hseparator6);
  gtk_box_pack_start (GTK_BOX (vbox73), hseparator6, TRUE, TRUE, 0);

  hbox49 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox49);
  gtk_box_pack_start (GTK_BOX (vbox73), hbox49, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox49), 3);

  label137 = gtk_label_new (_(" Tag ID  0x "));
  gtk_widget_show (label137);
  gtk_box_pack_start (GTK_BOX (hbox49), label137, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label137), GTK_JUSTIFY_CENTER);

  L_tag_id = gtk_entry_new ();
  gtk_widget_show (L_tag_id);
  gtk_box_pack_start (GTK_BOX (hbox49), L_tag_id, FALSE, FALSE, 0);
  gtk_widget_set_size_request (L_tag_id, 40, -1);
  gtk_widget_set_sensitive (L_tag_id, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (L_tag_id), 4);
  gtk_editable_set_editable (GTK_EDITABLE (L_tag_id), FALSE);
  gtk_entry_set_text (GTK_ENTRY (L_tag_id), _("8100"));
  gtk_entry_set_invisible_char (GTK_ENTRY (L_tag_id), 8226);

  label120 = gtk_label_new ("");
  gtk_widget_show (label120);
  gtk_box_pack_start (GTK_BOX (hbox49), label120, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label120), GTK_JUSTIFY_CENTER);

  hbox134 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox134);
  gtk_box_pack_start (GTK_BOX (vbox73), hbox134, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox134), 3);

  label14 = gtk_label_new (_("Priority"));
  gtk_widget_show (label14);
  gtk_box_pack_start (GTK_BOX (hbox134), label14, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_CENTER);

  L_optmenu2_bt = gtk_option_menu_new ();
  gtk_widget_show (L_optmenu2_bt);
  gtk_box_pack_start (GTK_BOX (hbox134), L_optmenu2_bt, FALSE, FALSE, 0);

  convertwidget8 = gtk_menu_new ();

  convertwidget9 = gtk_menu_item_new_with_mnemonic (_("0     (Best effort)"));
  gtk_widget_show (convertwidget9);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget9);

  convertwidget10 = gtk_menu_item_new_with_mnemonic (_("1    (Background)"));
  gtk_widget_show (convertwidget10);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget10);

  convertwidget11 = gtk_menu_item_new_with_mnemonic (_("2     (Undefined)"));
  gtk_widget_show (convertwidget11);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget11);

  convertwidget12 = gtk_menu_item_new_with_mnemonic (_("3  (Excellent effort)"));
  gtk_widget_show (convertwidget12);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget12);

  convertwidget13 = gtk_menu_item_new_with_mnemonic (_("4 (Controlled load)"));
  gtk_widget_show (convertwidget13);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget13);

  convertwidget14 = gtk_menu_item_new_with_mnemonic (_("5       (Video)"));
  gtk_widget_show (convertwidget14);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget14);

  convertwidget15 = gtk_menu_item_new_with_mnemonic (_("6       (Voice)"));
  gtk_widget_show (convertwidget15);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget15);

  convertwidget16 = gtk_menu_item_new_with_mnemonic (_("7 (Network control)"));
  gtk_widget_show (convertwidget16);
  gtk_container_add (GTK_CONTAINER (convertwidget8), convertwidget16);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (L_optmenu2_bt), convertwidget8);

  hbox135 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox135);
  gtk_box_pack_start (GTK_BOX (vbox73), hbox135, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox135), 3);

  checkbutton39 = gtk_check_button_new_with_mnemonic (_("Cfi"));
  gtk_widget_show (checkbutton39);
  gtk_box_pack_start (GTK_BOX (hbox135), checkbutton39, FALSE, FALSE, 0);

  label16 = gtk_label_new (_(" VLAN ID   0x  "));
  gtk_widget_show (label16);
  gtk_box_pack_start (GTK_BOX (hbox135), label16, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label16), GTK_JUSTIFY_CENTER);

  L_vlan_id = gtk_entry_new ();
  gtk_widget_show (L_vlan_id);
  gtk_box_pack_start (GTK_BOX (hbox135), L_vlan_id, FALSE, TRUE, 0);
  gtk_widget_set_size_request (L_vlan_id, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_vlan_id), 3);
  gtk_entry_set_text (GTK_ENTRY (L_vlan_id), _("001"));
  gtk_entry_set_invisible_char (GTK_ENTRY (L_vlan_id), 8226);

  label181 = gtk_label_new (_("802.1q VLAN fields"));
  gtk_widget_show (label181);
  gtk_frame_set_label_widget (GTK_FRAME (frame6), label181);

  frame7 = gtk_frame_new (NULL);
  gtk_widget_show (frame7);
  gtk_box_pack_start (GTK_BOX (hbox4), frame7, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame7), 10);
  gtk_widget_set_sensitive (frame7, FALSE);

  vbox74 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox74);
  gtk_container_add (GTK_CONTAINER (frame7), vbox74);

  hbox137 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox137);
  gtk_box_pack_start (GTK_BOX (vbox74), hbox137, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox137), 3);

  label20 = gtk_label_new (_("Type"));
  gtk_widget_show (label20);
  gtk_box_pack_start (GTK_BOX (hbox137), label20, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_CENTER);

  L_8023_llc_tbt = gtk_radio_button_new_with_mnemonic (NULL, _("LLC"));
  gtk_widget_show (L_8023_llc_tbt);
  gtk_box_pack_start (GTK_BOX (hbox137), L_8023_llc_tbt, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (L_8023_llc_tbt), L_8023_llc_tbt_group);
  L_8023_llc_tbt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (L_8023_llc_tbt));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (L_8023_llc_tbt), TRUE);

  L_8023_llcsnap_tbt = gtk_radio_button_new_with_mnemonic (NULL, _("LLC-SNAP"));
  gtk_widget_show (L_8023_llcsnap_tbt);
  gtk_box_pack_start (GTK_BOX (hbox137), L_8023_llcsnap_tbt, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (L_8023_llcsnap_tbt), L_8023_llc_tbt_group);
  L_8023_llc_tbt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (L_8023_llcsnap_tbt));

  hbox138 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox138);
  gtk_box_pack_start (GTK_BOX (vbox74), hbox138, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox138), 3);

  label21 = gtk_label_new (_("DSAP 0x"));
  gtk_widget_show (label21);
  gtk_box_pack_start (GTK_BOX (hbox138), label21, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_CENTER);

  L_dsap = gtk_entry_new ();
  gtk_widget_show (L_dsap);
  gtk_box_pack_start (GTK_BOX (hbox138), L_dsap, FALSE, FALSE, 0);
  gtk_widget_set_size_request (L_dsap, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_dsap), 2);
  gtk_entry_set_text (GTK_ENTRY (L_dsap), _("AA"));
  gtk_entry_set_invisible_char (GTK_ENTRY (L_dsap), 8226);

  label24 = gtk_label_new (_("SSAP 0x"));
  gtk_widget_show (label24);
  gtk_box_pack_start (GTK_BOX (hbox138), label24, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_CENTER);

  L_ssap = gtk_entry_new ();
  gtk_widget_show (L_ssap);
  gtk_box_pack_start (GTK_BOX (hbox138), L_ssap, FALSE, FALSE, 0);
  gtk_widget_set_size_request (L_ssap, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_ssap), 2);
  gtk_entry_set_text (GTK_ENTRY (L_ssap), _("AA"));
  gtk_entry_set_invisible_char (GTK_ENTRY (L_ssap), 8226);

  hbox139 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox139);
  gtk_box_pack_start (GTK_BOX (vbox74), hbox139, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox139), 3);

  label22 = gtk_label_new (_("Ctrl 0x"));
  gtk_widget_show (label22);
  gtk_box_pack_start (GTK_BOX (hbox139), label22, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label22), GTK_JUSTIFY_CENTER);

  L_ctrl = gtk_entry_new ();
  gtk_widget_show (L_ctrl);
  gtk_box_pack_start (GTK_BOX (hbox139), L_ctrl, TRUE, TRUE, 0);
  gtk_widget_set_size_request (L_ctrl, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (L_ctrl), 2);
  gtk_entry_set_text (GTK_ENTRY (L_ctrl), _("03"));
  gtk_entry_set_invisible_char (GTK_ENTRY (L_ctrl), 8226);

  label_oui = gtk_label_new (_(" OUI 0x"));
  gtk_widget_show (label_oui);
  gtk_box_pack_start (GTK_BOX (hbox139), label_oui, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (label_oui, FALSE);
  gtk_label_set_justify (GTK_LABEL (label_oui), GTK_JUSTIFY_CENTER);

  L_oui = gtk_entry_new ();
  gtk_widget_show (L_oui);
  gtk_box_pack_start (GTK_BOX (hbox139), L_oui, TRUE, TRUE, 0);
  gtk_widget_set_size_request (L_oui, 60, -1);
  gtk_widget_set_sensitive (L_oui, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (L_oui), 6);
  gtk_editable_set_editable (GTK_EDITABLE (L_oui), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (L_oui), 8226);

  hbox140 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox140);
  gtk_box_pack_start (GTK_BOX (vbox74), hbox140, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox140), 3);

  label_pid = gtk_label_new (_("PID 0x"));
  gtk_widget_show (label_pid);
  gtk_box_pack_start (GTK_BOX (hbox140), label_pid, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (label_pid, FALSE);
  gtk_label_set_justify (GTK_LABEL (label_pid), GTK_JUSTIFY_CENTER);

  L_pid = gtk_entry_new ();
  gtk_widget_show (L_pid);
  gtk_box_pack_start (GTK_BOX (hbox140), L_pid, TRUE, TRUE, 0);
  gtk_widget_set_size_request (L_pid, 40, -1);
  gtk_widget_set_sensitive (L_pid, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (L_pid), 4);
  gtk_editable_set_editable (GTK_EDITABLE (L_pid), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (L_pid), 8226);

  optionmenu6 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu6);
  gtk_box_pack_start (GTK_BOX (hbox140), optionmenu6, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (optionmenu6, FALSE);

  convertwidget17 = gtk_menu_new ();

  convertwidget18 = gtk_menu_item_new_with_mnemonic (_("IPv4"));
  gtk_widget_show (convertwidget18);
  gtk_container_add (GTK_CONTAINER (convertwidget17), convertwidget18);

  convertwidget19 = gtk_menu_item_new_with_mnemonic (_("IPv6"));
  gtk_widget_show (convertwidget19);
  gtk_container_add (GTK_CONTAINER (convertwidget17), convertwidget19);

  convertwidget20 = gtk_menu_item_new_with_mnemonic (_("ARP"));
  gtk_widget_show (convertwidget20);
  gtk_container_add (GTK_CONTAINER (convertwidget17), convertwidget20);

  convertwidget21 = gtk_menu_item_new_with_mnemonic (_("Other"));
  gtk_widget_show (convertwidget21);
  gtk_container_add (GTK_CONTAINER (convertwidget17), convertwidget21);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu6), convertwidget17);

  neki1 = gtk_label_new (_("802.3 LLC field values"));
  gtk_widget_show (neki1);
  gtk_frame_set_label_widget (GTK_FRAME (frame7), neki1);

  hbox68 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox68);
  gtk_box_pack_start (GTK_BOX (vbox17), hbox68, FALSE, FALSE, 10);

  frame14 = gtk_frame_new (NULL);
  gtk_widget_show (frame14);
  gtk_box_pack_start (GTK_BOX (hbox68), frame14, FALSE, FALSE, 80);
  gtk_container_set_border_width (GTK_CONTAINER (frame14), 10);

  hbox50 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox50);
  gtk_container_add (GTK_CONTAINER (frame14), hbox50);

  label63 = gtk_label_new (_("                          Next layer  ---->"));
  gtk_widget_show (label63);
  gtk_box_pack_start (GTK_BOX (hbox50), label63, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label63), GTK_JUSTIFY_CENTER);

  ippkt_radibt = gtk_radio_button_new_with_mnemonic (NULL, _("IPv4"));
  gtk_widget_show (ippkt_radibt);
  gtk_box_pack_start (GTK_BOX (hbox50), ippkt_radibt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (ippkt_radibt), ippkt_radibt_group);
  ippkt_radibt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (ippkt_radibt));

  IPv6_rdbt = gtk_radio_button_new_with_mnemonic (NULL, _("IPv6"));
  gtk_widget_show (IPv6_rdbt);
  gtk_box_pack_start (GTK_BOX (hbox50), IPv6_rdbt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (IPv6_rdbt), ippkt_radibt_group);
  ippkt_radibt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (IPv6_rdbt));

  arppkt_radiobt = gtk_radio_button_new_with_mnemonic (NULL, _("Arp packet"));
  gtk_widget_show (arppkt_radiobt);
  gtk_box_pack_start (GTK_BOX (hbox50), arppkt_radiobt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (arppkt_radiobt), ippkt_radibt_group);
  ippkt_radibt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (arppkt_radiobt));

  usedef2_radibt = gtk_radio_button_new_with_mnemonic (NULL, _("User defined payload"));
  gtk_widget_show (usedef2_radibt);
  gtk_box_pack_start (GTK_BOX (hbox50), usedef2_radibt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (usedef2_radibt), ippkt_radibt_group);
  ippkt_radibt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (usedef2_radibt));

  label179 = gtk_label_new (_("Link layer"));
  gtk_widget_show (label179);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label179);

  hbox71 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox71);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox71, FALSE, FALSE, 0);

  notebook2 = gtk_notebook_new ();
  gtk_widget_show (notebook2);
  gtk_box_pack_start (GTK_BOX (hbox71), notebook2, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (notebook2), 10);
  GTK_WIDGET_UNSET_FLAGS (notebook2, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook2), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook2), FALSE);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_container_add (GTK_CONTAINER (notebook2), frame2);

  vbox16 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox16);
  gtk_container_add (GTK_CONTAINER (frame2), vbox16);

  frame15 = gtk_frame_new (NULL);
  gtk_widget_show (frame15);
  gtk_box_pack_start (GTK_BOX (vbox16), frame15, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame15), 10);

  vbox26 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox26);
  gtk_container_add (GTK_CONTAINER (frame15), vbox26);

  hbox23 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox23);
  gtk_box_pack_start (GTK_BOX (vbox26), hbox23, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox23), 10);

  label50 = gtk_label_new (_("Version 0x"));
  gtk_widget_show (label50);
  gtk_box_pack_start (GTK_BOX (hbox23), label50, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label50), GTK_JUSTIFY_CENTER);

  entry26 = gtk_entry_new ();
  gtk_widget_show (entry26);
  gtk_box_pack_start (GTK_BOX (hbox23), entry26, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry26, 20, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry26), 1);
  gtk_entry_set_text (GTK_ENTRY (entry26), _("4"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry26), 8226);

  label51 = gtk_label_new (_("   Header length 0x"));
  gtk_widget_show (label51);
  gtk_box_pack_start (GTK_BOX (hbox23), label51, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label51), GTK_JUSTIFY_CENTER);

  entry27 = gtk_entry_new ();
  gtk_widget_show (entry27);
  gtk_box_pack_start (GTK_BOX (hbox23), entry27, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry27, 20, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry27), 1);
  gtk_entry_set_text (GTK_ENTRY (entry27), _("5"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry27), 8226);

  label52 = gtk_label_new (_("   TOS 0x"));
  gtk_widget_show (label52);
  gtk_box_pack_start (GTK_BOX (hbox23), label52, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label52), GTK_JUSTIFY_CENTER);

  entry28 = gtk_entry_new ();
  gtk_widget_show (entry28);
  gtk_box_pack_start (GTK_BOX (hbox23), entry28, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry28, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry28), 2);
  gtk_entry_set_text (GTK_ENTRY (entry28), _("00"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry28), 8226);

  button37 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button37);
  gtk_box_pack_start (GTK_BOX (hbox23), button37, FALSE, FALSE, 5);

  label53 = gtk_label_new (_("   Total length "));
  gtk_widget_show (label53);
  gtk_box_pack_start (GTK_BOX (hbox23), label53, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label53), GTK_JUSTIFY_CENTER);

  entry29 = gtk_entry_new ();
  gtk_widget_show (entry29);
  gtk_box_pack_start (GTK_BOX (hbox23), entry29, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry29, 50, -1);
  gtk_widget_set_sensitive (entry29, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry29), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry29), 8226);

  checkbutton21 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton21);
  gtk_box_pack_start (GTK_BOX (hbox23), checkbutton21, FALSE, FALSE, 3);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton21), TRUE);

  label54 = gtk_label_new (_("   Identification 0x"));
  gtk_widget_show (label54);
  gtk_box_pack_start (GTK_BOX (hbox23), label54, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label54), GTK_JUSTIFY_CENTER);

  entry30 = gtk_entry_new ();
  gtk_widget_show (entry30);
  gtk_box_pack_start (GTK_BOX (hbox23), entry30, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry30, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry30), 4);
  gtk_entry_set_text (GTK_ENTRY (entry30), _("1234"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry30), 8226);

  hbox24 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox24);
  gtk_box_pack_start (GTK_BOX (vbox26), hbox24, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox24), 10);

  label55 = gtk_label_new (_(" Flags "));
  gtk_widget_show (label55);
  gtk_box_pack_start (GTK_BOX (hbox24), label55, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label55), GTK_JUSTIFY_CENTER);

  entry31 = gtk_entry_new ();
  gtk_widget_show (entry31);
  gtk_box_pack_start (GTK_BOX (hbox24), entry31, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry31, 20, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry31), 1);
  gtk_entry_set_text (GTK_ENTRY (entry31), _("2"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry31), 8226);

  button78 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button78);
  gtk_box_pack_start (GTK_BOX (hbox24), button78, FALSE, FALSE, 5);

  label56 = gtk_label_new (_("   Fragment offset"));
  gtk_widget_show (label56);
  gtk_box_pack_start (GTK_BOX (hbox24), label56, FALSE, FALSE, 4);
  gtk_label_set_justify (GTK_LABEL (label56), GTK_JUSTIFY_CENTER);

  entry32 = gtk_entry_new ();
  gtk_widget_show (entry32);
  gtk_box_pack_start (GTK_BOX (hbox24), entry32, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry32, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry32), 4);
  gtk_entry_set_text (GTK_ENTRY (entry32), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry32), 8226);

  label57 = gtk_label_new (_("    TTL"));
  gtk_widget_show (label57);
  gtk_box_pack_start (GTK_BOX (hbox24), label57, FALSE, FALSE, 4);
  gtk_label_set_justify (GTK_LABEL (label57), GTK_JUSTIFY_CENTER);

  entry44 = gtk_entry_new ();
  gtk_widget_show (entry44);
  gtk_box_pack_start (GTK_BOX (hbox24), entry44, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry44, 35, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry44), 3);
  gtk_entry_set_text (GTK_ENTRY (entry44), _("255"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry44), 8226);

  label58 = gtk_label_new (_("    Protocol"));
  gtk_widget_show (label58);
  gtk_box_pack_start (GTK_BOX (hbox24), label58, FALSE, FALSE, 4);
  gtk_label_set_justify (GTK_LABEL (label58), GTK_JUSTIFY_CENTER);

  entry34 = gtk_entry_new ();
  gtk_widget_show (entry34);
  gtk_box_pack_start (GTK_BOX (hbox24), entry34, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry34, 35, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry34), 3);
  gtk_entry_set_text (GTK_ENTRY (entry34), _("17"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry34), 8226);

  optionmenu3 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu3);
  gtk_box_pack_start (GTK_BOX (hbox24), optionmenu3, FALSE, FALSE, 5);

  convertwidget22 = gtk_menu_new ();

  convertwidget23 = gtk_menu_item_new_with_mnemonic (_("Reserved"));
  gtk_widget_show (convertwidget23);
  gtk_container_add (GTK_CONTAINER (convertwidget22), convertwidget23);

  convertwidget24 = gtk_menu_item_new_with_mnemonic (_("ICMP"));
  gtk_widget_show (convertwidget24);
  gtk_container_add (GTK_CONTAINER (convertwidget22), convertwidget24);

  convertwidget25 = gtk_menu_item_new_with_mnemonic (_("IGMP"));
  gtk_widget_show (convertwidget25);
  gtk_container_add (GTK_CONTAINER (convertwidget22), convertwidget25);

  convertwidget26 = gtk_menu_item_new_with_mnemonic (_("TCP"));
  gtk_widget_show (convertwidget26);
  gtk_container_add (GTK_CONTAINER (convertwidget22), convertwidget26);

  convertwidget27 = gtk_menu_item_new_with_mnemonic (_("UDP"));
  gtk_widget_show (convertwidget27);
  gtk_container_add (GTK_CONTAINER (convertwidget22), convertwidget27);

  convertwidget28 = gtk_menu_item_new_with_mnemonic (_("Other"));
  gtk_widget_show (convertwidget28);
  gtk_container_add (GTK_CONTAINER (convertwidget22), convertwidget28);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu3), convertwidget22);

  label59 = gtk_label_new (_("   Header cks 0x"));
  gtk_widget_show (label59);
  gtk_box_pack_start (GTK_BOX (hbox24), label59, FALSE, FALSE, 4);
  gtk_label_set_justify (GTK_LABEL (label59), GTK_JUSTIFY_CENTER);

  entry35 = gtk_entry_new ();
  gtk_widget_show (entry35);
  gtk_box_pack_start (GTK_BOX (hbox24), entry35, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry35, 40, -1);
  gtk_widget_set_sensitive (entry35, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry35), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry35), 8226);

  ip_header_cks_cbt = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (ip_header_cks_cbt);
  gtk_box_pack_start (GTK_BOX (hbox24), ip_header_cks_cbt, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ip_header_cks_cbt), TRUE);

  frame16 = gtk_frame_new (NULL);
  gtk_widget_show (frame16);
  gtk_box_pack_start (GTK_BOX (vbox16), frame16, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame16), 10);

  hbox25 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox25);
  gtk_container_add (GTK_CONTAINER (frame16), hbox25);
  gtk_container_set_border_width (GTK_CONTAINER (hbox25), 10);

  label60 = gtk_label_new (_("Source IP"));
  gtk_widget_show (label60);
  gtk_box_pack_start (GTK_BOX (hbox25), label60, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label60), GTK_JUSTIFY_CENTER);

  entry38 = gtk_entry_new ();
  gtk_widget_show (entry38);
  gtk_box_pack_start (GTK_BOX (hbox25), entry38, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry38, 130, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry38), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry38), 8226);

  button24 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button24);
  gtk_box_pack_start (GTK_BOX (hbox25), button24, FALSE, FALSE, 5);

  label61 = gtk_label_new (_("     Destination IP "));
  gtk_widget_show (label61);
  gtk_box_pack_start (GTK_BOX (hbox25), label61, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label61), GTK_JUSTIFY_CENTER);

  entry37 = gtk_entry_new ();
  gtk_widget_show (entry37);
  gtk_box_pack_start (GTK_BOX (hbox25), entry37, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry37, 130, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry37), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry37), 8226);

  button25 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button25);
  gtk_box_pack_start (GTK_BOX (hbox25), button25, FALSE, FALSE, 5);

  label62 = gtk_label_new (_("       Options 0x"));
  gtk_widget_show (label62);
  gtk_box_pack_start (GTK_BOX (hbox25), label62, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label62), GTK_JUSTIFY_CENTER);

  entry39 = gtk_entry_new ();
  gtk_widget_show (entry39);
  gtk_box_pack_start (GTK_BOX (hbox25), entry39, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry39), 80);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry39), 8226);

  hbox69 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox69);
  gtk_box_pack_start (GTK_BOX (vbox16), hbox69, FALSE, FALSE, 5);

  frame17 = gtk_frame_new (NULL);
  gtk_widget_show (frame17);
  gtk_box_pack_start (GTK_BOX (hbox69), frame17, FALSE, FALSE, 80);
  gtk_container_set_border_width (GTK_CONTAINER (frame17), 10);

  hbox52 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox52);
  gtk_container_add (GTK_CONTAINER (frame17), hbox52);

  label64 = gtk_label_new (_("                          Next layer  ---->"));
  gtk_widget_show (label64);
  gtk_box_pack_start (GTK_BOX (hbox52), label64, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label64), GTK_JUSTIFY_CENTER);

  udp_bt = gtk_radio_button_new_with_mnemonic (NULL, _("UDP"));
  gtk_widget_show (udp_bt);
  gtk_box_pack_start (GTK_BOX (hbox52), udp_bt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (udp_bt), udp_bt_group);
  udp_bt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (udp_bt));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (udp_bt), TRUE);

  tcp_bt = gtk_radio_button_new_with_mnemonic (NULL, _("TCP"));
  gtk_widget_show (tcp_bt);
  gtk_box_pack_start (GTK_BOX (hbox52), tcp_bt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (tcp_bt), udp_bt_group);
  udp_bt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (tcp_bt));

  icmp_bt = gtk_radio_button_new_with_mnemonic (NULL, _("ICMP"));
  gtk_widget_show (icmp_bt);
  gtk_box_pack_start (GTK_BOX (hbox52), icmp_bt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (icmp_bt), udp_bt_group);
  udp_bt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (icmp_bt));

  igmp_bt = gtk_radio_button_new_with_mnemonic (NULL, _("IGMP"));
  gtk_widget_show (igmp_bt);
  gtk_box_pack_start (GTK_BOX (hbox52), igmp_bt, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (igmp_bt), udp_bt_group);
  udp_bt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (igmp_bt));

  ip_user_data_bt = gtk_radio_button_new_with_mnemonic (NULL, _("User defined payload"));
  gtk_widget_show (ip_user_data_bt);
  gtk_box_pack_start (GTK_BOX (hbox52), ip_user_data_bt, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (ip_user_data_bt), udp_bt_group);
  udp_bt_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (ip_user_data_bt));

  neki2 = gtk_label_new (_("IPv4 data"));
  gtk_widget_show (neki2);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), neki2);

  label4 = gtk_label_new (_("label4"));
  gtk_widget_show (label4);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 0), label4);
  gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_CENTER);

  frame13 = gtk_frame_new (NULL);
  gtk_widget_show (frame13);
  gtk_container_add (GTK_CONTAINER (notebook2), frame13);

  vbox79 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox79);
  gtk_container_add (GTK_CONTAINER (frame13), vbox79);

  frame466 = gtk_frame_new (NULL);
  gtk_widget_show (frame466);
  gtk_box_pack_start (GTK_BOX (vbox79), frame466, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame466), GTK_SHADOW_NONE);

  frame467 = gtk_frame_new (NULL);
  gtk_widget_show (frame467);
  gtk_container_add (GTK_CONTAINER (frame466), frame467);
  gtk_container_set_border_width (GTK_CONTAINER (frame467), 10);

  alignment8 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment8);
  gtk_container_add (GTK_CONTAINER (frame467), alignment8);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment8), 0, 0, 12, 0);

  vbox80 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox80);
  gtk_container_add (GTK_CONTAINER (alignment8), vbox80);

  hbox149 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox149);
  gtk_box_pack_start (GTK_BOX (vbox80), hbox149, FALSE, TRUE, 10);

  label363 = gtk_label_new (_("Ver 0x"));
  gtk_widget_show (label363);
  gtk_box_pack_start (GTK_BOX (hbox149), label363, FALSE, FALSE, 0);

  entry195 = gtk_entry_new ();
  gtk_widget_show (entry195);
  gtk_box_pack_start (GTK_BOX (hbox149), entry195, FALSE, TRUE, 5);
  gtk_widget_set_size_request (entry195, 19, -1);
  gtk_entry_set_text (GTK_ENTRY (entry195), _("6"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry195), 9679);
  gtk_entry_set_width_chars (GTK_ENTRY (entry195), 1);

  label364 = gtk_label_new (_(" Traffic Class 0x"));
  gtk_widget_show (label364);
  gtk_box_pack_start (GTK_BOX (hbox149), label364, FALSE, FALSE, 5);

  entry196 = gtk_entry_new ();
  gtk_widget_show (entry196);
  gtk_box_pack_start (GTK_BOX (hbox149), entry196, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry196, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry196), 2);
  gtk_entry_set_text (GTK_ENTRY (entry196), _("00"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry196), 9679);

  button90 = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (button90);
  gtk_box_pack_start (GTK_BOX (hbox149), button90, FALSE, FALSE, 5);

  label365 = gtk_label_new (_("   Flow label 0x "));
  gtk_widget_show (label365);
  gtk_box_pack_start (GTK_BOX (hbox149), label365, FALSE, FALSE, 0);

  entry197 = gtk_entry_new ();
  gtk_widget_show (entry197);
  gtk_box_pack_start (GTK_BOX (hbox149), entry197, FALSE, TRUE, 0);
  gtk_widget_set_size_request (entry197, 53, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry197), 5);
  gtk_entry_set_text (GTK_ENTRY (entry197), _("00000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry197), 9679);

  label366 = gtk_label_new (_("     Payload length "));
  gtk_widget_show (label366);
  gtk_box_pack_start (GTK_BOX (hbox149), label366, FALSE, FALSE, 0);

  entry198 = gtk_entry_new ();
  gtk_widget_show (entry198);
  gtk_box_pack_start (GTK_BOX (hbox149), entry198, FALSE, TRUE, 0);
  gtk_widget_set_size_request (entry198, 50, -1);
  gtk_widget_set_sensitive (entry198, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry198), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry198), 9679);

  checkbutton43 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton43);
  gtk_box_pack_start (GTK_BOX (hbox149), checkbutton43, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton43), TRUE);

  label367 = gtk_label_new (_("   Next Header 0x "));
  gtk_widget_show (label367);
  gtk_box_pack_start (GTK_BOX (hbox149), label367, FALSE, FALSE, 0);

  entry199 = gtk_entry_new ();
  gtk_widget_show (entry199);
  gtk_box_pack_start (GTK_BOX (hbox149), entry199, FALSE, TRUE, 0);
  gtk_widget_set_size_request (entry199, 26, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry199), 2);
  gtk_entry_set_text (GTK_ENTRY (entry199), _("11"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry199), 9679);

  label368 = gtk_label_new (_("     Hop Limit"));
  gtk_widget_show (label368);
  gtk_box_pack_start (GTK_BOX (hbox149), label368, FALSE, FALSE, 0);

  entry200 = gtk_entry_new ();
  gtk_widget_show (entry200);
  gtk_box_pack_start (GTK_BOX (hbox149), entry200, TRUE, TRUE, 3);
  gtk_widget_set_size_request (entry200, 37, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry200), 3);
  gtk_entry_set_text (GTK_ENTRY (entry200), _("255"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry200), 9679);

  frame468 = gtk_frame_new (NULL);
  gtk_widget_show (frame468);
  gtk_box_pack_start (GTK_BOX (vbox79), frame468, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame468), 10);

  alignment9 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment9);
  gtk_container_add (GTK_CONTAINER (frame468), alignment9);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment9), 0, 0, 12, 0);

  hbox152 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox152);
  gtk_container_add (GTK_CONTAINER (alignment9), hbox152);
  gtk_container_set_border_width (GTK_CONTAINER (hbox152), 10);

  label369 = gtk_label_new (_("Source IP"));
  gtk_widget_show (label369);
  gtk_box_pack_start (GTK_BOX (hbox152), label369, FALSE, FALSE, 0);

  entry201 = gtk_entry_new ();
  gtk_widget_show (entry201);
  gtk_box_pack_start (GTK_BOX (hbox152), entry201, FALSE, TRUE, 5);
  gtk_widget_set_size_request (entry201, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry201), 39);
  gtk_entry_set_text (GTK_ENTRY (entry201), _("0000:0000:0000:0000:0000:0000:0000:0000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry201), 9679);

  button88 = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (button88);
  gtk_box_pack_start (GTK_BOX (hbox152), button88, FALSE, FALSE, 6);

  label370 = gtk_label_new (_("  Dest IP"));
  gtk_widget_show (label370);
  gtk_box_pack_start (GTK_BOX (hbox152), label370, FALSE, FALSE, 5);

  entry202 = gtk_entry_new ();
  gtk_widget_show (entry202);
  gtk_box_pack_start (GTK_BOX (hbox152), entry202, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry202, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry202), 39);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry202), 9679);

  button89 = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (button89);
  gtk_box_pack_start (GTK_BOX (hbox152), button89, FALSE, FALSE, 5);

  frame469 = gtk_frame_new (NULL);
  gtk_widget_show (frame469);
  gtk_box_pack_start (GTK_BOX (vbox79), frame469, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame469), 10);

  alignment10 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment10);
  gtk_container_add (GTK_CONTAINER (frame469), alignment10);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment10), 0, 0, 12, 0);

  hbox153 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox153);
  gtk_container_add (GTK_CONTAINER (alignment10), hbox153);
  gtk_container_set_border_width (GTK_CONTAINER (hbox153), 10);

  label371 = gtk_label_new (_("Extension header "));
  gtk_widget_show (label371);
  gtk_box_pack_start (GTK_BOX (hbox153), label371, FALSE, FALSE, 0);

  entry203 = gtk_entry_new ();
  gtk_widget_show (entry203);
  gtk_box_pack_start (GTK_BOX (hbox153), entry203, TRUE, TRUE, 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry203), 9679);

  button91 = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (button91);
  gtk_box_pack_start (GTK_BOX (hbox153), button91, FALSE, FALSE, 5);

  hbox146 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox146);
  gtk_box_pack_start (GTK_BOX (vbox79), hbox146, TRUE, TRUE, 0);

  frame465 = gtk_frame_new (NULL);
  gtk_widget_show (frame465);
  gtk_box_pack_start (GTK_BOX (hbox146), frame465, FALSE, FALSE, 80);
  gtk_container_set_border_width (GTK_CONTAINER (frame465), 10);

  hbox147 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox147);
  gtk_container_add (GTK_CONTAINER (frame465), hbox147);

  label375 = gtk_label_new (_("                         Next layer   ---->"));
  gtk_widget_show (label375);
  gtk_box_pack_start (GTK_BOX (hbox147), label375, FALSE, FALSE, 0);

  radiobutton67 = gtk_radio_button_new_with_mnemonic (NULL, _("UDP"));
  gtk_widget_show (radiobutton67);
  gtk_box_pack_start (GTK_BOX (hbox147), radiobutton67, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton67), radiobutton67_group);
  radiobutton67_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton67));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton67), TRUE);

  radiobutton68 = gtk_radio_button_new_with_mnemonic (NULL, _("TCP"));
  gtk_widget_show (radiobutton68);
  gtk_box_pack_start (GTK_BOX (hbox147), radiobutton68, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton68), radiobutton67_group);
  radiobutton67_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton68));

  radiobutton69 = gtk_radio_button_new_with_mnemonic (NULL, _("ICMPv6"));
  gtk_widget_show (radiobutton69);
  gtk_box_pack_start (GTK_BOX (hbox147), radiobutton69, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton69), radiobutton67_group);
  radiobutton67_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton69));

  radiobutton71 = gtk_radio_button_new_with_mnemonic (NULL, _("User defined"));
  gtk_widget_show (radiobutton71);
  gtk_box_pack_start (GTK_BOX (hbox147), radiobutton71, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton71), radiobutton67_group);
  radiobutton67_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton71));

  neki3 = gtk_label_new (_("IPv6 data"));
  gtk_widget_show (neki3);
  gtk_frame_set_label_widget (GTK_FRAME (frame13), neki3);

  label117 = gtk_label_new (_("label117"));
  gtk_widget_show (label117);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 1), label117);
  gtk_label_set_justify (GTK_LABEL (label117), GTK_JUSTIFY_CENTER);

  frame4 = gtk_frame_new (NULL);
  gtk_widget_show (frame4);
  gtk_container_add (GTK_CONTAINER (notebook2), frame4);

  hbox56 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox56);
  gtk_container_add (GTK_CONTAINER (frame4), hbox56);

  frame18 = gtk_frame_new (NULL);
  gtk_widget_show (frame18);
  gtk_box_pack_start (GTK_BOX (hbox56), frame18, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame18), 10);

  scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow5);
  gtk_container_add (GTK_CONTAINER (frame18), scrolledwindow5);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow5, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_SHADOW_IN);

  text1 = gtk_text_view_new ();
  gtk_widget_show (text1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow5), text1);
  gtk_widget_set_size_request (text1, 350, -1);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text1), GTK_WRAP_WORD);

  hbox58 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox58);
  gtk_box_pack_start (GTK_BOX (hbox56), hbox58, FALSE, FALSE, 20);

  vbox31 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox31);
  gtk_box_pack_start (GTK_BOX (hbox58), vbox31, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox31), 10);

  frame19 = gtk_frame_new (NULL);
  gtk_widget_show (frame19);
  gtk_box_pack_start (GTK_BOX (vbox31), frame19, FALSE, FALSE, 0);

  vbox31_1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox31_1);
  gtk_container_add (GTK_CONTAINER (frame19), vbox31_1);
  gtk_container_set_border_width (GTK_CONTAINER (vbox31_1), 10);

  hbox59 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox59);
  gtk_box_pack_start (GTK_BOX (vbox31_1), hbox59, TRUE, TRUE, 10);

  label144 = gtk_label_new (_("Pattern "));
  gtk_widget_show (label144);
  gtk_box_pack_start (GTK_BOX (hbox59), label144, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label144), GTK_JUSTIFY_CENTER);

  entry79 = gtk_entry_new ();
  gtk_widget_show (entry79);
  gtk_box_pack_start (GTK_BOX (hbox59), entry79, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry79, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry79), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry79), 8226);

  hbox60 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox60);
  gtk_box_pack_start (GTK_BOX (vbox31_1), hbox60, FALSE, FALSE, 10);

  label145 = gtk_label_new (_("Length "));
  gtk_widget_show (label145);
  gtk_box_pack_start (GTK_BOX (hbox60), label145, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label145), GTK_JUSTIFY_CENTER);

  entry80 = gtk_entry_new ();
  gtk_widget_show (entry80);
  gtk_box_pack_start (GTK_BOX (hbox60), entry80, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry80, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry80), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry80), 8226);

  N_apply_pattern = gtk_button_new_with_mnemonic (_("Apply pattern"));
  gtk_widget_show (N_apply_pattern);
  gtk_box_pack_start (GTK_BOX (vbox31_1), N_apply_pattern, FALSE, FALSE, 10);

  N_select_payload = gtk_button_new_with_mnemonic (_("Select payload"));
  gtk_box_pack_start (GTK_BOX (vbox31), N_select_payload, FALSE, FALSE, 30);
  gtk_widget_set_sensitive (N_select_payload, FALSE);

  label146 = gtk_label_new ("");
  gtk_widget_show (label146);
  gtk_box_pack_start (GTK_BOX (hbox58), label146, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label146), GTK_JUSTIFY_CENTER);

  neki4 = gtk_label_new (_("User defined network payload"));
  gtk_widget_show (neki4);
  gtk_frame_set_label_widget (GTK_FRAME (frame4), neki4);

  label6 = gtk_label_new (_("User"));
  gtk_widget_show (label6);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 2), label6);
  gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_CENTER);

  vbox47 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox47);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox47);

  frame3 = gtk_frame_new (NULL);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (vbox47), frame3, FALSE, FALSE, 0);

  table1 = gtk_table_new (5, 9, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (frame3), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 10);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 6);

  label27 = gtk_label_new (_("HW type"));
  gtk_widget_show (label27);
  gtk_table_attach (GTK_TABLE (table1), label27, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label27), GTK_JUSTIFY_FILL);
  gtk_misc_set_alignment (GTK_MISC (label27), 0, 0.5);

  label28 = gtk_label_new (_("Prot type"));
  gtk_widget_show (label28);
  gtk_table_attach (GTK_TABLE (table1), label28, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label28), GTK_JUSTIFY_FILL);
  gtk_misc_set_alignment (GTK_MISC (label28), 0, 0.5);

  label29 = gtk_label_new (_("HW size"));
  gtk_widget_show (label29);
  gtk_table_attach (GTK_TABLE (table1), label29, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label29), GTK_JUSTIFY_FILL);
  gtk_misc_set_alignment (GTK_MISC (label29), 0, 0.5);

  label30 = gtk_label_new (_("Prot size"));
  gtk_widget_show (label30);
  gtk_table_attach (GTK_TABLE (table1), label30, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label30), GTK_JUSTIFY_FILL);
  gtk_misc_set_alignment (GTK_MISC (label30), 0, 0.5);

  label31 = gtk_label_new (_("Sender MAC"));
  gtk_widget_show (label31);
  gtk_table_attach (GTK_TABLE (table1), label31, 5, 6, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label31), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label31), 0, 0.5);

  label32 = gtk_label_new (_("Sender IP"));
  gtk_widget_show (label32);
  gtk_table_attach (GTK_TABLE (table1), label32, 5, 6, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label32), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label32), 0, 0.5);

  A_prottype = gtk_entry_new ();
  gtk_widget_show (A_prottype);
  gtk_table_attach (GTK_TABLE (table1), A_prottype, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_prottype, 60, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_prottype), 6);
  gtk_entry_set_text (GTK_ENTRY (A_prottype), _("0800"));
  gtk_entry_set_invisible_char (GTK_ENTRY (A_prottype), 8226);

  A_hwsize = gtk_entry_new ();
  gtk_widget_show (A_hwsize);
  gtk_table_attach (GTK_TABLE (table1), A_hwsize, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_hwsize, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_hwsize), 2);
  gtk_entry_set_text (GTK_ENTRY (A_hwsize), _("06"));
  gtk_entry_set_invisible_char (GTK_ENTRY (A_hwsize), 8226);

  A_protsize = gtk_entry_new ();
  gtk_widget_show (A_protsize);
  gtk_table_attach (GTK_TABLE (table1), A_protsize, 2, 3, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_protsize, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_protsize), 2);
  gtk_entry_set_text (GTK_ENTRY (A_protsize), _("04"));
  gtk_entry_set_invisible_char (GTK_ENTRY (A_protsize), 8226);

  A_sendermac = gtk_entry_new ();
  gtk_widget_show (A_sendermac);
  gtk_table_attach (GTK_TABLE (table1), A_sendermac, 6, 7, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_sendermac, 140, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_sendermac), 17);
  gtk_entry_set_invisible_char (GTK_ENTRY (A_sendermac), 8226);

  A_senderip = gtk_entry_new ();
  gtk_widget_show (A_senderip);
  gtk_table_attach (GTK_TABLE (table1), A_senderip, 6, 7, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_senderip, 130, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_senderip), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (A_senderip), 8226);

  A_hwtype = gtk_entry_new ();
  gtk_widget_show (A_hwtype);
  gtk_table_attach (GTK_TABLE (table1), A_hwtype, 2, 3, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_hwtype, 60, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_hwtype), 6);
  gtk_entry_set_text (GTK_ENTRY (A_hwtype), _("0001"));
  gtk_entry_set_invisible_char (GTK_ENTRY (A_hwtype), 8226);

  label118 = gtk_label_new ("");
  gtk_widget_show (label118);
  gtk_table_attach (GTK_TABLE (table1), label118, 8, 9, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label118), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label118), 0, 0.5);

  label36 = gtk_label_new ("");
  gtk_widget_show (label36);
  gtk_table_attach (GTK_TABLE (table1), label36, 8, 9, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label36), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label36), 0, 0.5);

  label37 = gtk_label_new ("");
  gtk_widget_show (label37);
  gtk_table_attach (GTK_TABLE (table1), label37, 8, 9, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label37), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label37), 0, 0.5);

  label38 = gtk_label_new ("");
  gtk_widget_show (label38);
  gtk_table_attach (GTK_TABLE (table1), label38, 8, 9, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label38), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label38), 0, 0.5);

  button33 = gtk_button_new_with_mnemonic (_("Select source IP&mac"));
  gtk_widget_show (button33);
  gtk_table_attach (GTK_TABLE (table1), button33, 7, 8, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  button34 = gtk_button_new_with_mnemonic (_("Apply local IP&mac"));
  gtk_table_attach (GTK_TABLE (table1), button34, 7, 8, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label147 = gtk_label_new (_("0x"));
  gtk_widget_show (label147);
  gtk_table_attach (GTK_TABLE (table1), label147, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label147), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label147), 0, 0.5);

  label148 = gtk_label_new (_("0x"));
  gtk_widget_show (label148);
  gtk_table_attach (GTK_TABLE (table1), label148, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label148), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label148), 0, 0.5);

  label149 = gtk_label_new (_("0x"));
  gtk_widget_show (label149);
  gtk_table_attach (GTK_TABLE (table1), label149, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label149), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label149), 0, 0.5);

  label150 = gtk_label_new (_("0x"));
  gtk_widget_show (label150);
  gtk_table_attach (GTK_TABLE (table1), label150, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label150), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label150), 0, 0.5);

  label151 = gtk_label_new ("");
  gtk_widget_show (label151);
  gtk_table_attach (GTK_TABLE (table1), label151, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label151), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label151), 0, 0.5);

  label152 = gtk_label_new ("");
  gtk_widget_show (label152);
  gtk_table_attach (GTK_TABLE (table1), label152, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label152), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label152), 0, 0.5);

  label153 = gtk_label_new ("");
  gtk_widget_show (label153);
  gtk_table_attach (GTK_TABLE (table1), label153, 3, 4, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label153), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label153), 0, 0.5);

  label154 = gtk_label_new ("");
  gtk_widget_show (label154);
  gtk_table_attach (GTK_TABLE (table1), label154, 3, 4, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label154), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label154), 0, 0.5);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_table_attach (GTK_TABLE (table1), hseparator1, 5, 6, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  label34 = gtk_label_new (_("Target IP"));
  gtk_widget_show (label34);
  gtk_table_attach (GTK_TABLE (table1), label34, 5, 6, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label34), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label34), 0, 0.5);

  label33 = gtk_label_new (_("Target MAC"));
  gtk_widget_show (label33);
  gtk_table_attach (GTK_TABLE (table1), label33, 5, 6, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label33), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label33), 0, 0.5);

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_show (hseparator2);
  gtk_table_attach (GTK_TABLE (table1), hseparator2, 6, 7, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  A_targetip = gtk_entry_new ();
  gtk_widget_show (A_targetip);
  gtk_table_attach (GTK_TABLE (table1), A_targetip, 6, 7, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_targetip, 130, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_targetip), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (A_targetip), 8226);

  A_targetmac = gtk_entry_new ();
  gtk_widget_show (A_targetmac);
  gtk_table_attach (GTK_TABLE (table1), A_targetmac, 6, 7, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (A_targetmac, 140, -1);
  gtk_entry_set_max_length (GTK_ENTRY (A_targetmac), 17);
  gtk_entry_set_invisible_char (GTK_ENTRY (A_targetmac), 8226);

  hseparator3 = gtk_hseparator_new ();
  gtk_widget_show (hseparator3);
  gtk_table_attach (GTK_TABLE (table1), hseparator3, 7, 8, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  button36 = gtk_button_new_with_mnemonic (_("Apply broadcast mac"));
  gtk_table_attach (GTK_TABLE (table1), button36, 7, 8, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  button35 = gtk_button_new_with_mnemonic (_("Select destination IP&mac"));
  gtk_widget_show (button35);
  gtk_table_attach (GTK_TABLE (table1), button35, 7, 8, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  radiobutton11 = gtk_radio_button_new_with_mnemonic (NULL, _("ARP reply (0x0002)"));
  gtk_widget_show (radiobutton11);
  gtk_table_attach (GTK_TABLE (table1), radiobutton11, 4, 5, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton11), radiobutton11_group);
  radiobutton11_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton11));

  radiobutton10 = gtk_radio_button_new_with_mnemonic (NULL, _("ARP request (0x0001)"));
  gtk_widget_show (radiobutton10);
  gtk_table_attach (GTK_TABLE (table1), radiobutton10, 4, 5, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton10), radiobutton11_group);
  radiobutton11_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton10));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton10), TRUE);

  label155 = gtk_label_new (_("     Message type"));
  gtk_widget_show (label155);
  gtk_table_attach (GTK_TABLE (table1), label155, 4, 5, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label155), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label155), 0, 0.5);

  hbox61 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox61);
  gtk_table_attach (GTK_TABLE (table1), hbox61, 4, 5, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  radiobutton17 = gtk_radio_button_new_with_mnemonic (NULL, _("other    0x"));
  gtk_widget_show (radiobutton17);
  gtk_box_pack_start (GTK_BOX (hbox61), radiobutton17, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton17), radiobutton11_group);
  radiobutton11_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton17));

  entry81 = gtk_entry_new ();
  gtk_widget_show (entry81);
  gtk_box_pack_start (GTK_BOX (hbox61), entry81, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry81, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry81), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry81), 8226);

  neki5 = gtk_label_new (_("Arp payload"));
  gtk_widget_show (neki5);
  gtk_frame_set_label_widget (GTK_FRAME (frame3), neki5);

  label_zapolni_arp_spodaj = gtk_label_new ("");
  gtk_widget_show (label_zapolni_arp_spodaj);
  gtk_box_pack_start (GTK_BOX (vbox47), label_zapolni_arp_spodaj, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label_zapolni_arp_spodaj), GTK_JUSTIFY_CENTER);

  label5 = gtk_label_new (_("label5"));
  gtk_widget_show (label5);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 3), label5);
  gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_CENTER);

  hbox70 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox70);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox70, FALSE, FALSE, 0);

  notebook4 = gtk_notebook_new ();
  gtk_widget_show (notebook4);
  gtk_box_pack_start (GTK_BOX (hbox70), notebook4, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (notebook4), 10);
  GTK_WIDGET_UNSET_FLAGS (notebook4, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook4), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook4), FALSE);

  frame9 = gtk_frame_new (NULL);
  gtk_widget_show (frame9);
  gtk_container_add (GTK_CONTAINER (notebook4), frame9);

  vbox19 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox19);
  gtk_container_add (GTK_CONTAINER (frame9), vbox19);

  frame22 = gtk_frame_new (NULL);
  gtk_widget_show (frame22);
  gtk_box_pack_start (GTK_BOX (vbox19), frame22, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame22), 10);

  hbox27 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox27);
  gtk_container_add (GTK_CONTAINER (frame22), hbox27);
  gtk_container_set_border_width (GTK_CONTAINER (hbox27), 10);

  label70 = gtk_label_new (_("Source port"));
  gtk_widget_show (label70);
  gtk_box_pack_start (GTK_BOX (hbox27), label70, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label70), GTK_JUSTIFY_CENTER);

  entry56 = gtk_entry_new ();
  gtk_widget_show (entry56);
  gtk_box_pack_start (GTK_BOX (hbox27), entry56, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry56, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry56), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry56), 8226);

  label71 = gtk_label_new ("");
  gtk_widget_show (label71);
  gtk_box_pack_start (GTK_BOX (hbox27), label71, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label71), GTK_JUSTIFY_CENTER);

  label73 = gtk_label_new (_("     Destination port"));
  gtk_widget_show (label73);
  gtk_box_pack_start (GTK_BOX (hbox27), label73, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label73), GTK_JUSTIFY_CENTER);

  entry41 = gtk_entry_new ();
  gtk_widget_show (entry41);
  gtk_box_pack_start (GTK_BOX (hbox27), entry41, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry41, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry41), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry41), 8226);

  label72 = gtk_label_new (_("     Length"));
  gtk_widget_show (label72);
  gtk_box_pack_start (GTK_BOX (hbox27), label72, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label72), GTK_JUSTIFY_CENTER);

  entry42 = gtk_entry_new ();
  gtk_widget_show (entry42);
  gtk_box_pack_start (GTK_BOX (hbox27), entry42, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry42, 50, -1);
  gtk_widget_set_sensitive (entry42, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry42), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry42), 8226);

  checkbutton3 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton3);
  gtk_box_pack_start (GTK_BOX (hbox27), checkbutton3, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton3), TRUE);

  label74 = gtk_label_new (_("   Checksum 0x"));
  gtk_widget_show (label74);
  gtk_box_pack_start (GTK_BOX (hbox27), label74, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label74), GTK_JUSTIFY_CENTER);

  entry43 = gtk_entry_new ();
  gtk_widget_show (entry43);
  gtk_box_pack_start (GTK_BOX (hbox27), entry43, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry43, 40, -1);
  gtk_widget_set_sensitive (entry43, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry43), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry43), 8226);

  checkbutton4 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton4);
  gtk_box_pack_start (GTK_BOX (hbox27), checkbutton4, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton4), TRUE);

  frame23 = gtk_frame_new (NULL);
  gtk_widget_show (frame23);
  gtk_box_pack_start (GTK_BOX (vbox19), frame23, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame23), 10);

  hbox28 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox28);
  gtk_container_add (GTK_CONTAINER (frame23), hbox28);
  gtk_container_set_border_width (GTK_CONTAINER (hbox28), 10);

  checkbutton5 = gtk_check_button_new_with_mnemonic (_("Udp payload    0x"));
  gtk_widget_show (checkbutton5);
  gtk_box_pack_start (GTK_BOX (hbox28), checkbutton5, FALSE, FALSE, 5);

  frame24 = gtk_frame_new (NULL);
  gtk_widget_show (frame24);
  gtk_box_pack_start (GTK_BOX (hbox28), frame24, FALSE, FALSE, 0);

  scrolledwindow7 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow7);
  gtk_container_add (GTK_CONTAINER (frame24), scrolledwindow7);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow7, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow7), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow7), GTK_SHADOW_IN);

  text3 = gtk_text_view_new ();
  gtk_widget_show (text3);
  gtk_container_add (GTK_CONTAINER (scrolledwindow7), text3);
  gtk_widget_set_size_request (text3, 350, -1);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text3), GTK_WRAP_WORD);

  label192 = gtk_label_new (_("         "));
  gtk_widget_show (label192);
  gtk_box_pack_start (GTK_BOX (hbox28), label192, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label192), GTK_JUSTIFY_CENTER);

  vbox52 = gtk_vbox_new (FALSE, 7);
  gtk_widget_show (vbox52);
  gtk_box_pack_start (GTK_BOX (hbox28), vbox52, FALSE, FALSE, 0);

  frame25 = gtk_frame_new (NULL);
  gtk_widget_show (frame25);
  gtk_box_pack_start (GTK_BOX (vbox52), frame25, FALSE, TRUE, 0);

  vbox53 = gtk_vbox_new (FALSE, 15);
  gtk_widget_show (vbox53);
  gtk_container_add (GTK_CONTAINER (frame25), vbox53);
  gtk_container_set_border_width (GTK_CONTAINER (vbox53), 10);

  hbox81 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox81);
  gtk_box_pack_start (GTK_BOX (vbox53), hbox81, TRUE, TRUE, 0);

  label193 = gtk_label_new (_("Pattern: "));
  gtk_widget_show (label193);
  gtk_box_pack_start (GTK_BOX (hbox81), label193, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label193), GTK_JUSTIFY_CENTER);

  entry89 = gtk_entry_new ();
  gtk_widget_show (entry89);
  gtk_box_pack_start (GTK_BOX (hbox81), entry89, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry89, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry89), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry89), 8226);

  hbox82 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox82);
  gtk_box_pack_start (GTK_BOX (vbox53), hbox82, TRUE, TRUE, 0);

  label194 = gtk_label_new (_("Length: "));
  gtk_widget_show (label194);
  gtk_box_pack_start (GTK_BOX (hbox82), label194, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label194), GTK_JUSTIFY_CENTER);

  entry90 = gtk_entry_new ();
  gtk_widget_show (entry90);
  gtk_box_pack_start (GTK_BOX (hbox82), entry90, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry90, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry90), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry90), 8226);

  udp_apply_pattern_button = gtk_button_new_with_mnemonic (_("Apply pattern"));
  gtk_widget_show (udp_apply_pattern_button);
  gtk_box_pack_start (GTK_BOX (vbox53), udp_apply_pattern_button, FALSE, FALSE, 0);

  udp_select_payload_button = gtk_button_new_with_mnemonic (_("Select payload"));
  gtk_widget_show (udp_select_payload_button);
  gtk_box_pack_start (GTK_BOX (vbox52), udp_select_payload_button, FALSE, FALSE, 10);

  label191 = gtk_label_new (_("          "));
  gtk_widget_show (label191);
  gtk_box_pack_start (GTK_BOX (hbox28), label191, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label191), GTK_JUSTIFY_CENTER);

  neki9 = gtk_label_new (_("UDP data"));
  gtk_widget_show (neki9);
  gtk_frame_set_label_widget (GTK_FRAME (frame9), neki9);

  label65 = gtk_label_new (_("label65"));
  gtk_widget_show (label65);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook4), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook4), 0), label65);
  gtk_label_set_justify (GTK_LABEL (label65), GTK_JUSTIFY_CENTER);

  vbox29 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox29);
  gtk_container_add (GTK_CONTAINER (notebook4), vbox29);

  frame8 = gtk_frame_new (NULL);
  gtk_widget_show (frame8);
  gtk_box_pack_start (GTK_BOX (vbox29), frame8, FALSE, FALSE, 0);

  vbox55 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox55);
  gtk_container_add (GTK_CONTAINER (frame8), vbox55);

  frame29 = gtk_frame_new (NULL);
  gtk_widget_show (frame29);
  gtk_box_pack_start (GTK_BOX (vbox55), frame29, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame29), 10);

  vbox18 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox18);
  gtk_container_add (GTK_CONTAINER (frame29), vbox18);

  hbox29 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox29);
  gtk_box_pack_start (GTK_BOX (vbox18), hbox29, FALSE, FALSE, 10);

  label75 = gtk_label_new (_("Source port"));
  gtk_widget_show (label75);
  gtk_box_pack_start (GTK_BOX (hbox29), label75, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label75), GTK_JUSTIFY_CENTER);

  entry46 = gtk_entry_new ();
  gtk_widget_show (entry46);
  gtk_box_pack_start (GTK_BOX (hbox29), entry46, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry46, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry46), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry46), 8226);

  label76 = gtk_label_new (_("     Destination port"));
  gtk_widget_show (label76);
  gtk_box_pack_start (GTK_BOX (hbox29), label76, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label76), GTK_JUSTIFY_CENTER);

  entry47 = gtk_entry_new ();
  gtk_widget_show (entry47);
  gtk_box_pack_start (GTK_BOX (hbox29), entry47, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry47, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry47), 5);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry47), 8226);

  label77 = gtk_label_new (_("     Sequence number"));
  gtk_widget_show (label77);
  gtk_box_pack_start (GTK_BOX (hbox29), label77, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label77), GTK_JUSTIFY_CENTER);

  entry48 = gtk_entry_new ();
  gtk_widget_show (entry48);
  gtk_box_pack_start (GTK_BOX (hbox29), entry48, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry48, 100, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry48), 10);
  gtk_entry_set_text (GTK_ENTRY (entry48), _("100"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry48), 8226);

  label78 = gtk_label_new (_("     Ack number"));
  gtk_widget_show (label78);
  gtk_box_pack_start (GTK_BOX (hbox29), label78, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label78), GTK_JUSTIFY_CENTER);

  entry49 = gtk_entry_new ();
  gtk_widget_show (entry49);
  gtk_box_pack_start (GTK_BOX (hbox29), entry49, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry49, 100, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry49), 10);
  gtk_entry_set_text (GTK_ENTRY (entry49), _("100"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry49), 8226);

  hbox30 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox30);
  gtk_box_pack_start (GTK_BOX (vbox18), hbox30, FALSE, FALSE, 10);

  label79 = gtk_label_new (_("Header length"));
  gtk_widget_show (label79);
  gtk_box_pack_start (GTK_BOX (hbox30), label79, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label79), GTK_JUSTIFY_CENTER);

  entry50 = gtk_entry_new ();
  gtk_widget_show (entry50);
  gtk_box_pack_start (GTK_BOX (hbox30), entry50, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry50, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry50), 2);
  gtk_entry_set_text (GTK_ENTRY (entry50), _("20"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry50), 8226);

  label80 = gtk_label_new (_("            Flags:   "));
  gtk_widget_show (label80);
  gtk_box_pack_start (GTK_BOX (hbox30), label80, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label80), GTK_JUSTIFY_CENTER);

  checkbutton22 = gtk_check_button_new_with_mnemonic (_("CWR "));
  gtk_widget_show (checkbutton22);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton22, FALSE, FALSE, 0);

  checkbutton23 = gtk_check_button_new_with_mnemonic (_("ECN "));
  gtk_widget_show (checkbutton23);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton23, FALSE, FALSE, 0);

  checkbutton7 = gtk_check_button_new_with_mnemonic (_("URG"));
  gtk_widget_show (checkbutton7);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton7, FALSE, FALSE, 5);

  checkbutton8 = gtk_check_button_new_with_mnemonic (_("ACK"));
  gtk_widget_show (checkbutton8);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton8, FALSE, FALSE, 5);

  checkbutton9 = gtk_check_button_new_with_mnemonic (_("PSH"));
  gtk_widget_show (checkbutton9);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton9, FALSE, FALSE, 5);

  checkbutton10 = gtk_check_button_new_with_mnemonic (_("RST"));
  gtk_widget_show (checkbutton10);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton10, FALSE, FALSE, 5);

  checkbutton11 = gtk_check_button_new_with_mnemonic (_("SYN"));
  gtk_widget_show (checkbutton11);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton11, FALSE, FALSE, 5);

  checkbutton12 = gtk_check_button_new_with_mnemonic (_("FIN"));
  gtk_widget_show (checkbutton12);
  gtk_box_pack_start (GTK_BOX (hbox30), checkbutton12, FALSE, FALSE, 5);

  hbox31 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox31);
  gtk_box_pack_start (GTK_BOX (vbox18), hbox31, FALSE, FALSE, 10);

  label81 = gtk_label_new (_("Window size"));
  gtk_widget_show (label81);
  gtk_box_pack_start (GTK_BOX (hbox31), label81, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label81), GTK_JUSTIFY_CENTER);

  entry51 = gtk_entry_new ();
  gtk_widget_show (entry51);
  gtk_box_pack_start (GTK_BOX (hbox31), entry51, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry51, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry51), 5);
  gtk_entry_set_text (GTK_ENTRY (entry51), _("4000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry51), 8226);

  label82 = gtk_label_new (_("     Checksum 0x"));
  gtk_widget_show (label82);
  gtk_box_pack_start (GTK_BOX (hbox31), label82, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label82), GTK_JUSTIFY_CENTER);

  entry52 = gtk_entry_new ();
  gtk_widget_show (entry52);
  gtk_box_pack_start (GTK_BOX (hbox31), entry52, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry52, 40, -1);
  gtk_widget_set_sensitive (entry52, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry52), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry52), 8226);

  checkbutton13 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton13);
  gtk_box_pack_start (GTK_BOX (hbox31), checkbutton13, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton13), TRUE);

  label83 = gtk_label_new (_("    Urgent pointer"));
  gtk_widget_show (label83);
  gtk_box_pack_start (GTK_BOX (hbox31), label83, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label83), GTK_JUSTIFY_CENTER);

  entry53 = gtk_entry_new ();
  gtk_widget_show (entry53);
  gtk_box_pack_start (GTK_BOX (hbox31), entry53, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry53, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry53), 5);
  gtk_entry_set_text (GTK_ENTRY (entry53), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry53), 8226);

  label84 = gtk_label_new (_("     Options 0x"));
  gtk_widget_show (label84);
  gtk_box_pack_start (GTK_BOX (hbox31), label84, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label84), GTK_JUSTIFY_CENTER);

  entry54 = gtk_entry_new ();
  gtk_widget_show (entry54);
  gtk_box_pack_start (GTK_BOX (hbox31), entry54, FALSE, FALSE, 0);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry54), 8226);

  frame30 = gtk_frame_new (NULL);
  gtk_widget_show (frame30);
  gtk_box_pack_start (GTK_BOX (vbox55), frame30, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame30), 10);

  hbox90 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox90);
  gtk_container_add (GTK_CONTAINER (frame30), hbox90);

  checkbutton14 = gtk_check_button_new_with_mnemonic (_("Tcp payload"));
  gtk_widget_show (checkbutton14);
  gtk_box_pack_start (GTK_BOX (hbox90), checkbutton14, FALSE, FALSE, 5);

  frame31 = gtk_frame_new (NULL);
  gtk_widget_show (frame31);
  gtk_box_pack_start (GTK_BOX (hbox90), frame31, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame31), 10);

  scrolledwindow8 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow8);
  gtk_container_add (GTK_CONTAINER (frame31), scrolledwindow8);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow8, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow8), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow8), GTK_SHADOW_IN);

  text4 = gtk_text_view_new ();
  gtk_widget_show (text4);
  gtk_container_add (GTK_CONTAINER (scrolledwindow8), text4);
  gtk_widget_set_size_request (text4, 350, -1);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text4), GTK_WRAP_WORD);

  label228 = gtk_label_new (_("       "));
  gtk_widget_show (label228);
  gtk_box_pack_start (GTK_BOX (hbox90), label228, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label228), GTK_JUSTIFY_CENTER);

  vbox56 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox56);
  gtk_box_pack_start (GTK_BOX (hbox90), vbox56, FALSE, FALSE, 0);

  frame32 = gtk_frame_new (NULL);
  gtk_widget_show (frame32);
  gtk_box_pack_start (GTK_BOX (vbox56), frame32, TRUE, TRUE, 10);

  vbox57 = gtk_vbox_new (FALSE, 15);
  gtk_widget_show (vbox57);
  gtk_container_add (GTK_CONTAINER (frame32), vbox57);
  gtk_container_set_border_width (GTK_CONTAINER (vbox57), 10);

  hbox91 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox91);
  gtk_box_pack_start (GTK_BOX (vbox57), hbox91, TRUE, TRUE, 0);

  label230 = gtk_label_new (_("Pattern: "));
  gtk_widget_show (label230);
  gtk_box_pack_start (GTK_BOX (hbox91), label230, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label230), GTK_JUSTIFY_CENTER);

  entry107 = gtk_entry_new ();
  gtk_widget_show (entry107);
  gtk_box_pack_start (GTK_BOX (hbox91), entry107, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry107, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry107), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry107), 8226);

  hbox92 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox92);
  gtk_box_pack_start (GTK_BOX (vbox57), hbox92, TRUE, TRUE, 0);

  label231 = gtk_label_new (_("Length: "));
  gtk_widget_show (label231);
  gtk_box_pack_start (GTK_BOX (hbox92), label231, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label231), GTK_JUSTIFY_CENTER);

  entry108 = gtk_entry_new ();
  gtk_widget_show (entry108);
  gtk_box_pack_start (GTK_BOX (hbox92), entry108, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry108, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry108), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry108), 8226);

  apply_tcp_pattern_bt = gtk_button_new_with_mnemonic (_("Apply pattern"));
  gtk_widget_show (apply_tcp_pattern_bt);
  gtk_box_pack_start (GTK_BOX (vbox57), apply_tcp_pattern_bt, FALSE, FALSE, 0);

  select_tpc_payload_bt = gtk_button_new_with_mnemonic (_("Select payload"));
  gtk_box_pack_start (GTK_BOX (vbox56), select_tpc_payload_bt, FALSE, FALSE, 10);
  gtk_widget_set_sensitive (select_tpc_payload_bt, FALSE);

  label229 = gtk_label_new (_("     "));
  gtk_widget_show (label229);
  gtk_box_pack_start (GTK_BOX (hbox90), label229, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label229), GTK_JUSTIFY_CENTER);

  neki6 = gtk_label_new (_("TCP data"));
  gtk_widget_show (neki6);
  gtk_frame_set_label_widget (GTK_FRAME (frame8), neki6);

  label66 = gtk_label_new (_("label66"));
  gtk_widget_show (label66);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook4), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook4), 1), label66);
  gtk_label_set_justify (GTK_LABEL (label66), GTK_JUSTIFY_CENTER);

  frame10 = gtk_frame_new (NULL);
  gtk_widget_show (frame10);
  gtk_container_add (GTK_CONTAINER (notebook4), frame10);

  vbox20 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox20);
  gtk_container_add (GTK_CONTAINER (frame10), vbox20);

  hbox33 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox33);
  gtk_box_pack_start (GTK_BOX (vbox20), hbox33, FALSE, FALSE, 10);

  label85 = gtk_label_new (_("Type  0x"));
  gtk_widget_show (label85);
  gtk_box_pack_start (GTK_BOX (hbox33), label85, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label85), GTK_JUSTIFY_CENTER);

  entry57 = gtk_entry_new ();
  gtk_widget_show (entry57);
  gtk_box_pack_start (GTK_BOX (hbox33), entry57, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry57, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry57), 2);
  gtk_entry_set_text (GTK_ENTRY (entry57), _("00"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry57), 8226);

  optionmenu4 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu4);
  gtk_box_pack_start (GTK_BOX (hbox33), optionmenu4, FALSE, FALSE, 7);

  convertwidget35 = gtk_menu_new ();

  convertwidget36 = gtk_menu_item_new_with_mnemonic (_("Echo reply"));
  gtk_widget_show (convertwidget36);
  gtk_container_add (GTK_CONTAINER (convertwidget35), convertwidget36);

  convertwidget37 = gtk_menu_item_new_with_mnemonic (_("Destination unreacheable"));
  gtk_widget_show (convertwidget37);
  gtk_container_add (GTK_CONTAINER (convertwidget35), convertwidget37);

  convertwidget38 = gtk_menu_item_new_with_mnemonic (_("Echo request"));
  gtk_widget_show (convertwidget38);
  gtk_container_add (GTK_CONTAINER (convertwidget35), convertwidget38);

  convertwidget39 = gtk_menu_item_new_with_mnemonic (_("Other"));
  gtk_widget_show (convertwidget39);
  gtk_container_add (GTK_CONTAINER (convertwidget35), convertwidget39);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu4), convertwidget35);

  frame12 = gtk_frame_new (NULL);
  gtk_widget_show (frame12);
  gtk_box_pack_start (GTK_BOX (vbox20), frame12, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame12), 5);

  notebook5 = gtk_notebook_new ();
  gtk_widget_show (notebook5);
  gtk_container_add (GTK_CONTAINER (frame12), notebook5);
  GTK_WIDGET_UNSET_FLAGS (notebook5, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook5), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook5), FALSE);

  vbox22 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox22);
  gtk_container_add (GTK_CONTAINER (notebook5), vbox22);

  hbox38 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox38);
  gtk_box_pack_start (GTK_BOX (vbox22), hbox38, FALSE, FALSE, 10);

  label105 = gtk_label_new (_("Code    0x"));
  gtk_widget_show (label105);
  gtk_box_pack_start (GTK_BOX (hbox38), label105, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label105), GTK_JUSTIFY_CENTER);

  entry62 = gtk_entry_new ();
  gtk_widget_show (entry62);
  gtk_box_pack_start (GTK_BOX (hbox38), entry62, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry62, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry62), 2);
  gtk_entry_set_text (GTK_ENTRY (entry62), _("00"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry62), 8226);

  label106 = gtk_label_new (_("        Checksum   0x"));
  gtk_widget_show (label106);
  gtk_box_pack_start (GTK_BOX (hbox38), label106, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label106), GTK_JUSTIFY_CENTER);

  entry63 = gtk_entry_new ();
  gtk_widget_show (entry63);
  gtk_box_pack_start (GTK_BOX (hbox38), entry63, FALSE, FALSE, 10);
  gtk_widget_set_size_request (entry63, 40, -1);
  gtk_widget_set_sensitive (entry63, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry63), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry63), 8226);

  checkbutton16 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton16);
  gtk_box_pack_start (GTK_BOX (hbox38), checkbutton16, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton16), TRUE);

  hbox40 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox40);
  gtk_box_pack_start (GTK_BOX (vbox22), hbox40, FALSE, FALSE, 10);

  label107 = gtk_label_new (_("Identifier    0x"));
  gtk_widget_show (label107);
  gtk_box_pack_start (GTK_BOX (hbox40), label107, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label107), GTK_JUSTIFY_CENTER);

  entry64 = gtk_entry_new ();
  gtk_widget_show (entry64);
  gtk_box_pack_start (GTK_BOX (hbox40), entry64, FALSE, FALSE, 4);
  gtk_widget_set_size_request (entry64, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry64), 4);
  gtk_entry_set_text (GTK_ENTRY (entry64), _("0001"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry64), 8226);

  label108 = gtk_label_new (_("     Seq. number   0x"));
  gtk_widget_show (label108);
  gtk_box_pack_start (GTK_BOX (hbox40), label108, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label108), GTK_JUSTIFY_CENTER);

  entry65 = gtk_entry_new ();
  gtk_widget_show (entry65);
  gtk_box_pack_start (GTK_BOX (hbox40), entry65, FALSE, FALSE, 4);
  gtk_widget_set_size_request (entry65, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry65), 4);
  gtk_entry_set_text (GTK_ENTRY (entry65), _("0001"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry65), 8226);

  hbox42 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox42);
  gtk_box_pack_start (GTK_BOX (vbox22), hbox42, FALSE, FALSE, 10);

  checkbutton17 = gtk_check_button_new_with_mnemonic (_("Data"));
  gtk_widget_show (checkbutton17);
  gtk_box_pack_start (GTK_BOX (hbox42), checkbutton17, FALSE, FALSE, 10);

  entry66 = gtk_entry_new ();
  gtk_widget_show (entry66);
  gtk_box_pack_start (GTK_BOX (hbox42), entry66, FALSE, FALSE, 10);
  gtk_entry_set_max_length (GTK_ENTRY (entry66), 1500);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry66), 8226);

  label104 = gtk_label_new (_("echo reply"));
  gtk_widget_show (label104);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 0), label104);
  gtk_label_set_justify (GTK_LABEL (label104), GTK_JUSTIFY_CENTER);

  vbox67 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox67);
  gtk_container_add (GTK_CONTAINER (notebook5), vbox67);

  hbox112 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox112);
  gtk_box_pack_start (GTK_BOX (vbox67), hbox112, TRUE, TRUE, 0);

  label284 = gtk_label_new (_("Code    0x"));
  gtk_widget_show (label284);
  gtk_box_pack_start (GTK_BOX (hbox112), label284, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label284), GTK_JUSTIFY_CENTER);

  entry157 = gtk_entry_new ();
  gtk_widget_show (entry157);
  gtk_box_pack_start (GTK_BOX (hbox112), entry157, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry157, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry157), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry157), 8226);

  label285 = gtk_label_new (_("        Checksum   0x"));
  gtk_widget_show (label285);
  gtk_box_pack_start (GTK_BOX (hbox112), label285, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label285), GTK_JUSTIFY_CENTER);

  entry158 = gtk_entry_new ();
  gtk_widget_show (entry158);
  gtk_box_pack_start (GTK_BOX (hbox112), entry158, FALSE, FALSE, 10);
  gtk_widget_set_size_request (entry158, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry158), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry158), 8226);

  checkbutton38 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton38);
  gtk_box_pack_start (GTK_BOX (hbox112), checkbutton38, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton38), TRUE);

  hbox113 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox113);
  gtk_box_pack_start (GTK_BOX (vbox67), hbox113, TRUE, TRUE, 0);

  label286 = gtk_label_new (_("Contents"));
  gtk_widget_show (label286);
  gtk_box_pack_start (GTK_BOX (hbox113), label286, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label286), GTK_JUSTIFY_CENTER);

  entry159 = gtk_entry_new ();
  gtk_widget_show (entry159);
  gtk_box_pack_start (GTK_BOX (hbox113), entry159, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry159), 1500);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry159), 8226);

  label283 = gtk_label_new (_("other"));
  gtk_widget_show (label283);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 1), label283);
  gtk_label_set_justify (GTK_LABEL (label283), GTK_JUSTIFY_CENTER);

  vbox21 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox21);
  gtk_container_add (GTK_CONTAINER (notebook5), vbox21);

  hbox34 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox34);
  gtk_box_pack_start (GTK_BOX (vbox21), hbox34, FALSE, FALSE, 5);

  label86 = gtk_label_new (_("Code      0x"));
  gtk_widget_show (label86);
  gtk_box_pack_start (GTK_BOX (hbox34), label86, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label86), GTK_JUSTIFY_CENTER);

  entry58 = gtk_entry_new ();
  gtk_widget_show (entry58);
  gtk_box_pack_start (GTK_BOX (hbox34), entry58, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry58, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry58), 2);
  gtk_entry_set_text (GTK_ENTRY (entry58), _("0"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry58), 8226);

  optionmenu5 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu5);
  gtk_box_pack_start (GTK_BOX (hbox34), optionmenu5, FALSE, FALSE, 10);

  convertwidget40 = gtk_menu_new ();

  convertwidget41 = gtk_menu_item_new_with_mnemonic (_("Network unreacheable"));
  gtk_widget_show (convertwidget41);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget41);

  convertwidget42 = gtk_menu_item_new_with_mnemonic (_("Host unreacheable"));
  gtk_widget_show (convertwidget42);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget42);

  convertwidget43 = gtk_menu_item_new_with_mnemonic (_("Protocol unreacheable"));
  gtk_widget_show (convertwidget43);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget43);

  convertwidget44 = gtk_menu_item_new_with_mnemonic (_("Port unreacheable"));
  gtk_widget_show (convertwidget44);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget44);

  convertwidget45 = gtk_menu_item_new_with_mnemonic (_("Fragmentation needed but FG-bit not set"));
  gtk_widget_show (convertwidget45);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget45);

  convertwidget46 = gtk_menu_item_new_with_mnemonic (_("Source route failed"));
  gtk_widget_show (convertwidget46);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget46);

  convertwidget47 = gtk_menu_item_new_with_mnemonic (_("Destination network unknown"));
  gtk_widget_show (convertwidget47);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget47);

  convertwidget48 = gtk_menu_item_new_with_mnemonic (_("Destination host unknown"));
  gtk_widget_show (convertwidget48);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget48);

  convertwidget49 = gtk_menu_item_new_with_mnemonic (_("Source host isolated"));
  gtk_widget_show (convertwidget49);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget49);

  convertwidget50 = gtk_menu_item_new_with_mnemonic (_("Destination network administratively prohibited"));
  gtk_widget_show (convertwidget50);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget50);

  convertwidget51 = gtk_menu_item_new_with_mnemonic (_("Destination host administratively prohibited"));
  gtk_widget_show (convertwidget51);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget51);

  convertwidget52 = gtk_menu_item_new_with_mnemonic (_("Network unreacheable for TOS"));
  gtk_widget_show (convertwidget52);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget52);

  convertwidget53 = gtk_menu_item_new_with_mnemonic (_("Host unreacheable for TOS"));
  gtk_widget_show (convertwidget53);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget53);

  convertwidget54 = gtk_menu_item_new_with_mnemonic (_("Comminication administratively prohibited by filtering"));
  gtk_widget_show (convertwidget54);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget54);

  convertwidget55 = gtk_menu_item_new_with_mnemonic (_("host precedence violation"));
  gtk_widget_show (convertwidget55);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget55);

  convertwidget56 = gtk_menu_item_new_with_mnemonic (_("Precedence cutoff in effect"));
  gtk_widget_show (convertwidget56);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget56);

  convertwidget57 = gtk_menu_item_new_with_mnemonic (_("Other"));
  gtk_widget_show (convertwidget57);
  gtk_container_add (GTK_CONTAINER (convertwidget40), convertwidget57);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu5), convertwidget40);

  hbox35 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox35);
  gtk_box_pack_start (GTK_BOX (vbox21), hbox35, FALSE, FALSE, 5);

  label87 = gtk_label_new (_("Checksum      0x"));
  gtk_widget_show (label87);
  gtk_box_pack_start (GTK_BOX (hbox35), label87, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label87), GTK_JUSTIFY_CENTER);

  entry59 = gtk_entry_new ();
  gtk_widget_show (entry59);
  gtk_box_pack_start (GTK_BOX (hbox35), entry59, FALSE, FALSE, 1);
  gtk_widget_set_size_request (entry59, 40, -1);
  gtk_widget_set_sensitive (entry59, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry59), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry59), 8226);

  checkbutton15 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton15);
  gtk_box_pack_start (GTK_BOX (hbox35), checkbutton15, FALSE, FALSE, 9);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton15), TRUE);

  label88 = gtk_label_new (_("    Unused    0x "));
  gtk_widget_show (label88);
  gtk_box_pack_start (GTK_BOX (hbox35), label88, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label88), GTK_JUSTIFY_CENTER);

  entry60 = gtk_entry_new ();
  gtk_widget_show (entry60);
  gtk_box_pack_start (GTK_BOX (hbox35), entry60, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry60, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry60), 8);
  gtk_entry_set_text (GTK_ENTRY (entry60), _("00000000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry60), 8226);

  hbox37 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox37);
  gtk_box_pack_start (GTK_BOX (vbox21), hbox37, FALSE, FALSE, 5);

  checkbutton24 = gtk_check_button_new_with_mnemonic (_("Data"));
  gtk_widget_show (checkbutton24);
  gtk_box_pack_start (GTK_BOX (hbox37), checkbutton24, FALSE, FALSE, 10);

  entry61 = gtk_entry_new ();
  gtk_widget_show (entry61);
  gtk_box_pack_start (GTK_BOX (hbox37), entry61, FALSE, FALSE, 8);
  gtk_entry_set_max_length (GTK_ENTRY (entry61), 1500);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry61), 8226);

  label89_2 = gtk_label_new (_("dest unrch"));
  gtk_widget_show (label89_2);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 2), label89_2);
  gtk_label_set_justify (GTK_LABEL (label89_2), GTK_JUSTIFY_CENTER);

  vbox25 = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (vbox25);
  gtk_container_add (GTK_CONTAINER (notebook5), vbox25);

  hbox43 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox43);
  gtk_box_pack_start (GTK_BOX (vbox25), hbox43, FALSE, FALSE, 5);

  label112 = gtk_label_new (_("Code      "));
  gtk_widget_show (label112);
  gtk_box_pack_start (GTK_BOX (hbox43), label112, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label112), GTK_JUSTIFY_CENTER);

  entry70 = gtk_entry_new ();
  gtk_widget_show (entry70);
  gtk_box_pack_start (GTK_BOX (hbox43), entry70, FALSE, FALSE, 30);
  gtk_widget_set_size_request (entry70, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry70), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry70), 8226);

  hbox44 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox44);
  gtk_box_pack_start (GTK_BOX (vbox25), hbox44, FALSE, FALSE, 5);

  label113 = gtk_label_new (_("Checksum      "));
  gtk_widget_show (label113);
  gtk_box_pack_start (GTK_BOX (hbox44), label113, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label113), GTK_JUSTIFY_CENTER);

  entry71 = gtk_entry_new ();
  gtk_widget_show (entry71);
  gtk_box_pack_start (GTK_BOX (hbox44), entry71, FALSE, FALSE, 2);
  gtk_widget_set_size_request (entry71, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry71), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry71), 8226);

  checkbutton18 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton18);
  gtk_box_pack_start (GTK_BOX (hbox44), checkbutton18, FALSE, FALSE, 0);

  label114 = gtk_label_new (_("                   Unused"));
  gtk_widget_show (label114);
  gtk_box_pack_start (GTK_BOX (hbox44), label114, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label114), GTK_JUSTIFY_CENTER);

  entry72 = gtk_entry_new ();
  gtk_widget_show (entry72);
  gtk_box_pack_start (GTK_BOX (hbox44), entry72, FALSE, FALSE, 10);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry72), 8226);

  hbox46 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox46);
  gtk_box_pack_start (GTK_BOX (vbox25), hbox46, FALSE, FALSE, 5);

  label115 = gtk_label_new (_("Data              "));
  gtk_widget_show (label115);
  gtk_box_pack_start (GTK_BOX (hbox46), label115, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label115), GTK_JUSTIFY_CENTER);

  entry73 = gtk_entry_new ();
  gtk_widget_show (entry73);
  gtk_box_pack_start (GTK_BOX (hbox46), entry73, FALSE, FALSE, 2);
  gtk_entry_set_text (GTK_ENTRY (entry73), _("!!! not yet implemented !!!"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry73), 8226);

  button31 = gtk_button_new_with_mnemonic (_("  Edit  "));
  gtk_widget_show (button31);
  gtk_box_pack_start (GTK_BOX (hbox46), button31, FALSE, FALSE, 5);

  label90 = gtk_label_new (_("src quen"));
  gtk_widget_show (label90);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 3), label90);
  gtk_label_set_justify (GTK_LABEL (label90), GTK_JUSTIFY_CENTER);

  label121 = gtk_label_new (_("redirect - not yet"));
  gtk_widget_show (label121);
  gtk_container_add (GTK_CONTAINER (notebook5), label121);
  gtk_label_set_justify (GTK_LABEL (label121), GTK_JUSTIFY_CENTER);

  label91 = gtk_label_new (_("redirect"));
  gtk_widget_show (label91);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 4), label91);
  gtk_label_set_justify (GTK_LABEL (label91), GTK_JUSTIFY_CENTER);

  vbox30 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox30);
  gtk_container_add (GTK_CONTAINER (notebook5), vbox30);

  hbox53 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox53);
  gtk_box_pack_start (GTK_BOX (vbox30), hbox53, FALSE, FALSE, 10);

  label133 = gtk_label_new (_("Code    0x"));
  gtk_widget_show (label133);
  gtk_box_pack_start (GTK_BOX (hbox53), label133, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label133), GTK_JUSTIFY_CENTER);

  entry74 = gtk_entry_new ();
  gtk_widget_show (entry74);
  gtk_box_pack_start (GTK_BOX (hbox53), entry74, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry74, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry74), 2);
  gtk_entry_set_text (GTK_ENTRY (entry74), _("00"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry74), 8226);

  label135 = gtk_label_new (_("        Checksum   0x"));
  gtk_widget_show (label135);
  gtk_box_pack_start (GTK_BOX (hbox53), label135, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label135), GTK_JUSTIFY_CENTER);

  entry77 = gtk_entry_new ();
  gtk_widget_show (entry77);
  gtk_box_pack_start (GTK_BOX (hbox53), entry77, FALSE, FALSE, 10);
  gtk_widget_set_size_request (entry77, 40, -1);
  gtk_widget_set_sensitive (entry77, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry77), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry77), 8226);

  checkbutton20 = gtk_check_button_new_with_mnemonic (_("Auto"));
  gtk_widget_show (checkbutton20);
  gtk_box_pack_start (GTK_BOX (hbox53), checkbutton20, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton20), TRUE);

  hbox54 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox54);
  gtk_box_pack_start (GTK_BOX (vbox30), hbox54, FALSE, FALSE, 10);

  label134 = gtk_label_new (_("Identifier    0x"));
  gtk_widget_show (label134);
  gtk_box_pack_start (GTK_BOX (hbox54), label134, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label134), GTK_JUSTIFY_CENTER);

  entry75 = gtk_entry_new ();
  gtk_widget_show (entry75);
  gtk_box_pack_start (GTK_BOX (hbox54), entry75, FALSE, FALSE, 4);
  gtk_widget_set_size_request (entry75, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry75), 4);
  gtk_entry_set_text (GTK_ENTRY (entry75), _("0001"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry75), 8226);

  label136 = gtk_label_new (_("     Seq. number   0x"));
  gtk_widget_show (label136);
  gtk_box_pack_start (GTK_BOX (hbox54), label136, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label136), GTK_JUSTIFY_CENTER);

  entry78 = gtk_entry_new ();
  gtk_widget_show (entry78);
  gtk_box_pack_start (GTK_BOX (hbox54), entry78, FALSE, FALSE, 4);
  gtk_widget_set_size_request (entry78, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry78), 4);
  gtk_entry_set_text (GTK_ENTRY (entry78), _("0001"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry78), 8226);

  hbox55 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox55);
  gtk_box_pack_start (GTK_BOX (vbox30), hbox55, FALSE, FALSE, 10);

  checkbutton19 = gtk_check_button_new_with_mnemonic (_("Data"));
  gtk_widget_show (checkbutton19);
  gtk_box_pack_start (GTK_BOX (hbox55), checkbutton19, FALSE, FALSE, 10);

  entry76 = gtk_entry_new ();
  gtk_widget_show (entry76);
  gtk_box_pack_start (GTK_BOX (hbox55), entry76, FALSE, FALSE, 10);
  gtk_entry_set_max_length (GTK_ENTRY (entry76), 1500);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry76), 8226);

  label92 = gtk_label_new (_("echo req"));
  gtk_widget_show (label92);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 5), label92);
  gtk_label_set_justify (GTK_LABEL (label92), GTK_JUSTIFY_CENTER);

  label123 = gtk_label_new (_("rout adv - not yet"));
  gtk_widget_show (label123);
  gtk_container_add (GTK_CONTAINER (notebook5), label123);
  gtk_label_set_justify (GTK_LABEL (label123), GTK_JUSTIFY_CENTER);

  label93 = gtk_label_new (_("rout adv"));
  gtk_widget_show (label93);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 6), label93);
  gtk_label_set_justify (GTK_LABEL (label93), GTK_JUSTIFY_CENTER);

  label124 = gtk_label_new (_("rout sol - not yet"));
  gtk_widget_show (label124);
  gtk_container_add (GTK_CONTAINER (notebook5), label124);
  gtk_label_set_justify (GTK_LABEL (label124), GTK_JUSTIFY_CENTER);

  label116 = gtk_label_new (_("rout sol"));
  gtk_widget_show (label116);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 7), label116);
  gtk_label_set_justify (GTK_LABEL (label116), GTK_JUSTIFY_CENTER);

  label125 = gtk_label_new (_("time exc - not yet"));
  gtk_widget_show (label125);
  gtk_container_add (GTK_CONTAINER (notebook5), label125);
  gtk_label_set_justify (GTK_LABEL (label125), GTK_JUSTIFY_CENTER);

  label94 = gtk_label_new (_("time exc"));
  gtk_widget_show (label94);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 8), label94);
  gtk_label_set_justify (GTK_LABEL (label94), GTK_JUSTIFY_CENTER);

  label126 = gtk_label_new (_("par prob - not yet"));
  gtk_widget_show (label126);
  gtk_container_add (GTK_CONTAINER (notebook5), label126);
  gtk_label_set_justify (GTK_LABEL (label126), GTK_JUSTIFY_CENTER);

  label95 = gtk_label_new (_("par prob"));
  gtk_widget_show (label95);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 9), label95);
  gtk_label_set_justify (GTK_LABEL (label95), GTK_JUSTIFY_CENTER);

  label127 = gtk_label_new (_("timest req - not yet"));
  gtk_widget_show (label127);
  gtk_container_add (GTK_CONTAINER (notebook5), label127);
  gtk_label_set_justify (GTK_LABEL (label127), GTK_JUSTIFY_CENTER);

  label96 = gtk_label_new (_("timest req"));
  gtk_widget_show (label96);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 10), label96);
  gtk_label_set_justify (GTK_LABEL (label96), GTK_JUSTIFY_CENTER);

  label128 = gtk_label_new (_("timest rep - not yet"));
  gtk_widget_show (label128);
  gtk_container_add (GTK_CONTAINER (notebook5), label128);
  gtk_label_set_justify (GTK_LABEL (label128), GTK_JUSTIFY_CENTER);

  label97 = gtk_label_new (_("timest rep"));
  gtk_widget_show (label97);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 11), label97);
  gtk_label_set_justify (GTK_LABEL (label97), GTK_JUSTIFY_CENTER);

  label129 = gtk_label_new (_("info req - not yet"));
  gtk_widget_show (label129);
  gtk_container_add (GTK_CONTAINER (notebook5), label129);
  gtk_label_set_justify (GTK_LABEL (label129), GTK_JUSTIFY_CENTER);

  label98 = gtk_label_new (_("info req"));
  gtk_widget_show (label98);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 12), label98);
  gtk_label_set_justify (GTK_LABEL (label98), GTK_JUSTIFY_CENTER);

  label130 = gtk_label_new (_("info rep - not yet"));
  gtk_widget_show (label130);
  gtk_container_add (GTK_CONTAINER (notebook5), label130);
  gtk_label_set_justify (GTK_LABEL (label130), GTK_JUSTIFY_CENTER);

  label99 = gtk_label_new (_("info rep"));
  gtk_widget_show (label99);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 13), label99);
  gtk_label_set_justify (GTK_LABEL (label99), GTK_JUSTIFY_CENTER);

  label131 = gtk_label_new (_("address mask req - not yet"));
  gtk_widget_show (label131);
  gtk_container_add (GTK_CONTAINER (notebook5), label131);
  gtk_label_set_justify (GTK_LABEL (label131), GTK_JUSTIFY_CENTER);

  label100 = gtk_label_new (_("adr mask req"));
  gtk_widget_show (label100);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 14), label100);
  gtk_label_set_justify (GTK_LABEL (label100), GTK_JUSTIFY_CENTER);

  label132 = gtk_label_new (_("addr mask rep - not yet"));
  gtk_widget_show (label132);
  gtk_container_add (GTK_CONTAINER (notebook5), label132);
  gtk_label_set_justify (GTK_LABEL (label132), GTK_JUSTIFY_CENTER);

  label101 = gtk_label_new (_("adr mask rep"));
  gtk_widget_show (label101);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 15), label101);
  gtk_label_set_justify (GTK_LABEL (label101), GTK_JUSTIFY_CENTER);

  label159 = gtk_label_new (_("Not yet implemented!!!"));
  gtk_widget_show (label159);
  gtk_container_add (GTK_CONTAINER (notebook5), label159);
  gtk_label_set_justify (GTK_LABEL (label159), GTK_JUSTIFY_CENTER);

  label158 = gtk_label_new (_("label158"));
  gtk_widget_show (label158);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook5), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook5), 16), label158);
  gtk_label_set_justify (GTK_LABEL (label158), GTK_JUSTIFY_CENTER);

  neki7 = gtk_label_new (_("ICMP data"));
  gtk_widget_show (neki7);
  gtk_frame_set_label_widget (GTK_FRAME (frame10), neki7);

  label67 = gtk_label_new (_("label67"));
  gtk_widget_show (label67);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook4), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook4), 2), label67);
  gtk_label_set_justify (GTK_LABEL (label67), GTK_JUSTIFY_CENTER);

  frame11 = gtk_frame_new (NULL);
  gtk_widget_show (frame11);
  gtk_container_add (GTK_CONTAINER (notebook4), frame11);

  vbox58 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox58);
  gtk_container_add (GTK_CONTAINER (frame11), vbox58);

  hbox62 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox62);
  gtk_box_pack_start (GTK_BOX (vbox58), hbox62, TRUE, TRUE, 0);

  frame20 = gtk_frame_new (NULL);
  gtk_widget_show (frame20);
  gtk_box_pack_start (GTK_BOX (hbox62), frame20, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame20), 10);

  scrolledwindow6 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow6);
  gtk_container_add (GTK_CONTAINER (frame20), scrolledwindow6);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow6, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow6), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow6), GTK_SHADOW_IN);

  text2 = gtk_text_view_new ();
  gtk_widget_show (text2);
  gtk_container_add (GTK_CONTAINER (scrolledwindow6), text2);
  gtk_widget_set_size_request (text2, 350, -1);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text2), GTK_WRAP_WORD);

  vbox32 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox32);
  gtk_box_pack_start (GTK_BOX (hbox62), vbox32, FALSE, FALSE, 20);

  frame21 = gtk_frame_new (NULL);
  gtk_widget_show (frame21);
  gtk_box_pack_start (GTK_BOX (vbox32), frame21, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame21), 10);

  vbox33 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox33);
  gtk_container_add (GTK_CONTAINER (frame21), vbox33);

  hbox63 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox63);
  gtk_box_pack_start (GTK_BOX (vbox33), hbox63, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox63), 10);

  label156 = gtk_label_new (_("Pattern "));
  gtk_widget_show (label156);
  gtk_box_pack_start (GTK_BOX (hbox63), label156, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label156), GTK_JUSTIFY_CENTER);

  entry82 = gtk_entry_new ();
  gtk_widget_show (entry82);
  gtk_box_pack_start (GTK_BOX (hbox63), entry82, TRUE, TRUE, 0);
  gtk_widget_set_size_request (entry82, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry82), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry82), 8226);

  hbox64 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox64);
  gtk_box_pack_start (GTK_BOX (vbox33), hbox64, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox64), 10);

  label157 = gtk_label_new (_("Length "));
  gtk_widget_show (label157);
  gtk_box_pack_start (GTK_BOX (hbox64), label157, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label157), GTK_JUSTIFY_CENTER);

  entry83 = gtk_entry_new ();
  gtk_widget_show (entry83);
  gtk_box_pack_start (GTK_BOX (hbox64), entry83, TRUE, TRUE, 0);
  gtk_widget_set_size_request (entry83, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry83), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry83), 8226);

  button39 = gtk_button_new_with_mnemonic (_("Apply pattern"));
  gtk_widget_show (button39);
  gtk_box_pack_start (GTK_BOX (vbox33), button39, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button39), 10);

  button38 = gtk_button_new_with_mnemonic (_("Select"));
  gtk_box_pack_start (GTK_BOX (vbox32), button38, FALSE, FALSE, 30);
  gtk_container_set_border_width (GTK_CONTAINER (button38), 10);
  gtk_widget_set_sensitive (button38, FALSE);

  napis_pod_ip_payload = gtk_label_new (_("  "));
  gtk_widget_show (napis_pod_ip_payload);
  gtk_box_pack_start (GTK_BOX (vbox58), napis_pod_ip_payload, FALSE, FALSE, 45);
  gtk_label_set_justify (GTK_LABEL (napis_pod_ip_payload), GTK_JUSTIFY_CENTER);

  neki8 = gtk_label_new (_("User defined transport payload"));
  gtk_widget_show (neki8);
  gtk_frame_set_label_widget (GTK_FRAME (frame11), neki8);

  label68 = gtk_label_new (_("label68"));
  gtk_widget_show (label68);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook4), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook4), 3), label68);
  gtk_label_set_justify (GTK_LABEL (label68), GTK_JUSTIFY_CENTER);

  frame45 = gtk_frame_new (NULL);
  gtk_widget_show (frame45);
  gtk_container_add (GTK_CONTAINER (notebook4), frame45);

  vbox69 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox69);
  gtk_container_add (GTK_CONTAINER (frame45), vbox69);

  hbox122 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox122);
  gtk_box_pack_start (GTK_BOX (vbox69), hbox122, FALSE, FALSE, 10);

  label305 = gtk_label_new (_("Type  0x"));
  gtk_widget_show (label305);
  gtk_box_pack_start (GTK_BOX (hbox122), label305, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label305), GTK_JUSTIFY_CENTER);

  entry166 = gtk_entry_new ();
  gtk_widget_show (entry166);
  gtk_box_pack_start (GTK_BOX (hbox122), entry166, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry166, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry166), 2);
  gtk_entry_set_text (GTK_ENTRY (entry166), _("11"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry166), 8226);

  optionmenu20 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu20);
  gtk_box_pack_start (GTK_BOX (hbox122), optionmenu20, FALSE, FALSE, 7);

  convertwidget60 = gtk_menu_new ();

  convertwidget61 = gtk_menu_item_new_with_mnemonic (_("0x11   V1, V2 Membership query"));
  gtk_widget_show (convertwidget61);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget61);

  convertwidget62 = gtk_menu_item_new_with_mnemonic (_("0x11   V3 Membership query"));
  gtk_widget_show (convertwidget62);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget62);

  convertwidget63 = gtk_menu_item_new_with_mnemonic (_("0x12   V1 Membership report"));
  gtk_widget_show (convertwidget63);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget63);

  convertwidget64 = gtk_menu_item_new_with_mnemonic (_("0x16   V2 Membership report"));
  gtk_widget_show (convertwidget64);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget64);

  convertwidget65 = gtk_menu_item_new_with_mnemonic (_("0x22   V3 Membership report"));
  gtk_widget_show (convertwidget65);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget65);

  convertwidget66 = gtk_menu_item_new_with_mnemonic (_("0x17   V1,V2, V3 Leave group"));
  gtk_widget_show (convertwidget66);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget66);

  convertwidget67 = gtk_menu_item_new_with_mnemonic (_("Other"));
  gtk_widget_show (convertwidget67);
  gtk_container_add (GTK_CONTAINER (convertwidget60), convertwidget67);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu20), convertwidget60);

  hbox124 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox124);
  gtk_box_pack_start (GTK_BOX (vbox69), hbox124, FALSE, FALSE, 5);

  label306 = gtk_label_new (_("Max response time  0x"));
  gtk_widget_show (label306);
  gtk_box_pack_start (GTK_BOX (hbox124), label306, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label306), GTK_JUSTIFY_CENTER);

  entry167 = gtk_entry_new ();
  gtk_widget_show (entry167);
  gtk_box_pack_start (GTK_BOX (hbox124), entry167, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry167, 30, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry167), 2);
  gtk_entry_set_text (GTK_ENTRY (entry167), _("00"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry167), 8226);

  label307 = gtk_label_new (_("        Checksum  0x"));
  gtk_widget_show (label307);
  gtk_box_pack_start (GTK_BOX (hbox124), label307, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label307), GTK_JUSTIFY_CENTER);

  entry168 = gtk_entry_new ();
  gtk_widget_show (entry168);
  gtk_box_pack_start (GTK_BOX (hbox124), entry168, FALSE, FALSE, 10);
  gtk_widget_set_size_request (entry168, 40, -1);
  gtk_widget_set_sensitive (entry168, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry168), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry168), 8226);

  checkbutton41 = gtk_check_button_new_with_mnemonic (_("auto"));
  gtk_widget_show (checkbutton41);
  gtk_box_pack_start (GTK_BOX (hbox124), checkbutton41, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton41), TRUE);

  notebook8 = gtk_notebook_new ();
  gtk_widget_show (notebook8);
  gtk_box_pack_start (GTK_BOX (vbox69), notebook8, FALSE, FALSE, 5);
  GTK_WIDGET_UNSET_FLAGS (notebook8, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook8), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook8), FALSE);

  vbox71 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox71);
  gtk_container_add (GTK_CONTAINER (notebook8), vbox71);

  hbox130 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox130);
  gtk_box_pack_start (GTK_BOX (vbox71), hbox130, FALSE, TRUE, 10);

  label315 = gtk_label_new (_("Group Address"));
  gtk_widget_show (label315);
  gtk_box_pack_start (GTK_BOX (hbox130), label315, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label315), GTK_JUSTIFY_CENTER);

  entry175 = gtk_entry_new ();
  gtk_widget_show (entry175);
  gtk_box_pack_start (GTK_BOX (hbox130), entry175, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry175, 150, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry175), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry175), 8226);

  label309 = gtk_label_new (_("label309"));
  gtk_widget_show (label309);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook8), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook8), 0), label309);
  gtk_label_set_justify (GTK_LABEL (label309), GTK_JUSTIFY_CENTER);

  vbox70 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox70);
  gtk_container_add (GTK_CONTAINER (notebook8), vbox70);

  hbox125 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox125);
  gtk_box_pack_start (GTK_BOX (vbox70), hbox125, FALSE, FALSE, 10);

  label308 = gtk_label_new (_("Group Address"));
  gtk_widget_show (label308);
  gtk_box_pack_start (GTK_BOX (hbox125), label308, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label308), GTK_JUSTIFY_CENTER);

  entry169 = gtk_entry_new ();
  gtk_widget_show (entry169);
  gtk_box_pack_start (GTK_BOX (hbox125), entry169, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry169, 150, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry169), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry169), 8226);

  hbox127 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox127);
  gtk_box_pack_start (GTK_BOX (vbox70), hbox127, TRUE, TRUE, 5);

  label311 = gtk_label_new (_("    Resv, S, QRV, QQIC  (2 bytes)  0x "));
  gtk_widget_show (label311);
  gtk_box_pack_start (GTK_BOX (hbox127), label311, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label311), GTK_JUSTIFY_CENTER);

  entry171 = gtk_entry_new ();
  gtk_widget_show (entry171);
  gtk_box_pack_start (GTK_BOX (hbox127), entry171, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry171, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry171), 4);
  gtk_entry_set_text (GTK_ENTRY (entry171), _("0000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry171), 8226);

  label312 = gtk_label_new (_("        Number of sources (N)  0x "));
  gtk_widget_show (label312);
  gtk_box_pack_start (GTK_BOX (hbox127), label312, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label312), GTK_JUSTIFY_CENTER);

  entry172 = gtk_entry_new ();
  gtk_widget_show (entry172);
  gtk_box_pack_start (GTK_BOX (hbox127), entry172, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry172, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry172), 4);
  gtk_entry_set_text (GTK_ENTRY (entry172), _("0000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry172), 8226);

  hbox128 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox128);
  gtk_box_pack_start (GTK_BOX (vbox70), hbox128, TRUE, TRUE, 5);

  label313 = gtk_label_new (_("    Source  addresses  0x "));
  gtk_widget_show (label313);
  gtk_box_pack_start (GTK_BOX (hbox128), label313, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label313), GTK_JUSTIFY_CENTER);

  entry173 = gtk_entry_new ();
  gtk_widget_show (entry173);
  gtk_box_pack_start (GTK_BOX (hbox128), entry173, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry173), 1000);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry173), 8226);

  label310 = gtk_label_new (_("igmp v3 query"));
  gtk_widget_show (label310);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook8), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook8), 1), label310);
  gtk_label_set_justify (GTK_LABEL (label310), GTK_JUSTIFY_CENTER);

  vbox72 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox72);
  gtk_container_add (GTK_CONTAINER (notebook8), vbox72);

  hbox131 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox131);
  gtk_box_pack_start (GTK_BOX (vbox72), hbox131, FALSE, FALSE, 10);

  label316 = gtk_label_new (_("Reserved 0x "));
  gtk_widget_show (label316);
  gtk_box_pack_start (GTK_BOX (hbox131), label316, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label316), GTK_JUSTIFY_CENTER);

  entry176 = gtk_entry_new ();
  gtk_widget_show (entry176);
  gtk_box_pack_start (GTK_BOX (hbox131), entry176, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry176, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry176), 4);
  gtk_entry_set_text (GTK_ENTRY (entry176), _("0000"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry176), 8226);

  label317 = gtk_label_new (_("        Number of group records 0x "));
  gtk_widget_show (label317);
  gtk_box_pack_start (GTK_BOX (hbox131), label317, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label317), GTK_JUSTIFY_CENTER);

  entry177 = gtk_entry_new ();
  gtk_widget_show (entry177);
  gtk_box_pack_start (GTK_BOX (hbox131), entry177, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry177, 40, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry177), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry177), 8226);

  hbox132 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox132);
  gtk_box_pack_start (GTK_BOX (vbox72), hbox132, FALSE, FALSE, 5);

  label318 = gtk_label_new (_("Group Records 0x"));
  gtk_widget_show (label318);
  gtk_box_pack_start (GTK_BOX (hbox132), label318, FALSE, FALSE, 15);
  gtk_label_set_justify (GTK_LABEL (label318), GTK_JUSTIFY_CENTER);

  entry178 = gtk_entry_new ();
  gtk_widget_show (entry178);
  gtk_box_pack_start (GTK_BOX (hbox132), entry178, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry178), 1000);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry178), 8226);

  label314 = gtk_label_new (_("V3 report"));
  gtk_widget_show (label314);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook8), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook8), 2), label314);
  gtk_label_set_justify (GTK_LABEL (label314), GTK_JUSTIFY_CENTER);

  frame461 = gtk_frame_new (NULL);
  gtk_widget_show (frame461);
  gtk_box_pack_start (GTK_BOX (vbox69), frame461, FALSE, FALSE, 2);
  gtk_container_set_border_width (GTK_CONTAINER (frame461), 10);

  hbox133 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox133);
  gtk_container_add (GTK_CONTAINER (frame461), hbox133);

  label320 = gtk_label_new (_("Calculate ethernet multicast address based on IP\nSet IP options field = Router Alert\nSet IP header length to 0x06, set TTL=1 "));
  gtk_widget_show (label320);
  gtk_box_pack_start (GTK_BOX (hbox133), label320, FALSE, FALSE, 15);

  frame46 = gtk_frame_new (NULL);
  gtk_widget_show (frame46);
  gtk_box_pack_start (GTK_BOX (hbox133), frame46, FALSE, FALSE, 0);

  button81 = gtk_button_new_with_mnemonic (_("  Apply  "));
  gtk_widget_show (button81);
  gtk_container_add (GTK_CONTAINER (frame46), button81);
  gtk_container_set_border_width (GTK_CONTAINER (button81), 7);

  neki0 = gtk_label_new (_("IGMP"));
  gtk_widget_show (neki0);
  gtk_frame_set_label_widget (GTK_FRAME (frame45), neki0);

  label69 = gtk_label_new (_("label69"));
  gtk_widget_show (label69);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook4), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook4), 4), label69);
  gtk_label_set_justify (GTK_LABEL (label69), GTK_JUSTIFY_CENTER);

  label376 = gtk_label_new (_("icmpv6"));
  gtk_widget_show (label376);
  gtk_container_add (GTK_CONTAINER (notebook4), label376);

  label319 = gtk_label_new (_("label319"));
  gtk_widget_show (label319);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook4), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook4), 5), label319);
  gtk_label_set_justify (GTK_LABEL (label319), GTK_JUSTIFY_CENTER);

  label1 = gtk_label_new (_("  Packet construction  "));
  gtk_widget_show (label1);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label1);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_CENTER);

  vbox59 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox59);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox59);

  frame34 = gtk_frame_new (NULL);
  gtk_widget_show (frame34);
  gtk_box_pack_start (GTK_BOX (vbox59), frame34, FALSE, FALSE, 0);

  label241 = gtk_label_new (_(" "));
  gtk_widget_show (label241);
  gtk_container_add (GTK_CONTAINER (frame34), label241);
  gtk_label_set_justify (GTK_LABEL (label241), GTK_JUSTIFY_CENTER);

  vertbox60 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vertbox60);
  gtk_box_pack_start (GTK_BOX (vbox59), vertbox60, FALSE, FALSE, 0);

  hbox97 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox97);
  gtk_box_pack_start (GTK_BOX (vertbox60), hbox97, TRUE, TRUE, 0);

  frame36 = gtk_frame_new (NULL);
  gtk_widget_show (frame36);
  gtk_box_pack_start (GTK_BOX (hbox97), frame36, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame36), 10);

  vbox60 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox60);
  gtk_container_add (GTK_CONTAINER (frame36), vbox60);
  gtk_container_set_border_width (GTK_CONTAINER (vbox60), 10);

  label243 = gtk_label_new (_("Number of packets to send ( 0 - 1000000 ) : "));
  gtk_widget_show (label243);
  gtk_box_pack_start (GTK_BOX (vbox60), label243, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label243), GTK_JUSTIFY_CENTER);

  hbox98 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox98);
  gtk_box_pack_start (GTK_BOX (vbox60), hbox98, FALSE, FALSE, 10);

  entry109 = gtk_entry_new ();
  gtk_widget_show (entry109);
  gtk_box_pack_start (GTK_BOX (hbox98), entry109, TRUE, FALSE, 0);
  gtk_widget_set_size_request (entry109, 70, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry109), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry109), 8226);

  checkbutton35 = gtk_check_button_new_with_mnemonic (_(" Infinite"));
  gtk_widget_show (checkbutton35);
  gtk_box_pack_start (GTK_BOX (hbox98), checkbutton35, TRUE, TRUE, 0);

  label244 = gtk_label_new (_("Delay between packets (us) :"));
  gtk_widget_show (label244);
  gtk_box_pack_start (GTK_BOX (vbox60), label244, FALSE, FALSE, 20);
  gtk_label_set_justify (GTK_LABEL (label244), GTK_JUSTIFY_CENTER);

  hbox99 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox99);
  gtk_box_pack_start (GTK_BOX (vbox60), hbox99, FALSE, FALSE, 0);

  entry110 = gtk_entry_new ();
  gtk_widget_show (entry110);
  gtk_box_pack_start (GTK_BOX (hbox99), entry110, TRUE, FALSE, 12);
  gtk_widget_set_size_request (entry110, 90, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry110), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry110), 8226);

  checkbutton37 = gtk_check_button_new_with_mnemonic (_(" Max speed"));
  gtk_widget_show (checkbutton37);
  gtk_box_pack_start (GTK_BOX (hbox99), checkbutton37, TRUE, TRUE, 0);

  label287 = gtk_label_new (_("\nTiming type:"));
  gtk_widget_show (label287);
  gtk_box_pack_start (GTK_BOX (vbox60), label287, FALSE, FALSE, 20);
  gtk_label_set_justify (GTK_LABEL (label287), GTK_JUSTIFY_CENTER);

  hbox114 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox114);
  gtk_box_pack_start (GTK_BOX (vbox60), hbox114, FALSE, TRUE, 0);

  radiobutton34 = gtk_radio_button_new_with_mnemonic (NULL, _("Absolute delay"));
  gtk_widget_show (radiobutton34);
  gtk_box_pack_start (GTK_BOX (hbox114), radiobutton34, TRUE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton34), radiobutton34_group);
  radiobutton34_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton34));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton34), TRUE);

  radiobutton35 = gtk_radio_button_new_with_mnemonic (NULL, _("Relative delay "));
  gtk_widget_show (radiobutton35);
  gtk_box_pack_start (GTK_BOX (hbox114), radiobutton35, TRUE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton35), radiobutton34_group);
  radiobutton34_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton35));

  frame37 = gtk_frame_new (NULL);
  gtk_widget_show (frame37);
  gtk_box_pack_start (GTK_BOX (hbox97), frame37, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame37), 10);

  table3 = gtk_table_new (6, 2, FALSE);
  gtk_widget_show (table3);
  gtk_container_add (GTK_CONTAINER (frame37), table3);
  gtk_container_set_border_width (GTK_CONTAINER (table3), 15);
  gtk_table_set_row_spacings (GTK_TABLE (table3), 7);
  gtk_table_set_col_spacings (GTK_TABLE (table3), 6);

  notebook7 = gtk_notebook_new ();
  gtk_widget_show (notebook7);
  gtk_table_attach (GTK_TABLE (table3), notebook7, 0, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  GTK_WIDGET_UNSET_FLAGS (notebook7, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook7), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook7), FALSE);

  optionmenu9 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu9);
  gtk_container_add (GTK_CONTAINER (notebook7), optionmenu9);

  convertwidget68 = gtk_menu_new ();

  convertwidget69 = gtk_menu_item_new_with_mnemonic (_("Do nothing"));
  gtk_widget_show (convertwidget69);
  gtk_container_add (GTK_CONTAINER (convertwidget68), convertwidget69);

  convertwidget70 = gtk_menu_item_new_with_mnemonic (_("MAC set random source address"));
  gtk_widget_show (convertwidget70);
  gtk_container_add (GTK_CONTAINER (convertwidget68), convertwidget70);

  convertwidget71 = gtk_menu_item_new_with_mnemonic (_("Change value for byte x"));
  gtk_widget_show (convertwidget71);
  gtk_container_add (GTK_CONTAINER (convertwidget68), convertwidget71);

  convertwidget72 = gtk_menu_item_new_with_mnemonic (_("Change values for byte x and y"));
  gtk_widget_show (convertwidget72);
  gtk_container_add (GTK_CONTAINER (convertwidget68), convertwidget72);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu9), convertwidget68);

  label290 = gtk_label_new (_("label290"));
  gtk_widget_show (label290);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook7), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook7), 0), label290);
  gtk_label_set_justify (GTK_LABEL (label290), GTK_JUSTIFY_CENTER);

  optionmenu16 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu16);
  gtk_container_add (GTK_CONTAINER (notebook7), optionmenu16);

  convertwidget73 = gtk_menu_new ();

  convertwidget74 = gtk_menu_item_new_with_mnemonic (_("Do nothing"));
  gtk_widget_show (convertwidget74);
  gtk_container_add (GTK_CONTAINER (convertwidget73), convertwidget74);

  convertwidget75 = gtk_menu_item_new_with_mnemonic (_("MAC set random source address"));
  gtk_widget_show (convertwidget75);
  gtk_container_add (GTK_CONTAINER (convertwidget73), convertwidget75);

  convertwidget76 = gtk_menu_item_new_with_mnemonic (_("IP set random source address"));
  gtk_widget_show (convertwidget76);
  gtk_container_add (GTK_CONTAINER (convertwidget73), convertwidget76);

  convertwidget77 = gtk_menu_item_new_with_mnemonic (_("MAC & IP set random source address"));
  gtk_widget_show (convertwidget77);
  gtk_container_add (GTK_CONTAINER (convertwidget73), convertwidget77);

  convertwidget78 = gtk_menu_item_new_with_mnemonic (_("Change value for byte x"));
  gtk_widget_show (convertwidget78);
  gtk_container_add (GTK_CONTAINER (convertwidget73), convertwidget78);

  convertwidget79 = gtk_menu_item_new_with_mnemonic (_("Change values for byte x and y"));
  gtk_widget_show (convertwidget79);
  gtk_container_add (GTK_CONTAINER (convertwidget73), convertwidget79);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu16), convertwidget73);

  label291 = gtk_label_new (_("label291"));
  gtk_widget_show (label291);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook7), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook7), 1), label291);
  gtk_label_set_justify (GTK_LABEL (label291), GTK_JUSTIFY_CENTER);

  optionmenu17 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu17);
  gtk_container_add (GTK_CONTAINER (notebook7), optionmenu17);

  convertwidget80 = gtk_menu_new ();

  convertwidget81 = gtk_menu_item_new_with_mnemonic (_("Do nothing"));
  gtk_widget_show (convertwidget81);
  gtk_container_add (GTK_CONTAINER (convertwidget80), convertwidget81);

  convertwidget82 = gtk_menu_item_new_with_mnemonic (_("MAC set random source address"));
  gtk_widget_show (convertwidget82);
  gtk_container_add (GTK_CONTAINER (convertwidget80), convertwidget82);

  convertwidget83 = gtk_menu_item_new_with_mnemonic (_("ARP reply set random source mac&ip address"));
  gtk_widget_show (convertwidget83);
  gtk_container_add (GTK_CONTAINER (convertwidget80), convertwidget83);

  convertwidget84 = gtk_menu_item_new_with_mnemonic (_("Change value for byte x"));
  gtk_widget_show (convertwidget84);
  gtk_container_add (GTK_CONTAINER (convertwidget80), convertwidget84);

  convertwidget85 = gtk_menu_item_new_with_mnemonic (_("Change values for byte x and y"));
  gtk_widget_show (convertwidget85);
  gtk_container_add (GTK_CONTAINER (convertwidget80), convertwidget85);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu17), convertwidget80);

  label292 = gtk_label_new (_("label292"));
  gtk_widget_show (label292);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook7), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook7), 2), label292);
  gtk_label_set_justify (GTK_LABEL (label292), GTK_JUSTIFY_CENTER);

  optionmenu18 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu18);
  gtk_container_add (GTK_CONTAINER (notebook7), optionmenu18);

  convertwidget86 = gtk_menu_new ();

  convertwidget87 = gtk_menu_item_new_with_mnemonic (_("Do nothing"));
  gtk_widget_show (convertwidget87);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget87);

  convertwidget88 = gtk_menu_item_new_with_mnemonic (_("MAC set random source address"));
  gtk_widget_show (convertwidget88);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget88);

  convertwidget89 = gtk_menu_item_new_with_mnemonic (_("IP set random source address"));
  gtk_widget_show (convertwidget89);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget89);

  convertwidget90 = gtk_menu_item_new_with_mnemonic (_("MAC & IP set random source address"));
  gtk_widget_show (convertwidget90);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget90);

  convertwidget91 = gtk_menu_item_new_with_mnemonic (_("TCP source port & IP source address set random"));
  gtk_widget_show (convertwidget91);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget91);

  convertwidget92 = gtk_menu_item_new_with_mnemonic (_("Change value for byte x"));
  gtk_widget_show (convertwidget92);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget92);

  convertwidget93 = gtk_menu_item_new_with_mnemonic (_("Change values for byte x and y"));
  gtk_widget_show (convertwidget93);
  gtk_container_add (GTK_CONTAINER (convertwidget86), convertwidget93);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu18), convertwidget86);

  label293 = gtk_label_new (_("label293"));
  gtk_widget_show (label293);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook7), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook7), 3), label293);
  gtk_label_set_justify (GTK_LABEL (label293), GTK_JUSTIFY_CENTER);

  optionmenu19 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu19);
  gtk_container_add (GTK_CONTAINER (notebook7), optionmenu19);

  convertwidget94 = gtk_menu_new ();

  convertwidget95 = gtk_menu_item_new_with_mnemonic (_("Do nothing"));
  gtk_widget_show (convertwidget95);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget95);

  convertwidget96 = gtk_menu_item_new_with_mnemonic (_("MAC set random source address"));
  gtk_widget_show (convertwidget96);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget96);

  convertwidget97 = gtk_menu_item_new_with_mnemonic (_("IP set random source address"));
  gtk_widget_show (convertwidget97);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget97);

  convertwidget98 = gtk_menu_item_new_with_mnemonic (_("MAC & IP set random source address"));
  gtk_widget_show (convertwidget98);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget98);

  convertwidget99 = gtk_menu_item_new_with_mnemonic (_("UDP first payload byte increase value by 1"));
  gtk_widget_show (convertwidget99);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget99);

  convertwidget100 = gtk_menu_item_new_with_mnemonic (_("RTP seq nr increase by 1 & RTP timestamp increase by 10 ms"));
  gtk_widget_show (convertwidget100);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget100);

  convertwidget101 = gtk_menu_item_new_with_mnemonic (_("RTP seq nr increase by 1 & RTP timestamp increase by 20 ms"));
  gtk_widget_show (convertwidget101);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget101);

  convertwidget102 = gtk_menu_item_new_with_mnemonic (_("RTP seq nr increase by 1 & RTP timestamp increase by 30 ms"));
  gtk_widget_show (convertwidget102);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget102);

  convertwidget103 = gtk_menu_item_new_with_mnemonic (_("Change byte x value"));
  gtk_widget_show (convertwidget103);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget103);

  convertwidget104 = gtk_menu_item_new_with_mnemonic (_("Change byte x and y values "));
  gtk_widget_show (convertwidget104);
  gtk_container_add (GTK_CONTAINER (convertwidget94), convertwidget104);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu19), convertwidget94);

  label294 = gtk_label_new (_("label294"));
  gtk_widget_show (label294);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook7), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook7), 4), label294);
  gtk_label_set_justify (GTK_LABEL (label294), GTK_JUSTIFY_CENTER);

  hbox117 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox117);
  gtk_table_attach (GTK_TABLE (table3), hbox117, 0, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  label295 = gtk_label_new (_("Byte x offset "));
  gtk_widget_show (label295);
  gtk_box_pack_start (GTK_BOX (hbox117), label295, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label295), GTK_JUSTIFY_CENTER);

  entry160 = gtk_entry_new ();
  gtk_widget_show (entry160);
  gtk_box_pack_start (GTK_BOX (hbox117), entry160, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry160, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry160), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry160), 8226);

  label296 = gtk_label_new (_("      Action "));
  gtk_widget_show (label296);
  gtk_box_pack_start (GTK_BOX (hbox117), label296, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label296), GTK_JUSTIFY_CENTER);

  optionmenu14 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu14);
  gtk_box_pack_start (GTK_BOX (hbox117), optionmenu14, FALSE, FALSE, 0);

  convertwidget105 = gtk_menu_new ();

  convertwidget106 = gtk_menu_item_new_with_mnemonic (_("Random 1 Byte"));
  gtk_widget_show (convertwidget106);
  gtk_container_add (GTK_CONTAINER (convertwidget105), convertwidget106);

  convertwidget107 = gtk_menu_item_new_with_mnemonic (_("Increment"));
  gtk_widget_show (convertwidget107);
  gtk_container_add (GTK_CONTAINER (convertwidget105), convertwidget107);

  convertwidget108 = gtk_menu_item_new_with_mnemonic (_("Decrement"));
  gtk_widget_show (convertwidget108);
  gtk_container_add (GTK_CONTAINER (convertwidget105), convertwidget108);

  random_2_bytes = gtk_menu_item_new_with_mnemonic (_("Random 2 Bytes"));
  gtk_widget_show (random_2_bytes);
  gtk_container_add (GTK_CONTAINER (convertwidget105), random_2_bytes);

  random_3_bytes = gtk_menu_item_new_with_mnemonic (_("Random 3 Bytes"));
  gtk_widget_show (random_3_bytes);
  gtk_container_add (GTK_CONTAINER (convertwidget105), random_3_bytes);

  random_4_bytes = gtk_menu_item_new_with_mnemonic (_("Random 4 Bytes"));
  gtk_widget_show (random_4_bytes);
  gtk_container_add (GTK_CONTAINER (convertwidget105), random_4_bytes);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu14), convertwidget105);

  label297 = gtk_label_new (_("     Range "));
  gtk_widget_show (label297);
  gtk_box_pack_start (GTK_BOX (hbox117), label297, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label297), GTK_JUSTIFY_CENTER);

  entry161 = gtk_entry_new ();
  gtk_widget_show (entry161);
  gtk_box_pack_start (GTK_BOX (hbox117), entry161, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry161, 83, -1);
  gtk_widget_set_sensitive (entry161, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry161), 10);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry161), 8226);

  label358 = gtk_label_new (_(" - up to 2^32"));
  gtk_widget_show (label358);
  gtk_box_pack_start (GTK_BOX (hbox117), label358, FALSE, FALSE, 0);

  hbox118 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox118);
  gtk_table_attach (GTK_TABLE (table3), hbox118, 0, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  label298 = gtk_label_new (_("Byte y offset "));
  gtk_widget_show (label298);
  gtk_box_pack_start (GTK_BOX (hbox118), label298, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label298), GTK_JUSTIFY_CENTER);

  entry162 = gtk_entry_new ();
  gtk_widget_show (entry162);
  gtk_box_pack_start (GTK_BOX (hbox118), entry162, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry162, 50, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry162), 4);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry162), 8226);

  label299 = gtk_label_new (_("      Action "));
  gtk_widget_show (label299);
  gtk_box_pack_start (GTK_BOX (hbox118), label299, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label299), GTK_JUSTIFY_CENTER);

  optionmenu15 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu15);
  gtk_box_pack_start (GTK_BOX (hbox118), optionmenu15, FALSE, FALSE, 0);

  convertwidget109 = gtk_menu_new ();

  convertwidget110 = gtk_menu_item_new_with_mnemonic (_("Random 1 Byte"));
  gtk_widget_show (convertwidget110);
  gtk_container_add (GTK_CONTAINER (convertwidget109), convertwidget110);

  convertwidget111 = gtk_menu_item_new_with_mnemonic (_("Increment"));
  gtk_widget_show (convertwidget111);
  gtk_container_add (GTK_CONTAINER (convertwidget109), convertwidget111);

  convertwidget112 = gtk_menu_item_new_with_mnemonic (_("Decrement"));
  gtk_widget_show (convertwidget112);
  gtk_container_add (GTK_CONTAINER (convertwidget109), convertwidget112);

  random_2_bytes2 = gtk_menu_item_new_with_mnemonic (_("Random 2 bytes"));
  gtk_widget_show (random_2_bytes2);
  gtk_container_add (GTK_CONTAINER (convertwidget109), random_2_bytes2);

  random_3_bytes2 = gtk_menu_item_new_with_mnemonic (_("Random 3 bytes"));
  gtk_widget_show (random_3_bytes2);
  gtk_container_add (GTK_CONTAINER (convertwidget109), random_3_bytes2);

  random_4_bytes2 = gtk_menu_item_new_with_mnemonic (_("Random 4 bytes"));
  gtk_widget_show (random_4_bytes2);
  gtk_container_add (GTK_CONTAINER (convertwidget109), random_4_bytes2);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu15), convertwidget109);

  label300 = gtk_label_new (_("      Range "));
  gtk_widget_show (label300);
  gtk_box_pack_start (GTK_BOX (hbox118), label300, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label300), GTK_JUSTIFY_CENTER);

  entry163 = gtk_entry_new ();
  gtk_widget_show (entry163);
  gtk_box_pack_start (GTK_BOX (hbox118), entry163, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry163, 83, -1);
  gtk_widget_set_sensitive (entry163, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry163), 10);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry163), 8226);

  label359 = gtk_label_new (_(" - up to 2^32"));
  gtk_widget_show (label359);
  gtk_box_pack_start (GTK_BOX (hbox118), label359, FALSE, FALSE, 0);

  hbox120 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox120);
  gtk_table_attach (GTK_TABLE (table3), hbox120, 0, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  hbox121 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox121);
  gtk_box_pack_start (GTK_BOX (hbox120), hbox121, TRUE, TRUE, 0);

  scrolledwindow10 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow10);
  gtk_box_pack_start (GTK_BOX (hbox121), scrolledwindow10, TRUE, TRUE, 0);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow10, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow10), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow10), GTK_SHADOW_IN);

  text5 = gtk_text_view_new ();
  gtk_widget_show (text5);
  gtk_container_add (GTK_CONTAINER (scrolledwindow10), text5);
  gtk_widget_set_size_request (text5, 350, -1);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (text5), FALSE);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text5), GTK_WRAP_WORD);

  vbox68 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox68);
  gtk_box_pack_start (GTK_BOX (hbox121), vbox68, FALSE, FALSE, 10);

  label303 = gtk_label_new (_("Ethernet\nCRC\n0x"));
  gtk_widget_show (label303);
  gtk_box_pack_start (GTK_BOX (vbox68), label303, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label303), GTK_JUSTIFY_CENTER);

  entry164 = gtk_entry_new ();
  gtk_widget_show (entry164);
  gtk_box_pack_start (GTK_BOX (vbox68), entry164, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entry164, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry164), 8);
  gtk_editable_set_editable (GTK_EDITABLE (entry164), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry164), 8226);

  label288 = gtk_label_new (_("                             Packet contents"));
  gtk_widget_show (label288);
  gtk_table_attach (GTK_TABLE (table3), label288, 0, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label288), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label288), 0, 0.5);

  label245 = gtk_label_new (_("Adjust paremeters while sending"));
  gtk_widget_show (label245);
  gtk_table_attach (GTK_TABLE (table3), label245, 0, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label245), GTK_JUSTIFY_CENTER);

  label2 = gtk_label_new (_("  Built gen"));
  gtk_widget_show (label2);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_CENTER);

  vbox62 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox62);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox62);

  frame39 = gtk_frame_new (NULL);
  gtk_widget_show (frame39);
  gtk_box_pack_start (GTK_BOX (vbox62), frame39, FALSE, FALSE, 0);

  label252 = gtk_label_new (_("  "));
  gtk_widget_show (label252);
  gtk_container_add (GTK_CONTAINER (frame39), label252);
  gtk_label_set_justify (GTK_LABEL (label252), GTK_JUSTIFY_CENTER);

  frame40 = gtk_frame_new (NULL);
  gtk_widget_show (frame40);
  gtk_box_pack_start (GTK_BOX (vbox62), frame40, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame40), 10);

  hbox102 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox102);
  gtk_container_add (GTK_CONTAINER (frame40), hbox102);
  gtk_container_set_border_width (GTK_CONTAINER (hbox102), 10);

  label253 = gtk_label_new (_("Send sequence                                                      Timing type"));
  gtk_widget_show (label253);
  gtk_box_pack_start (GTK_BOX (hbox102), label253, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label253), GTK_JUSTIFY_CENTER);

  radiobutton36 = gtk_radio_button_new_with_mnemonic (NULL, _("Absolute delay"));
  gtk_widget_show (radiobutton36);
  gtk_box_pack_start (GTK_BOX (hbox102), radiobutton36, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton36), radiobutton36_group);
  radiobutton36_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton36));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton36), TRUE);

  radiobutton37 = gtk_radio_button_new_with_mnemonic (NULL, _("Relative delay"));
  gtk_widget_show (radiobutton37);
  gtk_box_pack_start (GTK_BOX (hbox102), radiobutton37, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton37), radiobutton36_group);
  radiobutton36_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton37));

  label360 = gtk_label_new (_("             Stream mode  "));
  gtk_widget_show (label360);
  gtk_box_pack_start (GTK_BOX (hbox102), label360, FALSE, FALSE, 0);

  radiobutton61 = gtk_radio_button_new_with_mnemonic (NULL, _("Auto"));
  gtk_widget_show (radiobutton61);
  gtk_box_pack_start (GTK_BOX (hbox102), radiobutton61, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton61), radiobutton36_group);
  radiobutton36_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton61));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton61), TRUE);

  radiobutton62 = gtk_radio_button_new_with_mnemonic (NULL, _("Manual"));
  gtk_widget_show (radiobutton62);
  gtk_box_pack_start (GTK_BOX (hbox102), radiobutton62, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton62), radiobutton36_group);
  radiobutton36_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton62));

  frame41 = gtk_frame_new (NULL);
  gtk_widget_show (frame41);
  gtk_box_pack_start (GTK_BOX (vbox62), frame41, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame41), 10);

  scrolledwindow9 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow9);
  gtk_container_add (GTK_CONTAINER (frame41), scrolledwindow9);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow9, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow9), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  viewport2 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport2);
  gtk_container_add (GTK_CONTAINER (scrolledwindow9), viewport2);
  gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport2), GTK_SHADOW_NONE);

  table4 = gtk_table_new (13, 7, FALSE);
  gtk_widget_show (table4);
  gtk_container_add (GTK_CONTAINER (viewport2), table4);
  gtk_container_set_border_width (GTK_CONTAINER (table4), 10);
  gtk_table_set_row_spacings (GTK_TABLE (table4), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table4), 10);

  entry122 = gtk_entry_new ();
  gtk_widget_show (entry122);
  gtk_table_attach (GTK_TABLE (table4), entry122, 2, 3, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry122, 70, -1);
  gtk_widget_set_sensitive (entry122, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry122), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry122), 8226);

  entry123 = gtk_entry_new ();
  gtk_widget_show (entry123);
  gtk_table_attach (GTK_TABLE (table4), entry123, 2, 3, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry123, 70, -1);
  gtk_widget_set_sensitive (entry123, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry123), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry123), 8226);

  entry124 = gtk_entry_new ();
  gtk_widget_show (entry124);
  gtk_table_attach (GTK_TABLE (table4), entry124, 2, 3, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry124, 70, -1);
  gtk_widget_set_sensitive (entry124, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry124), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry124), 8226);

  entry125 = gtk_entry_new ();
  gtk_widget_show (entry125);
  gtk_table_attach (GTK_TABLE (table4), entry125, 2, 3, 7, 8,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry125, 70, -1);
  gtk_widget_set_sensitive (entry125, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry125), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry125), 8226);

  entry126 = gtk_entry_new ();
  gtk_widget_show (entry126);
  gtk_table_attach (GTK_TABLE (table4), entry126, 2, 3, 8, 9,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry126, 70, -1);
  gtk_widget_set_sensitive (entry126, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry126), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry126), 8226);

  entry127 = gtk_entry_new ();
  gtk_widget_show (entry127);
  gtk_table_attach (GTK_TABLE (table4), entry127, 2, 3, 9, 10,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry127, 70, -1);
  gtk_widget_set_sensitive (entry127, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry127), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry127), 8226);

  entry128 = gtk_entry_new ();
  gtk_widget_show (entry128);
  gtk_table_attach (GTK_TABLE (table4), entry128, 2, 3, 10, 11,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry128, 70, -1);
  gtk_widget_set_sensitive (entry128, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry128), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry128), 8226);

  entry129 = gtk_entry_new ();
  gtk_widget_show (entry129);
  gtk_table_attach (GTK_TABLE (table4), entry129, 2, 3, 11, 12,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry129, 70, -1);
  gtk_widget_set_sensitive (entry129, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry129), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry129), 8226);

  entry130 = gtk_entry_new ();
  gtk_widget_show (entry130);
  gtk_table_attach (GTK_TABLE (table4), entry130, 2, 3, 12, 13,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry130, 70, -1);
  gtk_widget_set_sensitive (entry130, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry130), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry130), 8226);

  label255 = gtk_label_new (_("Number"));
  gtk_widget_show (label255);
  gtk_table_attach (GTK_TABLE (table4), label255, 2, 3, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label255), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label255), 0, 0.5);

  label256 = gtk_label_new (_("Delay between"));
  gtk_widget_show (label256);
  gtk_table_attach (GTK_TABLE (table4), label256, 3, 4, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label256), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label256), 0, 0.5);

  label257 = gtk_label_new (_("Delay to next"));
  gtk_widget_show (label257);
  gtk_table_attach (GTK_TABLE (table4), label257, 4, 5, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label257), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label257), 0, 0.5);

  label254 = gtk_label_new (_("Packet name"));
  gtk_widget_show (label254);
  gtk_table_attach (GTK_TABLE (table4), label254, 0, 1, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label254), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label254), 0, 0.5);

  entry131 = gtk_entry_new ();
  gtk_widget_show (entry131);
  gtk_table_attach (GTK_TABLE (table4), entry131, 3, 4, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry131, 90, -1);
  gtk_widget_set_sensitive (entry131, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry131), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry131), 8226);

  entry132 = gtk_entry_new ();
  gtk_widget_show (entry132);
  gtk_table_attach (GTK_TABLE (table4), entry132, 3, 4, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry132, 90, -1);
  gtk_widget_set_sensitive (entry132, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry132), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry132), 8226);

  entry133 = gtk_entry_new ();
  gtk_widget_show (entry133);
  gtk_table_attach (GTK_TABLE (table4), entry133, 3, 4, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry133, 90, -1);
  gtk_widget_set_sensitive (entry133, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry133), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry133), 8226);

  entry134 = gtk_entry_new ();
  gtk_widget_show (entry134);
  gtk_table_attach (GTK_TABLE (table4), entry134, 3, 4, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry134, 90, -1);
  gtk_widget_set_sensitive (entry134, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry134), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry134), 8226);

  entry135 = gtk_entry_new ();
  gtk_widget_show (entry135);
  gtk_table_attach (GTK_TABLE (table4), entry135, 3, 4, 7, 8,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry135, 90, -1);
  gtk_widget_set_sensitive (entry135, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry135), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry135), 8226);

  entry136 = gtk_entry_new ();
  gtk_widget_show (entry136);
  gtk_table_attach (GTK_TABLE (table4), entry136, 3, 4, 8, 9,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry136, 90, -1);
  gtk_widget_set_sensitive (entry136, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry136), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry136), 8226);

  entry137 = gtk_entry_new ();
  gtk_widget_show (entry137);
  gtk_table_attach (GTK_TABLE (table4), entry137, 3, 4, 9, 10,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry137, 90, -1);
  gtk_widget_set_sensitive (entry137, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry137), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry137), 8226);

  entry138 = gtk_entry_new ();
  gtk_widget_show (entry138);
  gtk_table_attach (GTK_TABLE (table4), entry138, 3, 4, 10, 11,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry138, 90, -1);
  gtk_widget_set_sensitive (entry138, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry138), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry138), 8226);

  entry141 = gtk_entry_new ();
  gtk_widget_show (entry141);
  gtk_table_attach (GTK_TABLE (table4), entry141, 4, 5, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry141, 90, -1);
  gtk_widget_set_sensitive (entry141, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry141), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry141), 8226);

  entry142 = gtk_entry_new ();
  gtk_widget_show (entry142);
  gtk_table_attach (GTK_TABLE (table4), entry142, 4, 5, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry142, 90, -1);
  gtk_widget_set_sensitive (entry142, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry142), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry142), 8226);

  entry143 = gtk_entry_new ();
  gtk_widget_show (entry143);
  gtk_table_attach (GTK_TABLE (table4), entry143, 4, 5, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry143, 90, -1);
  gtk_widget_set_sensitive (entry143, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry143), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry143), 8226);

  entry144 = gtk_entry_new ();
  gtk_widget_show (entry144);
  gtk_table_attach (GTK_TABLE (table4), entry144, 4, 5, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry144, 90, -1);
  gtk_widget_set_sensitive (entry144, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry144), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry144), 8226);

  entry145 = gtk_entry_new ();
  gtk_widget_show (entry145);
  gtk_table_attach (GTK_TABLE (table4), entry145, 4, 5, 7, 8,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry145, 90, -1);
  gtk_widget_set_sensitive (entry145, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry145), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry145), 8226);

  entry146 = gtk_entry_new ();
  gtk_widget_show (entry146);
  gtk_table_attach (GTK_TABLE (table4), entry146, 4, 5, 8, 9,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry146, 90, -1);
  gtk_widget_set_sensitive (entry146, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry146), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry146), 8226);

  entry147 = gtk_entry_new ();
  gtk_widget_show (entry147);
  gtk_table_attach (GTK_TABLE (table4), entry147, 4, 5, 9, 10,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry147, 90, -1);
  gtk_widget_set_sensitive (entry147, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry147), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry147), 8226);

  entry148 = gtk_entry_new ();
  gtk_widget_show (entry148);
  gtk_table_attach (GTK_TABLE (table4), entry148, 4, 5, 10, 11,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry148, 90, -1);
  gtk_widget_set_sensitive (entry148, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry148), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry148), 8226);

  entry149 = gtk_entry_new ();
  gtk_widget_show (entry149);
  gtk_table_attach (GTK_TABLE (table4), entry149, 4, 5, 11, 12,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry149, 90, -1);
  gtk_widget_set_sensitive (entry149, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry149), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry149), 8226);

  entry150 = gtk_entry_new ();
  gtk_widget_show (entry150);
  gtk_table_attach (GTK_TABLE (table4), entry150, 4, 5, 12, 13,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry150, 90, -1);
  gtk_widget_set_sensitive (entry150, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry150), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry150), 8226);

  entry112 = gtk_entry_new ();
  gtk_widget_show (entry112);
  gtk_table_attach (GTK_TABLE (table4), entry112, 0, 1, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry112, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry112), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry112), 8226);

  entry113 = gtk_entry_new ();
  gtk_widget_show (entry113);
  gtk_table_attach (GTK_TABLE (table4), entry113, 0, 1, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry113, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry113), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry113), 8226);

  entry114 = gtk_entry_new ();
  gtk_widget_show (entry114);
  gtk_table_attach (GTK_TABLE (table4), entry114, 0, 1, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry114, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry114), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry114), 8226);

  entry115 = gtk_entry_new ();
  gtk_widget_show (entry115);
  gtk_table_attach (GTK_TABLE (table4), entry115, 0, 1, 7, 8,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry115, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry115), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry115), 8226);

  entry116 = gtk_entry_new ();
  gtk_widget_show (entry116);
  gtk_table_attach (GTK_TABLE (table4), entry116, 0, 1, 8, 9,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry116, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry116), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry116), 8226);

  entry117 = gtk_entry_new ();
  gtk_widget_show (entry117);
  gtk_table_attach (GTK_TABLE (table4), entry117, 0, 1, 9, 10,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry117, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry117), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry117), 8226);

  entry118 = gtk_entry_new ();
  gtk_widget_show (entry118);
  gtk_table_attach (GTK_TABLE (table4), entry118, 0, 1, 10, 11,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry118, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry118), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry118), 8226);

  entry119 = gtk_entry_new ();
  gtk_widget_show (entry119);
  gtk_table_attach (GTK_TABLE (table4), entry119, 0, 1, 11, 12,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry119, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry119), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry119), 8226);

  entry120 = gtk_entry_new ();
  gtk_widget_show (entry120);
  gtk_table_attach (GTK_TABLE (table4), entry120, 0, 1, 12, 13,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry120, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry120), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry120), 8226);

  entry139 = gtk_entry_new ();
  gtk_widget_show (entry139);
  gtk_table_attach (GTK_TABLE (table4), entry139, 3, 4, 11, 12,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry139, 90, -1);
  gtk_widget_set_sensitive (entry139, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry139), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry139), 8226);

  entry140 = gtk_entry_new ();
  gtk_widget_show (entry140);
  gtk_table_attach (GTK_TABLE (table4), entry140, 3, 4, 12, 13,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry140, 90, -1);
  gtk_widget_set_sensitive (entry140, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry140), 9);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry140), 8226);

  label268 = gtk_label_new (_("File:"));
  gtk_widget_show (label268);
  gtk_table_attach (GTK_TABLE (table4), label268, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label268), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label268), 0, 0.5);

  entry121 = gtk_entry_new ();
  gtk_widget_show (entry121);
  gtk_table_attach (GTK_TABLE (table4), entry121, 2, 3, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry121, 70, -1);
  gtk_widget_set_sensitive (entry121, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry121), 7);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry121), 8226);

  entry151 = gtk_entry_new ();
  gtk_widget_show (entry151);
  gtk_table_attach (GTK_TABLE (table4), entry151, 2, 3, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry151, 70, -1);
  gtk_widget_set_sensitive (entry151, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry151), 7);
  gtk_entry_set_text (GTK_ENTRY (entry151), _("1"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry151), 8226);

  checkbutton36 = gtk_check_button_new_with_mnemonic (_("Infinite"));
  gtk_widget_show (checkbutton36);
  gtk_table_attach (GTK_TABLE (table4), checkbutton36, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton36), TRUE);

  label271 = gtk_label_new (_("  Number of cycles"));
  gtk_widget_show (label271);
  gtk_table_attach (GTK_TABLE (table4), label271, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label271), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label271), 0, 0.5);

  label270 = gtk_label_new (_("Delay between (us)"));
  gtk_widget_show (label270);
  gtk_table_attach (GTK_TABLE (table4), label270, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label270), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label270), 0, 0.5);

  label272 = gtk_label_new (_(" "));
  gtk_widget_show (label272);
  gtk_table_attach (GTK_TABLE (table4), label272, 5, 6, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label272), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label272), 0, 0.5);

  entry111 = gtk_entry_new ();
  gtk_widget_show (entry111);
  gtk_table_attach (GTK_TABLE (table4), entry111, 0, 1, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry111, 300, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry111), 70);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry111), 8226);

  button65 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button65);
  gtk_table_attach (GTK_TABLE (table4), button65, 1, 2, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button66 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button66);
  gtk_table_attach (GTK_TABLE (table4), button66, 1, 2, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button67 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button67);
  gtk_table_attach (GTK_TABLE (table4), button67, 1, 2, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button68 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button68);
  gtk_table_attach (GTK_TABLE (table4), button68, 1, 2, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button69 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button69);
  gtk_table_attach (GTK_TABLE (table4), button69, 1, 2, 7, 8,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button70 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button70);
  gtk_table_attach (GTK_TABLE (table4), button70, 1, 2, 8, 9,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button71 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button71);
  gtk_table_attach (GTK_TABLE (table4), button71, 1, 2, 9, 10,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button72 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button72);
  gtk_table_attach (GTK_TABLE (table4), button72, 1, 2, 10, 11,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button73 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button73);
  gtk_table_attach (GTK_TABLE (table4), button73, 1, 2, 11, 12,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  button74 = gtk_button_new_with_mnemonic (_(" Select "));
  gtk_widget_show (button74);
  gtk_table_attach (GTK_TABLE (table4), button74, 1, 2, 12, 13,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton25 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton25);
  gtk_table_attach (GTK_TABLE (table4), checkbutton25, 6, 7, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton26 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton26);
  gtk_table_attach (GTK_TABLE (table4), checkbutton26, 6, 7, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton27 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton27);
  gtk_table_attach (GTK_TABLE (table4), checkbutton27, 6, 7, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton28 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton28);
  gtk_table_attach (GTK_TABLE (table4), checkbutton28, 6, 7, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton29 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton29);
  gtk_table_attach (GTK_TABLE (table4), checkbutton29, 6, 7, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton30 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton30);
  gtk_table_attach (GTK_TABLE (table4), checkbutton30, 6, 7, 8, 9,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton31 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton31);
  gtk_table_attach (GTK_TABLE (table4), checkbutton31, 6, 7, 9, 10,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton32 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton32);
  gtk_table_attach (GTK_TABLE (table4), checkbutton32, 6, 7, 10, 11,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton33 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton33);
  gtk_table_attach (GTK_TABLE (table4), checkbutton33, 6, 7, 11, 12,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton34 = gtk_check_button_new_with_mnemonic (_(" Disable"));
  gtk_widget_show (checkbutton34);
  gtk_table_attach (GTK_TABLE (table4), checkbutton34, 6, 7, 12, 13,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label258 = gtk_label_new (_("Enable / Disable"));
  gtk_widget_show (label258);
  gtk_table_attach (GTK_TABLE (table4), label258, 6, 7, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label258), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label258), 0, 0.5);

  label361 = gtk_label_new (_("        Mbit/s"));
  gtk_widget_show (label361);
  gtk_table_attach (GTK_TABLE (table4), label361, 5, 6, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label361), 0, 0.5);

  entry186 = gtk_entry_new ();
  gtk_widget_show (entry186);
  gtk_table_attach (GTK_TABLE (table4), entry186, 5, 6, 4, 5,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry186, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry186), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry186), 9679);

  entry187 = gtk_entry_new ();
  gtk_widget_show (entry187);
  gtk_table_attach (GTK_TABLE (table4), entry187, 5, 6, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry187, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry187), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry187), 9679);

  entry188 = gtk_entry_new ();
  gtk_widget_show (entry188);
  gtk_table_attach (GTK_TABLE (table4), entry188, 5, 6, 6, 7,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry188, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry188), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry188), 9679);

  entry189 = gtk_entry_new ();
  gtk_widget_show (entry189);
  gtk_table_attach (GTK_TABLE (table4), entry189, 5, 6, 7, 8,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry189, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry189), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry189), 9679);

  entry190 = gtk_entry_new ();
  gtk_widget_show (entry190);
  gtk_table_attach (GTK_TABLE (table4), entry190, 5, 6, 8, 9,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry190, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry190), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry190), 9679);

  entry191 = gtk_entry_new ();
  gtk_widget_show (entry191);
  gtk_table_attach (GTK_TABLE (table4), entry191, 5, 6, 9, 10,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry191, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry191), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry191), 9679);

  entry192 = gtk_entry_new ();
  gtk_widget_show (entry192);
  gtk_table_attach (GTK_TABLE (table4), entry192, 5, 6, 10, 11,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry192, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry192), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry192), 9679);

  entry193 = gtk_entry_new ();
  gtk_widget_show (entry193);
  gtk_table_attach (GTK_TABLE (table4), entry193, 5, 6, 11, 12,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry193, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry193), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry193), 9679);

  entry194 = gtk_entry_new ();
  gtk_widget_show (entry194);
  gtk_table_attach (GTK_TABLE (table4), entry194, 5, 6, 12, 13,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry194, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry194), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry194), 9679);

  entry185 = gtk_entry_new ();
  gtk_widget_show (entry185);
  gtk_table_attach (GTK_TABLE (table4), entry185, 5, 6, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 15, 0);
  gtk_widget_set_size_request (entry185, 80, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry185), 6);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry185), 9679);

  entry152 = gtk_entry_new ();
  gtk_widget_show (entry152);
  gtk_table_attach (GTK_TABLE (table4), entry152, 4, 5, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (entry152, 90, -1);
  gtk_widget_set_sensitive (entry152, FALSE);
  gtk_entry_set_max_length (GTK_ENTRY (entry152), 9);
  gtk_entry_set_text (GTK_ENTRY (entry152), _("1"));
  gtk_entry_set_invisible_char (GTK_ENTRY (entry152), 8226);

  button87 = gtk_button_new_with_mnemonic (_("Calculate"));
  gtk_widget_show (button87);
  gtk_table_attach (GTK_TABLE (table4), button87, 5, 6, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  hseparator5 = gtk_hseparator_new ();
  gtk_widget_show (hseparator5);
  gtk_table_attach (GTK_TABLE (table4), hseparator5, 0, 7, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 5);

  label3 = gtk_label_new (_("  seq gen"));
  gtk_widget_show (label3);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label3);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_CENTER);

  vbox77 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox77);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox77);

  frame464 = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (vbox77), frame464, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame464), 10);

  alignment7 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment7);
  gtk_container_add (GTK_CONTAINER (frame464), alignment7);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment7), 0, 0, 12, 0);

  vbox78 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox78);
  gtk_container_add (GTK_CONTAINER (alignment7), vbox78);

  hbox145 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox145);
  gtk_box_pack_start (GTK_BOX (vbox78), hbox145, TRUE, TRUE, 0);

  label351 = gtk_label_new (_(" Number of cycles            "));
  gtk_widget_show (label351);
  gtk_box_pack_start (GTK_BOX (hbox145), label351, FALSE, FALSE, 0);

  checkbutton42 = gtk_check_button_new_with_mnemonic (_("Infinite"));
  gtk_widget_show (checkbutton42);
  gtk_box_pack_start (GTK_BOX (hbox145), checkbutton42, FALSE, FALSE, 5);

  entry183 = gtk_entry_new ();
  gtk_widget_show (entry183);
  gtk_box_pack_start (GTK_BOX (hbox145), entry183, FALSE, FALSE, 5);
  gtk_widget_set_size_request (entry183, 70, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry183), 7);
  gtk_entry_set_text (GTK_ENTRY (entry183), _("1"));

  label352 = gtk_label_new (_("Delay between (us)"));
  gtk_widget_show (label352);
  gtk_box_pack_start (GTK_BOX (hbox145), label352, FALSE, FALSE, 10);

  entry184 = gtk_entry_new ();
  gtk_widget_show (entry184);
  gtk_box_pack_start (GTK_BOX (hbox145), entry184, FALSE, TRUE, 0);
  gtk_widget_set_size_request (entry184, 90, -1);
  gtk_entry_set_max_length (GTK_ENTRY (entry184), 9);
  gtk_entry_set_text (GTK_ENTRY (entry184), _("1"));

  hbox144 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox144);
  gtk_box_pack_start (GTK_BOX (vbox78), hbox144, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox144), 5);

  label350 = gtk_label_new (_("Sending type:               "));
  gtk_widget_show (label350);
  gtk_box_pack_start (GTK_BOX (hbox144), label350, FALSE, FALSE, 0);

  radiobutton59 = gtk_radio_button_new_with_mnemonic (NULL, _(" Keep delay between packets"));
  gtk_widget_show (radiobutton59);
  gtk_box_pack_start (GTK_BOX (hbox144), radiobutton59, FALSE, FALSE, 10);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton59), radiobutton59_group);
  radiobutton59_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton59));

  radiobutton60 = gtk_radio_button_new_with_mnemonic (NULL, _("As fast as possible"));
  gtk_widget_show (radiobutton60);
  gtk_box_pack_start (GTK_BOX (hbox144), radiobutton60, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton60), radiobutton59_group);
  radiobutton59_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton60));

  label349 = gtk_label_new ("");
  gtk_widget_show (label349);
  gtk_frame_set_label_widget (GTK_FRAME (frame464), label349);
  gtk_label_set_use_markup (GTK_LABEL (label349), TRUE);

  frame462 = gtk_frame_new (NULL);
  gtk_widget_show (frame462);
  gtk_box_pack_start (GTK_BOX (vbox77), frame462, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame462), 10);

  alignment5 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment5);
  gtk_container_add (GTK_CONTAINER (frame462), alignment5);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment5), 0, 0, 12, 0);

  scrolledwindow12 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow12);
  gtk_container_add (GTK_CONTAINER (alignment5), scrolledwindow12);

  clist2 = gtk_clist_new (8);
  gtk_widget_show (clist2);
  gtk_container_add (GTK_CONTAINER (scrolledwindow12), clist2);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 0, 50);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 1, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 2, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 3, 60);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 4, 140);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 5, 140);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 6, 100);
  gtk_clist_set_column_width (GTK_CLIST (clist2), 7, 80);
  gtk_clist_column_titles_show (GTK_CLIST (clist2));

  label334 = gtk_label_new (_("Nr."));
  gtk_widget_show (label334);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 0, label334);

  label335 = gtk_label_new (_("Time"));
  gtk_widget_show (label335);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 1, label335);

  label336 = gtk_label_new (_("Delta Time"));
  gtk_widget_show (label336);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 2, label336);

  label337 = gtk_label_new (_("Length"));
  gtk_widget_show (label337);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 3, label337);

  label338 = gtk_label_new (_("Source"));
  gtk_widget_show (label338);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 4, label338);

  label339 = gtk_label_new (_("Destination"));
  gtk_widget_show (label339);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 5, label339);

  label341 = gtk_label_new (_("Protocol"));
  gtk_widget_show (label341);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 6, label341);

  label342 = gtk_label_new (_("Info"));
  gtk_widget_show (label342);
  gtk_clist_set_column_widget (GTK_CLIST (clist2), 7, label342);

  label333 = gtk_label_new ("");
  gtk_widget_show (label333);
  gtk_frame_set_label_widget (GTK_FRAME (frame462), label333);
  gtk_label_set_use_markup (GTK_LABEL (label333), TRUE);

  button86 = gtk_button_new_with_mnemonic (_("button86"));
  gtk_box_pack_start (GTK_BOX (vbox77), button86, FALSE, FALSE, 0);

  label251 = gtk_label_new (_("kernel gen"));
  gtk_widget_show (label251);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 3), label251);
  gtk_label_set_justify (GTK_LABEL (label251), GTK_JUSTIFY_CENTER);

  statusbar1 = gtk_statusbar_new ();
  gtk_widget_show (statusbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar1, FALSE, FALSE, 0);

  g_signal_connect ((gpointer) window1, "destroy",
                    G_CALLBACK (on_window1_destroy),
                    NULL);
  g_signal_connect ((gpointer) select_database1, "activate",
                    G_CALLBACK (on_select_database1_activate),
                    NULL);
  g_signal_connect ((gpointer) exit1, "activate",
                    G_CALLBACK (on_exit1_activate),
                    NULL);
  g_signal_connect ((gpointer) about1, "activate",
                    G_CALLBACK (on_about1_activate),
                    NULL);
  g_signal_connect ((gpointer) Build_button, "clicked",
                    G_CALLBACK (on_Build_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) Gen_button, "clicked",
                    G_CALLBACK (on_Gen_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) Gensbt, "clicked",
                    G_CALLBACK (on_Gen_s_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) Genp, "clicked",
                    G_CALLBACK (on_Gen_p_clicked),
                    NULL);
  g_signal_connect ((gpointer) Load_button, "clicked",
                    G_CALLBACK (on_Load_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) Save_button, "clicked",
                    G_CALLBACK (on_Save_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) Reset_button, "clicked",
                    G_CALLBACK (on_Reset_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) button62, "clicked",
                    G_CALLBACK (on_button62_clicked),
                    NULL);
  g_signal_connect ((gpointer) Interface_button, "clicked",
                    G_CALLBACK (on_Interface_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) Send_button, "clicked",
                    G_CALLBACK (on_Send_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) Stop_button, "clicked",
                    G_CALLBACK (on_button61_clicked),
                    NULL);
  g_signal_connect ((gpointer) auto_get_mac_cbt, "toggled",
                    G_CALLBACK (on_auto_get_mac_cbt_toggled),
                    NULL);
  g_signal_connect ((gpointer) bt_ver2, "toggled",
                    G_CALLBACK (on_ver_II_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) bt_8023, "toggled",
                    G_CALLBACK (on_802_3_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) bt_8021q, "clicked",
                    G_CALLBACK (on_802_1q_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) L_dst_select_bt, "clicked",
                    G_CALLBACK (on_L_dst_select_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) L_src_select_bt, "clicked",
                    G_CALLBACK (on_L_src_select_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) L_optmenu1_bt, "clicked",
                    G_CALLBACK (on_L_optmenu1_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton2, "toggled",
                    G_CALLBACK (on_checkbutton2_toggled),
                    NULL);
  g_signal_connect ((gpointer) checkbutton40, "toggled",
                    G_CALLBACK (on_checkbutton40_toggled),
                    NULL);
  g_signal_connect ((gpointer) _0x1, "activate",
                    G_CALLBACK (on_0x1_activate),
                    NULL);
  g_signal_connect ((gpointer) _0x2, "activate",
                    G_CALLBACK (on_0x2_activate),
                    NULL);
  g_signal_connect ((gpointer) _0x3, "activate",
                    G_CALLBACK (on_0x3_activate),
                    NULL);
  g_signal_connect ((gpointer) _0x4, "activate",
                    G_CALLBACK (on_0x4_activate),
                    NULL);
  g_signal_connect ((gpointer) L_8023_llc_tbt, "toggled",
                    G_CALLBACK (on_L_8023_llc_tbt_toggled),
                    NULL);
  g_signal_connect ((gpointer) L_8023_llcsnap_tbt, "toggled",
                    G_CALLBACK (on_L_8023_llcsnap_tbt_toggled),
                    NULL);
  g_signal_connect ((gpointer) optionmenu6, "clicked",
                    G_CALLBACK (on_optionmenu6_clicked),
                    NULL);
  g_signal_connect ((gpointer) ippkt_radibt, "toggled",
                    G_CALLBACK (IP_packet_toggled),
                    NULL);
  g_signal_connect ((gpointer) IPv6_rdbt, "toggled",
                    G_CALLBACK (on_IPv6_rdbt_toggled),
                    NULL);
  g_signal_connect ((gpointer) arppkt_radiobt, "toggled",
                    G_CALLBACK (on_arppkt_radiobt_toggled),
                    NULL);
  g_signal_connect ((gpointer) usedef2_radibt, "toggled",
                    G_CALLBACK (on_usedef2_radibt_toggled),
                    NULL);
  g_signal_connect ((gpointer) button37, "clicked",
                    G_CALLBACK (on_button37_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton21, "toggled",
                    G_CALLBACK (on_checkbutton21_toggled),
                    NULL);
  g_signal_connect ((gpointer) button78, "clicked",
                    G_CALLBACK (on_button78_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu3, "clicked",
                    G_CALLBACK (on_optionmenu3_clicked),
                    NULL);
  g_signal_connect ((gpointer) ip_header_cks_cbt, "toggled",
                    G_CALLBACK (on_ip_header_cks_cbt_toggled),
                    NULL);
  g_signal_connect ((gpointer) button24, "clicked",
                    G_CALLBACK (on_button24_clicked),
                    NULL);
  g_signal_connect ((gpointer) button25, "clicked",
                    G_CALLBACK (on_button25_clicked),
                    NULL);
  g_signal_connect ((gpointer) udp_bt, "toggled",
                    G_CALLBACK (on_udp_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) tcp_bt, "toggled",
                    G_CALLBACK (on_tcp_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) icmp_bt, "toggled",
                    G_CALLBACK (on_icmp_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) igmp_bt, "toggled",
                    G_CALLBACK (on_igmp_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) ip_user_data_bt, "toggled",
                    G_CALLBACK (on_ip_user_data_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) checkbutton43, "toggled",
                    G_CALLBACK (on_checkbutton43_toggled),
                    NULL);
  g_signal_connect ((gpointer) radiobutton67, "toggled",
                    G_CALLBACK (on_radiobutton67_toggled),
                    NULL);
  g_signal_connect ((gpointer) radiobutton68, "toggled",
                    G_CALLBACK (on_radiobutton68_toggled),
                    NULL);
  g_signal_connect ((gpointer) radiobutton69, "toggled",
                    G_CALLBACK (on_radiobutton69_toggled),
                    NULL);
  g_signal_connect ((gpointer) radiobutton71, "toggled",
                    G_CALLBACK (on_radiobutton71_toggled),
                    NULL);
  g_signal_connect ((gpointer) N_apply_pattern, "clicked",
                    G_CALLBACK (on_N_apply_pattern_clicked),
                    NULL);
  g_signal_connect ((gpointer) N_select_payload, "clicked",
                    G_CALLBACK (on_N_select_payload_clicked),
                    NULL);
  g_signal_connect ((gpointer) button33, "clicked",
                    G_CALLBACK (on_button33_clicked),
                    NULL);
  g_signal_connect ((gpointer) button34, "clicked",
                    G_CALLBACK (on_button34_clicked),
                    NULL);
  g_signal_connect ((gpointer) button36, "clicked",
                    G_CALLBACK (on_button36_clicked),
                    NULL);
  g_signal_connect ((gpointer) button35, "clicked",
                    G_CALLBACK (on_button35_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton3, "toggled",
                    G_CALLBACK (on_checkbutton3_toggled),
                    NULL);
  g_signal_connect ((gpointer) checkbutton4, "toggled",
                    G_CALLBACK (on_checkbutton4_toggled),
                    NULL);
  g_signal_connect ((gpointer) udp_apply_pattern_button, "clicked",
                    G_CALLBACK (on_udp_apply_pattern_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) udp_select_payload_button, "clicked",
                    G_CALLBACK (on_udp_select_payload_button_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton13, "toggled",
                    G_CALLBACK (on_checkbutton13_toggled),
                    NULL);
  g_signal_connect ((gpointer) apply_tcp_pattern_bt, "clicked",
                    G_CALLBACK (on_apply_tcp_pattern_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) select_tpc_payload_bt, "clicked",
                    G_CALLBACK (on_select_tpc_payload_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu4, "clicked",
                    G_CALLBACK (on_optionmenu4_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton16, "toggled",
                    G_CALLBACK (on_checkbutton16_toggled),
                    NULL);
  g_signal_connect ((gpointer) optionmenu5, "clicked",
                    G_CALLBACK (on_optionmenu5_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton15, "toggled",
                    G_CALLBACK (on_checkbutton15_toggled),
                    NULL);
  g_signal_connect ((gpointer) checkbutton20, "toggled",
                    G_CALLBACK (on_checkbutton20_toggled),
                    NULL);
  g_signal_connect ((gpointer) button39, "clicked",
                    G_CALLBACK (on_button39_clicked),
                    NULL);
  g_signal_connect ((gpointer) button38, "clicked",
                    G_CALLBACK (on_button38_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu20, "clicked",
                    G_CALLBACK (on_igmpmessage_type_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton41, "toggled",
                    G_CALLBACK (on_igmp_checksum_bt_toggled),
                    NULL);
  g_signal_connect ((gpointer) button81, "clicked",
                    G_CALLBACK (on_button81_clicked),
                    NULL);
  g_signal_connect ((gpointer) checkbutton35, "toggled",
                    G_CALLBACK (on_checkbutton35_toggled),
                    NULL);
  g_signal_connect ((gpointer) checkbutton37, "toggled",
                    G_CALLBACK (on_checkbutton37_toggled),
                    NULL);
  g_signal_connect ((gpointer) optionmenu9, "clicked",
                    G_CALLBACK (on_optionmenu9_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu16, "clicked",
                    G_CALLBACK (on_optionmenu16_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu17, "clicked",
                    G_CALLBACK (on_optionmenu17_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu18, "clicked",
                    G_CALLBACK (on_optionmenu18_clicked),
                    NULL);
  g_signal_connect ((gpointer) optionmenu19, "clicked",
                    G_CALLBACK (on_optionmenu19_clicked),
                    NULL);
  g_signal_connect ((gpointer) entry160, "changed",
                    G_CALLBACK (on_entry160_changed),
                    NULL);
  g_signal_connect ((gpointer) optionmenu14, "clicked",
                    G_CALLBACK (on_optionmenu14_clicked),
                    NULL);
  g_signal_connect ((gpointer) entry162, "changed",
                    G_CALLBACK (on_entry162_changed),
                    NULL);
  g_signal_connect ((gpointer) optionmenu15, "clicked",
                    G_CALLBACK (on_optionmenu15_clicked),
                    NULL);
  g_signal_connect ((gpointer) radiobutton61, "toggled",
                    G_CALLBACK (on_radiobutton61_toggled),
                    NULL);
  g_signal_connect ((gpointer) radiobutton62, "toggled",
                    G_CALLBACK (on_radiobutton62_toggled),
                    NULL);
  g_signal_connect ((gpointer) checkbutton36, "toggled",
                    G_CALLBACK (on_checkbutton36_toggled),
                    NULL);
  g_signal_connect ((gpointer) button65, "clicked",
                    G_CALLBACK (on_button65_clicked),
                    NULL);
  g_signal_connect ((gpointer) button66, "clicked",
                    G_CALLBACK (on_button66_clicked),
                    NULL);
  g_signal_connect ((gpointer) button67, "clicked",
                    G_CALLBACK (on_button67_clicked),
                    NULL);
  g_signal_connect ((gpointer) button68, "clicked",
                    G_CALLBACK (on_button68_clicked),
                    NULL);
  g_signal_connect ((gpointer) button69, "clicked",
                    G_CALLBACK (on_button69_clicked),
                    NULL);
  g_signal_connect ((gpointer) button70, "clicked",
                    G_CALLBACK (on_button70_clicked),
                    NULL);
  g_signal_connect ((gpointer) button71, "clicked",
                    G_CALLBACK (on_button71_clicked),
                    NULL);
  g_signal_connect ((gpointer) button72, "clicked",
                    G_CALLBACK (on_button72_clicked),
                    NULL);
  g_signal_connect ((gpointer) button73, "clicked",
                    G_CALLBACK (on_button73_clicked),
                    NULL);
  g_signal_connect ((gpointer) button74, "clicked",
                    G_CALLBACK (on_button74_clicked),
                    NULL);
  g_signal_connect ((gpointer) button87, "clicked",
                    G_CALLBACK (on_button87_clicked),
                    NULL);
  g_signal_connect ((gpointer) clist2, "select_row",
                    G_CALLBACK (on_clist2_select_row),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (window1, menubar1, "menubar1");
  GLADE_HOOKUP_OBJECT (window1, file1, "file1");
  GLADE_HOOKUP_OBJECT (window1, file1_menu, "file1_menu");
  GLADE_HOOKUP_OBJECT (window1, select_database1, "select_database1");
  GLADE_HOOKUP_OBJECT (window1, exit1, "exit1");
  GLADE_HOOKUP_OBJECT (window1, help1, "help1");
  GLADE_HOOKUP_OBJECT (window1, help1_menu, "help1_menu");
  GLADE_HOOKUP_OBJECT (window1, about1, "about1");
  GLADE_HOOKUP_OBJECT (window1, hbox101, "hbox101");
  GLADE_HOOKUP_OBJECT (window1, toolbar1, "toolbar1");
  GLADE_HOOKUP_OBJECT (window1, toolitem1, "toolitem1");
  GLADE_HOOKUP_OBJECT (window1, Build_button, "Build_button");
  GLADE_HOOKUP_OBJECT (window1, vbox34, "vbox34");
  GLADE_HOOKUP_OBJECT (window1, pixmap15, "pixmap15");
  GLADE_HOOKUP_OBJECT (window1, label166, "label166");
  GLADE_HOOKUP_OBJECT (window1, toolitem2, "toolitem2");
  GLADE_HOOKUP_OBJECT (window1, Gen_button, "Gen_button");
  GLADE_HOOKUP_OBJECT (window1, vbox35, "vbox35");
  GLADE_HOOKUP_OBJECT (window1, pixmap3, "pixmap3");
  GLADE_HOOKUP_OBJECT (window1, label167, "label167");
  GLADE_HOOKUP_OBJECT (window1, toolitem3, "toolitem3");
  GLADE_HOOKUP_OBJECT (window1, Gensbt, "Gensbt");
  GLADE_HOOKUP_OBJECT (window1, vbox36, "vbox36");
  GLADE_HOOKUP_OBJECT (window1, pixmap4, "pixmap4");
  GLADE_HOOKUP_OBJECT (window1, label168, "label168");
  GLADE_HOOKUP_OBJECT (window1, toolitem13, "toolitem13");
  GLADE_HOOKUP_OBJECT (window1, Genp, "Genp");
  GLADE_HOOKUP_OBJECT (window1, vbox76, "vbox76");
  GLADE_HOOKUP_OBJECT (window1, image1, "image1");
  GLADE_HOOKUP_OBJECT (window1, label325, "label325");
  GLADE_HOOKUP_OBJECT (window1, toolitem5, "toolitem5");
  GLADE_HOOKUP_OBJECT (window1, label353, "label353");
  GLADE_HOOKUP_OBJECT (window1, toolitem6, "toolitem6");
  GLADE_HOOKUP_OBJECT (window1, Load_button, "Load_button");
  GLADE_HOOKUP_OBJECT (window1, vbox37, "vbox37");
  GLADE_HOOKUP_OBJECT (window1, pixmap5, "pixmap5");
  GLADE_HOOKUP_OBJECT (window1, label169, "label169");
  GLADE_HOOKUP_OBJECT (window1, toolitem7, "toolitem7");
  GLADE_HOOKUP_OBJECT (window1, Save_button, "Save_button");
  GLADE_HOOKUP_OBJECT (window1, vbox38, "vbox38");
  GLADE_HOOKUP_OBJECT (window1, pixmap6, "pixmap6");
  GLADE_HOOKUP_OBJECT (window1, label170, "label170");
  GLADE_HOOKUP_OBJECT (window1, toolitem8, "toolitem8");
  GLADE_HOOKUP_OBJECT (window1, Reset_button, "Reset_button");
  GLADE_HOOKUP_OBJECT (window1, vbox39, "vbox39");
  GLADE_HOOKUP_OBJECT (window1, pixmap16, "pixmap16");
  GLADE_HOOKUP_OBJECT (window1, label171, "label171");
  GLADE_HOOKUP_OBJECT (window1, toolitem9, "toolitem9");
  GLADE_HOOKUP_OBJECT (window1, button62, "button62");
  GLADE_HOOKUP_OBJECT (window1, vbox43, "vbox43");
  GLADE_HOOKUP_OBJECT (window1, pixmap11, "pixmap11");
  GLADE_HOOKUP_OBJECT (window1, label175, "label175");
  GLADE_HOOKUP_OBJECT (window1, toolitem12, "toolitem12");
  GLADE_HOOKUP_OBJECT (window1, label354, "label354");
  GLADE_HOOKUP_OBJECT (window1, toolitem16, "toolitem16");
  GLADE_HOOKUP_OBJECT (window1, Interface_button, "Interface_button");
  GLADE_HOOKUP_OBJECT (window1, vbox40, "vbox40");
  GLADE_HOOKUP_OBJECT (window1, pixmap8, "pixmap8");
  GLADE_HOOKUP_OBJECT (window1, label172, "label172");
  GLADE_HOOKUP_OBJECT (window1, toolitem17, "toolitem17");
  GLADE_HOOKUP_OBJECT (window1, label355, "label355");
  GLADE_HOOKUP_OBJECT (window1, toolitem18, "toolitem18");
  GLADE_HOOKUP_OBJECT (window1, Send_button, "Send_button");
  GLADE_HOOKUP_OBJECT (window1, vbox41, "vbox41");
  GLADE_HOOKUP_OBJECT (window1, pixmap9, "pixmap9");
  GLADE_HOOKUP_OBJECT (window1, label173, "label173");
  GLADE_HOOKUP_OBJECT (window1, toolitem19, "toolitem19");
  GLADE_HOOKUP_OBJECT (window1, Stop_button, "Stop_button");
  GLADE_HOOKUP_OBJECT (window1, vbox42, "vbox42");
  GLADE_HOOKUP_OBJECT (window1, pixmap10, "pixmap10");
  GLADE_HOOKUP_OBJECT (window1, label174, "label174");
  GLADE_HOOKUP_OBJECT (window1, notebook1, "notebook1");
  GLADE_HOOKUP_OBJECT (window1, vbox_13, "vbox_13");
  GLADE_HOOKUP_OBJECT (window1, label160, "label160");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (window1, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (window1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (window1, frame463, "frame463");
  GLADE_HOOKUP_OBJECT (window1, alignment6, "alignment6");
  GLADE_HOOKUP_OBJECT (window1, hbox143, "hbox143");
  GLADE_HOOKUP_OBJECT (window1, label344, "label344");
  GLADE_HOOKUP_OBJECT (window1, entry179, "entry179");
  GLADE_HOOKUP_OBJECT (window1, label346, "label346");
  GLADE_HOOKUP_OBJECT (window1, entry180, "entry180");
  GLADE_HOOKUP_OBJECT (window1, label347, "label347");
  GLADE_HOOKUP_OBJECT (window1, entry181, "entry181");
  GLADE_HOOKUP_OBJECT (window1, label348, "label348");
  GLADE_HOOKUP_OBJECT (window1, entry182, "entry182");
  GLADE_HOOKUP_OBJECT (window1, label343, "label343");
  GLADE_HOOKUP_OBJECT (window1, hbox72, "hbox72");
  GLADE_HOOKUP_OBJECT (window1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (window1, vbox17, "vbox17");
  GLADE_HOOKUP_OBJECT (window1, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (window1, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (window1, auto_get_mac_cbt, "auto_get_mac_cbt");
  GLADE_HOOKUP_OBJECT (window1, bt_ver2, "bt_ver2");
  GLADE_HOOKUP_OBJECT (window1, bt_8023, "bt_8023");
  GLADE_HOOKUP_OBJECT (window1, bt_8021q, "bt_8021q");
  GLADE_HOOKUP_OBJECT (window1, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (window1, frame5, "frame5");
  GLADE_HOOKUP_OBJECT (window1, vbox8, "vbox8");
  GLADE_HOOKUP_OBJECT (window1, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (window1, label7, "label7");
  GLADE_HOOKUP_OBJECT (window1, L_dst_mac, "L_dst_mac");
  GLADE_HOOKUP_OBJECT (window1, L_dst_select_bt, "L_dst_select_bt");
  GLADE_HOOKUP_OBJECT (window1, hbox6, "hbox6");
  GLADE_HOOKUP_OBJECT (window1, label8, "label8");
  GLADE_HOOKUP_OBJECT (window1, L_src_mac, "L_src_mac");
  GLADE_HOOKUP_OBJECT (window1, L_src_select_bt, "L_src_select_bt");
  GLADE_HOOKUP_OBJECT (window1, notebook_ethtype, "notebook_ethtype");
  GLADE_HOOKUP_OBJECT (window1, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (window1, label11, "label11");
  GLADE_HOOKUP_OBJECT (window1, L_ethtype, "L_ethtype");
  GLADE_HOOKUP_OBJECT (window1, L_optmenu1_bt, "L_optmenu1_bt");
  GLADE_HOOKUP_OBJECT (window1, convertwidget3, "convertwidget3");
  GLADE_HOOKUP_OBJECT (window1, convertwidget4, "convertwidget4");
  GLADE_HOOKUP_OBJECT (window1, convertwidget5, "convertwidget5");
  GLADE_HOOKUP_OBJECT (window1, convertwidget6, "convertwidget6");
  GLADE_HOOKUP_OBJECT (window1, convertwidget7, "convertwidget7");
  GLADE_HOOKUP_OBJECT (window1, label9, "label9");
  GLADE_HOOKUP_OBJECT (window1, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (window1, label12, "label12");
  GLADE_HOOKUP_OBJECT (window1, label143, "label143");
  GLADE_HOOKUP_OBJECT (window1, entry5, "entry5");
  GLADE_HOOKUP_OBJECT (window1, checkbutton2, "checkbutton2");
  GLADE_HOOKUP_OBJECT (window1, L_length, "L_length");
  GLADE_HOOKUP_OBJECT (window1, label180, "label180");
  GLADE_HOOKUP_OBJECT (window1, frame6, "frame6");
  GLADE_HOOKUP_OBJECT (window1, vbox73, "vbox73");
  GLADE_HOOKUP_OBJECT (window1, hseparator7, "hseparator7");
  GLADE_HOOKUP_OBJECT (window1, hbox136, "hbox136");
  GLADE_HOOKUP_OBJECT (window1, checkbutton40, "checkbutton40");
  GLADE_HOOKUP_OBJECT (window1, optionmenu21, "optionmenu21");
  GLADE_HOOKUP_OBJECT (window1, menu4, "menu4");
  GLADE_HOOKUP_OBJECT (window1, _0x1, "_0x1");
  GLADE_HOOKUP_OBJECT (window1, _0x2, "_0x2");
  GLADE_HOOKUP_OBJECT (window1, _0x3, "_0x3");
  GLADE_HOOKUP_OBJECT (window1, _0x4, "_0x4");
  GLADE_HOOKUP_OBJECT (window1, label304, "label304");
  GLADE_HOOKUP_OBJECT (window1, entry165, "entry165");
  GLADE_HOOKUP_OBJECT (window1, hseparator6, "hseparator6");
  GLADE_HOOKUP_OBJECT (window1, hbox49, "hbox49");
  GLADE_HOOKUP_OBJECT (window1, label137, "label137");
  GLADE_HOOKUP_OBJECT (window1, L_tag_id, "L_tag_id");
  GLADE_HOOKUP_OBJECT (window1, label120, "label120");
  GLADE_HOOKUP_OBJECT (window1, hbox134, "hbox134");
  GLADE_HOOKUP_OBJECT (window1, label14, "label14");
  GLADE_HOOKUP_OBJECT (window1, L_optmenu2_bt, "L_optmenu2_bt");
  GLADE_HOOKUP_OBJECT (window1, convertwidget8, "convertwidget8");
  GLADE_HOOKUP_OBJECT (window1, convertwidget9, "convertwidget9");
  GLADE_HOOKUP_OBJECT (window1, convertwidget10, "convertwidget10");
  GLADE_HOOKUP_OBJECT (window1, convertwidget11, "convertwidget11");
  GLADE_HOOKUP_OBJECT (window1, convertwidget12, "convertwidget12");
  GLADE_HOOKUP_OBJECT (window1, convertwidget13, "convertwidget13");
  GLADE_HOOKUP_OBJECT (window1, convertwidget14, "convertwidget14");
  GLADE_HOOKUP_OBJECT (window1, convertwidget15, "convertwidget15");
  GLADE_HOOKUP_OBJECT (window1, convertwidget16, "convertwidget16");
  GLADE_HOOKUP_OBJECT (window1, hbox135, "hbox135");
  GLADE_HOOKUP_OBJECT (window1, checkbutton39, "checkbutton39");
  GLADE_HOOKUP_OBJECT (window1, label16, "label16");
  GLADE_HOOKUP_OBJECT (window1, L_vlan_id, "L_vlan_id");
  GLADE_HOOKUP_OBJECT (window1, label181, "label181");
  GLADE_HOOKUP_OBJECT (window1, frame7, "frame7");
  GLADE_HOOKUP_OBJECT (window1, vbox74, "vbox74");
  GLADE_HOOKUP_OBJECT (window1, hbox137, "hbox137");
  GLADE_HOOKUP_OBJECT (window1, label20, "label20");
  GLADE_HOOKUP_OBJECT (window1, L_8023_llc_tbt, "L_8023_llc_tbt");
  GLADE_HOOKUP_OBJECT (window1, L_8023_llcsnap_tbt, "L_8023_llcsnap_tbt");
  GLADE_HOOKUP_OBJECT (window1, hbox138, "hbox138");
  GLADE_HOOKUP_OBJECT (window1, label21, "label21");
  GLADE_HOOKUP_OBJECT (window1, L_dsap, "L_dsap");
  GLADE_HOOKUP_OBJECT (window1, label24, "label24");
  GLADE_HOOKUP_OBJECT (window1, L_ssap, "L_ssap");
  GLADE_HOOKUP_OBJECT (window1, hbox139, "hbox139");
  GLADE_HOOKUP_OBJECT (window1, label22, "label22");
  GLADE_HOOKUP_OBJECT (window1, L_ctrl, "L_ctrl");
  GLADE_HOOKUP_OBJECT (window1, label_oui, "label_oui");
  GLADE_HOOKUP_OBJECT (window1, L_oui, "L_oui");
  GLADE_HOOKUP_OBJECT (window1, hbox140, "hbox140");
  GLADE_HOOKUP_OBJECT (window1, label_pid, "label_pid");
  GLADE_HOOKUP_OBJECT (window1, L_pid, "L_pid");
  GLADE_HOOKUP_OBJECT (window1, optionmenu6, "optionmenu6");
  GLADE_HOOKUP_OBJECT (window1, convertwidget17, "convertwidget17");
  GLADE_HOOKUP_OBJECT (window1, convertwidget18, "convertwidget18");
  GLADE_HOOKUP_OBJECT (window1, convertwidget19, "convertwidget19");
  GLADE_HOOKUP_OBJECT (window1, convertwidget20, "convertwidget20");
  GLADE_HOOKUP_OBJECT (window1, convertwidget21, "convertwidget21");
  GLADE_HOOKUP_OBJECT (window1, neki1, "neki1");
  GLADE_HOOKUP_OBJECT (window1, hbox68, "hbox68");
  GLADE_HOOKUP_OBJECT (window1, frame14, "frame14");
  GLADE_HOOKUP_OBJECT (window1, hbox50, "hbox50");
  GLADE_HOOKUP_OBJECT (window1, label63, "label63");
  GLADE_HOOKUP_OBJECT (window1, ippkt_radibt, "ippkt_radibt");
  GLADE_HOOKUP_OBJECT (window1, IPv6_rdbt, "IPv6_rdbt");
  GLADE_HOOKUP_OBJECT (window1, arppkt_radiobt, "arppkt_radiobt");
  GLADE_HOOKUP_OBJECT (window1, usedef2_radibt, "usedef2_radibt");
  GLADE_HOOKUP_OBJECT (window1, label179, "label179");
  GLADE_HOOKUP_OBJECT (window1, hbox71, "hbox71");
  GLADE_HOOKUP_OBJECT (window1, notebook2, "notebook2");
  GLADE_HOOKUP_OBJECT (window1, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (window1, vbox16, "vbox16");
  GLADE_HOOKUP_OBJECT (window1, frame15, "frame15");
  GLADE_HOOKUP_OBJECT (window1, vbox26, "vbox26");
  GLADE_HOOKUP_OBJECT (window1, hbox23, "hbox23");
  GLADE_HOOKUP_OBJECT (window1, label50, "label50");
  GLADE_HOOKUP_OBJECT (window1, entry26, "entry26");
  GLADE_HOOKUP_OBJECT (window1, label51, "label51");
  GLADE_HOOKUP_OBJECT (window1, entry27, "entry27");
  GLADE_HOOKUP_OBJECT (window1, label52, "label52");
  GLADE_HOOKUP_OBJECT (window1, entry28, "entry28");
  GLADE_HOOKUP_OBJECT (window1, button37, "button37");
  GLADE_HOOKUP_OBJECT (window1, label53, "label53");
  GLADE_HOOKUP_OBJECT (window1, entry29, "entry29");
  GLADE_HOOKUP_OBJECT (window1, checkbutton21, "checkbutton21");
  GLADE_HOOKUP_OBJECT (window1, label54, "label54");
  GLADE_HOOKUP_OBJECT (window1, entry30, "entry30");
  GLADE_HOOKUP_OBJECT (window1, hbox24, "hbox24");
  GLADE_HOOKUP_OBJECT (window1, label55, "label55");
  GLADE_HOOKUP_OBJECT (window1, entry31, "entry31");
  GLADE_HOOKUP_OBJECT (window1, button78, "button78");
  GLADE_HOOKUP_OBJECT (window1, label56, "label56");
  GLADE_HOOKUP_OBJECT (window1, entry32, "entry32");
  GLADE_HOOKUP_OBJECT (window1, label57, "label57");
  GLADE_HOOKUP_OBJECT (window1, entry44, "entry44");
  GLADE_HOOKUP_OBJECT (window1, label58, "label58");
  GLADE_HOOKUP_OBJECT (window1, entry34, "entry34");
  GLADE_HOOKUP_OBJECT (window1, optionmenu3, "optionmenu3");
  GLADE_HOOKUP_OBJECT (window1, convertwidget22, "convertwidget22");
  GLADE_HOOKUP_OBJECT (window1, convertwidget23, "convertwidget23");
  GLADE_HOOKUP_OBJECT (window1, convertwidget24, "convertwidget24");
  GLADE_HOOKUP_OBJECT (window1, convertwidget25, "convertwidget25");
  GLADE_HOOKUP_OBJECT (window1, convertwidget26, "convertwidget26");
  GLADE_HOOKUP_OBJECT (window1, convertwidget27, "convertwidget27");
  GLADE_HOOKUP_OBJECT (window1, convertwidget28, "convertwidget28");
  GLADE_HOOKUP_OBJECT (window1, label59, "label59");
  GLADE_HOOKUP_OBJECT (window1, entry35, "entry35");
  GLADE_HOOKUP_OBJECT (window1, ip_header_cks_cbt, "ip_header_cks_cbt");
  GLADE_HOOKUP_OBJECT (window1, frame16, "frame16");
  GLADE_HOOKUP_OBJECT (window1, hbox25, "hbox25");
  GLADE_HOOKUP_OBJECT (window1, label60, "label60");
  GLADE_HOOKUP_OBJECT (window1, entry38, "entry38");
  GLADE_HOOKUP_OBJECT (window1, button24, "button24");
  GLADE_HOOKUP_OBJECT (window1, label61, "label61");
  GLADE_HOOKUP_OBJECT (window1, entry37, "entry37");
  GLADE_HOOKUP_OBJECT (window1, button25, "button25");
  GLADE_HOOKUP_OBJECT (window1, label62, "label62");
  GLADE_HOOKUP_OBJECT (window1, entry39, "entry39");
  GLADE_HOOKUP_OBJECT (window1, hbox69, "hbox69");
  GLADE_HOOKUP_OBJECT (window1, frame17, "frame17");
  GLADE_HOOKUP_OBJECT (window1, hbox52, "hbox52");
  GLADE_HOOKUP_OBJECT (window1, label64, "label64");
  GLADE_HOOKUP_OBJECT (window1, udp_bt, "udp_bt");
  GLADE_HOOKUP_OBJECT (window1, tcp_bt, "tcp_bt");
  GLADE_HOOKUP_OBJECT (window1, icmp_bt, "icmp_bt");
  GLADE_HOOKUP_OBJECT (window1, igmp_bt, "igmp_bt");
  GLADE_HOOKUP_OBJECT (window1, ip_user_data_bt, "ip_user_data_bt");
  GLADE_HOOKUP_OBJECT (window1, neki2, "neki2");
  GLADE_HOOKUP_OBJECT (window1, label4, "label4");
  GLADE_HOOKUP_OBJECT (window1, frame13, "frame13");
  GLADE_HOOKUP_OBJECT (window1, vbox79, "vbox79");
  GLADE_HOOKUP_OBJECT (window1, frame466, "frame466");
  GLADE_HOOKUP_OBJECT (window1, frame467, "frame467");
  GLADE_HOOKUP_OBJECT (window1, alignment8, "alignment8");
  GLADE_HOOKUP_OBJECT (window1, vbox80, "vbox80");
  GLADE_HOOKUP_OBJECT (window1, hbox149, "hbox149");
  GLADE_HOOKUP_OBJECT (window1, label363, "label363");
  GLADE_HOOKUP_OBJECT (window1, entry195, "entry195");
  GLADE_HOOKUP_OBJECT (window1, label364, "label364");
  GLADE_HOOKUP_OBJECT (window1, entry196, "entry196");
  GLADE_HOOKUP_OBJECT (window1, button90, "button90");
  GLADE_HOOKUP_OBJECT (window1, label365, "label365");
  GLADE_HOOKUP_OBJECT (window1, entry197, "entry197");
  GLADE_HOOKUP_OBJECT (window1, label366, "label366");
  GLADE_HOOKUP_OBJECT (window1, entry198, "entry198");
  GLADE_HOOKUP_OBJECT (window1, checkbutton43, "checkbutton43");
  GLADE_HOOKUP_OBJECT (window1, label367, "label367");
  GLADE_HOOKUP_OBJECT (window1, entry199, "entry199");
  GLADE_HOOKUP_OBJECT (window1, label368, "label368");
  GLADE_HOOKUP_OBJECT (window1, entry200, "entry200");
  GLADE_HOOKUP_OBJECT (window1, frame468, "frame468");
  GLADE_HOOKUP_OBJECT (window1, alignment9, "alignment9");
  GLADE_HOOKUP_OBJECT (window1, hbox152, "hbox152");
  GLADE_HOOKUP_OBJECT (window1, label369, "label369");
  GLADE_HOOKUP_OBJECT (window1, entry201, "entry201");
  GLADE_HOOKUP_OBJECT (window1, button88, "button88");
  GLADE_HOOKUP_OBJECT (window1, label370, "label370");
  GLADE_HOOKUP_OBJECT (window1, entry202, "entry202");
  GLADE_HOOKUP_OBJECT (window1, button89, "button89");
  GLADE_HOOKUP_OBJECT (window1, frame469, "frame469");
  GLADE_HOOKUP_OBJECT (window1, alignment10, "alignment10");
  GLADE_HOOKUP_OBJECT (window1, hbox153, "hbox153");
  GLADE_HOOKUP_OBJECT (window1, label371, "label371");
  GLADE_HOOKUP_OBJECT (window1, entry203, "entry203");
  GLADE_HOOKUP_OBJECT (window1, button91, "button91");
  GLADE_HOOKUP_OBJECT (window1, hbox146, "hbox146");
  GLADE_HOOKUP_OBJECT (window1, frame465, "frame465");
  GLADE_HOOKUP_OBJECT (window1, hbox147, "hbox147");
  GLADE_HOOKUP_OBJECT (window1, label375, "label375");
  GLADE_HOOKUP_OBJECT (window1, radiobutton67, "radiobutton67");
  GLADE_HOOKUP_OBJECT (window1, radiobutton68, "radiobutton68");
  GLADE_HOOKUP_OBJECT (window1, radiobutton69, "radiobutton69");
  GLADE_HOOKUP_OBJECT (window1, radiobutton71, "radiobutton71");
  GLADE_HOOKUP_OBJECT (window1, neki3, "neki3");
  GLADE_HOOKUP_OBJECT (window1, label117, "label117");
  GLADE_HOOKUP_OBJECT (window1, frame4, "frame4");
  GLADE_HOOKUP_OBJECT (window1, hbox56, "hbox56");
  GLADE_HOOKUP_OBJECT (window1, frame18, "frame18");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow5, "scrolledwindow5");
  GLADE_HOOKUP_OBJECT (window1, text1, "text1");
  GLADE_HOOKUP_OBJECT (window1, hbox58, "hbox58");
  GLADE_HOOKUP_OBJECT (window1, vbox31, "vbox31");
  GLADE_HOOKUP_OBJECT (window1, frame19, "frame19");
  GLADE_HOOKUP_OBJECT (window1, vbox31_1, "vbox31_1");
  GLADE_HOOKUP_OBJECT (window1, hbox59, "hbox59");
  GLADE_HOOKUP_OBJECT (window1, label144, "label144");
  GLADE_HOOKUP_OBJECT (window1, entry79, "entry79");
  GLADE_HOOKUP_OBJECT (window1, hbox60, "hbox60");
  GLADE_HOOKUP_OBJECT (window1, label145, "label145");
  GLADE_HOOKUP_OBJECT (window1, entry80, "entry80");
  GLADE_HOOKUP_OBJECT (window1, N_apply_pattern, "N_apply_pattern");
  GLADE_HOOKUP_OBJECT (window1, N_select_payload, "N_select_payload");
  GLADE_HOOKUP_OBJECT (window1, label146, "label146");
  GLADE_HOOKUP_OBJECT (window1, neki4, "neki4");
  GLADE_HOOKUP_OBJECT (window1, label6, "label6");
  GLADE_HOOKUP_OBJECT (window1, vbox47, "vbox47");
  GLADE_HOOKUP_OBJECT (window1, frame3, "frame3");
  GLADE_HOOKUP_OBJECT (window1, table1, "table1");
  GLADE_HOOKUP_OBJECT (window1, label27, "label27");
  GLADE_HOOKUP_OBJECT (window1, label28, "label28");
  GLADE_HOOKUP_OBJECT (window1, label29, "label29");
  GLADE_HOOKUP_OBJECT (window1, label30, "label30");
  GLADE_HOOKUP_OBJECT (window1, label31, "label31");
  GLADE_HOOKUP_OBJECT (window1, label32, "label32");
  GLADE_HOOKUP_OBJECT (window1, A_prottype, "A_prottype");
  GLADE_HOOKUP_OBJECT (window1, A_hwsize, "A_hwsize");
  GLADE_HOOKUP_OBJECT (window1, A_protsize, "A_protsize");
  GLADE_HOOKUP_OBJECT (window1, A_sendermac, "A_sendermac");
  GLADE_HOOKUP_OBJECT (window1, A_senderip, "A_senderip");
  GLADE_HOOKUP_OBJECT (window1, A_hwtype, "A_hwtype");
  GLADE_HOOKUP_OBJECT (window1, label118, "label118");
  GLADE_HOOKUP_OBJECT (window1, label36, "label36");
  GLADE_HOOKUP_OBJECT (window1, label37, "label37");
  GLADE_HOOKUP_OBJECT (window1, label38, "label38");
  GLADE_HOOKUP_OBJECT (window1, button33, "button33");
  GLADE_HOOKUP_OBJECT (window1, button34, "button34");
  GLADE_HOOKUP_OBJECT (window1, label147, "label147");
  GLADE_HOOKUP_OBJECT (window1, label148, "label148");
  GLADE_HOOKUP_OBJECT (window1, label149, "label149");
  GLADE_HOOKUP_OBJECT (window1, label150, "label150");
  GLADE_HOOKUP_OBJECT (window1, label151, "label151");
  GLADE_HOOKUP_OBJECT (window1, label152, "label152");
  GLADE_HOOKUP_OBJECT (window1, label153, "label153");
  GLADE_HOOKUP_OBJECT (window1, label154, "label154");
  GLADE_HOOKUP_OBJECT (window1, hseparator1, "hseparator1");
  GLADE_HOOKUP_OBJECT (window1, label34, "label34");
  GLADE_HOOKUP_OBJECT (window1, label33, "label33");
  GLADE_HOOKUP_OBJECT (window1, hseparator2, "hseparator2");
  GLADE_HOOKUP_OBJECT (window1, A_targetip, "A_targetip");
  GLADE_HOOKUP_OBJECT (window1, A_targetmac, "A_targetmac");
  GLADE_HOOKUP_OBJECT (window1, hseparator3, "hseparator3");
  GLADE_HOOKUP_OBJECT (window1, button36, "button36");
  GLADE_HOOKUP_OBJECT (window1, button35, "button35");
  GLADE_HOOKUP_OBJECT (window1, radiobutton11, "radiobutton11");
  GLADE_HOOKUP_OBJECT (window1, radiobutton10, "radiobutton10");
  GLADE_HOOKUP_OBJECT (window1, label155, "label155");
  GLADE_HOOKUP_OBJECT (window1, hbox61, "hbox61");
  GLADE_HOOKUP_OBJECT (window1, radiobutton17, "radiobutton17");
  GLADE_HOOKUP_OBJECT (window1, entry81, "entry81");
  GLADE_HOOKUP_OBJECT (window1, neki5, "neki5");
  GLADE_HOOKUP_OBJECT (window1, label_zapolni_arp_spodaj, "label_zapolni_arp_spodaj");
  GLADE_HOOKUP_OBJECT (window1, label5, "label5");
  GLADE_HOOKUP_OBJECT (window1, hbox70, "hbox70");
  GLADE_HOOKUP_OBJECT (window1, notebook4, "notebook4");
  GLADE_HOOKUP_OBJECT (window1, frame9, "frame9");
  GLADE_HOOKUP_OBJECT (window1, vbox19, "vbox19");
  GLADE_HOOKUP_OBJECT (window1, frame22, "frame22");
  GLADE_HOOKUP_OBJECT (window1, hbox27, "hbox27");
  GLADE_HOOKUP_OBJECT (window1, label70, "label70");
  GLADE_HOOKUP_OBJECT (window1, entry56, "entry56");
  GLADE_HOOKUP_OBJECT (window1, label71, "label71");
  GLADE_HOOKUP_OBJECT (window1, label73, "label73");
  GLADE_HOOKUP_OBJECT (window1, entry41, "entry41");
  GLADE_HOOKUP_OBJECT (window1, label72, "label72");
  GLADE_HOOKUP_OBJECT (window1, entry42, "entry42");
  GLADE_HOOKUP_OBJECT (window1, checkbutton3, "checkbutton3");
  GLADE_HOOKUP_OBJECT (window1, label74, "label74");
  GLADE_HOOKUP_OBJECT (window1, entry43, "entry43");
  GLADE_HOOKUP_OBJECT (window1, checkbutton4, "checkbutton4");
  GLADE_HOOKUP_OBJECT (window1, frame23, "frame23");
  GLADE_HOOKUP_OBJECT (window1, hbox28, "hbox28");
  GLADE_HOOKUP_OBJECT (window1, checkbutton5, "checkbutton5");
  GLADE_HOOKUP_OBJECT (window1, frame24, "frame24");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow7, "scrolledwindow7");
  GLADE_HOOKUP_OBJECT (window1, text3, "text3");
  GLADE_HOOKUP_OBJECT (window1, label192, "label192");
  GLADE_HOOKUP_OBJECT (window1, vbox52, "vbox52");
  GLADE_HOOKUP_OBJECT (window1, frame25, "frame25");
  GLADE_HOOKUP_OBJECT (window1, vbox53, "vbox53");
  GLADE_HOOKUP_OBJECT (window1, hbox81, "hbox81");
  GLADE_HOOKUP_OBJECT (window1, label193, "label193");
  GLADE_HOOKUP_OBJECT (window1, entry89, "entry89");
  GLADE_HOOKUP_OBJECT (window1, hbox82, "hbox82");
  GLADE_HOOKUP_OBJECT (window1, label194, "label194");
  GLADE_HOOKUP_OBJECT (window1, entry90, "entry90");
  GLADE_HOOKUP_OBJECT (window1, udp_apply_pattern_button, "udp_apply_pattern_button");
  GLADE_HOOKUP_OBJECT (window1, udp_select_payload_button, "udp_select_payload_button");
  GLADE_HOOKUP_OBJECT (window1, label191, "label191");
  GLADE_HOOKUP_OBJECT (window1, neki9, "neki9");
  GLADE_HOOKUP_OBJECT (window1, label65, "label65");
  GLADE_HOOKUP_OBJECT (window1, vbox29, "vbox29");
  GLADE_HOOKUP_OBJECT (window1, frame8, "frame8");
  GLADE_HOOKUP_OBJECT (window1, vbox55, "vbox55");
  GLADE_HOOKUP_OBJECT (window1, frame29, "frame29");
  GLADE_HOOKUP_OBJECT (window1, vbox18, "vbox18");
  GLADE_HOOKUP_OBJECT (window1, hbox29, "hbox29");
  GLADE_HOOKUP_OBJECT (window1, label75, "label75");
  GLADE_HOOKUP_OBJECT (window1, entry46, "entry46");
  GLADE_HOOKUP_OBJECT (window1, label76, "label76");
  GLADE_HOOKUP_OBJECT (window1, entry47, "entry47");
  GLADE_HOOKUP_OBJECT (window1, label77, "label77");
  GLADE_HOOKUP_OBJECT (window1, entry48, "entry48");
  GLADE_HOOKUP_OBJECT (window1, label78, "label78");
  GLADE_HOOKUP_OBJECT (window1, entry49, "entry49");
  GLADE_HOOKUP_OBJECT (window1, hbox30, "hbox30");
  GLADE_HOOKUP_OBJECT (window1, label79, "label79");
  GLADE_HOOKUP_OBJECT (window1, entry50, "entry50");
  GLADE_HOOKUP_OBJECT (window1, label80, "label80");
  GLADE_HOOKUP_OBJECT (window1, checkbutton22, "checkbutton22");
  GLADE_HOOKUP_OBJECT (window1, checkbutton23, "checkbutton23");
  GLADE_HOOKUP_OBJECT (window1, checkbutton7, "checkbutton7");
  GLADE_HOOKUP_OBJECT (window1, checkbutton8, "checkbutton8");
  GLADE_HOOKUP_OBJECT (window1, checkbutton9, "checkbutton9");
  GLADE_HOOKUP_OBJECT (window1, checkbutton10, "checkbutton10");
  GLADE_HOOKUP_OBJECT (window1, checkbutton11, "checkbutton11");
  GLADE_HOOKUP_OBJECT (window1, checkbutton12, "checkbutton12");
  GLADE_HOOKUP_OBJECT (window1, hbox31, "hbox31");
  GLADE_HOOKUP_OBJECT (window1, label81, "label81");
  GLADE_HOOKUP_OBJECT (window1, entry51, "entry51");
  GLADE_HOOKUP_OBJECT (window1, label82, "label82");
  GLADE_HOOKUP_OBJECT (window1, entry52, "entry52");
  GLADE_HOOKUP_OBJECT (window1, checkbutton13, "checkbutton13");
  GLADE_HOOKUP_OBJECT (window1, label83, "label83");
  GLADE_HOOKUP_OBJECT (window1, entry53, "entry53");
  GLADE_HOOKUP_OBJECT (window1, label84, "label84");
  GLADE_HOOKUP_OBJECT (window1, entry54, "entry54");
  GLADE_HOOKUP_OBJECT (window1, frame30, "frame30");
  GLADE_HOOKUP_OBJECT (window1, hbox90, "hbox90");
  GLADE_HOOKUP_OBJECT (window1, checkbutton14, "checkbutton14");
  GLADE_HOOKUP_OBJECT (window1, frame31, "frame31");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow8, "scrolledwindow8");
  GLADE_HOOKUP_OBJECT (window1, text4, "text4");
  GLADE_HOOKUP_OBJECT (window1, label228, "label228");
  GLADE_HOOKUP_OBJECT (window1, vbox56, "vbox56");
  GLADE_HOOKUP_OBJECT (window1, frame32, "frame32");
  GLADE_HOOKUP_OBJECT (window1, vbox57, "vbox57");
  GLADE_HOOKUP_OBJECT (window1, hbox91, "hbox91");
  GLADE_HOOKUP_OBJECT (window1, label230, "label230");
  GLADE_HOOKUP_OBJECT (window1, entry107, "entry107");
  GLADE_HOOKUP_OBJECT (window1, hbox92, "hbox92");
  GLADE_HOOKUP_OBJECT (window1, label231, "label231");
  GLADE_HOOKUP_OBJECT (window1, entry108, "entry108");
  GLADE_HOOKUP_OBJECT (window1, apply_tcp_pattern_bt, "apply_tcp_pattern_bt");
  GLADE_HOOKUP_OBJECT (window1, select_tpc_payload_bt, "select_tpc_payload_bt");
  GLADE_HOOKUP_OBJECT (window1, label229, "label229");
  GLADE_HOOKUP_OBJECT (window1, neki6, "neki6");
  GLADE_HOOKUP_OBJECT (window1, label66, "label66");
  GLADE_HOOKUP_OBJECT (window1, frame10, "frame10");
  GLADE_HOOKUP_OBJECT (window1, vbox20, "vbox20");
  GLADE_HOOKUP_OBJECT (window1, hbox33, "hbox33");
  GLADE_HOOKUP_OBJECT (window1, label85, "label85");
  GLADE_HOOKUP_OBJECT (window1, entry57, "entry57");
  GLADE_HOOKUP_OBJECT (window1, optionmenu4, "optionmenu4");
  GLADE_HOOKUP_OBJECT (window1, convertwidget35, "convertwidget35");
  GLADE_HOOKUP_OBJECT (window1, convertwidget36, "convertwidget36");
  GLADE_HOOKUP_OBJECT (window1, convertwidget37, "convertwidget37");
  GLADE_HOOKUP_OBJECT (window1, convertwidget38, "convertwidget38");
  GLADE_HOOKUP_OBJECT (window1, convertwidget39, "convertwidget39");
  GLADE_HOOKUP_OBJECT (window1, frame12, "frame12");
  GLADE_HOOKUP_OBJECT (window1, notebook5, "notebook5");
  GLADE_HOOKUP_OBJECT (window1, vbox22, "vbox22");
  GLADE_HOOKUP_OBJECT (window1, hbox38, "hbox38");
  GLADE_HOOKUP_OBJECT (window1, label105, "label105");
  GLADE_HOOKUP_OBJECT (window1, entry62, "entry62");
  GLADE_HOOKUP_OBJECT (window1, label106, "label106");
  GLADE_HOOKUP_OBJECT (window1, entry63, "entry63");
  GLADE_HOOKUP_OBJECT (window1, checkbutton16, "checkbutton16");
  GLADE_HOOKUP_OBJECT (window1, hbox40, "hbox40");
  GLADE_HOOKUP_OBJECT (window1, label107, "label107");
  GLADE_HOOKUP_OBJECT (window1, entry64, "entry64");
  GLADE_HOOKUP_OBJECT (window1, label108, "label108");
  GLADE_HOOKUP_OBJECT (window1, entry65, "entry65");
  GLADE_HOOKUP_OBJECT (window1, hbox42, "hbox42");
  GLADE_HOOKUP_OBJECT (window1, checkbutton17, "checkbutton17");
  GLADE_HOOKUP_OBJECT (window1, entry66, "entry66");
  GLADE_HOOKUP_OBJECT (window1, label104, "label104");
  GLADE_HOOKUP_OBJECT (window1, vbox67, "vbox67");
  GLADE_HOOKUP_OBJECT (window1, hbox112, "hbox112");
  GLADE_HOOKUP_OBJECT (window1, label284, "label284");
  GLADE_HOOKUP_OBJECT (window1, entry157, "entry157");
  GLADE_HOOKUP_OBJECT (window1, label285, "label285");
  GLADE_HOOKUP_OBJECT (window1, entry158, "entry158");
  GLADE_HOOKUP_OBJECT (window1, checkbutton38, "checkbutton38");
  GLADE_HOOKUP_OBJECT (window1, hbox113, "hbox113");
  GLADE_HOOKUP_OBJECT (window1, label286, "label286");
  GLADE_HOOKUP_OBJECT (window1, entry159, "entry159");
  GLADE_HOOKUP_OBJECT (window1, label283, "label283");
  GLADE_HOOKUP_OBJECT (window1, vbox21, "vbox21");
  GLADE_HOOKUP_OBJECT (window1, hbox34, "hbox34");
  GLADE_HOOKUP_OBJECT (window1, label86, "label86");
  GLADE_HOOKUP_OBJECT (window1, entry58, "entry58");
  GLADE_HOOKUP_OBJECT (window1, optionmenu5, "optionmenu5");
  GLADE_HOOKUP_OBJECT (window1, convertwidget40, "convertwidget40");
  GLADE_HOOKUP_OBJECT (window1, convertwidget41, "convertwidget41");
  GLADE_HOOKUP_OBJECT (window1, convertwidget42, "convertwidget42");
  GLADE_HOOKUP_OBJECT (window1, convertwidget43, "convertwidget43");
  GLADE_HOOKUP_OBJECT (window1, convertwidget44, "convertwidget44");
  GLADE_HOOKUP_OBJECT (window1, convertwidget45, "convertwidget45");
  GLADE_HOOKUP_OBJECT (window1, convertwidget46, "convertwidget46");
  GLADE_HOOKUP_OBJECT (window1, convertwidget47, "convertwidget47");
  GLADE_HOOKUP_OBJECT (window1, convertwidget48, "convertwidget48");
  GLADE_HOOKUP_OBJECT (window1, convertwidget49, "convertwidget49");
  GLADE_HOOKUP_OBJECT (window1, convertwidget50, "convertwidget50");
  GLADE_HOOKUP_OBJECT (window1, convertwidget51, "convertwidget51");
  GLADE_HOOKUP_OBJECT (window1, convertwidget52, "convertwidget52");
  GLADE_HOOKUP_OBJECT (window1, convertwidget53, "convertwidget53");
  GLADE_HOOKUP_OBJECT (window1, convertwidget54, "convertwidget54");
  GLADE_HOOKUP_OBJECT (window1, convertwidget55, "convertwidget55");
  GLADE_HOOKUP_OBJECT (window1, convertwidget56, "convertwidget56");
  GLADE_HOOKUP_OBJECT (window1, convertwidget57, "convertwidget57");
  GLADE_HOOKUP_OBJECT (window1, hbox35, "hbox35");
  GLADE_HOOKUP_OBJECT (window1, label87, "label87");
  GLADE_HOOKUP_OBJECT (window1, entry59, "entry59");
  GLADE_HOOKUP_OBJECT (window1, checkbutton15, "checkbutton15");
  GLADE_HOOKUP_OBJECT (window1, label88, "label88");
  GLADE_HOOKUP_OBJECT (window1, entry60, "entry60");
  GLADE_HOOKUP_OBJECT (window1, hbox37, "hbox37");
  GLADE_HOOKUP_OBJECT (window1, checkbutton24, "checkbutton24");
  GLADE_HOOKUP_OBJECT (window1, entry61, "entry61");
  GLADE_HOOKUP_OBJECT (window1, label89_2, "label89_2");
  GLADE_HOOKUP_OBJECT (window1, vbox25, "vbox25");
  GLADE_HOOKUP_OBJECT (window1, hbox43, "hbox43");
  GLADE_HOOKUP_OBJECT (window1, label112, "label112");
  GLADE_HOOKUP_OBJECT (window1, entry70, "entry70");
  GLADE_HOOKUP_OBJECT (window1, hbox44, "hbox44");
  GLADE_HOOKUP_OBJECT (window1, label113, "label113");
  GLADE_HOOKUP_OBJECT (window1, entry71, "entry71");
  GLADE_HOOKUP_OBJECT (window1, checkbutton18, "checkbutton18");
  GLADE_HOOKUP_OBJECT (window1, label114, "label114");
  GLADE_HOOKUP_OBJECT (window1, entry72, "entry72");
  GLADE_HOOKUP_OBJECT (window1, hbox46, "hbox46");
  GLADE_HOOKUP_OBJECT (window1, label115, "label115");
  GLADE_HOOKUP_OBJECT (window1, entry73, "entry73");
  GLADE_HOOKUP_OBJECT (window1, button31, "button31");
  GLADE_HOOKUP_OBJECT (window1, label90, "label90");
  GLADE_HOOKUP_OBJECT (window1, label121, "label121");
  GLADE_HOOKUP_OBJECT (window1, label91, "label91");
  GLADE_HOOKUP_OBJECT (window1, vbox30, "vbox30");
  GLADE_HOOKUP_OBJECT (window1, hbox53, "hbox53");
  GLADE_HOOKUP_OBJECT (window1, label133, "label133");
  GLADE_HOOKUP_OBJECT (window1, entry74, "entry74");
  GLADE_HOOKUP_OBJECT (window1, label135, "label135");
  GLADE_HOOKUP_OBJECT (window1, entry77, "entry77");
  GLADE_HOOKUP_OBJECT (window1, checkbutton20, "checkbutton20");
  GLADE_HOOKUP_OBJECT (window1, hbox54, "hbox54");
  GLADE_HOOKUP_OBJECT (window1, label134, "label134");
  GLADE_HOOKUP_OBJECT (window1, entry75, "entry75");
  GLADE_HOOKUP_OBJECT (window1, label136, "label136");
  GLADE_HOOKUP_OBJECT (window1, entry78, "entry78");
  GLADE_HOOKUP_OBJECT (window1, hbox55, "hbox55");
  GLADE_HOOKUP_OBJECT (window1, checkbutton19, "checkbutton19");
  GLADE_HOOKUP_OBJECT (window1, entry76, "entry76");
  GLADE_HOOKUP_OBJECT (window1, label92, "label92");
  GLADE_HOOKUP_OBJECT (window1, label123, "label123");
  GLADE_HOOKUP_OBJECT (window1, label93, "label93");
  GLADE_HOOKUP_OBJECT (window1, label124, "label124");
  GLADE_HOOKUP_OBJECT (window1, label116, "label116");
  GLADE_HOOKUP_OBJECT (window1, label125, "label125");
  GLADE_HOOKUP_OBJECT (window1, label94, "label94");
  GLADE_HOOKUP_OBJECT (window1, label126, "label126");
  GLADE_HOOKUP_OBJECT (window1, label95, "label95");
  GLADE_HOOKUP_OBJECT (window1, label127, "label127");
  GLADE_HOOKUP_OBJECT (window1, label96, "label96");
  GLADE_HOOKUP_OBJECT (window1, label128, "label128");
  GLADE_HOOKUP_OBJECT (window1, label97, "label97");
  GLADE_HOOKUP_OBJECT (window1, label129, "label129");
  GLADE_HOOKUP_OBJECT (window1, label98, "label98");
  GLADE_HOOKUP_OBJECT (window1, label130, "label130");
  GLADE_HOOKUP_OBJECT (window1, label99, "label99");
  GLADE_HOOKUP_OBJECT (window1, label131, "label131");
  GLADE_HOOKUP_OBJECT (window1, label100, "label100");
  GLADE_HOOKUP_OBJECT (window1, label132, "label132");
  GLADE_HOOKUP_OBJECT (window1, label101, "label101");
  GLADE_HOOKUP_OBJECT (window1, label159, "label159");
  GLADE_HOOKUP_OBJECT (window1, label158, "label158");
  GLADE_HOOKUP_OBJECT (window1, neki7, "neki7");
  GLADE_HOOKUP_OBJECT (window1, label67, "label67");
  GLADE_HOOKUP_OBJECT (window1, frame11, "frame11");
  GLADE_HOOKUP_OBJECT (window1, vbox58, "vbox58");
  GLADE_HOOKUP_OBJECT (window1, hbox62, "hbox62");
  GLADE_HOOKUP_OBJECT (window1, frame20, "frame20");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow6, "scrolledwindow6");
  GLADE_HOOKUP_OBJECT (window1, text2, "text2");
  GLADE_HOOKUP_OBJECT (window1, vbox32, "vbox32");
  GLADE_HOOKUP_OBJECT (window1, frame21, "frame21");
  GLADE_HOOKUP_OBJECT (window1, vbox33, "vbox33");
  GLADE_HOOKUP_OBJECT (window1, hbox63, "hbox63");
  GLADE_HOOKUP_OBJECT (window1, label156, "label156");
  GLADE_HOOKUP_OBJECT (window1, entry82, "entry82");
  GLADE_HOOKUP_OBJECT (window1, hbox64, "hbox64");
  GLADE_HOOKUP_OBJECT (window1, label157, "label157");
  GLADE_HOOKUP_OBJECT (window1, entry83, "entry83");
  GLADE_HOOKUP_OBJECT (window1, button39, "button39");
  GLADE_HOOKUP_OBJECT (window1, button38, "button38");
  GLADE_HOOKUP_OBJECT (window1, napis_pod_ip_payload, "napis_pod_ip_payload");
  GLADE_HOOKUP_OBJECT (window1, neki8, "neki8");
  GLADE_HOOKUP_OBJECT (window1, label68, "label68");
  GLADE_HOOKUP_OBJECT (window1, frame45, "frame45");
  GLADE_HOOKUP_OBJECT (window1, vbox69, "vbox69");
  GLADE_HOOKUP_OBJECT (window1, hbox122, "hbox122");
  GLADE_HOOKUP_OBJECT (window1, label305, "label305");
  GLADE_HOOKUP_OBJECT (window1, entry166, "entry166");
  GLADE_HOOKUP_OBJECT (window1, optionmenu20, "optionmenu20");
  GLADE_HOOKUP_OBJECT (window1, convertwidget60, "convertwidget60");
  GLADE_HOOKUP_OBJECT (window1, convertwidget61, "convertwidget61");
  GLADE_HOOKUP_OBJECT (window1, convertwidget62, "convertwidget62");
  GLADE_HOOKUP_OBJECT (window1, convertwidget63, "convertwidget63");
  GLADE_HOOKUP_OBJECT (window1, convertwidget64, "convertwidget64");
  GLADE_HOOKUP_OBJECT (window1, convertwidget65, "convertwidget65");
  GLADE_HOOKUP_OBJECT (window1, convertwidget66, "convertwidget66");
  GLADE_HOOKUP_OBJECT (window1, convertwidget67, "convertwidget67");
  GLADE_HOOKUP_OBJECT (window1, hbox124, "hbox124");
  GLADE_HOOKUP_OBJECT (window1, label306, "label306");
  GLADE_HOOKUP_OBJECT (window1, entry167, "entry167");
  GLADE_HOOKUP_OBJECT (window1, label307, "label307");
  GLADE_HOOKUP_OBJECT (window1, entry168, "entry168");
  GLADE_HOOKUP_OBJECT (window1, checkbutton41, "checkbutton41");
  GLADE_HOOKUP_OBJECT (window1, notebook8, "notebook8");
  GLADE_HOOKUP_OBJECT (window1, vbox71, "vbox71");
  GLADE_HOOKUP_OBJECT (window1, hbox130, "hbox130");
  GLADE_HOOKUP_OBJECT (window1, label315, "label315");
  GLADE_HOOKUP_OBJECT (window1, entry175, "entry175");
  GLADE_HOOKUP_OBJECT (window1, label309, "label309");
  GLADE_HOOKUP_OBJECT (window1, vbox70, "vbox70");
  GLADE_HOOKUP_OBJECT (window1, hbox125, "hbox125");
  GLADE_HOOKUP_OBJECT (window1, label308, "label308");
  GLADE_HOOKUP_OBJECT (window1, entry169, "entry169");
  GLADE_HOOKUP_OBJECT (window1, hbox127, "hbox127");
  GLADE_HOOKUP_OBJECT (window1, label311, "label311");
  GLADE_HOOKUP_OBJECT (window1, entry171, "entry171");
  GLADE_HOOKUP_OBJECT (window1, label312, "label312");
  GLADE_HOOKUP_OBJECT (window1, entry172, "entry172");
  GLADE_HOOKUP_OBJECT (window1, hbox128, "hbox128");
  GLADE_HOOKUP_OBJECT (window1, label313, "label313");
  GLADE_HOOKUP_OBJECT (window1, entry173, "entry173");
  GLADE_HOOKUP_OBJECT (window1, label310, "label310");
  GLADE_HOOKUP_OBJECT (window1, vbox72, "vbox72");
  GLADE_HOOKUP_OBJECT (window1, hbox131, "hbox131");
  GLADE_HOOKUP_OBJECT (window1, label316, "label316");
  GLADE_HOOKUP_OBJECT (window1, entry176, "entry176");
  GLADE_HOOKUP_OBJECT (window1, label317, "label317");
  GLADE_HOOKUP_OBJECT (window1, entry177, "entry177");
  GLADE_HOOKUP_OBJECT (window1, hbox132, "hbox132");
  GLADE_HOOKUP_OBJECT (window1, label318, "label318");
  GLADE_HOOKUP_OBJECT (window1, entry178, "entry178");
  GLADE_HOOKUP_OBJECT (window1, label314, "label314");
  GLADE_HOOKUP_OBJECT (window1, frame461, "frame461");
  GLADE_HOOKUP_OBJECT (window1, hbox133, "hbox133");
  GLADE_HOOKUP_OBJECT (window1, label320, "label320");
  GLADE_HOOKUP_OBJECT (window1, frame46, "frame46");
  GLADE_HOOKUP_OBJECT (window1, button81, "button81");
  GLADE_HOOKUP_OBJECT (window1, neki0, "neki0");
  GLADE_HOOKUP_OBJECT (window1, label69, "label69");
  GLADE_HOOKUP_OBJECT (window1, label376, "label376");
  GLADE_HOOKUP_OBJECT (window1, label319, "label319");
  GLADE_HOOKUP_OBJECT (window1, label1, "label1");
  GLADE_HOOKUP_OBJECT (window1, vbox59, "vbox59");
  GLADE_HOOKUP_OBJECT (window1, frame34, "frame34");
  GLADE_HOOKUP_OBJECT (window1, label241, "label241");
  GLADE_HOOKUP_OBJECT (window1, vertbox60, "vertbox60");
  GLADE_HOOKUP_OBJECT (window1, hbox97, "hbox97");
  GLADE_HOOKUP_OBJECT (window1, frame36, "frame36");
  GLADE_HOOKUP_OBJECT (window1, vbox60, "vbox60");
  GLADE_HOOKUP_OBJECT (window1, label243, "label243");
  GLADE_HOOKUP_OBJECT (window1, hbox98, "hbox98");
  GLADE_HOOKUP_OBJECT (window1, entry109, "entry109");
  GLADE_HOOKUP_OBJECT (window1, checkbutton35, "checkbutton35");
  GLADE_HOOKUP_OBJECT (window1, label244, "label244");
  GLADE_HOOKUP_OBJECT (window1, hbox99, "hbox99");
  GLADE_HOOKUP_OBJECT (window1, entry110, "entry110");
  GLADE_HOOKUP_OBJECT (window1, checkbutton37, "checkbutton37");
  GLADE_HOOKUP_OBJECT (window1, label287, "label287");
  GLADE_HOOKUP_OBJECT (window1, hbox114, "hbox114");
  GLADE_HOOKUP_OBJECT (window1, radiobutton34, "radiobutton34");
  GLADE_HOOKUP_OBJECT (window1, radiobutton35, "radiobutton35");
  GLADE_HOOKUP_OBJECT (window1, frame37, "frame37");
  GLADE_HOOKUP_OBJECT (window1, table3, "table3");
  GLADE_HOOKUP_OBJECT (window1, notebook7, "notebook7");
  GLADE_HOOKUP_OBJECT (window1, optionmenu9, "optionmenu9");
  GLADE_HOOKUP_OBJECT (window1, convertwidget68, "convertwidget68");
  GLADE_HOOKUP_OBJECT (window1, convertwidget69, "convertwidget69");
  GLADE_HOOKUP_OBJECT (window1, convertwidget70, "convertwidget70");
  GLADE_HOOKUP_OBJECT (window1, convertwidget71, "convertwidget71");
  GLADE_HOOKUP_OBJECT (window1, convertwidget72, "convertwidget72");
  GLADE_HOOKUP_OBJECT (window1, label290, "label290");
  GLADE_HOOKUP_OBJECT (window1, optionmenu16, "optionmenu16");
  GLADE_HOOKUP_OBJECT (window1, convertwidget73, "convertwidget73");
  GLADE_HOOKUP_OBJECT (window1, convertwidget74, "convertwidget74");
  GLADE_HOOKUP_OBJECT (window1, convertwidget75, "convertwidget75");
  GLADE_HOOKUP_OBJECT (window1, convertwidget76, "convertwidget76");
  GLADE_HOOKUP_OBJECT (window1, convertwidget77, "convertwidget77");
  GLADE_HOOKUP_OBJECT (window1, convertwidget78, "convertwidget78");
  GLADE_HOOKUP_OBJECT (window1, convertwidget79, "convertwidget79");
  GLADE_HOOKUP_OBJECT (window1, label291, "label291");
  GLADE_HOOKUP_OBJECT (window1, optionmenu17, "optionmenu17");
  GLADE_HOOKUP_OBJECT (window1, convertwidget80, "convertwidget80");
  GLADE_HOOKUP_OBJECT (window1, convertwidget81, "convertwidget81");
  GLADE_HOOKUP_OBJECT (window1, convertwidget82, "convertwidget82");
  GLADE_HOOKUP_OBJECT (window1, convertwidget83, "convertwidget83");
  GLADE_HOOKUP_OBJECT (window1, convertwidget84, "convertwidget84");
  GLADE_HOOKUP_OBJECT (window1, convertwidget85, "convertwidget85");
  GLADE_HOOKUP_OBJECT (window1, label292, "label292");
  GLADE_HOOKUP_OBJECT (window1, optionmenu18, "optionmenu18");
  GLADE_HOOKUP_OBJECT (window1, convertwidget86, "convertwidget86");
  GLADE_HOOKUP_OBJECT (window1, convertwidget87, "convertwidget87");
  GLADE_HOOKUP_OBJECT (window1, convertwidget88, "convertwidget88");
  GLADE_HOOKUP_OBJECT (window1, convertwidget89, "convertwidget89");
  GLADE_HOOKUP_OBJECT (window1, convertwidget90, "convertwidget90");
  GLADE_HOOKUP_OBJECT (window1, convertwidget91, "convertwidget91");
  GLADE_HOOKUP_OBJECT (window1, convertwidget92, "convertwidget92");
  GLADE_HOOKUP_OBJECT (window1, convertwidget93, "convertwidget93");
  GLADE_HOOKUP_OBJECT (window1, label293, "label293");
  GLADE_HOOKUP_OBJECT (window1, optionmenu19, "optionmenu19");
  GLADE_HOOKUP_OBJECT (window1, convertwidget94, "convertwidget94");
  GLADE_HOOKUP_OBJECT (window1, convertwidget95, "convertwidget95");
  GLADE_HOOKUP_OBJECT (window1, convertwidget96, "convertwidget96");
  GLADE_HOOKUP_OBJECT (window1, convertwidget97, "convertwidget97");
  GLADE_HOOKUP_OBJECT (window1, convertwidget98, "convertwidget98");
  GLADE_HOOKUP_OBJECT (window1, convertwidget99, "convertwidget99");
  GLADE_HOOKUP_OBJECT (window1, convertwidget100, "convertwidget100");
  GLADE_HOOKUP_OBJECT (window1, convertwidget101, "convertwidget101");
  GLADE_HOOKUP_OBJECT (window1, convertwidget102, "convertwidget102");
  GLADE_HOOKUP_OBJECT (window1, convertwidget103, "convertwidget103");
  GLADE_HOOKUP_OBJECT (window1, convertwidget104, "convertwidget104");
  GLADE_HOOKUP_OBJECT (window1, label294, "label294");
  GLADE_HOOKUP_OBJECT (window1, hbox117, "hbox117");
  GLADE_HOOKUP_OBJECT (window1, label295, "label295");
  GLADE_HOOKUP_OBJECT (window1, entry160, "entry160");
  GLADE_HOOKUP_OBJECT (window1, label296, "label296");
  GLADE_HOOKUP_OBJECT (window1, optionmenu14, "optionmenu14");
  GLADE_HOOKUP_OBJECT (window1, convertwidget105, "convertwidget105");
  GLADE_HOOKUP_OBJECT (window1, convertwidget106, "convertwidget106");
  GLADE_HOOKUP_OBJECT (window1, convertwidget107, "convertwidget107");
  GLADE_HOOKUP_OBJECT (window1, convertwidget108, "convertwidget108");
  GLADE_HOOKUP_OBJECT (window1, random_2_bytes, "random_2_bytes");
  GLADE_HOOKUP_OBJECT (window1, random_3_bytes, "random_3_bytes");
  GLADE_HOOKUP_OBJECT (window1, random_4_bytes, "random_4_bytes");
  GLADE_HOOKUP_OBJECT (window1, label297, "label297");
  GLADE_HOOKUP_OBJECT (window1, entry161, "entry161");
  GLADE_HOOKUP_OBJECT (window1, label358, "label358");
  GLADE_HOOKUP_OBJECT (window1, hbox118, "hbox118");
  GLADE_HOOKUP_OBJECT (window1, label298, "label298");
  GLADE_HOOKUP_OBJECT (window1, entry162, "entry162");
  GLADE_HOOKUP_OBJECT (window1, label299, "label299");
  GLADE_HOOKUP_OBJECT (window1, optionmenu15, "optionmenu15");
  GLADE_HOOKUP_OBJECT (window1, convertwidget109, "convertwidget109");
  GLADE_HOOKUP_OBJECT (window1, convertwidget110, "convertwidget110");
  GLADE_HOOKUP_OBJECT (window1, convertwidget111, "convertwidget111");
  GLADE_HOOKUP_OBJECT (window1, convertwidget112, "convertwidget112");
  GLADE_HOOKUP_OBJECT (window1, random_2_bytes2, "random_2_bytes2");
  GLADE_HOOKUP_OBJECT (window1, random_3_bytes2, "random_3_bytes2");
  GLADE_HOOKUP_OBJECT (window1, random_4_bytes2, "random_4_bytes2");
  GLADE_HOOKUP_OBJECT (window1, label300, "label300");
  GLADE_HOOKUP_OBJECT (window1, entry163, "entry163");
  GLADE_HOOKUP_OBJECT (window1, label359, "label359");
  GLADE_HOOKUP_OBJECT (window1, hbox120, "hbox120");
  GLADE_HOOKUP_OBJECT (window1, hbox121, "hbox121");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow10, "scrolledwindow10");
  GLADE_HOOKUP_OBJECT (window1, text5, "text5");
  GLADE_HOOKUP_OBJECT (window1, vbox68, "vbox68");
  GLADE_HOOKUP_OBJECT (window1, label303, "label303");
  GLADE_HOOKUP_OBJECT (window1, entry164, "entry164");
  GLADE_HOOKUP_OBJECT (window1, label288, "label288");
  GLADE_HOOKUP_OBJECT (window1, label245, "label245");
  GLADE_HOOKUP_OBJECT (window1, label2, "label2");
  GLADE_HOOKUP_OBJECT (window1, vbox62, "vbox62");
  GLADE_HOOKUP_OBJECT (window1, frame39, "frame39");
  GLADE_HOOKUP_OBJECT (window1, label252, "label252");
  GLADE_HOOKUP_OBJECT (window1, frame40, "frame40");
  GLADE_HOOKUP_OBJECT (window1, hbox102, "hbox102");
  GLADE_HOOKUP_OBJECT (window1, label253, "label253");
  GLADE_HOOKUP_OBJECT (window1, radiobutton36, "radiobutton36");
  GLADE_HOOKUP_OBJECT (window1, radiobutton37, "radiobutton37");
  GLADE_HOOKUP_OBJECT (window1, label360, "label360");
  GLADE_HOOKUP_OBJECT (window1, radiobutton61, "radiobutton61");
  GLADE_HOOKUP_OBJECT (window1, radiobutton62, "radiobutton62");
  GLADE_HOOKUP_OBJECT (window1, frame41, "frame41");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow9, "scrolledwindow9");
  GLADE_HOOKUP_OBJECT (window1, viewport2, "viewport2");
  GLADE_HOOKUP_OBJECT (window1, table4, "table4");
  GLADE_HOOKUP_OBJECT (window1, entry122, "entry122");
  GLADE_HOOKUP_OBJECT (window1, entry123, "entry123");
  GLADE_HOOKUP_OBJECT (window1, entry124, "entry124");
  GLADE_HOOKUP_OBJECT (window1, entry125, "entry125");
  GLADE_HOOKUP_OBJECT (window1, entry126, "entry126");
  GLADE_HOOKUP_OBJECT (window1, entry127, "entry127");
  GLADE_HOOKUP_OBJECT (window1, entry128, "entry128");
  GLADE_HOOKUP_OBJECT (window1, entry129, "entry129");
  GLADE_HOOKUP_OBJECT (window1, entry130, "entry130");
  GLADE_HOOKUP_OBJECT (window1, label255, "label255");
  GLADE_HOOKUP_OBJECT (window1, label256, "label256");
  GLADE_HOOKUP_OBJECT (window1, label257, "label257");
  GLADE_HOOKUP_OBJECT (window1, label254, "label254");
  GLADE_HOOKUP_OBJECT (window1, entry131, "entry131");
  GLADE_HOOKUP_OBJECT (window1, entry132, "entry132");
  GLADE_HOOKUP_OBJECT (window1, entry133, "entry133");
  GLADE_HOOKUP_OBJECT (window1, entry134, "entry134");
  GLADE_HOOKUP_OBJECT (window1, entry135, "entry135");
  GLADE_HOOKUP_OBJECT (window1, entry136, "entry136");
  GLADE_HOOKUP_OBJECT (window1, entry137, "entry137");
  GLADE_HOOKUP_OBJECT (window1, entry138, "entry138");
  GLADE_HOOKUP_OBJECT (window1, entry141, "entry141");
  GLADE_HOOKUP_OBJECT (window1, entry142, "entry142");
  GLADE_HOOKUP_OBJECT (window1, entry143, "entry143");
  GLADE_HOOKUP_OBJECT (window1, entry144, "entry144");
  GLADE_HOOKUP_OBJECT (window1, entry145, "entry145");
  GLADE_HOOKUP_OBJECT (window1, entry146, "entry146");
  GLADE_HOOKUP_OBJECT (window1, entry147, "entry147");
  GLADE_HOOKUP_OBJECT (window1, entry148, "entry148");
  GLADE_HOOKUP_OBJECT (window1, entry149, "entry149");
  GLADE_HOOKUP_OBJECT (window1, entry150, "entry150");
  GLADE_HOOKUP_OBJECT (window1, entry112, "entry112");
  GLADE_HOOKUP_OBJECT (window1, entry113, "entry113");
  GLADE_HOOKUP_OBJECT (window1, entry114, "entry114");
  GLADE_HOOKUP_OBJECT (window1, entry115, "entry115");
  GLADE_HOOKUP_OBJECT (window1, entry116, "entry116");
  GLADE_HOOKUP_OBJECT (window1, entry117, "entry117");
  GLADE_HOOKUP_OBJECT (window1, entry118, "entry118");
  GLADE_HOOKUP_OBJECT (window1, entry119, "entry119");
  GLADE_HOOKUP_OBJECT (window1, entry120, "entry120");
  GLADE_HOOKUP_OBJECT (window1, entry139, "entry139");
  GLADE_HOOKUP_OBJECT (window1, entry140, "entry140");
  GLADE_HOOKUP_OBJECT (window1, label268, "label268");
  GLADE_HOOKUP_OBJECT (window1, entry121, "entry121");
  GLADE_HOOKUP_OBJECT (window1, entry151, "entry151");
  GLADE_HOOKUP_OBJECT (window1, checkbutton36, "checkbutton36");
  GLADE_HOOKUP_OBJECT (window1, label271, "label271");
  GLADE_HOOKUP_OBJECT (window1, label270, "label270");
  GLADE_HOOKUP_OBJECT (window1, label272, "label272");
  GLADE_HOOKUP_OBJECT (window1, entry111, "entry111");
  GLADE_HOOKUP_OBJECT (window1, button65, "button65");
  GLADE_HOOKUP_OBJECT (window1, button66, "button66");
  GLADE_HOOKUP_OBJECT (window1, button67, "button67");
  GLADE_HOOKUP_OBJECT (window1, button68, "button68");
  GLADE_HOOKUP_OBJECT (window1, button69, "button69");
  GLADE_HOOKUP_OBJECT (window1, button70, "button70");
  GLADE_HOOKUP_OBJECT (window1, button71, "button71");
  GLADE_HOOKUP_OBJECT (window1, button72, "button72");
  GLADE_HOOKUP_OBJECT (window1, button73, "button73");
  GLADE_HOOKUP_OBJECT (window1, button74, "button74");
  GLADE_HOOKUP_OBJECT (window1, checkbutton25, "checkbutton25");
  GLADE_HOOKUP_OBJECT (window1, checkbutton26, "checkbutton26");
  GLADE_HOOKUP_OBJECT (window1, checkbutton27, "checkbutton27");
  GLADE_HOOKUP_OBJECT (window1, checkbutton28, "checkbutton28");
  GLADE_HOOKUP_OBJECT (window1, checkbutton29, "checkbutton29");
  GLADE_HOOKUP_OBJECT (window1, checkbutton30, "checkbutton30");
  GLADE_HOOKUP_OBJECT (window1, checkbutton31, "checkbutton31");
  GLADE_HOOKUP_OBJECT (window1, checkbutton32, "checkbutton32");
  GLADE_HOOKUP_OBJECT (window1, checkbutton33, "checkbutton33");
  GLADE_HOOKUP_OBJECT (window1, checkbutton34, "checkbutton34");
  GLADE_HOOKUP_OBJECT (window1, label258, "label258");
  GLADE_HOOKUP_OBJECT (window1, label361, "label361");
  GLADE_HOOKUP_OBJECT (window1, entry186, "entry186");
  GLADE_HOOKUP_OBJECT (window1, entry187, "entry187");
  GLADE_HOOKUP_OBJECT (window1, entry188, "entry188");
  GLADE_HOOKUP_OBJECT (window1, entry189, "entry189");
  GLADE_HOOKUP_OBJECT (window1, entry190, "entry190");
  GLADE_HOOKUP_OBJECT (window1, entry191, "entry191");
  GLADE_HOOKUP_OBJECT (window1, entry192, "entry192");
  GLADE_HOOKUP_OBJECT (window1, entry193, "entry193");
  GLADE_HOOKUP_OBJECT (window1, entry194, "entry194");
  GLADE_HOOKUP_OBJECT (window1, entry185, "entry185");
  GLADE_HOOKUP_OBJECT (window1, entry152, "entry152");
  GLADE_HOOKUP_OBJECT (window1, button87, "button87");
  GLADE_HOOKUP_OBJECT (window1, hseparator5, "hseparator5");
  GLADE_HOOKUP_OBJECT (window1, label3, "label3");
  GLADE_HOOKUP_OBJECT (window1, vbox77, "vbox77");
  GLADE_HOOKUP_OBJECT (window1, frame464, "frame464");
  GLADE_HOOKUP_OBJECT (window1, alignment7, "alignment7");
  GLADE_HOOKUP_OBJECT (window1, vbox78, "vbox78");
  GLADE_HOOKUP_OBJECT (window1, hbox145, "hbox145");
  GLADE_HOOKUP_OBJECT (window1, label351, "label351");
  GLADE_HOOKUP_OBJECT (window1, checkbutton42, "checkbutton42");
  GLADE_HOOKUP_OBJECT (window1, entry183, "entry183");
  GLADE_HOOKUP_OBJECT (window1, label352, "label352");
  GLADE_HOOKUP_OBJECT (window1, entry184, "entry184");
  GLADE_HOOKUP_OBJECT (window1, hbox144, "hbox144");
  GLADE_HOOKUP_OBJECT (window1, label350, "label350");
  GLADE_HOOKUP_OBJECT (window1, radiobutton59, "radiobutton59");
  GLADE_HOOKUP_OBJECT (window1, radiobutton60, "radiobutton60");
  GLADE_HOOKUP_OBJECT (window1, label349, "label349");
  GLADE_HOOKUP_OBJECT (window1, frame462, "frame462");
  GLADE_HOOKUP_OBJECT (window1, alignment5, "alignment5");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow12, "scrolledwindow12");
  GLADE_HOOKUP_OBJECT (window1, clist2, "clist2");
  GLADE_HOOKUP_OBJECT (window1, label334, "label334");
  GLADE_HOOKUP_OBJECT (window1, label335, "label335");
  GLADE_HOOKUP_OBJECT (window1, label336, "label336");
  GLADE_HOOKUP_OBJECT (window1, label337, "label337");
  GLADE_HOOKUP_OBJECT (window1, label338, "label338");
  GLADE_HOOKUP_OBJECT (window1, label339, "label339");
  GLADE_HOOKUP_OBJECT (window1, label341, "label341");
  GLADE_HOOKUP_OBJECT (window1, label342, "label342");
  GLADE_HOOKUP_OBJECT (window1, label333, "label333");
  GLADE_HOOKUP_OBJECT (window1, button86, "button86");
  GLADE_HOOKUP_OBJECT (window1, label251, "label251");
  GLADE_HOOKUP_OBJECT (window1, statusbar1, "statusbar1");
{
GtkWidget *option_menu1;
GtkWidget *option_menu3;
GtkWidget *option_menu4;
GtkWidget *option_menu5;
GtkWidget *option_menu6;
GtkWidget *option_menu9;
GtkWidget *option_menu14;
GtkWidget *option_menu15;
GtkWidget *option_menu16;
GtkWidget *option_menu17;
GtkWidget *option_menu18;
GtkWidget *option_menu19;
GtkWidget *option_menu20;
option_menu1 = lookup_widget (window1, "L_optmenu1_bt");
option_menu3 = lookup_widget (window1, "optionmenu3");
option_menu4 = lookup_widget (window1, "optionmenu4");
option_menu5 = lookup_widget (window1, "optionmenu5");
option_menu6 = lookup_widget (window1, "optionmenu6");
option_menu9 = lookup_widget (window1, "optionmenu9");
option_menu14 = lookup_widget (window1, "optionmenu14");
option_menu15 = lookup_widget (window1, "optionmenu15");
option_menu16 = lookup_widget (window1, "optionmenu16");
option_menu17 = lookup_widget (window1, "optionmenu17");
option_menu18 = lookup_widget (window1, "optionmenu18");
option_menu19 = lookup_widget (window1, "optionmenu19");
option_menu20 = lookup_widget (window1, "optionmenu20");

gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu1)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_L_optmenu1_bt_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu3)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu3_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu4)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu4_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu5)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu5_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu6)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu6_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu9)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu9_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu14)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu14_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu15)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu15_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu16)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu16_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu17)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu17_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu18)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu18_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu19)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_optionmenu19_clicked), NULL);
gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (option_menu20)->menu),
	      "deactivate", GTK_SIGNAL_FUNC (on_igmpmessage_type_clicked), NULL);
}

  return window1;
}