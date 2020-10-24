static void get_max_puzzle_size(frontend *fe, int *x, int *y)
{
    RECT r, sr;

    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &sr, FALSE)) {
	*x = sr.right - sr.left;
	*y = sr.bottom - sr.top;
	r.left = 100;
	r.right = 200;
	r.top = 100;
	r.bottom = 200;
	AdjustWindowRectEx(&r, WINFLAGS, TRUE, 0);
	*x -= r.right - r.left - 100;
	*y -= r.bottom - r.top - 100;
    } else {
	*x = *y = INT_MAX;
    }

    if (fe->statusbar != NULL) {
	GetWindowRect(fe->statusbar, &sr);
	*y -= sr.bottom - sr.top;
    }
}