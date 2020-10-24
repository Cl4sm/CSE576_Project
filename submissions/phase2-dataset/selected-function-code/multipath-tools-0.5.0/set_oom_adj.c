static void
set_oom_adj (void)
{
#ifdef OOM_SCORE_ADJ_MIN
	int retry = 1;
	char *file = "/proc/self/oom_score_adj";
	int score = OOM_SCORE_ADJ_MIN;
#else
	int retry = 0;
	char *file = "/proc/self/oom_adj";
	int score = OOM_ADJUST_MIN;
#endif
	FILE *fp;
	struct stat st;
	char *envp;

	envp = getenv("OOMScoreAdjust");
	if (envp) {
		condlog(3, "Using systemd provided OOMScoreAdjust");
		return;
	}
	do {
		if (stat(file, &st) == 0){
			fp = fopen(file, "w");
			if (!fp) {
				condlog(0, "couldn't fopen %s : %s", file,
					strerror(errno));
				return;
			}
			fprintf(fp, "%i", score);
			fclose(fp);
			return;
		}
		if (errno != ENOENT) {
			condlog(0, "couldn't stat %s : %s", file,
				strerror(errno));
			return;
		}
#ifdef OOM_ADJUST_MIN
		file = "/proc/self/oom_adj";
		score = OOM_ADJUST_MIN;
#else
		retry = 0;
#endif
	} while (retry--);
	condlog(0, "couldn't adjust oom score");
}
