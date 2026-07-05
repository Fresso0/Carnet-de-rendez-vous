#include "participant.h"
#include "rdv.h"
#include "ldpersonne.h"
#include "ldrdv.h"
#include <sstream>
#include <fstream>


//CONSTRUCTEURS/DESTRUCTEUR
ldpersonne::ldpersonne():d_tete{nullptr}{}

ldpersonne::ldpersonne(const ldpersonne& lp):d_tete{nullptr}  //CONSTRCTEUR PAR RECOPIE
{
    chainonParticipant *crtLP= lp.d_tete;
    chainonParticipant *prec;

    while(crtLP!=nullptr)
    {
        chainonParticipant *nouv = new chainonParticipant(crtLP->d_participant);
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
        crtLP=crtLP->d_suiv;
    }

}
ldpersonne::~ldpersonne()
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr)
    {
        chainonParticipant *tmp=crt->d_suiv;
        delete crt;
        crt=tmp;
    }
    d_tete=nullptr;
}


//SURCHARGES
bool ldpersonne::operator==(const ldpersonne& lp)
{
    if(this==&lp) return true;

    chainonParticipant *crt=d_tete;
    chainonParticipant *crtLP=lp.d_tete;

    while(crt!=nullptr && crtLP!=nullptr && crt->d_participant==crtLP->d_participant) //tant quon n est pas a la fin
    {
        //passage au suivant                                                         //et que le chainon courant de this est egal
        crt=crt->d_suiv;                                                              // au chainon courant de lp
        crtLP=crtLP->d_suiv;
    }

    if(crt==nullptr && crtLP==nullptr) return true;
    return false;
}
bool ldpersonne::operator!=(const ldpersonne& lp)
{
    return !((*this)==lp);
}

//AUTRES

void ldpersonne::chargerParticipantsDepuisFichier()
{
    ifstream fichier("participants.log");
    if (!fichier)
    {
        return;
    }

    chargementEnCours = true; // ✅ On active le mode "chargement"
    string nom, prenom, email, tel;
    int ligneCount = 0;

    while (fichier >> nom >> prenom >> email >> tel)
    {
        ligneCount++;

        participant p(nom, prenom, tel, email);
        ajouterParticipant(p); // ✅ Appel normal
    }

    fichier.close();
    chargementEnCours = false; // ✅ Désactivation après le chargement
}




bool ldpersonne::trouverParticipant(string nom,string prenom,participant &p)
{
    chainonParticipant *crt = d_tete;
    while (crt != nullptr)
    {
        if (crt->d_participant.retourneNom() == nom &&
            crt->d_participant.retournePrenom() == prenom)
        {
            p = crt->d_participant;  // Copie du participant trouvé
            return true;  // Trouvé ✅
        }
        crt = crt->d_suiv;
    }
    return false;

}

void ldpersonne::ajouterParticipant(participant p)
{

    // Ajout du participant à la liste chaînée
    chainonParticipant *nouv = new chainonParticipant{p};
    chainonParticipant *crt = d_tete;
    chainonParticipant *prec = nullptr;

    if (d_tete == nullptr)
    {
        d_tete = nouv;
    }
    else
    {

        while (crt != nullptr && nouv->d_participant > crt->d_participant)
        {
            prec = crt;
            crt = crt->d_suiv;
        }

        if (prec != nullptr)
        {
            prec->d_suiv = nouv;
        }
        else
        {
            d_tete = nouv;
        }

        nouv->d_suiv = crt;
        if (crt != nullptr)
        {
            crt->d_prec = nouv;
        }
    }

    // ✅ Évite d'écrire dans `participants.log` si on est en train de charger les données
    if (!chargementEnCours)
    {
        ofstream fichier("participants.log", ios::app);
        if (!fichier)
        {
            cerr << "Erreur : Impossible d'ouvrir le fichier participants.log" << endl;
            return;
        }

        fichier << p.retourneNom() << " " << p.retournePrenom() << " " << p.retourneMail() << " " << p.retourneTel() << "\n";
        fichier.close();
    }
}


void ldpersonne::modifierParticipant(string nom,string prenom,string email,string tel)
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr && crt->d_participant.retourneNom()!=nom)
    {
        crt=crt->d_suiv;
    }
    crt->d_participant=crt->d_participant.setEmailTel(nom,prenom,email,tel);


}

bool ldpersonne::supprimerParticipant(participant &p)
{
    if (d_tete == nullptr)
        return false;

    chainonParticipant *crt = d_tete;
    chainonParticipant *prec = nullptr;

    // 🔍 Recherche du participant dans la liste chaînée
    while (crt != nullptr && p != crt->d_participant)
    {
        prec = crt;
        crt = crt->d_suiv;
    }

    if (crt == nullptr)
        return false;  // ❌ Participant non trouvé

    // ✅ Suppression du participant de la liste chaînée
    chainonParticipant *suiv = crt->d_suiv;
    delete crt;

    if (prec != nullptr)
    {
        prec->d_suiv = suiv;
    }
    else
    {
        d_tete = suiv;
    }

    if (suiv != nullptr)
    {
        suiv->d_prec = prec;
    }

    // ✅ Mise à jour du fichier `participants.log`
    ifstream fichierLecture("participants.log");
    ofstream fichierTemp("participants_temp.log");  // Nouveau fichier temporaire

    if (!fichierLecture || !fichierTemp)
    {
        cerr << "Erreur : Impossible de mettre à jour le fichier participants.log" << endl;
        return false;
    }

    string ligne;
    bool participantSupprime = false; // Vérifier si le participant a bien été supprimé

    while (getline(fichierLecture, ligne))
    {
        // 🔹 Lire ligne par ligne et reconstruire les infos du participant
        stringstream ss(ligne);
        string nom, prenom, email, tel;
        ss >> nom >> prenom >> email >> tel;

        // 🔍 Comparer avec le participant à supprimer
        if (nom == p.retourneNom() && prenom == p.retournePrenom() && email == p.retourneMail() && tel == p.retourneTel())
        {
            participantSupprime = true;
            continue; // ❌ Ne pas réécrire cette ligne dans le fichier temporaire
        }

        fichierTemp << ligne << "\n"; // ✅ Conserver les autres lignes
    }

    fichierLecture.close();
    fichierTemp.close();

    // ✅ Remplacer l'ancien fichier par le nouveau
    remove("participants.log");
    rename("participants_temp.log", "participants.log");

    return participantSupprime;
}



void ldpersonne::ajouterRDVP(string nom,string prenom, RDV r) //ajouter un RDV au participant
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr && (crt->d_participant.retourneNom() != nom || crt->d_participant.retournePrenom() != prenom))  //le nom est unique donc recherche par nom seulement
    {
        crt=crt->d_suiv;
    }

    crt->d_participant.ajouterRDV(r);
}


void ldpersonne::supprimerRDVP(string nom,string prenom,RDV r)
{
    chainonParticipant *crt=d_tete;
    while (crt != nullptr && (crt->d_participant.retourneNom() != nom || crt->d_participant.retournePrenom() != prenom))  //le nom est unique donc recherche par nom seulement
    {
        crt=crt->d_suiv;
    }

    crt->d_participant.supprimerRDV(r);
}









string ldpersonne::afficherParticipants() const
{

    string contenuListe;
    chainonParticipant *crt = d_tete;

    while (crt != nullptr)
    {
        ostringstream oss;
        oss << "Nom: " << crt->d_participant.retourneNom() << "\n"
            << "Prénom: " << crt->d_participant.retournePrenom() << "\n"
            << "Email: " << crt->d_participant.retourneMail() << "\n"
            << "Téléphone: " << crt->d_participant.retourneTel() << "\n"
            << "------------------------\n";

        contenuListe += oss.str();
        crt = crt->d_suiv;
    }

    return contenuListe.empty() ? "Aucun participant trouvé." : contenuListe;
}



void ldpersonne::sauvegarderListe(string nomFichier)
{
    int i=0;
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr)
    {
        i++;
        crt=crt->d_suiv;
    }
    ofstream ost;
    ost.open(nomFichier);
    crt=d_tete;
    //ost<<i<<endl;
    while(crt!=nullptr)
    {
        ost<<crt->d_participant.retourneNom()<<" ";
        ost<<crt->d_participant.retournePrenom()<<" ";
        ost<<crt->d_participant.retourneMail()<<" ";
        ost<<crt->d_participant.retourneTel()<<" ";
        ost<<endl;
        crt=crt->d_suiv;
    }
    ost.close();
}

void ldpersonne::sauvegarderRDVs()
{

    string nomFichier="CARNET EXPORT";
    chainonParticipant *crt=d_tete;
    ofstream ost;
    ost.open(nomFichier);

    while(crt!=nullptr)
    {
        ost<<"Participant : \n";
        ost<<crt->d_participant.retourneNom()<<" ";
        ost<<crt->d_participant.retournePrenom()<<" ";
        ost<<crt->d_participant.retourneMail()<<" ";
        ost<<crt->d_participant.retourneTel()<<" ";
        ost<<endl;

        ost<<"Liste de rendez-vous :\n";


        crt->d_participant.ecrireListeRdv(ost);


        ost<<endl;


        crt=crt->d_suiv;
    }
    ost.close();

}


string ldpersonne::afficherRDVs(string nom)
{
    string contenuListe = "";
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr && crt->d_participant.retourneNom()!=nom)
    {
        crt=crt->d_suiv;
    }

    ostringstream oss;
    oss << crt->d_participant.afficherListeRDV();
    contenuListe += oss.str() + "\n";


    return contenuListe;
}

bool ldpersonne::estOccupe(participant p,int j,int hd,int hf, int m, int a)
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr && crt->d_participant!=p)
    {
        crt=crt->d_suiv;
    }
    if(crt->d_participant.chercheRDVDate(hd,hf,j,m,a)) return true;
    return false;

}

bool ldpersonne::chevaucheRDV(participant p,RDV r)
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr && crt->d_participant !=p)   //on cherche le participant
    {
        crt=crt->d_suiv;
    }
    if(crt->d_participant.chercheRDVChevauchement(r)) return true;
    return false;
}

int ldpersonne::nombreRDVs(participant p)
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr && crt->d_participant!=p)
    {
        crt=crt->d_suiv;
    }
    return crt->d_participant.retourneNbRDV();
}

void ldpersonne::suppRDV(RDV r)
{
    chainonParticipant *crt=d_tete;
    while(crt!=nullptr )  //parcourt les participants de ldpersonne
    {
        for(int i=0;i<static_cast<int>(crt->d_participant.d_listeRDV.size());i++)  //parcourt de la liste de RDV de chaque participant crt
        {
            if(crt->d_participant.d_listeRDV[i]==r) //si on trouve le rdv dans la liste de rdv de crt
            {
                crt->d_participant.supprimerRDV(r);
            }
        }
        crt=crt->d_suiv;
    }
}

bool ldpersonne::participantsDisponibles(RDV rdv,RDV nouvrdv) //true si tlm dispo
{
    participant p;
    bool dsp=true;

    for(int i=0;i<static_cast<int>(rdv.d_listeParticipants.size());i++) //on parcourt les participants du rdv
    {
        chainonParticipant *crt=d_tete;
        while(crt!=nullptr && crt->d_participant!=rdv.d_listeParticipants[i])
        {
            crt=crt->d_suiv;
        }
        if(crt != nullptr && crt->d_participant.chercheRDVChevauchement(nouvrdv))
        {
            dsp=false;
        }


    }
    return dsp;
}

