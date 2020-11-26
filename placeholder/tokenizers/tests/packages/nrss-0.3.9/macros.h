#ifndef MACROSH
#define MACROSH

#define foreach_feed(_cur) \
	struct feed *_cur = NULL;\
	for(_cur = nc->feeds; _cur; _cur = _cur->next)

#define foreach_feed_from(_start, _cur)\
	struct feed *_cur = NULL;\
	for(_cur = _start; _cur; _cur = _cur->next)

#define foreach_feed_from_rev(_start, _cur)\
	struct feed *_cur = NULL;\
	for(_cur = _start; _cur; _cur = _cur->prev)

#define foreach_item(_data, _cur)\
	struct item *_cur = _data->items;\
	for(_cur = _data->items; _cur != NULL; _cur = _cur->next)

#define foreach_visible_item(_data, _cur)\
	struct item *_cur = _data->items;\
	for(int _i = 0; (_i < _data->nitems_visible)&&(_cur); _i++, _cur = _cur->next)

#define min(A, B) ((A < B) ? A : B)
#define max(A, B) ((A > B) ? A : B)
#define safeset(_x, _a) (_x = (_x ? _x : _a))

#define clear_status() mvwhline(ni->status, 1, 1, ' ', ni->width -2)

#define nw(_cur) (_cur->bits & ITEM_NEW)
#define setnw(_curf,_cur) _cur->bits |= ITEM_NEW; fcachebad(_curf)
#define usetnw(_curf,_cur) _cur->bits &= ~(ITEM_NEW); fcachebad(_curf)

#define unrd(_cur) (_cur->bits & ITEM_UNREAD)
#define setunrd(_curf,_cur) _cur->bits |= ITEM_UNREAD; fcachebad(_curf)
#define usetunrd(_curf,_cur) _cur->bits &= ~(ITEM_UNREAD); fcachebad(_curf)

#define icached(_cur) (_cur->bits & ITEM_CACHED)
#define icacheok(_cur) (_cur->bits |= ITEM_CACHED)
#define icachebad(_cur) (_cur->bits &= ~(ITEM_CACHED))

#define sh(_cur) (_cur->bits & FEED_SHRUNK)
#define setsh(_cur) _cur->bits |= FEED_SHRUNK; fcachebad(_cur)
#define usetsh(_cur) _cur->bits &= ~(FEED_SHRUNK); fcachebad(_cur)

#define ex(_cur) (_cur->bits & FEED_EXP)
#define setex(_cur) cur->bits |= FEED_EXP; fcachebad(_cur)
#define usetex(_cur) cur->bits &= ~(FEED_EXP); fcachebad(_cur)

#define fcached(_cur) (_cur->bits & FEED_CACHED)
#define fcacheok(_cur) (_cur->bits |= FEED_CACHED)
#define fcachebad(_cur) (_cur->bits &= ~(FEED_CACHED))

#define all_shrunk() (ni->bits & IFACE_ALL_SHRUNK)
#define all_soft_shrunk() (ni->bits & IFACE_ALL_SOFT_SHRUNK)


#define browser_wait() (nc->bits & CFG_BITS_BROWSER_WAIT)
#define offline_mode() (nc->bits & CFG_OFFLINE)
#define iface_mute() (nc->bits |= CFG_MUTED)
#define iface_unmute() (nc->bits &= ~(CFG_MUTED))
#define muted() (nc->bits & CFG_MUTED)

#define signal_alarm() (nc->bits & CFG_ALRM)
#define unset_alarm() (nc->bits &= ~(CFG_ALRM))

#define signal_wnch() (nc->bits & CFG_WNCH)
#define unset_wnch() (nc->bits &= ~(CFG_WNCH))

#define signal_pipe() (nc->bits & CFG_PIPE)
#define unset_pipe() (nc->bits &= ~(CFG_PIPE))

#define signal_child() (nc->children)
#define unset_child() (nc->children--)
#define set_child() (nc->children++)

#define columns_set() (nc->bits & CFG_SETCOLS)

#define reader_open() (ni->reader)
#define usage_open() (ni->usage)
#endif
