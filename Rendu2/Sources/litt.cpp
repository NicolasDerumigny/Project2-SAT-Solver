#include "../Header/litt.h"

void litt::set_litt(var* var_entry, bool neg){
    this->neg=neg;
    this->variable=var_entry;
    this->next_litt=nullptr;
}

void litt::print(){
    if (this->neg){
        cout<<"\t\t¬v"<<this->variable->id;
        cout<<" (and v"<<this->variable->id<<" = "<<this->variable->value<<")"<<endl;
    }
    else{
        cout<<"\t\tv"<<this->variable->id;
        cout<<" (and v"<<this->variable->id<<" = "<<this->variable->value<<")"<<endl;
    }
}
