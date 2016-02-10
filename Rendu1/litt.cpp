#include "litt.h"

void litt::set_litt(var* var_entry, bool neg){
    this->neg=neg;
    this->variable=var_entry;
}
