#include "expr.hpp"

using namespace std;

/*************************************/
/**********  Constants   ***********/
/***********************************/

EConst::EConst(int val) : value(val) {}

string EConst::to_string()
{
    ostringstream oss;
    oss << "v" << value;
    string result = oss.str();
    return result;
}

int EConst::eval()
{
    return value;
}

/****************************************/
/**********  Conjonctions   ***********/
/***************************************/

EConj::EConj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EConj::to_string()
{
    return "(" + op1->to_string() + " /\ " +  op2->to_string() + ")";
}

int EConj::eval()
{
    return op1->eval() + op2->eval();
}

/************************************/
/********  Disjonctions   **********/
/***********************************/

EDisj::EDisj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EDisj::to_string()
{
    return "(" + op1->to_string() + " \/ " +  op2->to_string() + ")";
}

int EDisj::eval()
{
    return op1->eval() * op2->eval();
}

/************************************/
/********  XOR  *******************/
/***********************************/

EXor::EXor(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EXor::to_string()
{
    return "(" + op1->to_string() + " X " +  op2->to_string() + ")";
}

int EXor::eval()
{
    return op1->eval() * op2->eval();
}

/************************************/
/********  IMPLY   ****************/
/***********************************/

EImply::EImply(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EImply::to_string()
{
    return "(" + op1->to_string() + " → " +  op2->to_string() + ")";
}

int EImply::eval()
{
    return op1->eval() * op2->eval();
}

/************************************/
/********  EQ   ********************/
/***********************************/

EEq::EEq(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EEq::to_string()
{
    return "(" + op1->to_string() + " ↔ " +  op2->to_string() + ")";
}

int EEq::eval()
{
    return op1->eval() * op2->eval();
}

/***********************************/
/********  ENOT  *****************/
/***********************************/

ENot::ENot(Expr * e1) : op1(e1) {}

string ENot::to_string()
{
    return "¬(" + op1->to_string() + ")";
}

int ENot::eval()
{
    return op1->eval();
}

/***********************************/
/********  VNOT                *******/
/***********************************/

VNot::VNot(int val) : value(val) {}

string VNot::to_string()
{
    ostringstream oss;
    oss << "¬v" << value;
    string result = oss.str();
    return result;
}

int VNot::eval()
{
    return value;
}

