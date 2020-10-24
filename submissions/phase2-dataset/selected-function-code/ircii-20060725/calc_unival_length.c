unsigned
calc_unival_length(const u_char* str)
{
	/* Returns the number of bytes taken by
	 * the given utf-8 code
	 */
	static const char sizes[16] =
	{ 1,1,1,1,1,1,1,1,
	  0,0,0,0,2,2,3,4 };
	return sizes[*str >> 4];
	/* 1-byte (0..7F):
	 * 0 1 2 3 4 5 6 7
	 * 2-byte (80..7FF):
	 *         C D
	 * 3-byte (800..FFFF):
	 *             E
	 * 4-byte (10000..1FFFFF):
	 *               F
	 * invalid:
	 * 8 9 A B (they can not begin a sequence)
	 *
	 * If utf8 is some day extended to use 5-byte
	 * codings, you need to double the sizes[] size
	 * and shift str by 3 instead of 4.
	 * You'd also need to modify
	 *  utf8_sequence() and calc_unival().
	 *
	 * Today, it seems unlikely that these encodings
	 * will be needed in practical applications such as
	 * an irc client. Many programs (such as Microsoft IE)
	 * don't even support 4-byte encodings.
	 * 2-3 -byte encodings are in daily use everywhere.
	 */
}
