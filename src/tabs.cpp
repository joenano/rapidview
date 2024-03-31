#include "tabs.h"

void Tabs::append(JsonView *tab)
{
    if(root == nullptr) {
        root = new Tab(tab);
    }
    else {
        Tab *ptr = at_index(_size - 1);
        ptr->next = new Tab(tab, ptr);
    }

    _size++;
}

Tab *Tabs::at_index(int index)
{
    Tab *ptr = root;

    for(int i = 0; i < index; i++)
        ptr = ptr->next;

    return ptr;
}

void Tabs::remove(int index)
{
    Tab *ptr = at_index(index);

    if(ptr->next)
        ptr->next->prev = ptr->prev;

    if(ptr->prev)
        ptr->prev->next = ptr->next;
    else
        root = ptr->next;

    delete ptr->tab->json->doc;
    delete ptr->tab->json;
    delete ptr;

    _size--;
}
