#ifndef SET_SEQ_H
#define SET_SEQ_H

using Set = tree; // множество
using Iterator = tree_iterator; // итератор множества
using Seq = std::vector<Iterator>; // последовательность элементов

// ИТЕРАТОР ВСТАВКИ
template<typename Container, typename Iter = Iterator>
class outiterator: public iterator<output_iterator_tag, typename Container::value_type>
{
    protected:
        Container& container; // Контейнер для вставки элементов
        Iter iter; // Текущее значение итератора чтения
    public:
        outiterator(Container &c, Iter it) : container(c), iter(it) {}
        // Присваивание - вставка ключа в контейнер
        const outiterator<Container>& operator= (const typename Container::value_type& value)
        {
            iter = container.insert(iter, value);
            return *this;
        }

        // Присваивание копии фиктивное
        const outiterator<Container>& operator= (const outiterator<Container> &) { return *this; }
        outiterator<Container>& operator* () {return *this;} // Разыменование - пустая операция
        outiterator<Container>& operator++ () {return *this;} // Инкремент - пустая операция
        outiterator<Container>& operator++ (int) {return *this;} // Инкремент - пустая операция
};

// Функция для создания итератора вставки - аргумент при вызове алгоритма
template<typename Container, typename Iter>
inline outiterator<Container, Iter> outinserter(Container& c, Iter It)
{
    return outiterator<Container, Iter>(c, It);
}

class set_seq
/*
    Комбинированная структура, хранящая множество с последовательностью.

    Множество - АВЛ-Дерево.
    Последовательность - вектор итераторов на элементы множества.
*/
{
    static size_t tags; // количетво тегов
    char tag; // тег струтуры
    Set set_; // множество
    Seq seq_; // последовательность

    public:
        using key_type = int;
        using value_type = int;
        using key_compare = less<int>;

        set_seq(): tag('A' + tags++) {}
        set_seq(int);
        set_seq(set_seq &&);
        set_seq(const set_seq &);
        template <typename iter>
        set_seq(iter, iter);
        set_seq& operator=(const set_seq &);
        set_seq& operator=(set_seq &&);

        pair<Iterator, bool> insert(int, Iterator);
        Iterator insert(const Iterator& where, const int& k) { return insert(k, where).first; }

        // операции над последовательностью
        void erase(int, int);
        void concat(const set_seq&);
        void change(const set_seq&, int);

        // операции над множеством
        set_seq& operator &= (const set_seq &);
        set_seq operator & (const set_seq & other) const
        { set_seq result(*this); return (result &= other); }
        set_seq& operator+= (const set_seq & other);
        set_seq operator + (const set_seq & other) const
        {set_seq result(*this); return (result += other);}
        set_seq& operator -= (const set_seq &);
        set_seq operator - (const set_seq & other) const
        {set_seq result(*this); return (result -= other);}
        set_seq& operator ^= (const set_seq &);
        set_seq operator ^ (const set_seq & other) const
        {set_seq result(*this); return (result ^= other);}

        void display(bool);
        int power() const { return seq_.size(); }
};
// создание случайного множества с заданной мощностью
set_seq::set_seq(int power) : set_seq()
{
    for(int i = 0; i < power; ++i)
        seq_.push_back(set_.insert(std::rand()%(power*2)).first);
}
// копирование с перемещением
set_seq::set_seq (set_seq && source)
    : tag(source.tag), set_(std::move(source.set_)), seq_(std::move(source.seq_)) {}
// копирование
set_seq::set_seq (const set_seq & source): tag(source.tag)
{
    for (auto x : source.seq_)
        seq_.push_back(set_.insert(*x).first);
}
// констуктор из подпоследовательности
template <typename iter>
set_seq::set_seq (iter b, iter e): set_seq()
{
    for (auto i = b; i != e; ++i)
        seq_.push_back(set_.insert(*i).first);
}


pair<Iterator, bool> set_seq::insert(int key, Iterator it = nullptr)
{
    auto r = set_.insert(key, it);
    seq_.push_back(r.first);
    return r;
}

void set_seq::erase(int p1, int p2)
/*
    Из последовательности исключается часть,
    ограниченная порядковыми номерами от p1 до p2.
*/
{
    int p = power();
    p1 = std::min(p1, p);
    p2 = std::min(p2+1, p);
    if(p1 <= p2)
    {
        Set temp;
        Seq stemp;
        for(int i = 0; i < p1; ++i)
            stemp.push_back(temp.insert(*seq_[i]).first);
        for(int i = p2; i < p; ++i)
            stemp.push_back(temp.insert(*seq_[i]).first);
        set_.swap(temp);
        seq_.swap(stemp);
    }
}

void set_seq::concat(const set_seq & other)
/*
    Вторая последовательность подсоединяется
    к концу первой, образуя ее продолжение.
*/
{
    for(auto x : other.seq_)
        seq_.push_back(set_.insert(*x).first);
}

void set_seq::change(const set_seq & other, int pos = 0)
/*
    Вторая последовательность заменяет элементы
    первой, начиная с заданной позиции pos.
*/
{
    if(pos >= power())
        concat(other);
    else
    {
        Seq stemp(seq_.begin(), seq_.begin() + pos);
        // при помощи итератора с обратной вставкой присоединяем последовательность other
        std::copy(other.seq_.begin(), other.seq_.end(), back_inserter(stemp));

        int q = pos + other.power();
        if (q < power())
            std::copy(seq_.begin() + q, seq_.end(), back_inserter(stemp));

        Set temp;
        seq_.clear();
        for (auto x : stemp)
            seq_.push_back(temp.insert(*x).first);
        set_.swap(temp);
    }
}

set_seq& set_seq::operator= (const set_seq & other)
{
    set_seq temp;
    for (auto x: other.seq_)
        temp.seq_.push_back(temp.set_.insert(*x).first);
    seq_.swap(temp.seq_);
    set_.swap(temp.set_);
    return *this;
}

set_seq& set_seq::operator= (set_seq && other)
{
    seq_.swap(other.seq_);
    set_.swap(other.set_);
    return *this;
}

set_seq& set_seq::operator&= (const set_seq & other)
{
    set_seq temp;
    set_intersection(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

set_seq& set_seq::operator+= (const set_seq & other)
{
    set_seq temp;
    set_union(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

set_seq& set_seq::operator-= (const set_seq & other)
{
    set_seq temp;
    set_difference(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

set_seq& set_seq::operator^= (const set_seq & other)
{
    set_seq temp;
    set_symmetric_difference(set_.begin(), set_.end(),
        other.set_.begin(), other.set_.end(),
        outinserter(temp, Iterator(nullptr)));
    set_.swap(temp.set_);
    seq_.swap(temp.seq_);
	return *this;
}

void set_seq::display(bool tree_flag = false)
{
    std::cout << "\nSET (" << tag << "): ";
    for(auto x : set_)
        std::cout << x << ' ';
    std::cout << "\nSEQUENCE: (" << tag << "): < ";
    for (auto i : seq_)
        std::cout << *i << ' ';
    std::cout << ">\n";
    if (tree_flag)
        set_.display();
}

#endif
