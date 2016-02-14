#include <vector>
//#include <stdlib.h>
//#include <time.h>

var* getFreeVar() {
	for (auto& s:v_var) {
		if (s->value == -1) {
			return s;
		}
	}
	return nullptr;
}

void assignValue(var* variable) {
	variable->value=1;//Par défaut on assigne à la variable la valeur 1.
	
}
