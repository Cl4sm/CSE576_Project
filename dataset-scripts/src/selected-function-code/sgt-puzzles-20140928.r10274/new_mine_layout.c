static char *new_mine_layout(int w, int h, int n, int x, int y, int unique,
			     random_state *rs, char **game_desc)
{
    char *grid;

#ifdef TEST_OBFUSCATION
    static int tested_obfuscation = FALSE;
    if (!tested_obfuscation) {
	/*
	 * A few simple test vectors for the obfuscator.
	 * 
	 * First test: the 28-bit stream 1234567. This divides up
	 * into 1234 and 567[0]. The SHA of 56 70 30 (appending
	 * "0") is 15ce8ab946640340bbb99f3f48fd2c45d1a31d30. Thus,
	 * we XOR the 16-bit string 15CE into the input 1234 to get
	 * 07FA. Next, we SHA that with "0": the SHA of 07 FA 30 is
	 * 3370135c5e3da4fed937adc004a79533962b6391. So we XOR the
	 * 12-bit string 337 into the input 567 to get 650. Thus
	 * our output is 07FA650.
	 */
	{
	    unsigned char bmp1[] = "\x12\x34\x56\x70";
	    obfuscate_bitmap(bmp1, 28, FALSE);
	    printf("test 1 encode: %s\n",
		   memcmp(bmp1, "\x07\xfa\x65\x00", 4) ? "failed" : "passed");
	    obfuscate_bitmap(bmp1, 28, TRUE);
	    printf("test 1 decode: %s\n",
		   memcmp(bmp1, "\x12\x34\x56\x70", 4) ? "failed" : "passed");
	}
	/*
	 * Second test: a long string to make sure we switch from
	 * one SHA to the next correctly. My input string this time
	 * is simply fifty bytes of zeroes.
	 */
	{
	    unsigned char bmp2[50];
	    unsigned char bmp2a[50];
	    memset(bmp2, 0, 50);
	    memset(bmp2a, 0, 50);
	    obfuscate_bitmap(bmp2, 50 * 8, FALSE);
	    /*
	     * SHA of twenty-five zero bytes plus "0" is
	     * b202c07b990c01f6ff2d544707f60e506019b671. SHA of
	     * twenty-five zero bytes plus "1" is
	     * fcb1d8b5a2f6b592fe6780b36aa9d65dd7aa6db9. Thus our
	     * first half becomes
	     * b202c07b990c01f6ff2d544707f60e506019b671fcb1d8b5a2.
	     * 
	     * SHA of that lot plus "0" is
	     * 10b0af913db85d37ca27f52a9f78bba3a80030db. SHA of the
	     * same string plus "1" is
	     * 3d01d8df78e76d382b8106f480135a1bc751d725. So the
	     * second half becomes
	     * 10b0af913db85d37ca27f52a9f78bba3a80030db3d01d8df78.
	     */
	    printf("test 2 encode: %s\n",
		   memcmp(bmp2, "\xb2\x02\xc0\x7b\x99\x0c\x01\xf6\xff\x2d\x54"
			  "\x47\x07\xf6\x0e\x50\x60\x19\xb6\x71\xfc\xb1\xd8"
			  "\xb5\xa2\x10\xb0\xaf\x91\x3d\xb8\x5d\x37\xca\x27"
			  "\xf5\x2a\x9f\x78\xbb\xa3\xa8\x00\x30\xdb\x3d\x01"
			  "\xd8\xdf\x78", 50) ? "failed" : "passed");
	    obfuscate_bitmap(bmp2, 50 * 8, TRUE);
	    printf("test 2 decode: %s\n",
		   memcmp(bmp2, bmp2a, 50) ? "failed" : "passed");
	}
    }
#endif

    grid = minegen(w, h, n, x, y, unique, rs);

    if (game_desc)
        *game_desc = describe_layout(grid, w * h, x, y, TRUE);

    return grid;
}