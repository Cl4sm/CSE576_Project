int midend_process_key(midend *me, int x, int y, int button)
{
    int ret = 1;

    /*
     * Harmonise mouse drag and release messages.
     * 
     * Some front ends might accidentally switch from sending, say,
     * RIGHT_DRAG messages to sending LEFT_DRAG, half way through a
     * drag. (This can happen on the Mac, for example, since
     * RIGHT_DRAG is usually done using Command+drag, and if the
     * user accidentally releases Command half way through the drag
     * then there will be trouble.)
     * 
     * It would be an O(number of front ends) annoyance to fix this
     * in the front ends, but an O(number of back ends) annoyance
     * to have each game capable of dealing with it. Therefore, we
     * fix it _here_ in the common midend code so that it only has
     * to be done once.
     * 
     * The possible ways in which things can go screwy in the front
     * end are:
     * 
     *  - in a system containing multiple physical buttons button
     *    presses can inadvertently overlap. We can see ABab (caps
     *    meaning button-down and lowercase meaning button-up) when
     *    the user had semantically intended AaBb.
     * 
     *  - in a system where one button is simulated by means of a
     *    modifier key and another button, buttons can mutate
     *    between press and release (possibly during drag). So we
     *    can see Ab instead of Aa.
     * 
     * Definite requirements are:
     * 
     *  - button _presses_ must never be invented or destroyed. If
     *    the user presses two buttons in succession, the button
     *    presses must be transferred to the backend unchanged. So
     *    if we see AaBb , that's fine; if we see ABab (the button
     *    presses inadvertently overlapped) we must somehow
     *    `correct' it to AaBb.
     * 
     *  - every mouse action must end up looking like a press, zero
     *    or more drags, then a release. This allows back ends to
     *    make the _assumption_ that incoming mouse data will be
     *    sane in this regard, and not worry about the details.
     * 
     * So my policy will be:
     * 
     *  - treat any button-up as a button-up for the currently
     *    pressed button, or ignore it if there is no currently
     *    pressed button.
     * 
     *  - treat any drag as a drag for the currently pressed
     *    button, or ignore it if there is no currently pressed
     *    button.
     * 
     *  - if we see a button-down while another button is currently
     *    pressed, invent a button-up for the first one and then
     *    pass the button-down through as before.
     * 
     * 2005-05-31: An addendum to the above. Some games might want
     * a `priority order' among buttons, such that if one button is
     * pressed while another is down then a fixed one of the
     * buttons takes priority no matter what order they're pressed
     * in. Mines, in particular, wants to treat a left+right click
     * like a left click for the benefit of users of other
     * implementations. So the last of the above points is modified
     * in the presence of an (optional) button priority order.
     *
     * A further addition: we translate certain keyboard presses to
     * cursor key 'select' buttons, so that a) frontends don't have
     * to translate these themselves (like they do for CURSOR_UP etc),
     * and b) individual games don't have to hard-code button presses
     * of '\n' etc for keyboard-based cursors. The choice of buttons
     * here could eventually be controlled by a runtime configuration
     * option.
     */
    if (IS_MOUSE_DRAG(button) || IS_MOUSE_RELEASE(button)) {
        if (me->pressed_mouse_button) {
            if (IS_MOUSE_DRAG(button)) {
                button = me->pressed_mouse_button +
                    (LEFT_DRAG - LEFT_BUTTON);
            } else {
                button = me->pressed_mouse_button +
                    (LEFT_RELEASE - LEFT_BUTTON);
            }
        } else
            return ret;                /* ignore it */
    } else if (IS_MOUSE_DOWN(button) && me->pressed_mouse_button) {
	/*
	 * If the new button has lower priority than the old one,
	 * don't bother doing this.
	 */
	if (me->ourgame->flags &
	    BUTTON_BEATS(me->pressed_mouse_button, button))
	    return ret;		       /* just ignore it */

        /*
         * Fabricate a button-up for the previously pressed button.
         */
        ret = ret && midend_really_process_key
            (me, x, y, (me->pressed_mouse_button +
                        (LEFT_RELEASE - LEFT_BUTTON)));
    }

    /*
     * Translate keyboard presses to cursor selection.
     */
    if (button == '\n' || button == '\r')
      button = CURSOR_SELECT;
    if (button == ' ')
      button = CURSOR_SELECT2;

    /*
     * Normalise both backspace characters (8 and 127) to \b. Easier
     * to do this once, here, than to require all front ends to
     * carefully generate the same one - now each front end can
     * generate whichever is easiest.
     */
    if (button == '\177')
	button = '\b';

    /*
     * Now send on the event we originally received.
     */
    ret = ret && midend_really_process_key(me, x, y, button);

    /*
     * And update the currently pressed button.
     */
    if (IS_MOUSE_RELEASE(button))
        me->pressed_mouse_button = 0;
    else if (IS_MOUSE_DOWN(button))
        me->pressed_mouse_button = button;

    return ret;
}