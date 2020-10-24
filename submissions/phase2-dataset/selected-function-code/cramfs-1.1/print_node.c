static void print_node(struct entry *e)
{
	char info[10];
	char type = '?';

	if (S_ISREG(e->mode)) type = 'f';
	else if (S_ISDIR(e->mode)) type = 'd';
	else if (S_ISLNK(e->mode)) type = 'l';
	else if (S_ISCHR(e->mode)) type = 'c';
	else if (S_ISBLK(e->mode)) type = 'b';
	else if (S_ISFIFO(e->mode)) type = 'p';
	else if (S_ISSOCK(e->mode)) type = 's';

	if (S_ISCHR(e->mode) || (S_ISBLK(e->mode))) {
		/* major/minor numbers can be as high as 2^12 or 4096 */
		snprintf(info, 10, "%4d,%4d", major(e->size), minor(e->size));
	}
	else {
		/* size be as high as 2^24 or 16777216 */
		snprintf(info, 10, "%9d", e->size);
	}

	printf("%c %04o %s %5d:%-3d %s\n",
	       type, e->mode & ~S_IFMT, info, e->uid, e->gid, e->name);
}
