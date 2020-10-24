bubble(n, a)
	int n;			/* length of a */
	int *a;
{
	int r, w, t;

	for (w = r = 0; r < n; r++) {
		if (a[w] < 0) {
			if (a[r] >= 0) {
				t = a[w];
				a[w] = a[r];
				a[r] = t;
				w++;
			}

		} else {
			w++;
		}
	}
	return w;
}
