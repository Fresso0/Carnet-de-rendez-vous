#ifndef FENETRECARNET_H
#define FENETRECARNET_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QMessageBox>
class ldpersonne;
class ldrdv;
class participant;
class RDV;


class fenetrecarnet : public QMainWindow
{
    Q_OBJECT

public:
    fenetrecarnet(QWidget *parent = nullptr);
    ~fenetrecarnet();
    void creeInterface();
    void appliquerStyleBoutons();
    void ajouterPersonne();
    void modifierPersonne(participant p, QDialog* fenetreListe);
    void supprimerPersonne(participant p, QDialog* fenetreListe);
    void afficherPersonnes();
    void demarrer();

    void ajouterRDV();
    void modifierRDV(RDV r,QDialog* fenetreListe);
    void supprimerRDV(RDV r, QDialog* fenetreListe);
    void afficherRDVs();

    void ajouterParticipantAUnRDV(participant p, QDialog* fenetreListe);
    void afficherParticipantsDUnRDV();
    void afficherRDVsDate();
    void afficherRDVsDUnParticipant();
    void rechercher();

    void exporterParticipant();
    void importerParticipant();

    void exporterRDV();
    void importerRDV();

    void exporterCarnet();

    void retirer(participant p, QDialog* fenetreListe);

    void quitter();

    void on_calendarWidget_selectionChanged();

    void afficherAjouterPersonne();
    void afficherModifierPersonne();
    void afficherSupprimerPersonne();

    void afficherAjouterRDV();
    void afficherModifierRDV();
    void afficherSupprimerRDV();

    void afficherPageAjouter();
    void afficherPageAjouterRDV();
    void afficherPageExporterParticipant();
    void afficherPageImporterParticipant();
    void afficherPageExporterRDV();
    void afficherPageImporterRDV();
    void afficherPageAfficherTousRDVsPersonne();
    void afficherPageAfficherDate();
    void afficherPageAfficherRechercherPRdv();
    void afficherPageAfficherPRdv();


private:










    QPushButton*  btnP;
    QPushButton*  btnRdv;
    QPushButton*  btnEx;
    QPushButton*  btnQ;



    QStackedWidget* stackedWidget;
    QWidget* widgetAjouterPersonne;
    QWidget* widgetModifierPersonne;
    QWidget* widgetSupprimerPersonne;
    QWidget* widgetAjouterRDV;
    QWidget* widgetModifierRDV;
    QWidget* widgetSupprimerRDV;
    QTableWidget* participantsTableWidget;
    QLabel* nomLabelA;
    QLabel* prenomLabelA ;
    QLabel* emailLabelA ;
    QLabel* telephoneLabelA ;
    QLineEdit* nomLineEditA;
    QLineEdit* prenomLineEditA ;
    QLineEdit* emailLineEditA ;
    QLineEdit* telephoneLineEditA ;
    QPushButton* ajouterParticipantButtonA ;
    QPushButton* afficherTousParticipantsButton;

    QLabel* nomLabelM;
    QLabel* prenomLabelM ;
    QLabel* emailLabelM ;
    QLabel* telephoneLabelM ;
    QLineEdit* nomLineEditM;
    QLineEdit* prenomLineEditM ;
    QLineEdit* nouveauEmailLineEditM ;
    QLineEdit* nouveauTelephoneLineEditM ;
    QPushButton* modifierParticipantButtonM;


    QLabel* nomLabelS;
    QLabel* prenomLabelS ;
    QLineEdit* nomLineEditS;
    QLineEdit* prenomLineEditS;
    QPushButton* supprimerParticipantButtonS;
    QPushButton* afficherParticipantButton;
    QPushButton* ajouterParticipantRDVButton;


    QLabel* nomLabelR ;
    QLabel* dateLabel ;
    QLabel* heureDebutLabel ;
    QLabel* heureFinLabel ;
    QLineEdit* nomLineEditR ;
    QLineEdit* dateLineEdit ;
    QLineEdit* heureDebutLineEdit;
    QLineEdit* heureFinLineEdit ;
    QPushButton* ajouterRDVButton ;
    QPushButton* supprimerRDVButton;
    QPushButton* afficherRDVButton;
    QPushButton* modifierRdvButton;


    QLabel* nomLabelAPR;
    QLabel* prenomLabelAPR ;
    QLineEdit* nomLineEditAPR;
    QLineEdit* prenomLineEditAPR ;
    QLabel* objectifLabelAPR;
    QLineEdit* objectifLineEditAPR;

    QLabel* nomLabelAffRsP;
    QLabel* prenomLabelAffRsP;
    QLineEdit* nomLineEditAffRsP;
    QLineEdit* prenomLineEditAffRsP;
    QPushButton* afficherTousRDVsPersonne;

    QLabel* nomLabelAffRD ;
    QLineEdit* objectifLineEditAffRD ;
    QPushButton* afficherRDVDateButton;

    /***********************************************************/
    QTableWidget* participantsTableWidgetR;
    QLabel* objectifLabelAR;
    QLabel* dateLabelAR ;
    QLabel* heureDebutLabelAR ;
    QLabel* heureFinLabelAR ;
    QLineEdit* objectifLineEditAR;
    QLineEdit* dateLineEditAR ;
    QLineEdit* heureDebutLineEditAR ;
    QLineEdit* heureFinLineEditAR ;
    QPushButton* ajouterRDVButtonAR ;

    QLabel* objectifLabelMR;
    QLabel* dateLabelMR ;
    QLabel* heureDebutLabelMR ;
    QLabel* heureFinLabelMR ;
    QLineEdit* objectifLineEditMR;
    QLineEdit* dateLineEditMR ;
    QLineEdit* heureDebutLineEditMR ;
    QLineEdit* heureFinLineEditMR ;
    QPushButton* modifierRDVButtonMR;

    QLabel* nomLabelSR ;
    QLineEdit* objectifLineEditSR;
    QPushButton* supprimerRDVButtonSR;
    QPushButton* afficherTousRDVsButton;

    QLabel* nomLabelRechercheP;
    QLabel* prenomLabelRechercheP;
    QLineEdit* objectifLineEditRechercheP ;
    QLineEdit* prenomobjectifLineEditRechercheP;
    QLabel* dateLabelRechercheP ;
    QLineEdit* dateLineEditRechercheP ;
    QLabel* hDebutLabelRechercheP ;
    QLineEdit* hDebutLineEditRechercheP ;
    QLabel* hFinLabelRechercheP ;
    QLineEdit* hFinLineEditRechercheP ;
    QPushButton* rehcercherRDVPButtonRR ;

    QLabel* nomLabelAffPR ;
    QLineEdit* nomLineEditAffPR ;
    QPushButton* afficherParticipantsRDVButton ;

    QLabel* nomLabelFichierExportParticipants;
    QLineEdit* nomLineEditFichierExportParticipants ;
    QPushButton* exporterPButton;

    QLabel* nomLabelFichierImportParticipants;
    QLineEdit* nomLineEditFichierImportParticipants ;
    QPushButton* importerPButton;

    QLabel* nomLabelFichierExportRDV;
    QLineEdit* nomLineEditFichierExportRDV ;
    QPushButton* exporterRButton;

    QLabel* nomLabelFichierImportRDV;
    QLineEdit* nomLineEditFichierImportRDV ;
    QPushButton* importerRButton;

    QPushButton* quitterButton;

    QLabel* nomLabelRetirerP;
    QLabel* prenomLabelRetirerP ;
    QLineEdit* nomLineEditRetirerP;
    QLineEdit* prenomLineEditRetirerP;
    QPushButton* retirerButton;
    QLabel* nomRDVLabelRetirerParticipant;
    QLineEdit* nomRDVLineEditRetirerParticipant;

    QPushButton* exporterCarnetButton;
};
#endif // FENETRECARNET_H
