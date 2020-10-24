{
	DIR *dir;
	struct dirent *dirent;
	char filename[PATH_MAX];

	sprintf(filename, "/proc/%i/task/", process->pid);


	dir = opendir(filename);
	if (!dir)
		return;
	while ((dirent = readdir(dir))) {
		FILE *file;
		char *line = NULL;
		size_t dummy;
		int pid;
		if (dirent->d_name[0]=='.')
			continue;
		pid = strtoull(dirent->d_name, NULL, 10);
		if (pid<=0) /* not a process */
			continue;


		sprintf(filename, "/proc/%i/task/%i/latency", process->pid, pid);
		file = fopen(filename, "r+");
		if (!file)
			continue;
		/* wipe first line*/
		if (getline(&line, &dummy, file) < 0) {
			free(line);
			continue;
		}
		free(line);
		while (!feof(file)) {
			struct latency_line *ln;
			char *c, *c2;
			line = NULL;
			if (getline(&line, &dummy, file) < 0) {
				free(line);
				break;
			}
			if (strlen(line)<2) {
				free(line);
				break;
			}
			ln = malloc(sizeof(struct latency_line));
			memset(ln, 0, sizeof(struct latency_line));
			ln->count = strtoull(line, &c, 10);
			ln->time = strtoull(c, &c, 10);
			ln->max = strtoull(c, &c, 10);
			fixup_reason(ln, c);

			if (ln->max > process->max)
				process->max = ln->max;
			add_to_process(process, ln);
			process->used = 1;
			free(line);
			line = NULL;
		}
		/* reset for next time */
		fprintf(file, "erase\n");
		fclose(file);
	}
	/* 100 usec minimum */
	if (!firsttime) {
		struct latency_line *ln, *ln2;
			
		ln = malloc(sizeof(struct latency_line));
		ln2 = malloc(sizeof(struct latency_line));
		memset(ln, 0, sizeof(struct latency_line));
		
		if (process->delaycount)
			ln->count = process->delaycount;
		else
			ln->count = 1;
		if (process->totaldelay > 0.00001)
			ln->time = process->totaldelay * 1000;
		else
			ln->time = process->maxdelay * 1000;    
		ln->max = process->maxdelay * 1000;    
		strcpy(ln->reason, "Scheduler: waiting for cpu");
		if (ln->max > process->max)
			process->max = ln->max;
		memcpy(ln2, ln, sizeof(struct latency_line));
		add_to_process(process, ln);
		add_to_global(ln2);
		process->used = 1;
	}
	closedir(dir);
}
