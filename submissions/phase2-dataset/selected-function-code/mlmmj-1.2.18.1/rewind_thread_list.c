static void rewind_thread_list(void * state)
{
	thread_list_state *s = (thread_list_state *)state;
	int i, j, archivefd, thread_idx;
	char *line, *tmp, *subj, *from;
	char *archivename;
	int num_threads = 0;
	struct thread *threads = NULL;
	char buf[45];

	if (s->cur_thread != -1) {
		/* We have gathered the data already; just rewind */
		s->cur_thread = 0;
		s->cur_mail = -1;
		return;
	}

	for (i=s->firstindex; i<=s->lastindex; i++) {

		snprintf(buf, sizeof(buf), "%d", i);

		archivename = concatstr(3, s->listdir, "/archive/", buf);
		archivefd = open(archivename, O_RDONLY);
		myfree(archivename);

		if (archivefd < 0)
			continue;

		subj = NULL;
		from = NULL;

		while ((line = gethdrline(archivefd))) {
			if (strcmp(line, "\n") == 0) {
				myfree(line);
				break;
			}
			if (strncasecmp(line, "Subject: ", 9) == 0) {
				myfree(subj);
				subj = unistr_header_to_utf8(line + 9);
			}
			if (strncasecmp(line, "From: ", 6) == 0) {
				myfree(from);
				from = unistr_header_to_utf8(line + 6);
			}
			myfree(line);
		}

		if (!subj) {
			subj = mystrdup("no subject");
		}

		if (!from) {
			from = mystrdup("anonymous");
		}

		tmp = subj;
		for (;;) {
			if (isspace(*tmp)) {
				tmp++;
				continue;
			}
			if (strncasecmp(tmp, "Re:", 3) == 0) {
				tmp += 3;
				continue;
			}
			break;
		}
		/* tmp is now the clean subject */

		thread_idx = -1;
		for (j=0; j<num_threads; j++) {
			if (strcmp(subj, threads[j].subject) == 0) {
				thread_idx = j;
				break;
			}
		}
		if (thread_idx == -1) {
			num_threads++;
			threads = myrealloc(threads,
					num_threads*sizeof(struct thread));
			threads[num_threads-1].subject = concatstr(2,tmp,"\n");
			threads[num_threads-1].num_mails = 0;
			threads[num_threads-1].mails = NULL;
			thread_idx = num_threads-1;
		}

		threads[thread_idx].num_mails++;
		threads[thread_idx].mails = myrealloc(threads[thread_idx].mails,
				threads[thread_idx].num_mails*sizeof(struct mail));
		threads[thread_idx].mails[threads[thread_idx].num_mails-1].idx = i;
		threads[thread_idx].mails[threads[thread_idx].num_mails-1].from =
				concatstr(5, "      ", buf, " - ", from, "\n");

		myfree(subj);
		myfree(from);

		close(archivefd);
	}

	s->num_threads = num_threads;
	s->threads = threads;
	s->cur_thread = 0;
	s->cur_mail = -1;
}