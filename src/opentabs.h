#ifndef OPENTABS_H
#define OPENTABS_H

#include "jsontab.h"


struct OpenTab
{
    JsonTab *tab;

    OpenTab *prev;
    OpenTab *next;

    OpenTab(JsonTab *tab, OpenTab *prev = nullptr): tab(tab), prev(prev), next(nullptr) {};
};

class OpenTabs
{
private:
    int _size;

    OpenTab *root;

    OpenTab *end();

public:
    OpenTabs(): _size(0), root(nullptr) {};

    OpenTab *at_index(int index);

    void append(JsonTab *tab);
    void remove(int index);

    int size() { return _size; };
};

#endif // OPENTABS_H
