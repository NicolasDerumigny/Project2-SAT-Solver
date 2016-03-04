#include <vector>
#include "../Header/assignation.h"
#include <stdlib.h>
#include <time.h>

var* getFreeVar() {
	for (var* s:v_var) {
        if (s != nullptr && s->value == -1) {
			return s;
		}
	}
	return nullptr;
}

var* getRandFreeVar() {
	srand(time(NULL));
	var* new_var = nullptr;
	bool no_more = true;
	for (var* s:v_var) {
		if (s != nullptr && s->value == -1) {
			no_more = false;
			break;
		}
	}
	if (no_more)
		return nullptr;
	unsigned int i_var;
	while (new_var == nullptr) {
		i_var = rand() % v_var.size();
		if (v_var[i_var] != nullptr && v_var[i_var]->value == -1) {
			new_var = v_var[i_var];
		}
	}
	return new_var;
}

//voir var::assignValue avec value=1 par défaut et bet=true
