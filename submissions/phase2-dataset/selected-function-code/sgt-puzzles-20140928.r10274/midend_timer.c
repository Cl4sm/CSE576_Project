void midend_timer(midend *me, float tplus)
{
    int need_redraw = (me->anim_time > 0 || me->flash_time > 0);

    me->anim_pos += tplus;
    if (me->anim_pos >= me->anim_time ||
        me->anim_time == 0 || !me->oldstate) {
	if (me->anim_time > 0)
	    midend_finish_move(me);
    }

    me->flash_pos += tplus;
    if (me->flash_pos >= me->flash_time || me->flash_time == 0) {
	me->flash_pos = me->flash_time = 0;
    }

    if (need_redraw)
        midend_redraw(me);

    if (me->timing) {
	float oldelapsed = me->elapsed;
	me->elapsed += tplus;
	if ((int)oldelapsed != (int)me->elapsed)
	    status_bar(me->drawing, me->laststatus ? me->laststatus : "");
    }

    midend_set_timer(me);
}