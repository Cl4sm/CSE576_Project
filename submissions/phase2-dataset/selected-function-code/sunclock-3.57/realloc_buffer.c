{
	char *ptr;
	int i;
	switch(mode) {

		case 0: {
			i = amax+AMAX+tabsize+1;
			if (i<0) return NULL;
			ptr = realloc(edbuf, (size_t)i);
			if (ptr) {
				amax += AMAX;
				i = ptr - edbuf;
				edbuf += i;
				file_end += i;
				cur_pos += i;
				line_start += i;
				last_pos += i;
				old_pos += i;
				screen_start += i;
				mk += i;
				bstart += i;
				bend += i;
				if (last_mk) last_mk += i;
			}
			return ptr;
		}

		case 1: {
			i = bmax+BMAX+tabsize+1;
			if (i<0) return NULL;
			ptr = realloc(bb, (size_t)i);
			if (ptr) {
				bmax += BMAX;
				bb = ptr;
			}
			return ptr;
		}

		case 2: {
			i = umax+UMAX+1;
			if (i<0) return NULL;
			ptr = realloc(unbuf, (size_t)i);
			if (ptr) {
				umax += UMAX;
				i = (int)ptr - (int)unbuf;
				unbuf = ptr;
				undop = (void*)undop+i;
				undosp = (void*)undosp+i;
			}
			return ptr;
		}
	}
	return NULL;
}
