	bdaddr_t wiimote_addr;

	if (argc != 3) {
		fprintf(stderr, "Syntax: %s <wiimote-bluetooth-id> <pdf>\n", argv[0]);
		fprintf(stderr, "Put your wiimote in discoverable mode after starting wiipdf\n");
		fprintf(stderr, "(Press button 1 and 2 at the same time)\n");
		return 1;
	}

	if (str2ba(argv[1], &wiimote_addr)) {
		fprintf(stderr, "Invalid bluetooth ID (use hcitool scan)\n");
		return 1;
	}

	pdf_file = argv[2];

	printf("Waiting for wiimote %s...\n", argv[1]);
	if ((wiimote = cwiid_open(&wiimote_addr, 0)) == NULL) {
		fprintf(stderr, "Could not connect to wiimote\n");
		return 1;
	}
	printf("Connection to wiimote established\n");
	/* Set LEDs 2 and 3 to on to signalize a successful link */
	cwiid_set_led(wiimote, CWIID_LED2_ON | CWIID_LED3_ON);

	if (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, CWIID_RPT_STATUS | CWIID_RPT_BTN)) {
		fprintf(stderr, "Could not enable reporting mode\n");
		return 2;
	}

	if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) {
		fprintf(stderr, "Could not enable message mode\n");
		return 3;
	}

	if (cwiid_set_mesg_callback(wiimote, msg_callback)) {
		fprintf(stderr, "Could not set callback\n");
		return 4;
	}

	/* Fortunately, xpdf blocks, so we just run as long as xpdf runs */
	launch_xpdf();

	return 0;
}
