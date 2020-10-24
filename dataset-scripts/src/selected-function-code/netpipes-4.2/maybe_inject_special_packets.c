void maybe_inject_special_packets(buf, len, size)
     char	*buf;
     int	*len;
     int	size;
{
    struct special_packet	*sp;
    if (*len>0)
	return;			/* can't fit one in */

    sp = special_packet_queue;

    if (sp ==0)
	return;			/* no special packets */

    if (sp->len > size) {
	memcpy(buf, sp->buf, size);
	memmove(sp->buf, sp->buf + size, sp->len -size);
	sp->len -= size;
	*len = size;
    } else {
	memcpy(buf, sp->buf, sp->len);
	*len = sp->len;

	special_packet_queue = sp->next;

	free(sp->buf);
	free(sp);
    }
}