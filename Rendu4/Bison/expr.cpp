#include "expr.hpp"
#include "../Header/clause.h"
#include "../Header/var.h"
#include "../Header/litt.h"
#include "../Header/formule.h"



/*************************************/
/**********  Constants   ************/
/***********************************/
Expr::~Expr(){

}


EConst::EConst(int val) : value(val) {}

std::string EConst::to_string()
{
    std::ostringstream oss;
    oss << "v" << value;
    std::string result = oss.str();
    return result;
}

void EConst::eval()
{
    formule* new_form;
    new_form= new formule();
    new_form->set_formule((unsigned int) value, false);
    this->form=new_form;
}

void EConst::eval_tseitin()
{
    formule *form1 = new formule,
            *form2 = new formule;

    var* new_var=nullptr;
    set_var_tseitin(&new_var);//la variable est Ep,
    varRoot=new_var;//la variable est Ep,
    form1->set_formule((unsigned int) value, false);//la variable est x
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;

    {
        litt *l11 = new litt;
        l11->set_litt(varRoot,true);
        // on rajoute non Ep
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
        l11->prev_litt=form1->f_ClauseUnsatisfied->f_ElementAlive;
    }

    {
        litt *l21 = new litt, *l22 = new litt;
        l21->set_litt(varRoot,false);
        //on ajoute Ep
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //on rajoute non x
        l21->next_litt=l22;
        l22->prev_litt=l21;
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
    }

    varRoot->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);

    form1->merge(form2);
    this->form=form1;
}

/*****************************************/
/**********  Conjonctions   *************/
/***************************************/

EConj::EConj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

std::string EConj::to_string()
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

    formule *form1 = new formule,
            *form2 = new formule,
            *form3 = new formule;

    var* new_var=nullptr;
    set_var_tseitin(&new_var);//la variable est Ep,
    varRoot=new_var;//la variable est Ep,
    form1->set_formule_var(op1->varRoot,false);//la variable est Ep1
    form2->set_formule_var(op2->varRoot,false);//la variable est Ep2
    form3->f_ClauseUnsatisfied = new clause;
    form3->l_ClauseUnsatisfied = form3->f_ClauseUnsatisfied;

    {
        litt *l11 = new litt;
        l11->set_litt(varRoot,true);
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
        l11->prev_litt=form1->f_ClauseUnsatisfied->f_ElementAlive;
    }

    {
        litt *l21 = new litt;
        l21->set_litt(varRoot,true);
        //nonp
        form2->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l21;
        l21->prev_litt=form2->f_ClauseUnsatisfied->f_ElementAlive;
    }

    {
        litt *l31 = new litt,
                *l32 = new litt,
                *l33 = new litt;
        l31->set_litt(varRoot,false);
        //Ep
        l32->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //Ep1
        l33->set_litt(form2->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        //Ep2
        l31->next_litt=l32;
        l32->prev_litt=l31;
        l32->next_litt=l33;
        l33->prev_litt=l32;
        form3->f_ClauseUnsatisfied->f_ElementAlive=l31;
        form3->f_ClauseUnsatisfied->l_ElementAlive=l33;
    }

    varRoot->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form3->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form3->f_ClauseUnsatisfied);
    form2->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form3->f_ClauseUnsatisfied);

    form1->merge(form2);
    form1->merge(form3);
    (op1->form)->merge(form1);
    this->form=op1->form;
}


/*************************************/
/********  Disjonctions   ***********/
/***********************************/

EDisj::EDisj(Expr * e1, Expr * e2) : op1(e1), op2(e2) {}

std::string EDisj::to_string()
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

    formule *form1 = new formule,
            *form2 = new formule,
            *form3 = new formule;

    var* new_var=nullptr;
    set_var_tseitin(&new_var);//la variable est Ep,
    varRoot=new_var;//la variable est Ep,
    form1->set_formule_var(op1->varRoot,false);//la variable est Ep1
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;
    form3->set_formule_var(op2->varRoot,true);//la variable est non Ep2

    {//form1
        litt *l11 = new litt,
                *l13 = new litt;
        l11->set_litt(varRoot,true);//nnEp
        l13->set_litt(form3->f_ClauseUnsatisfied->f_ElementAlive->variable,false);//Ep2
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        l11->prev_litt=form1->f_ClauseUnsatisfied->f_ElementAlive;
        l11->next_litt=l13;
        l13->prev_litt=l11;
        form1->f_ClauseUnsatisfied->l_ElementAlive=l13;
    }

    {//form2
        litt *l21 = new litt, *l22 = new litt;
        l21->set_litt(varRoot,false);
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,true);
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
        l22->prev_litt=l21;
    }

    {//form3
        litt *l31 = new litt;
        l31->set_litt(varRoot,false);
        form3->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l31;
        form3->f_ClauseUnsatisfied->l_ElementAlive=l31;
        l31->prev_litt=form3->f_ClauseUnsatisfied->f_ElementAlive;
    }

    varRoot->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form3->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    form3->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form1->f_ClauseUnsatisfied);

    form1->merge(form2);
    form1->merge(form3);
    (op1->form)->merge(form1);
    this->form=op1->form;
}

/************************************/
/********  VNOT              *******/
/**********************************/


VNot::VNot(int val) : value(val) {}

std::string VNot::to_string()
{
    std::ostringstream oss;
    oss << "¬v" << value;
    std::string result = oss.str();
    return result;
}

void VNot::eval()
{
    formule* new_form = new formule();
    new_form->set_formule((unsigned int) value, true);
    this->form=new_form;
}

void VNot::eval_tseitin()
{
    formule *form1 = new formule,
            *form2 = new formule;
    var* new_var=nullptr;
    set_var_tseitin(&new_var);//la variable est Ep,
    varRoot=new_var;
    form1->set_formule((unsigned int) value, true);//la variable est non x
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;

    {
        litt *l11 = new litt;
        l11->set_litt(varRoot,true);
        // on rajoute non Ep
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
        l11->prev_litt=form1->f_ClauseUnsatisfied->f_ElementAlive;
    }

    {
        litt *l21 = new litt, *l22 = new litt;
        l21->set_litt(varRoot,false);
        //on ajoute Ep
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on rajoute x
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
        l21->next_litt=l22;
        l22->prev_litt=l21;
    }

    varRoot->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);

    form1->merge(form2);
    this->form=form1;
}


/************************************/
/********  ENOT  *******************/
/**********************************/

ENot::ENot(Expr * e1) : op1(e1) {}

std::string ENot::to_string()
{
    return "¬(" + op1->to_string() + ")";
}

void ENot::eval()
{
    std::cerr<<"Fatal error : Negated expression shall never be used in input, use -tseitin for this."<<std::endl;
    exit(-1);
}


void ENot::eval_tseitin()
{
    op1->eval_tseitin();
    formule *form1 = new formule,
            *form2 = new formule;

    var* new_var=nullptr;
    set_var_tseitin(&new_var);//la variable est Ep,
    varRoot=new_var;
    //la variable est Ep,
    form1->set_formule_var(op1->varRoot,true);
    //la variable est non Ep1
    form2->f_ClauseUnsatisfied = new clause;
    form2->l_ClauseUnsatisfied = form2->f_ClauseUnsatisfied;

    {
        litt *l11 = new litt;
        l11->set_litt(varRoot,true);
        //on ajoute non Ep
        form1->f_ClauseUnsatisfied->l_ElementAlive=l11;
        form1->f_ClauseUnsatisfied->f_ElementAlive->next_litt=l11;
        l11->prev_litt=form1->f_ClauseUnsatisfied->f_ElementAlive;
    }

    {
        litt *l21 = new litt,
                *l22 = new litt;
        l21->set_litt(varRoot,false);
        //on ajoute Ep
        l22->set_litt(form1->f_ClauseUnsatisfied->f_ElementAlive->variable,false);
        //on ajoute Ep1
        l21->next_litt=l22;
        l22->prev_litt=l21;
        form2->f_ClauseUnsatisfied->f_ElementAlive=l21;
        form2->f_ClauseUnsatisfied->l_ElementAlive=l22;
    }

    varRoot->clauseInto.push_back(form2->f_ClauseUnsatisfied);
    varRoot->clauseInto.push_back(form1->f_ClauseUnsatisfied);
    form1->f_ClauseUnsatisfied->f_ElementAlive->variable->clauseInto.push_back(form2->f_ClauseUnsatisfied);

    form1->merge(form2);
    (op1->form)->merge(form1);
    this->form=op1->form;
}

/************************************/
/********  XOR  ********************/
/**********************************/

EXor::EXor(Expr * e1, Expr * e2) : EDisj(new EConj(e2, new ENot(e1)),new EConj(e1, new ENot(e2))) {}

void EXor::eval()
{
    std::cerr<<"Fatal error : Xor expression shall never be used in input, use -tseitin for this."<<std::endl;
    exit(-1);
}

/************************************/
/********  IMPLY   *****************/
/**********************************/


EImply::EImply(Expr * e1, Expr * e2) : EDisj(e2, new ENot(e1)) {}

void EImply::eval()
{
    std::cerr<<"Fatal error : Negated expression shall never be used in input, use -tseitin for this."<<std::endl;
    exit(-1);
}

/************************************/
/********  EQ   ********************/
/**********************************/

EEq::EEq(Expr * e1, Expr * e2) : EDisj(new EConj(e1,e2),new EConj(new ENot(e1),new ENot(e2))) {}

void EEq::eval()
{
    std::cerr<<"Fatal error : Negated expression shall never be used in input, use -tseitin for this."<<std::endl;
    exit(-1);
}
