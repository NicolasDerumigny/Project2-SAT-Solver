#include <vector>
#include "assignation.h"
//#include <stdlib.h>
//#include <time.h>

var* getFreeVar() {
	for (auto& s:v_var) {
        if (s != nullptr && s->value == -1) {
			return s;
		}
	}
	return nullptr;
}

//voir var::assignValue avec value=1 par défaut et bet=true
