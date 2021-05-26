//
// Created by xjs on 2020/10/9.
//
#include "include/_tctl_rb_tree.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "include/_tctl_iterator.h"
#include "../include/tctl_stack.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include <assert.h>
#include <string.h>

#define Import CLASS, OBJECT, METACLASS, ITERATOR, RB_TREE, STACK, ANY

enum _Color {_RED, _BLACK};

struct RB_treeNode {
    struct RB_treeNode *parent;
    struct RB_treeNode *left;
    struct RB_treeNode *right;
    enum _Color color;
    void *base_ptr;
};

struct RB_treeClass {
    Iterator (*begin)(void *_self);
    Iterator (*end)(void *_self);
    size_t (*size)(void *_self);
    bool (*empty)(void *_self);
    Iterator (*insert_unique)(void *_self, const void *_x);
    Iterator (*insert_equal)(void *_self, const void *_x);
    void (*erase)(void *_self, Iterator);
    Iterator (*find)(void *_self, const void *_x);
    size_t (*count)(void *_self, const void *_x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, RB_tree t);
};

struct RB_tree {
    void *class;
    size_t nmemb;
    struct RB_treeNode *header;
    Compare cmp;
};

struct RB_treeIter {
    struct RB_treeNode *node;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static Iterator _insert_unique(const void *_x);
static Iterator _insert_equal(const void *_x);
static void _erase(Iterator);
static Iterator _find(const void *_x);
static size_t _count(const void *_x);
static void _clear(void);
static void _swap(RB_tree t);
//RB_treeclass
static void *_rb_treeclass_ctor(void *_self, va_list *app);
//RB_tree
static void *_rb_tree_ctor(void *_self, va_list *app);
static void *_rb_tree_dtor(void *_self);
static Iterator _rb_tree_begin(const void *_self);
static Iterator _rb_tree_end(const void *_self);
static size_t _rb_tree_size(const void *_self);
static bool _rb_tree_empty(const void *_self);
static Iterator _rb_tree_insert_unique(void *_self, const void *_x);
static Iterator _rb_tree_insert_equal(void *_self, const void *_x);
static void _rb_tree_erase(void *_self, Iterator iter);
static Iterator _rb_tree_find(void *_self, const void *_x);
static size_t _rb_tree_count(void *_self, const void *_x);
static void _rb_tree_clear(void *_self);
static void _rb_tree_swap(void *_self, RB_tree _t);
//iterator
static void _iter_assign(void *_self, const void *_x);
static void _iter_inc(void *_self);
static void _iter_dec(void *_self);
static bool _iter_equal(const void *_self, const void *_x);
static void *_iter_ctor(void *_self, va_list *app);
static void *_iter_derefer(const void *_self);
static Iterator _iter_reverse_iterator(void *_self);
//init
static const void *__RB_treeIter = NULL;
static const void *__RRB_treeIter = NULL;
static const void *__RB_tree = NULL;
static const void *__RB_treeClass = NULL;
volatile static struct RB_treeSelector RB_treeS = {
    {0},
    _begin,
    _end,
    _size,
    _empty,
    _insert_unique,
    _insert_equal,
    _erase,
    _find,
    _count,
    _clear,
    _swap
};
const struct RB_treeSelector *_RB_treeS = NULL;

static void initRB_tree(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!_RB_treeS) {
        _RB_treeS = (void*)&RB_treeS;
        memcpy((void*)&RB_treeS, _ClassS, sizeof(*_ClassS));
    }
    if (!__RB_treeIter) {
        __RB_treeIter = new(_IteratorClass(), "RB_treeIter",
                         T(Iterator), sizeof(struct RB_treeIter) + classSz(T(Iterator)),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->dec, _iter_dec,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         _IteratorS->reverse_iterator, _iter_reverse_iterator,
                         Selector, _IteratorS, NULL);
    }
    if (!__RRB_treeIter) {
        __RRB_treeIter = new(_IteratorClass(), "RRB_treeIter",
                         T(Iterator), sizeof(struct RB_treeIter) + classSz(T(Iterator)),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_dec,
                         _ClassS->dec, _iter_inc,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         _IteratorS->reverse_iterator, _iter_reverse_iterator,
                         Selector, _IteratorS, NULL);
    }
    if (!__RB_treeClass) {
        __RB_treeClass = new(T(MetaClass), "RB_treeClass",
                           T(Class), sizeof(struct RB_treeClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _rb_treeclass_ctor, NULL);
    }
    if (!__RB_tree) {
        __RB_tree = new(__RB_treeClass, "RB_tree",
                       T(Object), sizeof(struct RB_tree) + classSz(T(Object)),
                       _MetaClassS->ctor, _rb_tree_ctor,
                       _MetaClassS->dtor, _rb_tree_dtor,
                       RB_treeS.begin, _rb_tree_begin,
                       RB_treeS.end, _rb_tree_end,
                       RB_treeS.size, _rb_tree_size,
                       RB_treeS.empty, _rb_tree_empty,
                       RB_treeS.insert_unique, _rb_tree_insert_unique,
                       RB_treeS.insert_equal, _rb_tree_insert_equal,
                       RB_treeS.erase, _rb_tree_erase,
                       RB_treeS.find, _rb_tree_find,
                       RB_treeS.count, _rb_tree_count,
                       RB_treeS.clear, _rb_tree_clear,
                       RB_treeS.swap, _rb_tree_swap,
                       Selector, _RB_treeS, NULL);
    }
}

const void *_RB_tree(void)
{
    if (!__RB_tree)
        initRB_tree();
    return __RB_tree;
}

const void *_RB_treeClass(void)
{
    if (!__RB_treeClass)
        initRB_tree();
    return __RB_treeClass;
}

static const void *_RB_treeIter(void)
{
    if (!__RB_treeIter)
        initRB_tree();
    return __RB_treeIter;
}

static const void *_RRB_treeIter(void)
{
    if (!__RRB_treeIter)
        initRB_tree();
    return __RRB_treeIter;
}

//private
static struct RB_treeNode *_creat_rb_node(void)
{
    struct RB_treeNode *node = allocate(sizeof(struct RB_treeNode));
    node->parent = node->left = node->right = NULL;
    node->color = _RED;
    node->base_ptr = NULL;
    return node;
}

static void _free_rb_node(struct RB_treeNode *node)
{
    deallocate(node, sizeof(struct RB_treeNode));
}

static bool _get_left_right_node(struct RB_treeNode *node)
{
    return node->parent->right == node;
}

static struct RB_treeNode *_minimum(struct RB_treeNode *root)
{
    struct RB_treeNode *node = root;
    while (node->left)
        node = node->left;
    return node;
}

static struct RB_treeNode *_maximum(struct RB_treeNode *root)
{
    struct RB_treeNode *node = root;
    while (node->right)
        node = node->right;
    return node;
}

static struct RB_treeNode *get_uncle_node(struct RB_treeNode *node)
{
    struct RB_treeNode *g_parent = node->parent->parent;
    if (_get_left_right_node(node->parent))
        return g_parent->left;
    else
        return g_parent->right;
}

static void _turn_left_node(struct RB_treeNode *drop_node)
{
    struct RB_treeNode *parent = drop_node->parent;
    //父亲节点变成爷爷节点
    if (parent->parent == drop_node)//如果爷爷节点为header节点
        parent->parent = drop_node->right;
    else if (_get_left_right_node(drop_node))
        parent->right = drop_node->right;
    else
        parent->left = drop_node->right;
    //指向父亲节点的指针指向新节点
    drop_node->parent = drop_node->right;
    //右节点指针指向新父亲节点的左子树
    drop_node->right = drop_node->parent->left;
    //修改新父节点的父节点
    drop_node->parent->parent = parent;
    //修改新父节点的左子树
    drop_node->parent->left = drop_node;
    //修改新右子树的父节点
    if (drop_node->right)
        drop_node->right->parent = drop_node;
}


static void _turn_right_node(struct RB_treeNode *drop_node)
{
    struct RB_treeNode *parent = drop_node->parent;
    if (parent->parent == drop_node)
        parent->parent = drop_node->left;
    else if (_get_left_right_node(drop_node))
        parent->right = drop_node->left;
    else
        parent->left = drop_node->left;
    drop_node->parent = drop_node->left;
    drop_node->left = drop_node->parent->right;
    drop_node->parent->parent = parent;
    drop_node->parent->right = drop_node;
    if (drop_node->left)
        drop_node->left->parent = drop_node;
}

static void _balance_tree_insert(struct RB_tree *self, struct RB_treeNode *cur)
{
    if (self->header->parent == cur) {//该节点为根节点
        cur->color = _BLACK;
    } else if(cur->parent->color == _RED) {//父节点是红色
        struct RB_treeNode *uncle_node = get_uncle_node(cur);
        if (uncle_node && uncle_node->color == _RED) {//叔叔节点是红色
            uncle_node->color = _BLACK;
            cur->parent->color = _BLACK;
            cur->parent->parent->color = _RED;
            _balance_tree_insert(self, cur->parent->parent);
        } else {//叔叔节点是黑色
            if (!_get_left_right_node(cur->parent)) {//插入节点的父节点在爷爷节点的左侧
                if (_get_left_right_node(cur)) {//插入节点在父节点的右侧
                    _turn_left_node(cur->parent);
                    _turn_right_node(cur->parent);
                } else {//插入节点在父节点的左侧
                    _turn_right_node(cur->parent->parent);
                    cur = cur->parent;//将cur赋值为新转动到父节点的地址
                }
            } else {//插入节点在爷爷节点右侧
                if (!_get_left_right_node(cur)) {//插入节点在父亲节点左侧
                    _turn_right_node(cur->parent);
                    _turn_left_node(cur->parent);
                } else {//插入节点在父亲节点右侧
                    _turn_left_node(cur->parent->parent);
                    cur = cur->parent;//将cur赋值为新转动到父节点的地址
                }
            }
            //调整旋转后的节点颜色
            cur->color = _BLACK;
            cur->left->color = cur->right->color = _RED;
        }
    }
}

static void _balance_tree_erase(struct RB_tree *self, struct RB_treeNode *cur)
{
    struct RB_treeNode *parent = cur->parent;
    struct RB_treeNode *brother_node = _get_left_right_node(cur) ? parent->left : parent->right;
    if (cur->parent->parent == cur) {//删除节点为根 2
        self->header->left = self->header->right = self->header->parent = self->header;
        return;
    }
    else if (cur->color == _RED) {//删除节点是红色 1
        return;
    } else {//删除节点是黑色
        if (cur->right && cur->right->color == _RED) {//删除节点右节点是红色 3
            cur->right->color = _BLACK;
            return;
        } else if (brother_node->color == _BLACK) {//删除节点的兄弟节点是黑色
            if ((!brother_node->left || brother_node->left->color == _BLACK) &&
                (!brother_node->right || brother_node->right->color == _BLACK)) {//兄弟节点的子节点是黑色
                if (parent->color == _BLACK) {//父亲节点为黑色 8
                    brother_node->color = _RED;
                    _balance_tree_erase(self, parent);
                } else {//父亲节点为红色 4
                    brother_node->color = _RED;
                    parent->color = _BLACK;
                }
            } else if (_get_left_right_node(brother_node)) {//兄弟节点在父节点右侧
                if (brother_node->right && brother_node->right->color == _RED) {//兄弟节点的右节点是红色 5
                    brother_node->color = parent->color;
                    parent->color = _BLACK;
                    brother_node->right->color = _BLACK;
                    _turn_left_node(parent);
                } else if (brother_node->left && brother_node->left->color == _RED) {//兄弟节点的左节点是红色 6
                    _turn_right_node(brother_node);
                    brother_node = brother_node->parent;
                    brother_node->color = _BLACK;
                    brother_node->right->color = _RED;
                    //回到'5'的状态
                    brother_node->color = parent->color;
                    parent->color = _BLACK;
                    brother_node->right->color = _BLACK;
                    _turn_left_node(parent);
                }
            } else {//兄弟节点在父节点左侧 7
                if (brother_node->left && brother_node->left->color == _RED) {//兄弟节点的左节点是红色
                    brother_node->color = parent->color;
                    parent->color = _BLACK;
                    brother_node->left->color = _BLACK;
                    _turn_right_node(parent);
                } else if (brother_node->right && brother_node->right->color == _RED) {//兄弟节点的右节点是红色
                    _turn_left_node(brother_node);
                    brother_node = brother_node->parent;
                    brother_node->color = _BLACK;
                    brother_node->left->color = _RED;
                    //回到'兄弟节点左节点是红色'的状态
                    brother_node->color = parent->color;
                    parent->color = _BLACK;
                    brother_node->left->color = _BLACK;
                    _turn_left_node(parent);
                }
            }
        } else {//兄弟节点是红色
            if (!_get_left_right_node(cur)) {//删除节点为父亲节点左边 9
                brother_node->color = parent->color;
                parent->color = _RED;
                _turn_left_node(parent);
                _balance_tree_erase(self, cur);
            } else {//删除节点为父亲节点右边 10
                brother_node->color = parent->color;
                parent->color = _RED;
                _turn_right_node(parent);
                _balance_tree_erase(self, cur);
            }
        }
    }
}

static struct RB_treeNode *_insert_aux(struct RB_tree *self, struct RB_treeNode *node, struct RB_treeNode **access_node, const void *_x)
{
    struct RB_treeNode *new_node = _creat_rb_node();
    new_node->parent = node;
    *access_node = new_node;
    _balance_tree_insert(self, new_node);
    //赋值
    size_t memb_size = classSz(self->class);
    new_node->base_ptr = allocate(memb_size);
    construct(self->class, new_node->base_ptr, _x, VAEND);
    self->nmemb++;
    return new_node;
}

static void _erase_aux(struct RB_tree *self, struct RB_treeNode *node)
{
    struct RB_treeNode *rep_node = node->right;
    if (!rep_node)
        rep_node = node;
    else
        while (rep_node->left)
            rep_node = rep_node->left;
    if (node != rep_node) {
        void *temp = node->base_ptr;
        node->base_ptr = rep_node->base_ptr;
        rep_node->base_ptr = temp;
    }
    _balance_tree_erase(self, rep_node);
    //删除
    size_t memb_size = classSz(self->class);
    struct RB_treeNode *parent = rep_node->parent;
    struct RB_treeNode *next_node = rep_node == node ? rep_node->left : rep_node->right;
    if (rep_node == rep_node->parent->parent) {
        next_node = next_node ? next_node : self->header;
        if (next_node == self->header)
            parent->left = parent->right = parent->parent = next_node;
        else
            parent->right = parent->parent = next_node;
    } else if (!_get_left_right_node(rep_node)) {
        parent->left = next_node;
        if (next_node)
            next_node->parent = parent;
    } else {
        parent->right = next_node;
        if (next_node)
            next_node->parent = parent;
    }
    destroy(rep_node->base_ptr);
    deallocate(rep_node->base_ptr, memb_size);
    _free_rb_node(rep_node);
    self->nmemb--;
}

static bool _find_aux(struct RB_tree *self, const void *_x, struct RB_treeNode **parent, size_t *is_unique)
{
    struct RB_treeNode **next = &self->header->parent;
    *parent = self->header;
    if (self->header->parent == self->header) {//树里没有节点
        *is_unique = 0;
        return false;
    }
    int flag = 1;
    size_t save_is_unique = 0;

    while (*next && ((flag = self->cmp(_x, (*next)->base_ptr)) ||
                     !*is_unique))
    {
        if (!flag)
            save_is_unique++;
        *parent = *next;
        next = flag < 0 ? &(*next)->left : &(*next)->right;
    }
    if (*next && !flag)
        save_is_unique++;
    *is_unique = save_is_unique;
    return &(*parent)->right == next;
}

static struct RB_treeNode *copyTree(struct RB_treeNode *node, const void *class)
{
    size_t memb_size = classSz(class);
    struct RB_treeNode *new_tree = _creat_rb_node();
    new_tree->color = node->color;
    new_tree->base_ptr = allocate(memb_size);
    construct(class, new_tree->base_ptr, node->base_ptr, VAEND);

    Stack stack_left = new(T(Stack), T(Any), VAEND);
    Stack stack_right = new(T(Stack), T(Any), VAEND);

    THIS(stack_left).push(VA(node, ANYONE));
    THIS(stack_right).push(VA(new_tree, ANYONE));

    while (!THIS(stack_left).empty())
    {
        Any l = THIS(stack_left).top();
        Any r = THIS(stack_right).top();
        struct RB_treeNode *pleft = *(struct RB_treeNode**)THIS(l).value();
        struct RB_treeNode *pright = *(struct RB_treeNode**)THIS(r).value();
        THIS(stack_left).pop();  
        THIS(stack_right).pop();
        if (pleft->right != NULL) {
            THIS(stack_left).push(VA(pleft->right, ANYONE));
            struct RB_treeNode *n = _creat_rb_node();
            n->color = pleft->right->color;
            n->parent = pright;
            n->base_ptr = allocate(memb_size);
            construct(class, n->base_ptr, pleft->right->base_ptr, VAEND);
            pright->right = n;
            THIS(stack_right).push(VA(n, ANYONE));
        }
        if (pleft->left != NULL) {
            THIS(stack_left).push(VA(pleft->left, ANYONE));
            struct RB_treeNode *n = _creat_rb_node();
            n->color = pleft->left->color;
            n->parent = pright;
            n->base_ptr = allocate(memb_size);
            construct(class, n->base_ptr, pleft->left->base_ptr, VAEND);
            pright->left = n;
            THIS(stack_right).push(VA(n, ANYONE));
        }
    }
    delete(stack_left);
    delete(stack_right);
    return new_tree;
}

static void _deal_RB_tree_Args(void *_self, void *args[], int n)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    if (classOf(args[0]) == T(Any)) {
        Any any = args[0];
        assert(THIS(any).type() == FUNC);
        self->cmp = *(Compare*)&TEMP_VAR(void*, THIS(any).value());
    } else {
        assert(classOf(args[0]) == __RB_tree); //复制一个RB_tree
        struct RB_tree *L = offsetOf(args[0], __RB_tree);
        assert(L->class == self->class);
        self->cmp = *(Compare*)&L->cmp;
        if (L->header->parent != L->header) { //当被复制的树不为空
            ARP_CreatePool();
            struct RB_treeNode *node = copyTree(L->header->parent, L->class);
            node->parent = self->header;
            self->header->parent = node;
            self->header->left = _minimum(node);
            self->header->right = _maximum(node);
            self->nmemb = L->nmemb;
            ARP_FreePool();
        }
    }
}

//public
//Iterator
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__RB_treeIter, _self, app);
    struct RB_treeIter *self = offsetOf(_self, __RB_treeIter);
    void *t = va_arg(*app, void*);
    if (classOf(t) == __RB_treeIter || classOf(t) == __RRB_treeIter) {
        struct RB_treeIter *it;
        if (classOf(t) == __RB_treeIter)
            it = offsetOf(t, __RB_treeIter);
        else
            it = offsetOf(t, __RRB_treeIter);
        *self = *it;
    } else {
        Any any = t;
        self->node = *(struct RB_treeNode**)THIS(any).value();
    }
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    const struct RB_treeIter *self = offsetOf(_self, __RB_treeIter);
    const struct RB_treeIter *x = offsetOf(_x, __RB_treeIter);
    return self->node == x->node;
}

static void _iter_inc(void *_self)
{
    struct RB_treeIter *self = offsetOf(_self, __RB_treeIter);
    struct RB_treeNode *node = self->node;
    if(node->right != NULL)
    {
        node = node->right;
        while(node->left != NULL)
            node=node->left;
    } else {//往父节点上寻找
        struct RB_treeNode *parents=node->parent;
        while(node == parents->right)  //为父节点右孩子时，继续往上找
        {
            node = parents;
            parents = parents->parent;
        }
        if (node->right != parents)
            node = parents;
    }
    self->node = node;
}

static void _iter_dec(void *_self)
{
    struct RB_treeIter *self = offsetOf(_self, __RB_treeIter);
    struct RB_treeNode *node = self->node;
    if (node->color == _RED && node->parent->parent == node)
        node = node->right;
    else if (node->left != NULL) {
        node = node->left;
        while (node->right != NULL)
            node = node->right;
    } else {
        struct RB_treeNode *parents = node->parent;
        while (parents->left == node)
        {
            node = parents;
            parents = parents->parent;
        }
        node = parents;
    }
    self->node = node;
}

static void _iter_assign(void *_self, const void *_x)
{
    struct RB_treeIter *self = offsetOf(_self, __RB_treeIter);
    struct RB_treeIter *x = offsetOf(_x, __RB_treeIter);
    self->node = x->node;
}

static void *_iter_derefer(const void *_self)
{
    struct RB_treeIter *self = offsetOf(_self, __RB_treeIter);
    return self->node->base_ptr;
}

static Iterator _iter_reverse_iterator(void *_self)
{
    Iterator it = (void*)_self;
    if (classOf(_self) == __RB_treeIter) {
        void *mem = ARP_MallocARelDtor(classSz(__RB_treeIter), destroy);
        return construct(_RRB_treeIter(), mem, it, VAEND);
    } else {
        void *mem = ARP_MallocARelDtor(classSz(__RRB_treeIter), destroy);
        return construct(_RB_treeIter(), mem, it, VAEND);
    }
}

//RB_treeClass
static void *_rb_treeclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__RB_treeClass, _self, app);
    struct _RB_treeClass *self = offsetOf(_self, __RB_treeClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&RB_treeS + sizeof(RB_treeS._));
    voidf *end = (voidf*)((char*)&RB_treeS + sizeof(RB_treeS));
    voidf *self_begin = (voidf*)self;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(self_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return _self;
}

//RB_tree
static void *_rb_tree_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__RB_tree, _self, app);
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    void *t = va_arg(*app, void*);
    self->class = t;
    self->nmemb = 0;
    self->cmp = NULL;
    struct RB_treeNode *node= _creat_rb_node();
    node->parent = node->left = node->right = node;
    self->header = node;
    void *args[1];
    int i = 0;
    while ((t = va_arg(*app, void*)) != VAEND)
    {
        assert(i < 1);
        args[i++] = t;
    }
    if (i)
        _deal_RB_tree_Args(_self, args, i);
    assert(self->cmp);
    return _self;
}

static void *_rb_tree_dtor(void *_self)
{
    _self = super_dtor(__RB_tree, _self);
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    _rb_tree_clear(_self);
    _free_rb_node(self->header);
    return _self;
}

static Iterator _rb_tree_begin(const void *_self)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    void *mem = ARP_MallocARelDtor(classSz(__RB_treeIter), destroy);
    return construct(__RB_treeIter, mem, VA(BidirectionalIter), self->class, VA(self->header->left, ANYONE), VAEND);
}

static Iterator _rb_tree_end(const void *_self)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    void *mem = ARP_MallocARelDtor(classSz(__RB_treeIter), destroy);
    return construct(__RB_treeIter, mem, VA(BidirectionalIter), self->class, VA(self->header, ANYONE), VAEND);
}

static size_t _rb_tree_size(const void *_self)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    return self->nmemb;
}

static bool _rb_tree_empty(const void *_self)
{
    return _rb_tree_size(_self);
}

static Iterator _rb_tree_insert_unique(void *_self, const void *_x)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    struct RB_treeNode *new_node;
    struct RB_treeNode *parent;
    size_t is_unique = true;
    bool left_right = _find_aux(self, _x, &parent, &is_unique);
    if (is_unique)
        return _rb_tree_end(_self);
    if (parent == self->header) {//没有根节点
        new_node = _insert_aux(self, self->header, &self->header->parent, _x);
        self->header->left = self->header->right = new_node;
    } else {
        struct RB_treeNode **node = left_right ? &parent->right : &parent->left;//判断插入节点是父节点的左节点还是右节点
        new_node = _insert_aux(self, parent, node, _x);
    }
    //重新分配header的指向,left总是指向最小值,right总是指向最大值
    if (left_right)
        self->header->right = _maximum(self->header->right);
    else
        self->header->left = _minimum(self->header->left);
    void *mem = ARP_MallocARelDtor(classSz(__RB_treeIter), destroy);
    return construct(__RB_treeIter, mem, VA(BidirectionalIter), self->class, VA(new_node, ANYONE), VAEND);
}

static Iterator _rb_tree_insert_equal(void *_self, const void *_x)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    struct RB_treeNode *new_node;
    struct RB_treeNode *parent;
    size_t is_unique = false;
    bool left_right = _find_aux(self, _x, &parent, &is_unique);
    if (parent->parent == parent) {//没有根节点
        new_node = _insert_aux(self, self->header, &self->header->parent, _x);
        self->header->left = self->header->right = new_node;
    } else {
        struct RB_treeNode **node = left_right ? &parent->right : &parent->left;//判断插入节点是父节点的左节点还是右节点
        new_node = _insert_aux(self, parent, node, _x);
    }
    //重新分配header的指向,left总是指向最小值,right总是指向最大值
    if (left_right)
        self->header->right = _maximum(self->header->right);
    else
        self->header->left = _minimum(self->header->left);
    void *mem = ARP_MallocARelDtor(classSz(__RB_treeIter), destroy);
    return construct(__RB_treeIter, mem, VA(BidirectionalIter), self->class, VA(new_node, ANYONE), VAEND);
}

static void _rb_tree_erase(void *_self, Iterator iter)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    struct RB_treeIter *it = offsetOf(iter, __RB_treeIter);
    if (self->header == it->node)
        return;
    _erase_aux(self, it->node);
    self->header->left = _minimum(self->header->parent);
    self->header->right = _maximum(self->header->parent);
}

static Iterator _rb_tree_find(void *_self, const void *_x)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    size_t is_unique = true;
    struct RB_treeNode *parent;
    bool left_right = _find_aux(self, _x, &parent, &is_unique);
    if (!is_unique)
        return _rb_tree_end(_self);
    else {
        struct RB_treeNode *node;
        void *mem = ARP_MallocARelDtor(classSz(__RB_treeIter), destroy);
        if (parent == self->header)
            node = parent->parent;
        else
            node = !left_right ? parent->left : parent->right;
        return construct(__RB_treeIter, mem, VA(BidirectionalIter), self->class, VA(node, ANYONE), VAEND);
    }
}

static size_t _rb_tree_count(void *_self, const void *_x)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    size_t is_unique = false;
    struct RB_treeNode *parent;
    _find_aux(self, _x, &parent, &is_unique);
    return is_unique;
}

static void _rb_tree_clear(void *_self)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    struct RB_treeNode *node = self->header->left, *next;
    size_t memb_size = classSz(self->class);
    while (node != self->header)
    {
        node = _minimum(node);
        node = _maximum(node);
        if (node->left)
            continue;
        next = node->parent;
        if (!_get_left_right_node(node))
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
        destroy(node->base_ptr);
        deallocate(node->base_ptr, memb_size);
        _free_rb_node(node);
        node = next;
    }
    self->nmemb = 0;
    self->header->parent = self->header->left = self->header->right = self->header;
}

static void _rb_tree_swap(void *_self, RB_tree _t)
{
    struct RB_tree *self = offsetOf(_self, __RB_tree);
    struct RB_tree *t = offsetOf(_t, __RB_tree);
    struct RB_tree temp = *self;
    *self = *t;
    *t = temp;
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->empty);
    return class->empty(_self);
}

static Iterator _insert_unique(const void *_x)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->insert_unique);
    return class->insert_unique(_self, _x);
}

static Iterator _insert_equal(const void *_x)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->insert_equal);
    return class->insert_equal(_self, _x);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _find(const void *_x)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->find);
    return class->find(_self, _x);
}

static size_t _count(const void *_x)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->count);
    return class->count(_self, _x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(RB_tree t)
{
    void *_self = pop_this();
    const struct RB_treeClass *class = offsetOf(classOf(_self), __RB_treeClass);
    assert(class->swap);
    class->swap(_self, t);
}
