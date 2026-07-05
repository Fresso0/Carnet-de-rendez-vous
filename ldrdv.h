#ifndef LDRDV_H
#define LDRDV_H

#include "rdv.h"
#include "participant.h"
#include "ldpersonne.h"

class RDV;
class ldpersonne;


class chainonRDV
{
    friend class ldrdv;

public:
    RDV& getRDV() { return d_rdv; }
    chainonRDV* getSuiv() const { return d_suiv; }
    chainonRDV* getPrec() const { return d_prec; }

private:

    chainonRDV(RDV & r):d_rdv{r},d_prec{nullptr},d_suiv{nullptr}{};
    RDV d_rdv;
    chainonRDV *d_prec;
    chainonRDV *d_suiv;
};

class ldrdv
{

    friend class ldpersonne;
public:

    ldrdv();
    ldrdv(const ldrdv&l);
    ~ldrdv();


    chainonRDV* getTete() { return d_tete; }
    bool operator==(const ldrdv &l);
    bool operator!=(const ldrdv &l);
    bool chercheParNom(const string &nom, RDV &r);
    void ajtRdv(RDV &r);
    void modifierRdv(string nom,int heured,int heuref,int j, int m, int a);
    bool suprRdv(RDV &r);
    void ajouterParticipantRdv(string nom, participant &p);
    string afficherRdvDate(int j, int m, int a);
    void retirerParticipantRdv(string nom,participant &p);
    string afficherListe()const;
    void sauvegardeListe(string nomFichier);
    void enregistrerDansFichier(const RDV& r);
    void chargerRdvDepuisFichier();
    string afficherParticipantsRDV(string nom);
    bool participantsDispo(RDV r,RDV nouveauRdv,ldpersonne lp);




private:
    chainonRDV *d_tete;
    bool chargementEnCours = false;

};


#endif
