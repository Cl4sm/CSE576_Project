void write_clip(HWND hwnd, char *data)
{
    HGLOBAL clipdata;
    int len, i, j;
    char *data2;
    void *lock;

    /*
     * Windows expects CRLF in the clipboard, so we must convert
     * any \n that has come out of the puzzle backend.
     */
    len = 0;
    for (i = 0; data[i]; i++) {
	if (data[i] == '\n')
	    len++;
	len++;
    }
    data2 = snewn(len+1, char);
    j = 0;
    for (i = 0; data[i]; i++) {
	if (data[i] == '\n')
	    data2[j++] = '\r';
	data2[j++] = data[i];
    }
    assert(j == len);
    data2[j] = '\0';

    clipdata = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, len + 1);
    if (!clipdata) {
        sfree(data2);
	return;
    }
    lock = GlobalLock(clipdata);
    if (!lock) {
        GlobalFree(clipdata);
        sfree(data2);
	return;
    }
    memcpy(lock, data2, len);
    ((unsigned char *) lock)[len] = 0;
    GlobalUnlock(clipdata);

    if (OpenClipboard(hwnd)) {
	EmptyClipboard();
	SetClipboardData(CF_TEXT, clipdata);
	CloseClipboard();
    } else
	GlobalFree(clipdata);

    sfree(data2);
}