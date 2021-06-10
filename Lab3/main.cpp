#include <iostream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <vector>
#include <string>
#include <cstring>

#include "screen.h"
#include "AVL.h"
#include "set_seq.h"

const int N = 52; // мощность генерируемых множеств
screen tree::s; // поле для вывода деревьев на экран (static)
size_t set_seq::tags = 0; // обнуляем счётчик тегов

int main()
{
    int select;
    std::cout << "1 - Example\n2 - Random generate\n> ";
    cin >> select;
    if (select == 1)
    {
        std::vector<int> a_ = {2, 3, 1},
        b_ = {1, 3, 4, 5},
        c_ = {4, 5, 6, 15, 9},
        d_ = {1, 4, 3, 7, 15, 9, 18, 90},
        e_ = {2, 0, 5, 6, 15, 4, 27, 18};

        set_seq a(a_.begin(), a_.end()),
        b(b_.begin(), b_.end()),
        c(c_.begin(), c_.end()),
        d(d_.begin(), d_.end()),
        e(e_.begin(), e_.end()),
        f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        cout << "\n\nSet operation (A | B)^(C & D) \\ E:\n";
        f = (a + b) ^ (c & d) - e;
        f.display(true);

        cout << "\n\nSequence operations: \n\n";
        cout << "Erase elements from 3 to 6 in D:";
        d.erase(3, 6);
        d.display(true);

        cout << "\nConcat B with C:";
        b.concat(c);
        b.display(true);

        cout << "\nChange A in C from 1 position:";
        c.change(a, 1);
        c.display(true);
    }
    else
    {
        set_seq a(N), b(N), c(N), d(N), e(N), f;

        a.display(); b.display(); c.display(); d.display(); e.display();

        cout << "\n\nSet operation (A | B)^(C & D) \\ E:\n";
        f = ((a + b) ^ (c & d)) - e;
        f.display();

        cout << "\n\nSequence operations: \n";
        cout << "Erase elements from 3 to 6 in D:";
        d.erase(3, 6);
        d.display();

        cout << "\nConcat B with C:";
        b.concat(c);
        b.display();

        cout << "\nChange A in C from 1 position:";
        c.change(a, 1);
        c.display();

    }
    return 0;
}
