#ifndef LDPERSONNE_H
#define LDPERSONNE_H


#include "participant.h"
#include "rdv.h"



class participant;
class ldrdv;

class chainonParticipant
{   friend class menu;
    friend class ldpersonne;

public:
    participant& getParticipant() { return d_participant; }
    chainonParticipant* getSuiv() const { return d_suiv; }
    chainonParticipant* getPrec() const { return d_prec; }


private:

    chainonParticipant(participant p):d_participant(p),d_prec(nullptr),d_suiv(nullptr){};
    participant d_participant;
    chainonParticipant *d_prec,*d_suiv;


};

class ldpersonne
{

public:
    //CONSTRUCTEURS/DESTRUCTEUR
    ldpersonne();
    ldpersonne(const ldpersonne& lp);
    ~ldpersonne();

    chainonParticipant* getTete() { return d_tete; }

    //SURCHARGES
    bool operator==(const ldpersonne& lp);
    bool operator!=(const ldpersonne& lp);

    //AUTRES
    //bool trouverParticipant(string nom, participant &p);
    bool trouverParticipant(string nom, string prenom, participant &p);

    void ajouterParticipant(participant p);
    void modifierParticipant(string nom, string prenom, string email, string tel);
    bool supprimerParticipant(participant &p);
    void ajouterRDVP(string nom, string prenom, RDV r);
    void supprimerRDVP(string nom, string prenom, RDV r);



    string afficherParticipants() const;
    string afficherRDVs(string nom);
    void chargerParticipantsDepuisFichier();
    bool estOccupe(participant p, int j, int hd, int hf, int m, int a);
    int nombreRDVs(participant p);
    bool chevaucheRDV(participant p,RDV r);
    void suppRDV(RDV r);
    bool participantsDisponibles(RDV rdv, RDV nouvrdv);
    void sauvegarderListe(string nomFichier);
    void sauvegarderRDVs();


    void retirerRDV(RDV r);


private:
    chainonParticipant *d_tete;
    bool chargementEnCours = false;
    friend class menu;

};


#endif // LDPERSONNE_H
