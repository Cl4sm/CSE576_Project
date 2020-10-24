void parse_args(int argc, const char *argv[]) {
  int i;
  while(argc--) {
    if(argv[argc][0] == '-') {
      i = 0;
      while(argv[argc][++i] != 0) {
				switch(argv[argc][i]) {
				case 'x': setSettingi("show_crash_texture", 0); break;
				case 'F': setSettingi("show_fps", 0); break;
				case 't': setSettingi("show_floor_texture", 0); break;
				case 'c': setSettingi("show_ai_status", 0); break;
				case 'g': setSettingi("show_glow", 0); break;
				case 'w': setSettingi("show_wall", 0); break;
				case 'C': setSettingi("show_ai_status", 1); break;
				case 'i': setSettingi("windowMode", 1); break;
				case 'O': setSettingi("softwareRendering", 1); break;
				case '1': /* default is 4 */
					setSettingi("width", 320);
					setSettingi("height", 240);
					break;
				case '2': 
					setSettingi("width", 512);
					setSettingi("height", 384);
					break;
				case '3': 
					setSettingi("width", 640);
					setSettingi("height", 480);
					break;
				case '4': 
					setSettingi("width", 800);
					setSettingi("height", 600);
					break;
				case '5':
					setSettingi("width", 1024);
					setSettingi("height", 768);
					break;
				case '6': 
					setSettingi("width", 1280);
					setSettingi("height", 1024);
					break;
				case '7': 
					setSettingi("width", 1600);
					setSettingi("height", 1200);
					break;
				case 's':
					setSettingi("playMusic", 0);
					setSettingi("playEffects", 0);
					break;
				case 'h':
				default:
					printf("Usage: %s [-FftwbghcCsk1234simo]\n\n", argv[0]);
					printf("Options:\n\n");
					printf("-F\tdon't display FPS counter\n");
					printf("-t\tdon't display floor texture, use lines instead"
								 "(huge speed gain)\n");
					printf("-w\tdon't display walls (speed gain)\n");
					printf("-x\tdon't show crash texture (speed gain)\n");
					printf("-g\tdon't show glows (small speed gain)\n");
					printf("-c\tdon't show ai status\n");
					printf("-C\tshow ai status (default: on)\n");
																				
					printf("-1\tSet resolution to 320x240\n");
					printf("-2\tSet resolution to 512x384\n");
					printf("-3\tSet resolution to 640x480\n");
					printf("-4\tSet resolution to 800x600 (default)\n");
					printf("-5\tSet resolution to 1024x768\n");
					printf("-6\tSet resolution to 1280x1024\n");
					printf("-7\tSet resolution to 1600x1200\n");

					printf("-s\tDon't play sound\n");
					/* printf("-v\tStart in demo/screensaver mode\n"); */
					printf("-i\tforce startup in a window\n");
					printf("-O\tenable software rendering optimization (only if "
								 "you don't have a 3d card!)\n");
					printf("-h\tthis help\n");
					exit(0); /* OK: end of program */
				}
      }
    }
  }
}
