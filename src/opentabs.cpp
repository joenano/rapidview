#include "opentabs.h"

void OpenTabs::append(JsonTab *tab)
{
    if(root == nullptr) {
        root = new OpenTab(tab);
    }
    else {
        OpenTab *ptr = end();
        ptr->next = new OpenTab(tab, ptr);
    }

    _size++;
}

OpenTab *OpenTabs::at_index(int index)
{
    OpenTab *ptr = root;

    for(int i = 0; i < index; i++)
        ptr = ptr->next;

    return ptr;
}

OpenTab *OpenTabs::end()
{
    OpenTab *ptr = root;

    for(int i = 1; i < _size; i++)
        ptr = ptr->next;

    return ptr;
}

void OpenTabs::remove(int index)
{
    OpenTab *ptr = at_index(index);

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
