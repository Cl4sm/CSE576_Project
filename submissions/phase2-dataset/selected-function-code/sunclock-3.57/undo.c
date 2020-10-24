void undo()
{
		/* dont undo nothing, but say nothing is unchanged */
		if(undone) { bell(); return; }
		if(undop != undosp) {
			if (undop->link == VOID_LINK)
				undop = unbuf;
			else
				undop = undop->link + unbuf;
		}
		in_undo++;
		goto_ptr(undop->pos + edbuf);
		if(undop->length < 0) { /* is delete so insert */
				/* delete flag is negative length */
				file_resize(undop->pos + edbuf, undop->pos + edbuf - undop->length);
				memcpy(undop->pos + edbuf, undop->str, - undop->length);
				u_ins(undop->pos + edbuf, - undop->length);
				ytot_ins(); 	/* adjust ytot when inserting */
		}
		else {			/* is insert so delete */
				file_resize(undop->pos + edbuf + undop->length, undop->pos + edbuf);
		}
		goto_ptr(undop->pos + edbuf);
		if(undop->link==VOID_LINK) {
				undone = -1;
				flag[CHG] = 0;
				show_top();
		}
		in_undo--;
		flag[SHW] = 1;
}
