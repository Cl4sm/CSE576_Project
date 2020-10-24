void parse_processes(void)
{
	DIR *dir;
	struct dirent *dirent;
	char filename[PATH_MAX];
	struct process *process;

	pidmax = 0;

	dir = opendir("/proc");
	if (!dir)
		return;
	while ((dirent = readdir(dir))) {
		FILE *file;
		char *line;
		size_t dummy;
		int pid;
		if (dirent->d_name[0]=='.')
			continue;
		pid = strtoull(dirent->d_name, NULL, 10);
		if (pid<=0) /* not a process */
			continue;

		process = find_create_process(pid);
		process->exists = 1;

		sprintf(filename, "/proc/%i/status", pid);
		file = fopen(filename, "r");
		if (file) {
			char *q;
			line = NULL;
			if (getline(&line, &dummy, file) >= 0) {
				strncpy(&process->name[0], &line[6], 64);
				q = strchr(process->name, '\n');
				if (q) *q=0;
				fclose(file);
			}
			free(line);
			line = NULL;
		}

		if (process->name && prefered_process && strcmp(process->name, prefered_process)==0) {
			pid_with_max = pid;
			pidmax = INT_MAX;
		}

		sprintf(filename, "/proc/%i/sched", pid);
		file = fopen(filename, "r+");
		if (file) {
			char *q;
			double d;
			while (!feof(file)) {
				line = NULL;
				if (getline(&line, &dummy, file) < 0) {
					free(line);
					break;
				}
				q = strchr(line, ':');
				if (strstr(line, "se.wait_max") && q) {
					sscanf(q+1,"%lf", &d);
					process->maxdelay = d;
				}
				if (strstr(line, "se.wait_sum") && q) {
					sscanf(q+1,"%lf", &d);
					process->totaldelay = d;
				}
				if (strstr(line, "se.wait_count") && q) {
					sscanf(q+1,"%lf", &d);
					process->delaycount = d;
				}
				free(line);
				line = NULL;
			}
			fprintf(file,"erase");
			fclose(file);
		}

		sprintf(filename, "/proc/%i/statm", pid);
		file = fopen(filename, "r");
		if (file) {
			line = NULL;
			if (getline(&line, &dummy, file) >= 0) {
				if (strcmp(line, "0 0 0 0 0 0 0\n")==0)
					process->kernelthread = 1;
			}
			fclose(file);
			free(line);
			line = NULL;
		}

		parse_process(process);

		if (process->latencies) {
			process->latencies = g_list_sort(process->latencies, comparef);
			if (!process->kernelthread && pidmax < process->max) {	
				pidmax = process->max;
				pid_with_max = process->pid;
			}
			procs = g_list_append(procs, process);
		};
	}
	closedir(dir);
}
