#include <vector>

var* getFreeVar() {
	for (auto& s:v_var) {
		if (s.second->value == -1) {
			return s.second;
		}
	}
	return nullptr;
}
