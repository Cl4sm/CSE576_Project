int bvsnprintf(char *buf, int size, const char *fmt, va_list args)
{
	int len;
	unsigned long num;
	int i, base;
	u32 x;
	char *str, *start;
	const char *s;
	char ipbuf[STD_ADDRESS_P_LENGTH+1];
	struct iface *iface;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (start=str=buf ; *fmt ; ++fmt, size-=(str-start), start=str) {
		if (*fmt != '%') {
			if (!size)
				return -1;
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}
		
		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		/* default base */
		base = 10;

		if (field_width > size)
			return -1;
		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			continue;

		case 'm':
			if (flags & SPECIAL) {
				if (!errno)
					continue;
				if (size < 2)
					return -1;
				*str++ = ':';
				*str++ = ' ';
				start += 2;
				size -= 2;
			}
			s = strerror(errno);
			goto str;
		case 'M':
			s = strerror(va_arg(args, int));
			goto str;
		case 's':
			s = va_arg(args, char *);
			if (!s)
				s = "<NULL>";

		str:
			len = strlen(s);
			if (precision >= 0 && len > precision)
				len = precision;
			if (len > size)
				return -1;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			continue;

		case 'p':
			if (field_width == -1) {
				field_width = 2*sizeof(void *);
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags, size);
			if (!str)
				return -1;
			continue;


		case 'n':
			if (qualifier == 'l') {
				long * ip = va_arg(args, long *);
				*ip = (str - buf);
			} else {
				int * ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			continue;

		/* IP address */
		case 'I':
			if (flags & SPECIAL)
				ip_ntox(va_arg(args, ip_addr), ipbuf);
			else {
				ip_ntop(va_arg(args, ip_addr), ipbuf);
				if (field_width == 1)
					field_width = STD_ADDRESS_P_LENGTH;
			}
			s = ipbuf;
			goto str;

		/* Interface scope after link-local IP address */
		case 'J':
			iface = va_arg(args, struct iface *);
			if (!iface)
				continue;
			if (!size)
				return -1;

			*str++ = '%';
			start++;
			size--;

			s = iface->name;
			goto str;

		/* Router/Network ID - essentially IPv4 address in u32 value */
		case 'R':
			x = va_arg(args, u32);
			bsprintf(ipbuf, "%d.%d.%d.%d",
				 ((x >> 24) & 0xff),
				 ((x >> 16) & 0xff),
				 ((x >> 8) & 0xff),
				 (x & 0xff));
			s = ipbuf;
			goto str;

		/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			if (size < 2)
				return -1;
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}
		if (qualifier == 'l')
			num = va_arg(args, unsigned long);
		else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (short) num;
		} else if (flags & SIGN)
			num = va_arg(args, int);
		else
			num = va_arg(args, unsigned int);
		str = number(str, num, base, field_width, precision, flags, size);
		if (!str)
			return -1;
	}
	if (!size)
		return -1;
	*str = '\0';
	return str-buf;
}
