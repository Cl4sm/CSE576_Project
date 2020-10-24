void *
rbtree_next (const void *const data)
{
    const struct rbtree_node *node = DATA2NODE (data), *tmp;

    if (node->r != 0)
    {
        node = node->r;
        while (node->l != 0)
            node = node->l;

        return NODE2DATA (node);
    }

    tmp = node->p;
    while (tmp != 0 && tmp->r == node)
    {
        node = tmp;
        tmp = tmp->p;
    }

    return tmp != 0 ? NODE2DATA (tmp) : 0;
}