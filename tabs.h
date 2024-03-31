#ifndef TABS_H
#define TABS_H

#include "jsonview.h"


struct Tab
{
    JsonView *tab;

    Tab *prev;
    Tab *next;

    Tab(JsonView *tab, Tab *prev = nullptr): tab(tab), prev(prev), next(nullptr) {};
};

class Tabs
{
private:
    int _size;

    Tab *root;

public:
    Tabs(): _size(0), root(nullptr) {};

    Tab *at_index(int index);

    void append(JsonView *tab);
    void remove(int index);

    int size() { return _size; };
};

#endif // TABS_H
