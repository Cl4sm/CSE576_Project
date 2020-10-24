int main ()
{
    char *pbuf;
    //  char *test = "ÀÏ»¢²»³ÔÈË This is not really a a´óÊÂ";
    char *test = "->  -   41:[/2€‰BIG5-€‰BIG5-¤H¥Í¦p¹Ú¤ÛªwB]  3 cjk /2€‰BIG5-€‰BIG5-¤¤¤å¦r¥X¤£¨Ó0000 ";
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
