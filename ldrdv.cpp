#include "ldrdv.h"
#include "ldpersonne.h"
#include "rdv.h"
#include "participant.h"
#include <iostream>
#include <sstream>
#include <fstream>

//CONSTRUCTEURS/DESTRUCTEURS
ldrdv::ldrdv():d_tete{nullptr}
{}
ldrdv::ldrdv(const ldrdv&l):d_tete{nullptr}   //constructeur par recopie
{
    chainonRDV *crtLR= l.d_tete;
    chainonRDV *prec;

    while(crtLR!=nullptr)
    {
        chainonRDV *nouv=new chainonRDV{crtLR->d_rdv};
        if(d_tete==nullptr)
        {
            d_tete=nouv;
        }
        else
        {
            //chainage
            prec->d_suiv=nouv;
            nouv->d_prec=prec;
        }
        //passage au suivant
        prec=nouv;
        crtLR=crtLR->d_suiv;
    }

}
ldrdv::~ldrdv()
{
    chainonRDV *crt=d_tete;
    while(crt!=nullptr)
    {
        chainonRDV *tmp=crt->d_suiv;
        delete crt;
        crt=tmp;
    }
    d_tete=nullptr;
}

//SURCHARGES
bool ldrdv::operator==(const ldrdv &l)
{
    if(this==&l) return true;

    chainonRDV *crt=d_tete;
    chainonRDV *crtL=l.d_tete;

    while(crt!=nullptr && crtL!=nullptr && crt->d_rdv==crtL->d_rdv) //tant quon n est pas a la fin
    {
        //passage au suivant                                                         //et que le chainon courant de this est egal
        crt=crt->d_suiv;                                                              // au chainon courant de lp
        crtL=crtL->d_suiv;
    }

    if(crt==nullptr && crtL==nullptr) return true;
    return false;
}

bool ldrdv::operator!=(const ldrdv &l)
{
    return !((*this)==l);
}

//AUTRES METHODES
bool ldrdv::chercheParNom(const string &nom, RDV &r) //nom rdv=unique
{
    chainonRDV *crt=d_tete;
    while(crt!=nullptr && crt->d_rdv.retourneNom()!=nom)
    {
        crt=crt->d_suiv;
    }
    if(crt==nullptr) return false;
    else
    {
        r=crt->d_rdv;
        return true;
    }
}



void ldrdv::ajtRdv(RDV &r)
{
    chainonRDV *nouv = new chainonRDV{r};

    // Vérification de l'allocation mémoire
    if (!nouv) {
        cerr << "Erreur d'allocation mémoire" << endl;
        return;
    }

    // Cas de la liste vide
    if (d_tete == nullptr) {
        d_tete = nouv;
        enregistrerDansFichier(r);  // Enregistrer dans le fichier après ajout
        return;
    }

    chainonRDV *crt = d_tete;
    chainonRDV *prec = nullptr;

    // Cas où on insère au début de la liste
    if (nouv->d_rdv < d_tete->d_rdv) {
        nouv->d_suiv = d_tete;
        d_tete->d_prec = nouv;
        d_tete = nouv;
        enregistrerDansFichier(r);
        return;
    }

    // Cas où on insère après le premier élément
    while (crt != nullptr && nouv->d_rdv > crt->d_rdv) {
        prec = crt;
        crt = crt->d_suiv;
    }

    // Cas où le rendez-vous doit être ajouté à la fin de la liste
    if (crt == nullptr) {
        prec->d_suiv = nouv;
        nouv->d_prec = prec;
    } else {
        // Cas général : insertion au milieu
        nouv->d_suiv = crt;
        nouv->d_prec = prec;
        if (crt != nullptr) {
            crt->d_prec = nouv;
        }
        if (prec != nullptr) {
            prec->d_suiv = nouv;
        }
    }

    enregistrerDansFichier(r);
}




void ldrdv::enregistrerDansFichier(const RDV& r)
{
    if (!chargementEnCours) // Si le chargement est en cours, ne pas toucher au fichier
    {
        // Ouvrir le fichier en mode écriture pour tout réécrire (ici, on remplace l'ancien fichier)
        ofstream fichier("rdv.log", ios::trunc);
        if (!fichier) {
            cerr << "Erreur : Impossible d'ouvrir le fichier rdv.log" << endl;
            return;
        }

        // Parcourir la liste chaînée des RDV et réécrire le contenu du fichier
        chainonRDV* crt = d_tete; // Démarrer à partir de la tête de la liste
        while (crt != nullptr) {
            RDV rdv = crt->d_rdv;  // Récupérer l'objet RDV du maillon actuel

            // Écrire les informations du rendez-vous dans le fichier
            fichier << rdv.retourneNom() << " "
                    << rdv.retourneJour() << "/"
                    << rdv.retourneMois() << "/"
                    << rdv.retourneAn() << " "
                    << rdv.retourneHeureDebut() << "-"
                    << rdv.retourneHeureFin() << "\n";

            crt = crt->d_suiv;  // Passer au maillon suivant
        }

        fichier.close();  // Fermer le fichier après l'écriture
    }
}






void ldrdv::chargerRdvDepuisFichier()
{
    ifstream fichier("rdv.log");
    if (!fichier)
    {
        cerr << "Erreur : Impossible d'ouvrir le fichier rdv.log" << endl;
        return;
    }

    chargementEnCours = true; // Activation du mode "chargement"

    string ligne;
    while (getline(fichier, ligne))
    {
        stringstream ss(ligne);
        string nom;
        int jour, mois, an, heureDebut, heureFin;

        // Lecture du nom, de la date et des heures
        ss >> nom >> jour;
        char slash1, slash2;
        ss >> slash1 >> mois >> slash2 >> an; // La date est au format jour/mois/année
        ss >> heureDebut; // Heure de début
        char tiret; // Tiret entre les heures
        ss >> tiret >> heureFin; // Heure de fin

        // Vérification que les données sont valides
        if (nom.empty() || ss.fail()) {
            cerr << "Erreur de format dans le fichier rdv.log" << endl;
            continue;
        }

        // Création du rendez-vous
        RDV r(nom, jour, mois, an, heureDebut, heureFin);
        ajtRdv(r);  // Ajouter le rendez-vous à la liste chaînée
    }

    fichier.close();
    chargementEnCours = false; // Fin du chargement
}





void ldrdv::modifierRdv(string nom, int heured, int heuref, int j, int m, int a)
{
    chainonRDV *crt=d_tete;
    while(crt!=nullptr && crt->d_rdv.retourneNom()!=nom)
    {
        crt=crt->d_suiv;
    }
    crt->d_rdv.setHoraireJour(heured,heuref,j,m,a);
}


bool ldrdv::suprRdv(RDV &r)
{
    if(d_tete!=nullptr)
    {
        chainonRDV *crt=d_tete;
        chainonRDV *prec=nullptr;

        while(crt!=nullptr && crt->d_rdv!=r)
        {
            prec=crt;
            crt=crt->d_suiv;
        }



        if(crt!=nullptr)
        {
            chainonRDV *suiv=crt->d_suiv;
            delete crt;

            if(prec!=nullptr)   //pas en tete
            {
                prec->d_suiv=suiv;

            }
            else // en tete
            {
                d_tete=suiv;
            }
            if(suiv!=nullptr) //pas a la fin
            {
                suiv->d_prec=prec;
            }
            return true;


        }
        else{
            return false;
        }
    }
    else
    {
        return false;
    }
}

void ldrdv::ajouterParticipantRdv(string nom, participant &p) //ajt un participant p a un rdv nommé nom
{
    chainonRDV *crt=d_tete;
    while(crt!=nullptr && crt->d_rdv.retourneNom()!=nom)  //le nom est unique donc recherche par nom seulement
    {
        crt=crt->d_suiv;
    }

    // ✅ Vérification avant d'accéder à `crt->d_rdv`
    if (crt != nullptr)
    {
        crt->d_rdv.ajtParticipant(p);
    }


}


void ldrdv::retirerParticipantRdv(string nom,participant &p)
{
    chainonRDV *crt=d_tete;
    while(crt!=nullptr && crt->d_rdv.retourneNom()!=nom)  //le nom est unique donc recherche par nom seulement
    {
        crt=crt->d_suiv;
    }

    // ✅ Vérification avant d’accéder à `d_rdv`
    if (crt != nullptr)
    {
        crt->d_rdv.retirerParticipant(p);
    }

}
/*
void ldrdv::retirerParticipantRdv(string nom, participant &p) {
    chainonRDV *crt = d_tete;
    while (crt != nullptr && crt->d_rdv.retourneNom() != nom) {
        crt = crt->d_suiv;  // Recherche le rendez-vous avec le nom spécifié
    }

    if (crt != nullptr) {
        // Si le rendez-vous existe, on retire le participant
        crt->d_rdv.retirerParticipant(p);  // Retirer le participant du rendez-vous
    }
}*/




string ldrdv::afficherRdvDate(int j,int m, int a)
{
    string contenuListe = "";
    int cpt=0;
    chainonRDV *crt=d_tete;
    string vide="";
    while(crt!=nullptr)
    {ostringstream oss;

        if(crt->d_rdv.retourneJour()==j && crt->d_rdv.retourneMois()==m && crt->d_rdv.retourneAn()==a)
        {


            oss<<crt->d_rdv;
            contenuListe += oss.str() + "\n";
            cpt++;
        }
        crt=crt->d_suiv;
    }

    if(cpt==0) return vide;
    else
        return contenuListe;
}



//PR TEST

string ldrdv::afficherListe()const
{
    string contenuListe = "";

    chainonRDV *crt=d_tete;
    while(crt!=nullptr)
    {
        ostringstream oss;
        oss<<crt->d_rdv;
        contenuListe += oss.str() + "\n";
        crt=crt->d_suiv;
    }
    if(contenuListe=="")
        contenuListe="Aucun résultat";

    return contenuListe;

}

void ldrdv::sauvegardeListe(string nomFichier)
{
    int i=0;
    chainonRDV *crt=d_tete;
    while(crt!=nullptr)
    {
        i++;
        crt=crt->d_suiv;
    }
    ofstream ost;
    ost.open(nomFichier);
    crt=d_tete;
    ost<<i<<endl;
    while(crt!=nullptr)
    {
        ost<<crt->d_rdv.retourneNom()<<" ";
        ost<<crt->d_rdv.retourneJour()<<"/"<<crt->d_rdv.retourneMois()<<"/"<<crt->d_rdv.retourneAn()<<" ";
        ost<<crt->d_rdv.retourneHeureDebut()<<" ";
        ost<<crt->d_rdv.retourneHeureFin()<<" ";
        ost<<endl;
        crt=crt->d_suiv;
    }
    ost.close();
}


string ldrdv::afficherParticipantsRDV(string nom)
{
    string contenuListe = "";
    chainonRDV *crt=d_tete;
    while(crt!=nullptr && crt->d_rdv.retourneNom()!=nom) //cherche le rdv nommé nom
    {

        crt=crt->d_suiv;
    }
    ostringstream oss;
    oss<<crt->d_rdv.afficherListeParticipants();
    contenuListe += oss.str() + "\n";


    return contenuListe;
}

bool ldrdv::participantsDispo(RDV r, RDV nouveauRdv, ldpersonne lp)  //true = tlm dispo
{
    chainonRDV *crt=d_tete;
    bool pasdisp=false;
    while(crt!=nullptr && crt->d_rdv!=r )
    {
        crt=crt->d_suiv;
    }

    return lp.participantsDisponibles(crt->d_rdv,nouveauRdv);




}

