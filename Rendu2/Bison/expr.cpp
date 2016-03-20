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
    formule *formRoot= new formule,
            *form1 = new formule,
            *form2 = new formule;

    formRoot->set_formule_tseitin(false);//la variable est Ep,
    form1->set_formule(value, false);//la variable est x

    {
        litt *l11 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        // on rajoute non Ep
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
    }

    {
        litt *l21 = new litt, *l22 = new litt;
        form2->f_ClauseUnsatisfied = new clause;
        form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on ajoute Ep
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //on rajoute non x
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
    }

    formRoot->merge(form1);
    formRoot->merge(form2);
    this->form=formRoot;
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

formule *EConj::eval_tseitin()//op1 et op2 seront des formules
{
    op1->form=op1->eval();
    op2->form=op2->eval();
    (op1->form)->merge(op2->form);

    formule *formRoot= new formule,
            *form1 = new formule,
            *form2 = new formule,
            *form3 = new formule;

    formRoot->set_formule_tseitin(false);//la variable est Ep,
    form1->set_formule_tseitin(false);//la variable est Ep1
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;
    form3->set_formule_tseitin(false);//la variable est Ep2

    {
        litt *l11 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
    }

    {
        litt *l21 = new litt;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        form2->f_ClauseUnsatisfied->l_ElementAlive=l21;
    }

    {
        litt *l31 = new litt,
                *l32 = new litt,
                *l33 = new litt;
        l31->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //Ep
        l32->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //Ep1
        l33->set_litt(form3->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //Ep2
        l31->next_litt=l32;
        l32->next_litt=l33;
        form3->f_ClauseUnsatisfied->l_ElementAlive=l31;
        form3->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l33;
    }



    formRoot->merge(form1);
    formRoot->merge(form2);
    formRoot->merge(form3);
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
    op1->form->f_ClauseUnsatisfied->merge(op2->form->f_ClauseUnsatisfied);
    op1->form->f_ClauseSatisfied->merge(op2->form->f_ClauseSatisfied);
    this->form=op1->form;
    return this->form;
}

formule* EDisj::eval_tseitin()
//we assume that the formula are only composed of one unique clause, that we merge together
{
    op1->form=op1->eval();
    op2->form=op2->eval();
    (op1->form)->merge(op2->form);

    formule *formRoot= new formule,
            *form1 = new formule,
            *form2 = new formule,
            *form3 = new formule;

    formRoot->set_formule_tseitin(false);//la variable est Ep,
    form1->set_formule_tseitin(false);//la variable est Ep1
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;
    form3->set_formule_tseitin(true);//la variable est non Ep2

    {
        litt *l11 = new litt,
                *l13 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        l13->set_litt(form3->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        form1->f_ClauseUnsatisfied->l_ElementAlive=l13;
        l11->next_litt=l13;
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
    }

    {
        litt *l21 = new litt, *l22 = new litt;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
    }

    {
        litt *l31 = new litt;
        l31->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        form3->f_ClauseUnsatisfied->l_ElementAlive=l31;
        form3->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l31;
    }

    (op1->form)->merge(formRoot);
    formRoot->merge(form1);
    formRoot->merge(form2);
    formRoot->merge(form3);
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
    formule* new_form = new formule();
    new_form->set_formule(value, true);
    this->form=new_form;
    return this->form;
}

formule* VNot::eval_tseitin()
{
    formule *formRoot= new formule,
            *form1 = new formule,
            *form2 = new formule;

    formRoot->set_formule_tseitin(false);//la variable est Ep,
    form1->set_formule(value,false);//la variable est x
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;

    {
        litt *l11 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        // on rajoute non Ep
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
    }

    {
        litt *l21 = new litt, *l22 = new litt;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on ajoute Ep
        l22->set_litt(form2->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //on rajoute non x
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
    }

    formRoot->merge(form1);
    formRoot->merge(form2);
    this->form=formRoot;
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
    formule *formRoot= new formule,
            *form1 = new formule,
            *form2 = new formule;

    formRoot->set_formule_tseitin(false);
    //la variable est Ep,
    form1->set_formule_tseitin(true);
    //la variable est non Ep1
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;

    {
        litt *l11 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //on ajoute non Ep
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
    }

    {
        litt *l21 = new litt,
                *l22 = new litt;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on ajoute Ep
        l21->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on ajoute Ep1
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
    }


    formRoot->merge(form1);
    formRoot->merge(form2);
    (op1->form)->merge(formRoot);
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
