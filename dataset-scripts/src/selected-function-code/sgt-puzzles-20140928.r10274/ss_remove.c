static void ss_remove(struct setstore *ss, struct set *s)
{
    struct set *next = s->next, *prev = s->prev;

#ifdef SOLVER_DIAGNOSTICS
    printf("removing set %d,%d %03x\n", s->x, s->y, s->mask);
#endif
    /*
     * Remove s from the todo list.
     */
    if (prev)
	prev->next = next;
    else if (s == ss->todo_head)
	ss->todo_head = next;

    if (next)
	next->prev = prev;
    else if (s == ss->todo_tail)
	ss->todo_tail = prev;

    s->todo = FALSE;

    /*
     * Remove s from the tree.
     */
    del234(ss->sets, s);

    /*
     * Destroy the actual set structure.
     */
    sfree(s);
}