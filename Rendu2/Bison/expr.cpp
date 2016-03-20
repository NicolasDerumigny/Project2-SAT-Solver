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

formule* EConst::eval_tseitin()
{
    formule* new_form;
    new_form= new formule();
    new_form->new_formule_tsetin_var(value,true);
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

formule* EConj::eval_tseitin()//op1 et op2 seront des formules
{
    op1->form=op1->eval();
    op2->form=op2->eval();
    (op1->form)->merge(op2->form);

    //TODO !!!
    litt* lroot,l11,l12,l13,l21,l22,l31,l32;
    clause* croot,c11,c12,c13,c21,c22,c31,c32;
    formule* formRoot,form1,form2;
    var* p, p1, p2;
    p=new var();
    p1=new var();
    p2=new var();
    int id=v_var_tseitin.size();
    p.set_var(id);
    p1.set_var(id+1);
    p2.set_var(id+2);
    root=new_litt;

    lroot.set_litt(p,true);
    croot.set_clause(lroot);


    l11=new litt;
    l11.set_litt(p,true);
    c11.set_clause(l11);
    l12=new litt;
    l12.set_litt(p1,false);
    c12.set_clause(l12);
    c11.merge(c12);
    l13=new litt;
    l13.set_litt(p2,false);
    c13.set_clause(l13);
    c11.merge(c13);


    l21=new litt;
    l21.set_litt(p,false);
    c21.set_clause(l21);
    l22=new litt;
    l22.set_litt(p1,true);
    c22.set_clause(l22);
    c21.merge(c22);

    l31=new litt;
    l31.set_litt(p,false);
    c31.set_clause(l31);
    l32=new litt;
    l32.set_litt(p2,true);

    (op1->form)->merge(formRoot);
    this->form=op1->form;
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
    op1->form->f_ClauseUnsatisfied->merge(op2->f_ClauseUnsatisfied);
    op1->form->f_ClauseSatisfied->merge(op2->f_ClauseSatisfied);
    this->form=op1->form;
    }
    return this->form;
}

formule* EDisj::eval_tseitin()
//we assume that the formula are only composed of one unique clause, that we merge together
{
    op1->form=op1->eval_tseitin();
    op2->form=op2->eval_tseitin();
    (op1->form)->merge(op2->form);
    formule* form_tseitin;
    form_tseitin = new formule();
    form_tseitin->new_formule_tsetin_disj();
    (op1->form)->merge(form_tseitin);
    this->form=op1->form;
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

formule* VNot::eval_tseitin()
{
    formule* new_form;
    new_form= new formule();
    new_form->new_formule_tsetin_var(value,false);
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
    cerr<<"Fatal error : Negated expression shall never be use in input, use -tseitin for this."<<endl;
    exit(-1);
    return op1->eval();
}


formule* ENot::eval_tseitin()
{
    op1->form=op1->eval_tseitin();
    formule* form_tseitin;
    form_tseitin = new formule();
    form_tseitin->new_formule_tsetin_enot();
    (op1->form)->merge(form_tseitin);
    this->form=op1->form;
    return this->form;
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
