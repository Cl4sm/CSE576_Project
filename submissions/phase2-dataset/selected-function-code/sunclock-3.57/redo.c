void redo()
{
		/* dont redo nothing */
		if(undop == undosp) { bell(); return; }
		in_undo++;
		goto_ptr(undop->pos + edbuf);

		if(undop->length > 0) { /* is insert so insert */
				file_resize(undop->pos + edbuf, undop->pos + edbuf + undop->length);
				memcpy(undop->pos + edbuf, undop->str, undop->length);
				u_ins(undop->pos + edbuf, undop->length);
				ytot_ins(); 	/* adjust ytot when inserting */
		}
		else {			/* is delete< so delete */
				file_resize(undop->pos + edbuf - undop->length, undop->pos + edbuf);
		}
		goto_ptr(undop->pos + edbuf);

		undop = (void*)undop->str + abs(undosp->length);
		undone = 0;
		in_undo--;
		flag[SHW] = 1;
}
