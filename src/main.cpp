#include "ListFunctions.h"

//================================================================================

int main()
{
    struct list lst = {};

    list_ctor(&lst);

    // Here you can write list functions...

//=============================
    // push_after(&lst, lst.tail_node, 10);

    // elem_t a = 0;
    // push_after(&lst, lst.tail_node, 10);
    // push_after(&lst, lst.tail_node, 20);
    // push_after(&lst, lst.tail_node, 30);
    // push_after(&lst, lst.tail_node, 40);
    // push_after(&lst, lst.tail_node, 50);
    // head_remove(&lst, &a);
    // head_remove(&lst, &a);
    // push_after(&lst, lst.tail_node, 60);
    // push_after(&lst, lst.tail_node, 70);
    // push_after(&lst, lst.tail_node, 80);
    // list_linearize(&lst);
    // head_remove(&lst, &a);
    // head_remove(&lst, &a);
    // push_after(&lst, 1, 100);
    // push_after(&lst, 1, 200);
    // push_after(&lst, 1, 300);
    // node_remove(&lst, 1, &a);

//=============================
    
    if(lst.error_code == _SUCCESS)
    {
        list_dtor(&lst);
    }
}
