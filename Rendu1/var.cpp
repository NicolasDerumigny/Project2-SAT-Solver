#include "var.h"

void var::set_var(int id){
    this->id=id;
    this->value=-1;
}

void var::updateStatus(bool alive){
	for (auto& cl:this->clauseInto)
		for (auto& li:cl->mElementAlive)
			if (alive == true)
				//TODO
}
