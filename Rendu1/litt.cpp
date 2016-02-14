#include "litt.h"

void litt::set_litt(var* var_entry, bool neg){
    this->neg=neg;
    this->variable=var_entry;
}

void litt::print(){
    if (this->neg)
        cout<<"\t\t¬v"<<this->variable->id<<endl;
    else
        cout<<"\t\tv"<<this->variable->id<<endl;
}
