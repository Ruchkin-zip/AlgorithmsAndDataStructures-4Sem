#ifndef AVL_H
#define AVL_H

using namespace std;

const int B[] = {-1,1};

// УЗЕЛ ДЕРЕВА
struct node
{
    int key; // ключ узла
    signed char balance_factor; // баланс (разность поддеревьев узла)
    int duplicates; // количество дубликатов ключа
    node* nodes[2]; // левое и правое поддерево

    node(int k): key(k), balance_factor(0), duplicates(0) {nodes[0] = nodes[1] = nullptr;}
    node(const node&) = delete;
    ~node(){delete nodes[1]; delete nodes[0];}
    void display(char**, int, int);
};

using Stack = stack<pair<node*, int>>;

//ИТЕРАТОР ЧТЕНИЯ
struct tree_iterator: public std::iterator<std::forward_iterator_tag, int>
{
    node* ptr; //Реальный указатель на элемент контейнера
    Stack st; // стек с путем от корня дерева

    tree_iterator(node *p = nullptr) : ptr(p) {}
    tree_iterator(node *p, Stack s) : ptr(p), st(s) {}
    bool operator== (const tree_iterator & other) const { return ptr == other.ptr; }
    bool operator!= (const tree_iterator & other) const { return !(ptr == other.ptr); }
    tree_iterator& operator++(); // Инкремент по контейнеру
    tree_iterator operator++(int) { tree_iterator temp(*this); ++*this; return temp; }
    pointer operator->() { return &ptr->key; } //Косвенное разыменование
    reference operator*(){ return ptr->key; } //Прямое разыменование
};
// прохождение узлов по порядку
tree_iterator& tree_iterator::operator++()
{
    if (!ptr)
        return *this;

    if (ptr->nodes[1])
    {
        st.push(make_pair(ptr, 1));
        ptr = ptr->nodes[1];
        for(;ptr->nodes[0];ptr = ptr->nodes[0]) st.push(make_pair(ptr, 0));
    }
    else
    {
        pair <node*, int> pp(ptr, 1);
        while (!st.empty() && pp.second) {pp = st.top(); st.pop();}
        if (pp.second)
            ptr = nullptr;
        else
            ptr = pp.first;
    }

    return *this;
}

// АВЛ ДЕРЕВО
class tree
{
    node *root; // корень дерева
    int h, count; // высота и мощность дерева

    node* find_element(node*, int) const;
    public:
        // конструкторы/деструкторы:
        tree(): root(nullptr), h(0), count(0) {}
        tree(const tree &other): tree() {for(auto x = other.begin(); x != other.end(); ++x) insert(*x);}
        tree(tree && other): tree() {swap(other);}
        ~tree(){ delete root; }
        void swap(tree &);

        // доступ:
        tree_iterator begin() const;
        tree_iterator end() const { return tree_iterator(nullptr); }
        bool empty() const { return (root == nullptr); }
        int size() const { return count; }
        int height() const {return h;}

        // вставка/удаление
        pair<tree_iterator, bool> insert(int, tree_iterator = tree_iterator(nullptr));
        tree_iterator insert(const tree_iterator& where, const int& k) { return insert(k, where).first; }
        bool erase(int);

        // другие методы
        void display();
        tree_iterator find(int) const;
        tree & operator= (const tree &);
        tree & operator= (tree && other) {swap(other); return *this;}
};

void tree::swap(tree & other)
{
    std::swap(root, other.root);
    std::swap(h, other.h);
    std::swap(count, other.count);
}

tree_iterator tree::begin() const
{
    Stack st;
    node *p(root);
    if (p)
        for(;p->nodes[0];p = p->nodes[0]) st.push(make_pair(p, 0));

    return tree_iterator(p, move(st));
}

node* tree::find_element(node* _node, int key) const
{
    if (_node == nullptr || key == _node->key)
        return _node;
    else
        return find_element(_node->nodes[!(key < _node->key)], key);
}

tree_iterator tree::find(int value) const
{
    node* element = find_element(root, value);
    if (element == nullptr)
        return end();
    else
        return tree_iterator(element);
}

pair<tree_iterator, bool> tree::insert(int k, tree_iterator where)
{
    node *q{root}, *p;
    Stack St;

    if (!where.ptr) // свободная вставка
    {
        if (empty())
        {
            root = new node(k);
            count = h = 1;
            St.push(make_pair(root, 1));
            return make_pair(tree_iterator(root, move(St)), true);
        }
    }
    else // с места предыдущей вствки
    {
        q = where.ptr;
        St = move(where.st);
    }

    while (q) // поиск места вставки
    {
        if (k == q->key)
        {
            ++(q->duplicates);
            return (make_pair(tree_iterator(q, move(St)), false));
        }
        p = q;
        int a{k < q->key}; //Спускаемся вниз по дереву
        q = q->nodes[!a];
        St.push(make_pair(p, 1 - a));
    }

    q = new node(k); //Вставлен новый узел в точке q
    if (p)
        p->nodes[!(k < p->key)] = q;
    else
        root = q;

    node *new_node = q;

    while (!St.empty())
    //Движемся к корню, опрокидывая балансы, ищем не нуль
    {
        auto pa = St.top(); St.pop();
        p = pa.first;
        int a = pa.second;

        if (!(p->balance_factor))
        {
            p->balance_factor = B[a]; //Замена 0 на +-1
            if (p == root) //Дерево подросло
            {
                ++h;
                break;
            }
            else
                q = p;
        }
        else if (p->balance_factor == -B[a])
        { // Сбалансировалось
            p->balance_factor = 0; //Замена +-1 на 0
            break;
        }
        // Перебалансировка
        else if (p->balance_factor == q->balance_factor)
        {	//Случай 1: Однократный поворот
            p->nodes[a] = q->nodes[1 - a];
            q->nodes[1 - a] = p;
            p->balance_factor = q->balance_factor = 0;
            if(p == root)
                p = root = q;
            else
                St.top().first->nodes[St.top().second] = p = q;
            break;
        }
        else
        { //Случай 2: Двукратный поворот
            node *r(q->nodes[1 - a]);
            p->nodes[a] = r->nodes[1 - a];
            q->nodes[1 - a] = r->nodes[a];
            r->nodes[1 - a] = p;
            r->nodes[a] = q;
            if (r->balance_factor == B[a])
            {
                p->balance_factor = -B[a];
                q->balance_factor = 0;
            }
            else if (r->balance_factor == -B[a])
            {
                 p->balance_factor = 0;
                 q->balance_factor = B[a];
            }
            else
            {
                p->balance_factor = q->balance_factor = 0;
            }
            r->balance_factor = 0;
            if (p == root)
                p = root = r;
            else
                St.top().first->nodes[St.top().second] = p = r;
            break;
        }
    }

    ++count;
    return make_pair(tree_iterator(new_node, move(St)), true);
}

tree & tree::operator= (const tree & other)
{
    tree temp;
    for (auto x: other)
        temp.insert(x);
    swap(temp);
    return *this;
}

#endif
