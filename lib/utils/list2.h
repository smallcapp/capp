#ifndef __LIST2_H_db28ba3d004741668e4ee130c2803b69__
#define __LIST2_H_db28ba3d004741668e4ee130c2803b69__
/******************************************************************************/
static inline bool
is_in_list(struct list_head *node)
{
    return (node->next && node->prev) && false==list_empty(node);
}
#define list_first(ptr)     (ptr)->next

/*
* mlist: mixed(list & hash) list
*/
struct mlist_node {
    struct hlist_node hash;
    struct list_head  list;
};

struct mlist_table {
    struct hlist_head *hash;
    uint32_t size;
    
    struct list_head  list;
    uint32_t count;
};

static inline int
__mlist_table_init(struct mlist_table *table, uint32_t size)
{
    INIT_LIST_HEAD(&table->list);
    
    table->hash = (struct hlist_head *)os_zalloc(size * sizeof(struct hlist_head));
    if (NULL==table->hash) {
        return -ENOMEM;
    }
    table->size = size;
    
    return 0;
}

static inline int
mlist_table_init(struct mlist_table *table, uint32_t size)
{
    if (NULL==table) {
        return os_assertV(-EINVAL1);
    }
    else {
        return __mlist_table_init(table, size);
    }
}

static inline struct mlist_node *
__mlist_find(
    struct mlist_table *table, 
    int (*hash)(void),
    bool (*eq)(struct mlist_node *node)
)
{
    struct mlist_node *node;
    struct hlist_head *head = &table->hash[(*hash)()];

    hlist_for_each_entry(node, head, hash) {
        if ((*eq)(node)) {
            return node;
        }
    }

    return NULL;
}

static inline struct mlist_node *
__mlist_insert(
    struct mlist_table *table, 
    struct mlist_node *node,
    int (*hash)(struct mlist_node *node)
)
{
    if (NULL==node) {
        return NULL; /* NOT assert */
    }
    else if (is_in_list(&node->list)) {
        return os_assertV(NULL);
    }
    
    list_add(&node->list, &table->list);
    hlist_add_head(&node->hash, &table->hash[(*hash)(node)]);
    table->count++;
    
    return node;
}

static inline int
__mlist_remove(struct mlist_table *table, struct mlist_node *node)
{
    if (false==is_in_list(&node->list)) {
        return os_assertV(-ENOINLIST);
    }
    
    list_del(&node->list);
    hlist_del(&node->hash);
    table->count--;
    
    return 0;
}

static inline int
__mlist_foreach(
    struct mlist_table *table, 
    multi_value_t (*cb)(struct mlist_node *node)
)
{
    struct mlist_node *node, *tmp;
    multi_value_u mv;
    
    list_for_each_entry_safe(node, tmp, &table->list, list) {
        mv.value = (*cb)(node);
        if (mv2_is_break(mv)) {
            return mv2_result(mv);
        }
    }

    return 0;
}

static inline struct mlist_node *
mlist_find(
    struct mlist_table *table, 
    int (*hash)(void),
    bool (*eq)(struct mlist_node *node)
)
{
    if (NULL==table) {
        return os_assertV(NULL);
    }
    else if (NULL==hash) {
        return os_assertV(NULL);
    }
    else if (NULL==eq) {
        return os_assertV(NULL);
    }
    else {
        return __mlist_find(table, hash, eq);
    }
}

static inline struct mlist_node *
mlist_insert(
    struct mlist_table *table, 
    int (*data_hash)(void),
    int (*node_hash)(struct mlist_node *node),
    bool (*eq)(struct mlist_node *node),
    struct mlist_node *(*new)(void)
)
{
    if (NULL==table) {
        return os_assertV(NULL);
    }
    else if (NULL==data_hash) {
        return os_assertV(NULL);
    }
    else if (NULL==node_hash) {
        return os_assertV(NULL);
    }
    else if (NULL==eq) {
        return os_assertV(NULL);
    }
    else if (NULL==new) {
        return os_assertV(NULL);
    }
    
    struct mlist_node *node = __mlist_find(table, data_hash, eq);
    if (node) {
        return node;
    } else {
        return __mlist_insert(table, (*new)(), node_hash);
    }
}

static inline int
mlist_remove(struct mlist_table *table, struct mlist_node *node)
{
    if (NULL==table) {
        return os_assertV(-EINVAL1);
    } if (NULL==node) {
        return os_assertV(-EINVAL2);
    } else {
        return __mlist_remove(table, node);
    }
}

static inline int
mlist_foreach(
    struct mlist_table *table, 
    multi_value_t (*cb)(struct mlist_node *node)
)
{
    if (NULL==table) {
        return os_assertV(-EINVAL0);
    }
    else if (NULL==cb) {
        return os_assertV(-EINVAL1);
    }
    else {
        return __mlist_foreach(table, cb);
    }
}
/******************************************************************************/
/*
* clist: cache(array & list & hash) list
*/
struct clist_node {
    struct list_head list;
    bool using;
};

struct clist_ops {
    struct clist_node* (*new)(void);
    void (*free)(struct clist_node *);
    
    int (*to_use)(struct clist_node *);
    int (*to_cache)(struct clist_node *);
};

struct clist_table {
    struct clist_node **array;
    uint32_t count;
    
    struct list_head  used;
    uint32_t used_count;
    
    struct list_head  cached;

    struct clist_ops *ops;
};

static inline int
__clist_table_check(struct clist_table *table)
{
    if (NULL==table ||
        0==table->count ||
        NULL==table->ops ||
        NULL==table->ops->new ||
        NULL==table->ops->free ||
        NULL==table->ops->to_use ||
        NULL==table->ops->to_cache) {

        return -EKEYBAD;
    } else {
        return 0;
    }
}

static inline void
clist_table_fini(struct clist_table *table)
{
    int i;

    if (__clist_table_check(table)) {
        os_assert(0);
        return;
    }
    
    for (i=0; i<table->count; i++) {
        struct clist_node *item = table->array[i];
        if (item) {
            list_del(&item->list);

            (*table->ops->free)(item);
        }
    }
    os_free(table->array);
}

static inline int
clist_table_init(struct clist_table *table)
{
    int i, err;

    if (__clist_table_check(table)) {
        return os_assertV(-EKEYBAD);
    }
    
    INIT_LIST_HEAD(&table->used);
    INIT_LIST_HEAD(&table->cached);
    
    table->array = (struct clist_node **)os_zalloc(table->count * sizeof(struct clist_node *));
    if (NULL==table->array) {
        err = -ENOMEM; goto error;
    }

    for (i=0; i<table->count; i++) {
        struct clist_node *item = (*table->ops->new)();
        if (NULL==item) {
            err = -ENOMEM; goto error;
        }
        list_add(&item->list, &table->cached);

        table->array[i] = item;
    }
    
    return 0;
error:
    clist_table_fini(table);
}

static inline bool
__clist_cache_is_empty(struct clist_table *table)
{
    return table->count==table->used_count;
}
#define __clist_using_is_full(_table)   __clist_cache_is_empty(_table)

static inline bool
__clist_cache_is_full(struct clist_table *table)
{
    return 0==table->used_count;
}
#define __clist_using_is_empty(_table)   __clist_cache_is_full(_table)

static inline struct clist_node *
clist_pick(struct clist_table *table)
{
    if (__clist_cache_is_empty(table)) {
        return NULL;
    }

    struct clist_node *node = list_first_entry(&table->cached, struct clist_node, list);

    list_del(&node->list);
    list_add(&node->list, &table->used);
    node->using = true;
    table->used_count++;

    (*table->ops->to_use)(node);
    
    return node;
}

static inline int
clist_unpick(struct clist_table *table, struct clist_node *node)
{
    if (false==node->using && false==is_in_list(&node->list)) {
        return os_assertV(-ENOINLIST);
    }
    
    list_del(&node->list);
    list_add(&node->list, &table->cached);
    node->using = false;
    table->used_count--;
    
    (*table->ops->to_cache)(node);
    
    return 0;
}

#if 0
#define clist_foreach_used_entry(pos, table, type, member) \
    for (pos = container_of(list_first_entry(&table->used, struct clist_node, list), type, member); \
	     &pos->member.list != &table->used; \
	     pos = container_of(list_next_entry(&pos->member.list, list), type, member))

#define clist_foreach_used_entry_safe(pos, n, table, type, member) \
    for (pos = container_of(list_first_entry(&table->used, struct clist_node, list), type, member), \
         n = container_of(list_next_entry(&pos->member.list, list), type, member); \
         &pos->member.list != &table->used; \
         pos = n, n = container_of(list_next_entry(&n->member.list, list), type, member))
#endif

/******************************************************************************/
#endif /* __LIST2_H_db28ba3d004741668e4ee130c2803b69__ */
