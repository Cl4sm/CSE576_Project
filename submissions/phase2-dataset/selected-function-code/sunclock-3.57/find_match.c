{
	char *s = cur_pos;
	int dire, cnt;
	char ch, mch;

	if (!s) return;

	ch = *s;
	switch (ch) {
	case '(':
		mch = ')'; dire = 0; break;
	case ')':
		mch = '('; dire = 1; break;
	case '[':
		mch = ']'; dire = 0; break;
	case ']':
		mch = '['; dire = 1; break;
	case '{':
		mch = '}'; dire = 0; break;
	case '}':
		mch = '{'; dire = 1; break;
	default:
		return;
	}
	cnt = 0;
	if (dire) {
		while (--s >= edbuf) {
			if (*s == ch) cnt++;
			else if (*s == mch) {
				if (!cnt) goto match;
				cnt--;
			}
		}
	} else {
		while (++s < file_end) {
			if (*s == ch) cnt++;
			else if (*s == mch) {
				if (!cnt) goto match;
				cnt--;
			}
		}
	}
	return;
match:
	goto_ptr(s);
	flag[SHW]=1;
}
