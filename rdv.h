#ifndef RDV_H
#define RDV_H

#include <iostream>
#include <vector>
using namespace std;

class participant;

class RDV
{
    friend class participant;
    friend class ldpersonne;
    friend class ldrdv;
public:

    RDV();
    RDV(const string &nom,const int &j,const int& m, const int& a,const int& hd,const int&hf);

    // Constructeur de copie explicite
    RDV(const RDV& other) = default;

    friend ostream& operator<<(ostream &os,const RDV& r);
    RDV &operator=(const RDV &r);
    bool operator<(const RDV &r);
    bool operator>(const RDV &r);
    bool operator==(const RDV &r);
    bool operator!=(const RDV &r);

    string retourneNom() const;
    int retourneHeureDebut() const;
    int retourneHeureFin() const;
    int retourneJour() const;
    int retourneMois() const;
    int retourneAn() const;

    //AFFICHAGE
    string afficherListeParticipants() const;

    //AUTRES METHODES
    bool chevauchementRDV(const RDV &r);  //true si this et r se chevauchent

    void ajtParticipant(const participant &p);
    void retirerParticipant(participant &p);
    vector<participant> getParticipants() const;

    void setHoraireJour(int hd,int hf,int j,int m, int a);



private:
    string d_nom;
    int d_jour,d_mois,d_an;
    int d_hDebut,d_hFin;
    vector<participant> d_listeParticipants;

};

#endif















