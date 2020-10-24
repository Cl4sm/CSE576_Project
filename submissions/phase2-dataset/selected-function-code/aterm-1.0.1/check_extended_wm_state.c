Bool
check_extended_wm_state()	
{
	CARD32         *items;
	long          nitems = 0;
	unsigned long new_state = 0;
	Bool changed = False ;

	if (read_32bit_proplist (TermWin.parent, _XA_NET_WM_STATE, 6, &items, &nitems))
	{
		int i ;
		for( i = 0 ; i < nitems ; ++i ) 
		{	
			if( items[i] == _XA_NET_WM_STATE_STICKY )
				set_flags( new_state, WM_AtermStateSticky	);
			else if( items[i] == _XA_NET_WM_STATE_SHADED )
				set_flags( new_state, WM_AtermStateShaded );
			else if( items[i] == _XA_NET_WM_STATE_HIDDEN )
				set_flags( new_state, WM_AtermStateHidden );
		}
		free (items);
		changed = (get_flags(ExtWM.flags, WM_AtermStateSticky|WM_AtermStateShaded|WM_AtermStateHidden) != new_state) ; 
		clear_flags(ExtWM.flags, WM_AtermStateSticky|WM_AtermStateShaded|WM_AtermStateHidden);
		set_flags( ExtWM.flags, new_state );
	}else
	{	
		changed = get_flags(ExtWM.flags, WM_AtermStateSticky|WM_AtermStateShaded|WM_AtermStateHidden);
		clear_flags(ExtWM.flags, WM_AtermStateSticky|WM_AtermStateShaded|WM_AtermStateHidden);
	}

	return changed;
}
