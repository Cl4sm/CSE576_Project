int filledPolygonColor (SDL_Surface *dst, Sint16 *vx, Sint16 *vy, int n, int color)
{
	int result;
	int i;
	int y;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	
	/* Sanity check */
	if (n<3) {
	 return -1;
	}
	
	/* Allocate temp array, only grow array */
	if (!gfxPrimitivesPolyAllocated) {
	 gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
	 gfxPrimitivesPolyAllocated = n;
	} else {
	 if (gfxPrimitivesPolyAllocated<n) {
 	  gfxPrimitivesPolyInts = (int *) realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
	  gfxPrimitivesPolyAllocated = n;
	 }
	}		

	/* Determine Y maxima */
	miny = vy[0];
	maxy = vy[0];
	for (i=1; (i < n); i++) {
		if (vy[i] < miny) {
		 miny = vy[i];
		} else if (vy[i] > maxy) {
		 maxy = vy[i];
		}
	}
	
	/* Draw, scanning y */
	result=0;
	for (y=miny; (y <= maxy); y++) {
		ints = 0;
		for (i=0; (i < n); i++) {
			if (!i) {
				ind1 = n-1;
				ind2 = 0;
			} else {
				ind1 = i-1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			} else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			} else {
				continue;
			}
			if ((y >= y1) && (y < y2)) {
				gfxPrimitivesPolyInts[ints++] = (y-y1) * (x2-x1) / (y2-y1) + x1;
			} else if ((y == maxy) && (y > y1) && (y <= y2)) {
				gfxPrimitivesPolyInts[ints++] = (y-y1) * (x2-x1) / (y2-y1) + x1;
			}
		}
		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), gfxPrimitivesCompareInt);

		for (i=0; (i<ints); i+=2) {
			result |= hlineColor(dst, gfxPrimitivesPolyInts[i], gfxPrimitivesPolyInts[i+1], y, color);
		}
	}
	
 return (result);
}