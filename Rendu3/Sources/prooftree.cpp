#include "prooftree.h"

void setNewProofTree(){
    prooftree_fs.open("proof_tree.tex",fstream::out);
    prooftree_fs<<"\\documentclass[a4paper,12pt]{article}\n\n";
    prooftree_fs<<"\\usepackage{bussproofs}\n";
    prooftree_fs<<"\\usepackage{amssymb}\n";
    prooftree_fs<<"\\usepackage[utf8]{inputenc}\n";
    prooftree_fs<<"\\usepackage[francais]{babel}\n\n";
    prooftree_fs<<"\\begin{document}\n\n";
    prooftree_fs<<"\\begin{prooftree}\n";
}


//writingfristassign pour initialiser les premisses !!!!!

void writingAssign(assignation* ass){
    if (ass->bet){
        //c'est un paris : deduction a partir des deux premisses
        //(x=vrai prouve faux, x=faux prouve faux)
        if(ass->variable->value==1){
            prooftree_fs<<"\\AxiomC{}\n";
            //l'axiome de la derivation en cours
            prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str()<<",\\; X_{"<<ass->variable->id<<"}=0";
            prooftree_fs<<"\\vdash \\bot$}\n";
            prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str()<<"\\vdash X_{"<<ass->variable->id<<"}=1$}\n";
            prooftree_fs<<"\\AxiomC{}\n";
            //l'axiome de la prochaine derivation
        }

        if(ass->variable->value==0)
            prooftree_fs<<"\\BinaryInfC{$"<<instance->proof_str()<<"\\vdash \\bot$}\n";
            //si on a deja annulé le paris

    }else{
        //il s'agit d'une deduction : equivalence logique : deduction unitaire"
        prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str()<<"\\to \\bot$}\n";
    }
}


void deleteProofTree(){
    //nope, il suffit de prends ceux qui sont a faux, pas a vrai !!
    prooftree_fs<<"\\BinaryInfC{$"<<instance->proof_str()<<"\\vdash \\bot$}\n";
    prooftree_fs<<"\\UnaryInfC{$"<<instance->proof_str()<<"\\to \\bot$}\n";
    prooftree_fs<<"\\end{prooftree}\n\n";
    prooftree_fs<<"\\end{document}";
    prooftree_fs.close();
}
