int iet_prio(const char *dev, char * args)
{
	char preferredip_buff[255] = "";
	char *preferredip = &preferredip_buff[0];
	// Phase 1 : checks. If anyone fails, return prio 0.
	// check if args exists
	if (!args) {
		dc_log(0, "need prio_args with preferredip set");
		return 0;
	}
	// check if args format is OK
	if (sscanf(args, "preferredip=%s", preferredip) ==1) {}
	else {
		dc_log(0, "unexpected prio_args format");
		return 0;
	}
	// check if ip is not too short
	if (strlen(preferredip) <= 7) {
		dc_log(0, "prio args: preferredip too short");
		return 0;
	}
	// Phase 2 : find device in /dev/disk/by-path to match device/ip
	DIR           *dir_p;
	struct dirent *dir_entry_p;
	enum { BUFFERSIZE = 1024 };
	char buffer[BUFFERSIZE];
	char fullpath[BUFFERSIZE] = "/dev/disk/by-path/";
	dir_p = opendir(fullpath);

	// loop to find device in /dev/disk/by-path
	while( NULL != (dir_entry_p = readdir(dir_p))) {
		if (dir_entry_p->d_name[0] != '.') {
			char path[BUFFERSIZE] = "/dev/disk/by-path/";
			strcat(path,dir_entry_p->d_name);
			ssize_t nchars = readlink(path, buffer, sizeof(buffer)-1);
			if (nchars != -1) {
				char *device;
				device = find_regex(buffer,"(sd[a-z]+)");
				// if device parsed is the right one
				if (device!=NULL && strncmp(device, dev, strlen(device)) == 0) {
					char *ip;
					ip = find_regex(dir_entry_p->d_name,"([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})");
					// if prefferedip and ip fetched matches
					if (ip!=NULL && strncmp(ip, preferredip, strlen(ip)) == 0) {
						// high prio
						free(ip);
						closedir(dir_p);
						return 20;
					}
					free(ip);
				}
				free(device);
			}
			else {
				printf("error\n");
			}
		}
	}
	// nothing found, low prio
	closedir(dir_p);
	return 10;
}
