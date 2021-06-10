#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <vector>
#include <chrono>

#include "AVL.h"
#include "set_seq.h"

void prepare_and(set_seq& one, set_seq& two, const int quantity, int lim)
{
	for (int i = 0; i < quantity; ++i)
    {
		int x = rand()%lim;
        one.insert(x);
        two.insert(x);
	}
}

size_t set_seq::tags = 0;

int main(int argc, char* argv[])
{
    using namespace std::chrono;
    // srand((unsigned int)7);
    ofstream fout("in.txt");
    srand((unsigned int)time(nullptr));

    auto MaxMul = 50;
    int middle_power = 0, set_count = 0;
    int iterations = 201;
    int max_ = 2000;
    bool debug = false;

    if (argc == 4)
    {
        iterations = atoi(argv[1]);
        max_ = atoi(argv[2]);
        debug = atoi(argv[3]);
    }
    auto Used = [&] (set_seq & t){ middle_power += t.power(); ++set_count;};
    auto rand = [] (int d) { return std::rand()%d; };
    auto DebOut = [debug] (set_seq & t) { if(debug) { t.display(); }};

    for (int p = rand(max_) + 5; iterations > 0; --iterations, p = rand(max_) + 5)
    {
        int U = 3*p; // Устанавливаем мощность универсума
        //=== Данные ===
        set_seq A(p, U), B(p, U), C(p, U), D(p, U), E(p, U), F(0, U);

        auto t1 = high_resolution_clock::now();

        int q_and(rand(MaxMul) + 1);
        prepare_and(A, F, q_and, U);
        if (debug) A.display(); Used(A);
        if (debug) F.display(); Used(F);

        //=== Цепочка операций ===
        // (Операция пропускается (skipped!), если аргументы некорректны)
        // Идёт суммирование мощностей множеств и подсчёт их количества,
        // измеряется время выполнения цепочки
        if (debug) cout << "\n=== F&=A ===(" << q_and << ") ";
        F&=A; DebOut(F); Used(F);

        int q_union(rand(MaxMul) + 1);
        prepare_and(B, F, q_union, U);
        if (debug) F.display(), B.display(); middle_power += q_union; Used(B);
        if (debug) cout << "\n=== F+=B ===(" << q_union << ") ";
        F+=B; DebOut(F); Used(F);

        int q_sub(rand(MaxMul) + 1);
        prepare_and(B, F, q_sub, U);
        if (debug) F.display(), B.display(); middle_power += q_sub; Used(B);
        if (debug) cout << "\n=== F-=B ===(" << q_sub << ") ";
        F-=B; DebOut(F); Used(F);

        int q_s_sub(rand(MaxMul) + 1);
        prepare_and(C, F, q_s_sub, U);
        if (debug) F.display(), C.display(); middle_power += q_s_sub; Used(C);
        if (debug) cout << "\n=== F^=C ===(" << q_s_sub << ") ";
        F^=C; DebOut(F); Used(F);

        int a, b;
        if (F.power()) a = rand(F.power()), b = rand(F.power());
        else a = b = 0;
        if (debug) cout << "\n=== F.erase (" << a << "," << b << ")===";
        if (a>b) swap(a, b);
        F.erase(a, b); DebOut(F); Used(F);

        if (debug) cout << "\n=== F.concat(E) ===";
        if (debug) E.display(), F.display(); Used(E);
        F.concat(E); DebOut(F); Used(F);

        int e = rand(F.power());
        if (debug) cout << "\n=== F.change (D, " << e << ") ===";
        if (debug) D.display(); Used(D);
        F.change(D, e); DebOut(F); Used(F);

        auto t2 = high_resolution_clock::now();
        auto dt = duration_cast<duration<double>>(t2-t1);
        middle_power /= set_count;
        if (!debug) fout << middle_power << ' ' << dt.count() << endl; //Выдача в файл
        cout << "\n=== END === (" << p << " : " << set_count << " * " << middle_power << " DT=" << (dt.count()) <<")\n";
        middle_power = 0; set_count = 0;
    }
    return 0;
}
