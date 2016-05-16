#include "union.h"
template<typename T>
union_set<T>::union_set(const T first_elt){
    value=first_elt;
    weight=1;
    label=this;
}

template<typename T>
void merge(union_set<T> *set1, union_set<T> *set2){
    union_set<T> * lab1, * lab2;
    lab1=set1->find();
    lab2=set2->find();
    if (lab1->weight>lab2->weight){
        lab2->label=lab1->label;
        lab1->weight+=lab2->weight;
    }else{
        lab1->label=lab2->label;
        lab2->weight+=lab1->weight;
    }
}

template<typename T>
union_set<T> * union_set<T>::find(){
    union_set<T> * course=this, * label_final=nullptr, * save;
    unsigned long ex_weight=0;
    while(course->label!=course)
        course=course->label;
    label_final=course;
    course=this;
    while(course->label!=course){
        save=course->label;
        save->weight-=ex_weight;
        ex_weight+=save->weight;
        course->label=label_final;
        course=save;
    }
    return label_final;
}

template<typename T>
void union_set<T>::add(T new_value){
    union_set * new_union = new union_set<T>(new_value);
    merge(new_union,this);
}
