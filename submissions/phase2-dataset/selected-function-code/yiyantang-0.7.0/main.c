int main ()
{
    char *pbuf;
    //  char *test = "�ϻ������� This is not really a a����";
    char *test = "->  -   41:[/2��BIG5-��BIG5-�H�ͦp�ڤ۪wB]  3 cjk /2��BIG5-��BIG5-����r�X����0000 ";
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
