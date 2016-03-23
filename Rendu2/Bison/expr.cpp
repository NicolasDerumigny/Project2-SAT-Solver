#include "expr.hpp"
#include "clause.h"
#include "var.h"
#include "litt.h"
#include "formule.h"

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

void EConst::eval()
{
    formule* new_form;
    new_form= new formule();
    new_form->set_formule(value, false);
    this->form=new_form;
}

void EConst::eval_tseitin()
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
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
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

    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(formRoot->f_ClauseUnsatisfied);

    formRoot->merge(form1);
    formRoot->merge(form2);
    this->form=formRoot;
}

/*****************************************/
/**********  Conjonctions   *************/
/***************************************/

EConj::EConj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EConj::to_string()
{
    return "(" + op1->to_string() + ")" + " /\\ " + "(" +  op2->to_string() + ")";
}

void EConj::eval()//op1 et op2 seront des formules
{
    op1->eval();
    op2->eval();
    op2->form->merge(op1->form);
    this->form=op2->form;
}

void EConj::eval_tseitin()//op1 et op2 seront des formules
{
    op1->eval_tseitin();
    op2->eval_tseitin();
    (op1->form)->merge(op2->form);

    formule *formRoot= new formule,
            *form1 = new formule,
            *form2 = new formule,
            *form3 = new formule;

    formRoot->set_formule_tseitin(false);//la variable est Ep,
    form1->set_formule_tseitin(false);//la variable est Ep1
    form2->set_formule_tseitin(false);//la variable est Ep2
    form3->f_ClauseUnsatisfied = new clause;
    form3->l_ClauseUnsatisfied = form3->f_ClauseUnsatisfied;
    form3->l_ClauseUnsatisfied->next_clause=nullptr;

    {
        litt *l11 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
    }

    {
        litt *l21 = new litt;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //nonp
        form2->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l21;
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
        l33->set_litt(form2->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //Ep2
        l31->next_litt=l32;
        l32->next_litt=l33;
        form3->f_ClauseUnsatisfied->f_ElementAlive=l31;
        form3->f_ClauseUnsatisfied->l_ElementAlive=l33;
    }

    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form3->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form3->f_ClauseUnsatisfied);
    form2->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form3->f_ClauseUnsatisfied);

    formRoot->merge(form1);
    formRoot->merge(form2);
    formRoot->merge(form3);
    (op1->form)->merge(formRoot);
    this->form=op1->form;
}


/*************************************/
/********  Disjonctions   ***********/
/***********************************/

EDisj::EDisj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

string EDisj::to_string()
{
    return "(" + op1->to_string() + ") "+ " \\/ " + " (" + op2->to_string() +")";
}

void EDisj::eval()
//we assume that the formula are only composed of one unique clause, that we merge together
{
    op1->eval();
    op2->eval();
    op1->form->f_ClauseUnsatisfied->merge(op2->form->f_ClauseUnsatisfied);
    op1->form->f_ClauseSatisfied->merge(op2->form->f_ClauseSatisfied);
    this->form=op1->form;
	delete op2->form;
}

void EDisj::eval_tseitin()
//we assume that the formula are only composed of one unique clause, that we merge together
{
    op1->eval_tseitin();
    op2->eval_tseitin();
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

    {//form1
        litt *l11 = new litt,
                *l13 = new litt;
        l11->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,true);//nnEp
        l13->set_litt(form3->f_ClauseUnsatisfied->f_ElementAlive->variable,false);//Ep2
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        l11->next_litt=l13;
        form1->f_ClauseUnsatisfied->l_ElementAlive=l13;
    }

    {//form2
        litt *l21 = new litt, *l22 = new litt;
        l21->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
    }

    {//form3
        litt *l31 = new litt;
        l31->set_litt(formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        form3->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l31;
        form3->f_ClauseUnsatisfied->l_ElementAlive=l31;
    }

    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form3->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    form3->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form1->f_ClauseUnsatisfied);


    formRoot->merge(form1);
    formRoot->merge(form2);
    formRoot->merge(form3);
    (op1->form)->merge(formRoot);
    this->form=op1->form;
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

void VNot::eval()
{
    formule* new_form = new formule();
    new_form->set_formule(value, true);
    this->form=new_form;
}

void VNot::eval_tseitin()
{
    ENot *new_expr=new ENot(new EConst (this->value));
    new_expr->eval_tseitin();
    this->form = new_expr->form;
    delete new_expr;
}


/************************************/
/********  ENOT  *******************/
/**********************************/

ENot::ENot(Expr * e1) : op1(e1) {}

string ENot::to_string()
{
    return "¬(" + op1->to_string() + ")";
}

void ENot::eval()
{
    cerr<<"Fatal error : Negated expression shall never be used in input, use -tseitin for this."<<endl;
    exit(-1);
}


void ENot::eval_tseitin()
{
    op1->eval_tseitin();
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
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on ajoute Ep1
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
    }

    formRoot->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(formRoot->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form1->f_ClauseUnsatisfied);

    formRoot->merge(form1);
    formRoot->merge(form2);
    (op1->form)->merge(formRoot);
    this->form=op1->form;
}

/************************************/
/********  XOR  ********************/
/**********************************/

EXor::EXor(Expr * e1, Expr * e2) : EDisj(new EConj(e2, new ENot(e1)),new EConj(e1, new ENot(e2))) {}

void EXor::eval()
{
    cerr<<"Fatal error : Xor expression shall never be used in input, use -tseitin for this."<<endl;
    exit(-1);
}

/************************************/
/********  IMPLY   *****************/
/**********************************/


EImply::EImply(Expr * e1, Expr * e2) : EDisj(e2, new ENot(e1)) {}

void EImply::eval()
{
    cerr<<"Fatal error : Negated expression shall never be used in input, use -tseitin for this."<<endl;
    exit(-1);
}

/************************************/
/********  EQ   ********************/
/**********************************/

EEq::EEq(Expr * e1, Expr * e2) : EDisj(new EConj(e1,e2),new EConj(new ENot(e1),new ENot(e2))) {}

void EEq::eval()
{
    cerr<<"Fatal error : Negated expression shall never be used in input, use -tseitin for this."<<endl;
    exit(-1);
}
