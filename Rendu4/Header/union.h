#ifndef UNION_H
#define UNION_H
#ifdef __clang__
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

template<typename T>
class union_set{
private :
    union_set * label;
    unsigned long weight;
public :
    union_set(const T first_elt);
    T value;


    void add(T new_value);
    union_set * find();
};

template <typename T>
void merge(union_set<T> * set1, union_set<T> * set2);

#endif // UNION_H
