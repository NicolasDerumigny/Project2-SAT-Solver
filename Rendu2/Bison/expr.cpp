#include "expr.hpp"
#include "../Header/clause.h"
#include "../Header/var.h"
#include "../Header/litt.h"
#include "../Header/formule.h"

using namespace std;

/*************************************/
/**********  Constants   ************/
/***********************************/

EConst::EConst(int val) : value(val) {}

string EConst::to_string()
{
    ostringstream oss;
    oss << "v" << value;
    string result = oss.str();
    return result;
}

formule* EConst::eval()
{
    formule* new_form;
    new_form= new formule();
    new_form->set_formule(value, false);
    this->form=new_form;
    return this->form;
}

/*****************************************/
/**********  Conjonctions   *************/
/***************************************/

EConj::EConj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EConj::to_string()
{
    return "(" + op1->to_string() + ")" + " /\\ " + "(" +  op2->to_string() + ")";
}

formule* EConj::eval()//op1 et op2 seront des formules
{
    op1->form=op1->eval();
    op2->form=op2->eval();
    op2->form->merge(op1->form);
    this->form=op2->form;
    return this->form;
}

/*************************************/
/********  Disjonctions   ***********/
/***********************************/

EDisj::EDisj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EDisj::to_string()
{
    return op1->to_string() + " \\/ " +  op2->to_string();
}

formule* EDisj::eval()
//we assume that the formula are only composed of one unique clause, that we merge together
{
    op1->form=op1->eval();
    op2->form=op2->eval();
    (op1->form)->merge(op2->form);
    this->form=op1->form;
    return this->form;
}

formule* EDisj::eval_tseitin()
//we assume that the formula are only composed of one unique clause, that we merge together
{
    op1->form=op1->eval_tseitin();
    op2->form=op2->eval_tseitin();
    (op1->form)->merge(op2->form);
    this->form=op1->form;
    var* var_form, var_form1, var_form2;
    var_form = new var;
    var_form1 = new var;
    var_form2 = new var;
    v_var_tseitin.push_back(var_form);
    v_var_tseitin.push_back(var_form1);
    v_var_tseitin.push_back(var_form2);

    return this->form;
}

/************************************/
/********  VNOT              *******/
/**********************************/


VNot::VNot(int val) : value(val) {}

string VNot::to_string()
{
    ostringstream oss;
    oss << "¬v" << value;
    string result = oss.str();
    return result;
}

formule* VNot::eval()
{
    formule* new_form;
    new_form= new formule();
    new_form->set_formule(value, true);
    this->form=new_form;
    return this->form;
}

/************************************/
/********  ENOT  *******************/
/**********************************/

ENot::ENot(Expr * e1) : op1(e1) {}

string ENot::to_string()
{
    return "¬(" + op1->to_string() + ")";
}

formule* ENot::eval()
{
    return op1->eval();
}


/************************************/
/********  XOR  ********************/
/**********************************/
/*
EXor::EXor(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EXor::to_string()
{
    return "(" + op1->to_string() + " X " +  op2->to_string() + ")";
}

formule* EXor::eval()
{
    return op1->eval() * op2->eval();
}
*/
/************************************/
/********  IMPLY   *****************/
/**********************************/
/*

EImply::EImply(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EImply::to_string()
{
    return "(" + op1->to_string() + " → " +  op2->to_string() + ")";
}

formule* EImply::eval()
{
    return op1->eval() * op2->eval();
}
*/
/************************************/
/********  EQ   ********************/
/**********************************/
/*
EEq::EEq(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EEq::to_string()
{
    return "(" + op1->to_string() + " ↔ " +  op2->to_string() + ")";
}

formule* EEq::eval()
{
    return op1->eval() * op2->eval();
}
*/
