static int setmunge(int x1, int y1, int mask1, int x2, int y2, int mask2,
		    int diff)
{
    /*
     * Adjust the second set so that it has the same x,y
     * coordinates as the first.
     */
    if (abs(x2-x1) >= 3 || abs(y2-y1) >= 3) {
	mask2 = 0;
    } else {
	while (x2 > x1) {
	    mask2 &= ~(4|32|256);
	    mask2 <<= 1;
	    x2--;
	}
	while (x2 < x1) {
	    mask2 &= ~(1|8|64);
	    mask2 >>= 1;
	    x2++;
	}
	while (y2 > y1) {
	    mask2 &= ~(64|128|256);
	    mask2 <<= 3;
	    y2--;
	}
	while (y2 < y1) {
	    mask2 &= ~(1|2|4);
	    mask2 >>= 3;
	    y2++;
	}
    }

    /*
     * Invert the second set if `diff' is set (we're after A &~ B
     * rather than A & B).
     */
    if (diff)
	mask2 ^= 511;

    /*
     * Now all that's left is a logical AND.
     */
    return mask1 & mask2;
}