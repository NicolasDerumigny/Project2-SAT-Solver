#ifndef DEF_EXPR
#define DEF_EXPR

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
    virtual int eval()=0;
};

/***********************************/
/**********  Constants   ***********/
/***********************************/

class EConst : public Expr
{
public:
    EConst(int val); 
    virtual std::string to_string();
    virtual int eval();
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
    virtual std::string to_string();
    virtual int eval();
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
    virtual std::string to_string();
    virtual int eval();
private:
    Expr * op1, * op2;
};

/***********************************/
/********   Implications    ********/
/***********************************/

class EImply : public Expr
{
public:
    EImply(Expr * e1, Expr * e2);
    virtual std::string to_string();
    virtual int eval();
private:
    Expr * op1, * op2;
};

/***********************************/
/********   XOR    ********/
/***********************************/

class EXor : public Expr
{
public:
    EXor(Expr * e1, Expr * e2);
    virtual std::string to_string();
    virtual int eval();
private:
    Expr * op1, * op2;
};

/***********************************/
/********   Equal    ********/
/***********************************/

class EEq : public Expr
{
public:
    EEq(Expr * e1, Expr * e2);
    virtual std::string to_string();
    virtual int eval();
private:
    Expr * op1, * op2;
};

/***********************************/
/********** Negation d'une expression ***********/
/***********************************/

class ENot : public Expr
{
public:
    ENot(Expr * e1);
    virtual std::string to_string();
    virtual int eval();
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
    virtual std::string to_string();
    virtual int eval();
private:
    int value;
};

#endif // ! DEF_EXPR

