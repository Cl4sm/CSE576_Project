extern int
is_loop_device (const char *device)
{
	struct stat statbuf;
	int loopmajor;
#if 1
	loopmajor = 7;
#else
	FILE *procdev;
	char line[100], *cp;

	loopmajor = 0;

	if ((procdev = fopen(PROC_DEVICES, "r")) != NULL) {
		
		while (fgets (line, sizeof(line), procdev)) {
			
			if ((cp = strstr (line, " loop\n")) != NULL) {
				*cp='\0';
				loopmajor=atoi(line);
				break;
			}
		}

		fclose(procdev);
	}
#endif
	return (loopmajor && stat(device, &statbuf) == 0 &&
		S_ISBLK(statbuf.st_mode) &&
		major(statbuf.st_rdev) == loopmajor);
}
