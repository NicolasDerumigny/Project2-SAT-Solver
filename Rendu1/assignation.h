#ifndef ASSIGNATION_H
#define ASSIGNATION_H

class var;

class assignation
{
public:
    void set_assign(var* variable,bool bet);
	void updateLitt(bool alive);
	void updateClause(bool alive);
	void updateStatus(bool alive);

    bool bet;
    //True si l'assignation est dûe à un paris
    var* variable;
};

#endif // ASSIGNATION_H
