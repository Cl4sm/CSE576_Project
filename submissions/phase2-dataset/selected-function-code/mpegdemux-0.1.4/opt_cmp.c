static
int opt_cmp (const mpegd_option_t *opt1, const mpegd_option_t *opt2)
{
	int c1, c2;

	c1 = (opt1->name1 <= 255) ? tolower (opt1->name1) : opt1->name1;
	c2 = (opt2->name1 <= 255) ? tolower (opt2->name1) : opt2->name1;

	if (c1 < c2) {
		return (-1);
	}
	else if (c1 > c2) {
		return (1);
	}
	else if (opt1->name1 < opt2->name1) {
		return (1);
	}
	else if (opt1->name1 > opt2->name1) {
		return (-1);
	}

	return (0);
}
