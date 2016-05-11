#ifndef DEF_EXPR
#define DEF_EXPR

#include "../Header/var.h"
#include "../Header/litt.h"
#include "../Header/clause.h"
#include "../Header/formule.h"
#include <iostream>
#include <sstream>
#include <string>


/***********************************/
/*********  Expressions   **********/
/***********************************/

class Expr
{
public:
    Expr(){}
    virtual std::string to_string()=0;
    virtual void eval()=0;
    virtual void eval_tseitin()=0;
    formule *form=nullptr;
    var *varRoot=nullptr;
    virtual ~Expr();
};

/***********************************/
/**********  Constants   ***********/
/***********************************/

class EConst : public Expr
{
public:
    EConst(int val);
    std::string to_string();
    void eval();
    void eval_tseitin();
	~EConst(){}
    char padding[4];
private:
    int value;
};

/***********************************/
/********** Conjonctions ***********/
/***********************************/

class EConj : public Expr
{
public:
    EConj(Expr * e1, Expr * e2);
    std::string to_string();
    void eval();
    void eval_tseitin();
	~EConj(){delete op1; delete op2;}
private:
    Expr * op1, * op2;
};

/***********************************/
/********   Disjonctions    ********/
/***********************************/

class EDisj : public Expr
{
public:
    EDisj(Expr * e1, Expr * e2);
    std::string to_string();
    void eval();
    void eval_tseitin();
	~EDisj(){delete op1; delete op2;}
private:
    Expr * op1, * op2;
};

/***********************************/
/********   Implications    ********/
/***********************************/


class EImply : public EDisj
{
public:
    EImply(Expr * e1, Expr * e2);
    void eval();
    ~EImply(){}
};

/***********************************/
/********   XOR    ********/
/***********************************/

class EXor : public EDisj
{
public:
    EXor(Expr * e1, Expr * e2);
    void eval();
    ~EXor(){}
};

/***********************************/
/********   Equal    ********/
/***********************************/

class EEq : public EDisj
{
public:
    EEq(Expr * e1, Expr * e2);
    void eval();
    ~EEq(){/*rien ! le destructeur de EDisj fera le reste*/}
};

/***********************************/
/********** Negation d'une expression ***********/
/***********************************/

class ENot : public Expr
{
public:
    ENot(Expr * e1);
    std::string to_string();
    void eval();
    void eval_tseitin();
    ~ENot(){}
private:
    Expr * op1;
};

/***********************************************/
/********   Negation d'une variable   ********/
/***********************************************/

class VNot : public Expr
{
public:
    VNot(int val);
    std::string to_string();
    void eval();
    void eval_tseitin();
    ~VNot(){}
    char padding[4];
private:
    int value;
};

#endif // ! DEF_EXPR

