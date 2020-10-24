static int
pvmclaimo(mid)
	int mid;
{
	int tid;					/* task */
	int n;						/* length or event code */
	char *p;
	int ptid;
	struct tobuf *op;

/*
	if (TRACE) {
		checktrace(jp, cc, len, cod, src);
		return 0;
	}
*/

	pvm_upkint(&tid, 1, 1);
	pvm_upkint(&n, 1, 1);

	if (!tobuflist) {
		pvmlogprintf("pvmclaimo() message from task t%x\n", tid);
		goto done;
	}

	if (n > 0) {	/* output printed by task */
		if (!(op = tobuf_find(tid))) {
			pvmlogprintf("pvmclaimo() bogus message, no task t%x\n", tid);
			goto done;
		}
		if (n + op->o_len >= op->o_maxl) {
			op->o_maxl = op->o_len + n + 1;
/*
			printf("REALLOC t%x buf to %d\n", tid, op->o_maxl);
*/
			if (op->o_buf)
				op->o_buf = TREALLOC(op->o_buf, op->o_maxl, char);
			else
				op->o_buf = TALLOC(op->o_maxl, char, "");
		}
		pvm_upkbyte(op->o_buf + op->o_len, n, 1);
		op->o_buf[op->o_len + n] = 0;
/*
		printf("UNPACK t%x {%s}\n", tid, op->o_buf + op->o_len);
*/
		p = op->o_buf + op->o_len;
		op->o_len += n;
/* XXX would be more efficient to remove all lines before copying remainder */
		while (p = CINDEX(p, '\n')) {
			*p++ = 0;
			if (outlogff)
				if (pvmshowtaskid)
					fprintf(outlogff, "[t%x] %s\n", tid, op->o_buf);
				else
					fprintf(outlogff, "%s\n", op->o_buf);
			op->o_len -= p - op->o_buf;
			BCOPY(p, op->o_buf, op->o_len + 1);
			p = op->o_buf;
		}

	} else {
		switch (n) {

		case TO_EOF:	/* EOF from task */
			if (!(op = tobuf_find(tid))) {
				pvmlogprintf("pvmclaimo() bogus message, no task t%x\n", tid);
				goto done;
			}
			if (op->o_len > 0) {
				if (outlogff)
					if (pvmshowtaskid)
						fprintf(outlogff, "[t%x] %s\n", tid, op->o_buf);
					else
						fprintf(outlogff, "%s\n", op->o_buf);
				op->o_len = 0;
			}
			op->o_flag |= GOTEOF;
			if ((op->o_flag & TASKSTATE) == GOTBOTH) {
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] EOF\n", tid);
				tobuf_free(op);
			}
			break;

		case TO_SPAWN:	/* spawn creating new task */
			if (!(op = tobuf_find(tid))) {
				op = tobuf_new(tid);
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] BEGIN\n", tid);
			}
			op->o_flag |= GOTSPAWN;
			if ((op->o_flag & TASKSTATE) == GOTBOTH) {
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] EOF\n", tid);
				tobuf_free(op);
			}

			break;

		case TO_NEW:	/* new task starting up */
			if (!(op = tobuf_find(tid))) {
				op = tobuf_new(tid);
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] BEGIN\n", tid);
			}
			break;

		default:
			pvmlogprintf("pvmclaimo() bogus message from task t%x\n", tid);
			break;
		}
	}

done:
	pvm_freebuf(mid);
	return 0;
}
