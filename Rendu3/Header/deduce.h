#ifndef DEDUCE_H
#define DEDUCE_H
#include <vector>
#include <utility>
#include <algorithm>
#include "formule.h"
#include "litt.h"
#include "global_variable_extern.h"
using namespace std;

bool assignUniqueLitt();
bool assignUniquePolarity();
bool assignUnique();

#endif // DEDUCE_H
