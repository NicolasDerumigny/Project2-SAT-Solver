#include "prooftree.h"

void setNewProofTree(){
    prooftree_fs.open("proof_tree.tex",fstream::out);
    prooftree_fs<<"\\documentclass[a4paper,12pt]{article}\n\n";
    prooftree_fs<<"\\usepackage{bussproofs}\n";
    prooftree_fs<<"\\usepackage{amssymb}\n";
    prooftree_fs<<"\\usepackage[utf8]{inputenc}\n\n";
    prooftree_fs<<"\\usepackage{lscape}\n";
    prooftree_fs<<"\\begin{document}\n\n";
    prooftree_fs<<"\\begin{landscape}\n";
    prooftree_fs<<"\\begin{prooftree}\n";
}


void writeAxiom(){
    prooftree_fs<<"\\AxiomC{}\n";
}

void writeDeduction(assignation* ass){
    prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str(false,false)<<"\\vdash X_{"<<ass->variable->id<<"}=";
    prooftree_fs<<to_string(ass->variable->value)<<"$}\n";
}

void writeAssign(assignation* ass){
    if (bets[assignations.size()-1]){
        //c'est un paris : deduction a partir des deux premisses
        //(x=vrai prouve faux, x=faux prouve faux)
        if(ass->bet){
            prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str(false,false)<<",\\; X_{"<<ass->variable->id<<"}=0";
            prooftree_fs<<"\\vdash \\bot$}\n";
            prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str(false,false)<<"\\vdash X_{"<<ass->variable->id<<"}=1$}\n";

        }else{
            //si on a deja annulé le paris
            prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str(false,false)<<",\\; X_{"<<ass->variable->id<<"}=1";
            prooftree_fs<<"\\vdash \\bot$}\n";
            prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str(false,false)<<"\\vdash X_{"<<ass->variable->id<<"}=0$}\n";
        }
    }
}

void writeBinary(){
     prooftree_fs<<"\\BinaryInfC{$"<<instance->proof_str(false,false)<<"\\vdash \\bot$}\n";
}


void deleteProofTree(){
    //nope, il suffit de prends ceux qui sont a faux, pas a vrai !!
    prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str(true,true)<<"\\to \\bot$}\n";
    prooftree_fs<<"\\end{prooftree}\n\n";
    prooftree_fs<<"\\end{landscape}\n";
    prooftree_fs<<"\\end{document}";
    prooftree_fs.close();
}
