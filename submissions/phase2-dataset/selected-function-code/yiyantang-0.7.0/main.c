int main ()
{
    char *pbuf;
    //  char *test = "老虎不吃人 This is not really a a大事";
    char *test = "->  -   41:[/2�塀IG5-�塀IG5-ネ冠ほ獁B]  3 cjk /2�塀IG5-�塀IG5-いゅぃㄓ0000 ";
    int ret, count;

    ret = 0;
    count = strlen (test);
    pbuf = test;
    printf ("count: %d, ret: %d\n", count, ret);
    for (;;)
    {
	ret = hzsegment (pbuf, count);
	write (STDOUT_FILENO, pbuf, ret);
	write (STDOUT_FILENO, "\n", 1);
	count -= ret;
	pbuf += ret;
	printf ("count: %d, ret: %d\n", count, ret);
	if (count <= 0)
	    return 0;
    }
	return 0;
}
