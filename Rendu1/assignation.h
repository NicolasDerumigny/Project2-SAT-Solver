#ifndef ASSIGNATION_H
#include "var.h"
#define ASSIGNATION_H

class assignation
{
public:
    void set_assign(var* variable,bool bet);

    bool bet;//True si l'assignation est dûe à un paris
    var* variable;
};

#endif // ASSIGNATION_H
