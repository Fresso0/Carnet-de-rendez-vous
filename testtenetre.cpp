#include "testtenetre.h"


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QMenu>
#include <QDateEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <fstream>
#include <QPixmap>
#include <QAction>
#include <QCalendarWidget>
#include <QFontDatabase>
#include <QComboBox>
#include <QSpinBox>
#include <QListWidget>
#include <QInputDialog>
#include <QApplication>
#include "ldpersonne.h"
#include "ldrdv.h"
#include "participant.h"
#include "rdv.h"

ldpersonne listeP;
ldrdv listeRDV;

string prenom,nom,email, nomFichier,tel;
int jour,hdebut,hfin,choix,nb,mois,an;


testtenetre::testtenetre(QWidget *parent)
    : QMainWindow(parent)
{
    creeInterface();
    this->setStyleSheet("{background-image: url(../../images/background.jpg);}");
    appliquerStyleBoutons();
}

testtenetre::~testtenetre() {}

void testtenetre::creeInterface()
{
    auto central = new QWidget(this);
    auto layoutPrincipal {new QVBoxLayout {central}};
    setCentralWidget(central);
    central->setLayout(layoutPrincipal);


    QLabel *label = new QLabel(this);
    QPixmap pixmap("../../images/carnet.png");
    QPixmap scaledPixmap = pixmap.scaled(450, 450, Qt::KeepAspectRatio);
    label->setPixmap(scaledPixmap);
    label->setScaledContents(true);

    auto carnetT = new QLabel{tr("Carnet de Rendez-Vous")};
    carnetT->setStyleSheet("color: white; font-weight: bold; font-size: 22px; padding-top : 100px;");
    layoutPrincipal->addWidget(label,0,Qt::AlignCenter);
    layoutPrincipal->addWidget(carnetT,0,Qt::AlignCenter);

    // Menu
    QMenu *menuFichier = menuBar()->addMenu("Fichier");
    QAction *ajouterAction = menuBar()->addAction("Personne");
    QAction *ajouterRdv = menuBar()->addAction("Rendez-Vous");
    QMenu *menuAffichage = menuBar()->addMenu("Affichage");

    // Actions pour le menu "Fichier"
    QAction *exporterParticipant = new QAction("Exporter les participants dans un fichier", this);
    QAction *importerParticipant = new QAction("Importer des participants depuis un fichier", this);
    QAction *exporterRDV = new QAction("Exporter des RDV dans un fichier", this);
    QAction *importerRDV = new QAction("Importer un RDV depuis un fichier", this);


    menuFichier->addAction(exporterParticipant);
    menuFichier->addAction(importerParticipant);
    menuFichier->addAction(exporterRDV);
    menuFichier->addAction(importerRDV);


    // Actions pour le menu "Affichage"
    QAction *afficherRdvP = new QAction("Les Rendez-vous d'un Participant", this);
    QAction *afficherDate = new QAction("Les Rendez-vous pour une date", this);
    QAction *RechercherPRdv = new QAction("Rechercher si une personne à un rendez-vous", this);
    QAction *ParticipantRdv = new QAction("Les participants d'un rendez-vous", this);
    menuAffichage->addAction(afficherRdvP);
    menuAffichage->addAction(afficherDate);
    menuAffichage->addAction(RechercherPRdv);
    menuAffichage->addAction(ParticipantRdv);


    // Connecter l'action à une méthode (afficherPageAjouter)
    connect(ajouterAction, &QAction::triggered, this, &testtenetre::afficherPageAjouter);
    connect(ajouterRdv, &QAction::triggered, this, &testtenetre::afficherPageAjouterRDV);
    connect(exporterParticipant, &QAction::triggered, this, &testtenetre::afficherPageExporterParticipant);
    connect(importerParticipant, &QAction::triggered, this, &testtenetre::afficherPageImporterParticipant);
    connect(exporterRDV, &QAction::triggered, this, &testtenetre::afficherPageExporterRDV);
    connect(importerRDV, &QAction::triggered, this, &testtenetre::afficherPageImporterRDV);
    connect(afficherRdvP, &QAction::triggered, this, &testtenetre::afficherPageAfficherTousRDVsPersonne);
    connect(afficherDate, &QAction::triggered, this, &testtenetre::afficherPageAfficherDate);
    connect(RechercherPRdv, &QAction::triggered, this, &testtenetre::afficherPageAfficherRechercherPRdv);
    connect(ParticipantRdv, &QAction::triggered, this, &testtenetre::afficherPageAfficherPRdv);


    appliquerStyleBoutons();


    auto statut = statusBar();
    btnP = new QPushButton{tr("Afficher tous les participants")};
    btnP->setStyleSheet("background-color: #657B83; color: white; border-radius: 5px; padding: 5px;");
    btnRdv = new QPushButton{tr("Afficher tous les rendez-vous")};
    btnRdv->setStyleSheet("background-color: #657B83; color: white; border-radius: 5px; padding: 5px;");
    btnEx = new QPushButton{tr("Exporter")};
    btnEx->setStyleSheet("background-color: #0C7F9B; color: white; border-radius: 5px; padding: 5px;");
    btnQ = new QPushButton{tr("Quitter")};
    btnQ->setStyleSheet("background-color: red; color: white; border-radius: 5px; padding: 5px;");
    statut->setStyleSheet("background-color: #3D4653; color: white;");
    statut->addWidget(btnP,1);
    statut->addWidget(btnRdv,1);
    statut->addWidget(btnEx,1);
    statut->addWidget(btnQ,1);


    central->setStyleSheet("background-color: #0F0F1E;");

    connect(btnP , &QPushButton::clicked, this, &testtenetre::afficherPersonnes);
    connect(btnRdv , &QPushButton::clicked, this, &testtenetre::afficherRDVs);
    connect( btnEx, &QPushButton::clicked, this, &testtenetre::exporterCarnet);
    connect( btnQ, &QPushButton::clicked, this, &testtenetre::quitter);

    this->setMinimumSize(700,570);
    this->setMaximumSize(700,800);
    this->showMaximized();
}

void testtenetre::afficherPageAjouter()
{
    // Créer l'interface pour ajouter une personne
    auto pageAjouterPersonne = new QWidget(this);
    auto layoutAjouter = new QVBoxLayout(pageAjouterPersonne);
    auto ajouterPerson = new QLabel("Ajouter une personne");
    layoutAjouter->addWidget(ajouterPerson,0,Qt::AlignCenter);
    ajouterPerson->setStyleSheet("font-size: 16px; font-weight: bold;");

    nomLabelA = new QLabel("Nom : ");
    nomLineEditA = new QLineEdit;
    nomLineEditA->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    prenomLabelA = new QLabel("Prénom : ");
    prenomLineEditA = new QLineEdit;
    prenomLineEditA->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    emailLabelA = new QLabel("Email : ");
    emailLineEditA = new QLineEdit;
    emailLineEditA->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    telephoneLabelA = new QLabel("Téléphone : ");
    telephoneLineEditA = new QLineEdit;
    telephoneLineEditA->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");

    layoutAjouter->addWidget(nomLabelA);
    layoutAjouter->addWidget(nomLineEditA);
    layoutAjouter->addWidget(prenomLabelA);
    layoutAjouter->addWidget(prenomLineEditA);
    layoutAjouter->addWidget(emailLabelA);
    layoutAjouter->addWidget(emailLineEditA);
    layoutAjouter->addWidget(telephoneLabelA);
    layoutAjouter->addWidget(telephoneLineEditA);


    ajouterParticipantButtonA = new QPushButton("Ajouter");
    ajouterParticipantButtonA->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");
    layoutAjouter->addWidget(ajouterParticipantButtonA);
    setCentralWidget(pageAjouterPersonne); // Met à jour le widget central
    pageAjouterPersonne->setStyleSheet("background-color: #0F0F1E; color: white;");
    connect(ajouterParticipantButtonA , &QPushButton::clicked, this, &testtenetre::ajouterPersonne);
}

void testtenetre::afficherPageAjouterRDV()
{
    // Créer le widget pour ajouter un rendez-vous
    auto pageAjouterRDV = new QWidget(this);
    auto layoutAjouterR = new QVBoxLayout(pageAjouterRDV);

    // Créer des labels et des champs pour les informations du rendez-vous
    objectifLabelAR = new QLabel("Nom du rendez-vous : ");
    dateLabelAR = new QLabel("Date : ");
    heureDebutLabelAR = new QLabel("Heure de début : ");
    heureFinLabelAR = new QLabel("Heure de fin : ");

    objectifLineEditAR = new QLineEdit(); // Champ pour le nom du rendez-vous
    objectifLineEditAR->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    dateLineEditAR = new QLineEdit(); // Champ pour la date (qui sera remplie avec le calendrier)
    dateLineEditAR->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    heureDebutLineEditAR = new QLineEdit(); // Champ pour l'heure de début
    heureDebutLineEditAR->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    heureFinLineEditAR = new QLineEdit(); // Champ pour l'heure de fin
    heureFinLineEditAR->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");

    // Créer un calendrier pour sélectionner la date
    QCalendarWidget* calendar = new QCalendarWidget(this);
    // Style CSS pour le QCalendarWidget
    QString styleCalendar = R"(
    QCalendarWidget {
        border: 2px solid #0088aa; /* Couleur du contour du calendrier */
        background-color: #f0f0f0; /* Couleur de fond du calendrier */
        font-size: 12px; /* Taille de la police des jours */
    }

    QCalendarWidget QAbstractItemView::item {
        border: 1px solid transparent; /* Bord transparent autour des jours */
        padding: 5px;
    }

    QCalendarWidget QAbstractItemView::item:selected {
        background-color: #0088aa; /* Couleur de fond du jour sélectionné */
        color: white; /* Couleur du texte du jour sélectionné */
    }

    QCalendarWidget QAbstractItemView::item:hover {
        background-color: #00b5cc; /* Couleur de fond au survol d'un jour */
    }

    QCalendarWidget QTableView::item {
        text-align: center;
    }
)";

    // Appliquer le style à l'objet QCalendarWidget
    calendar->setStyleSheet(styleCalendar);








    QPushButton* button = new QPushButton("Sélectionner la date");
    button->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Créer le bouton "Ajouter" pour valider le rendez-vous
    ajouterRDVButtonAR = new QPushButton("Ajouter");
    ajouterRDVButtonAR->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets dans le layout
    layoutAjouterR->addWidget(objectifLabelAR);
    layoutAjouterR->addWidget(objectifLineEditAR);

    layoutAjouterR->addWidget(dateLabelAR);
    layoutAjouterR->addWidget(calendar);

    layoutAjouterR->addWidget(button);
    layoutAjouterR->addWidget(dateLineEditAR);

    layoutAjouterR->addWidget(heureDebutLabelAR);
    layoutAjouterR->addWidget(heureDebutLineEditAR);

    layoutAjouterR->addWidget(heureFinLabelAR);
    layoutAjouterR->addWidget(heureFinLineEditAR);

    layoutAjouterR->addWidget(ajouterRDVButtonAR);

    // Mettre à jour le widget central
    setCentralWidget(pageAjouterRDV);
    pageAjouterRDV->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connecter le bouton "Sélectionner" au calendrier
    connect(button, &QPushButton::clicked, this, [=]() {
        // Mettre à jour la date dans le champ 'dateLineEditAR' quand l'utilisateur sélectionne une date
        QDate selectedDate = calendar->selectedDate();
        QString dateString = selectedDate.toString("dd/MM/yyyy");
        dateLineEditAR->setText(dateString);
    });

    // Connecter le bouton "Ajouter" à une méthode pour ajouter le rendez-vous
    connect(ajouterRDVButtonAR, &QPushButton::clicked, this, &testtenetre::ajouterRDV);
}
void testtenetre::afficherPageExporterParticipant()
{
    auto pageExporter = new QWidget(this);
    auto layoutExporter = new QVBoxLayout(pageExporter);

    // Ajouter des widgets pour l'exportation des participants dans un fichier texte
    nomLabelFichierExportParticipants = new QLabel("Nom du fichier", this);
    nomLineEditFichierExportParticipants = new QLineEdit(this);
    nomLineEditFichierExportParticipants->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    exporterPButton = new QPushButton("Exporter", this);
    exporterPButton->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutExporter->addWidget(nomLabelFichierExportParticipants);
    layoutExporter->addWidget(nomLineEditFichierExportParticipants);
    layoutExporter->addWidget(exporterPButton);

    // Appliquer le layout à la page d'exportation
    pageExporter->setLayout(layoutExporter);

    // Mettre à jour le widget central pour afficher la page d'exportation
    setCentralWidget(pageExporter);
    pageExporter->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton à la méthode d'exportation
    connect(exporterPButton, &QPushButton::clicked, this, &testtenetre::exporterParticipant);

}

void testtenetre::afficherPageImporterParticipant() {
    // Créer l'interface pour importer des participants
    auto pageImporter = new QWidget(this);
    auto layoutImporter = new QVBoxLayout(pageImporter);

    // Ajouter des widgets pour l'importation des participants depuis un fichier texte
    nomLabelFichierImportParticipants = new QLabel("Nom du fichier", this);
    nomLineEditFichierImportParticipants = new QLineEdit(this);
    nomLineEditFichierImportParticipants->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    importerPButton = new QPushButton("Importer", this);
    importerPButton->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutImporter->addWidget(nomLabelFichierImportParticipants);
    layoutImporter->addWidget(nomLineEditFichierImportParticipants);
    layoutImporter->addWidget(importerPButton);

    // Appliquer le layout à la page d'importation
    pageImporter->setLayout(layoutImporter);

    // Mettre à jour le widget central pour afficher la page d'importation
    setCentralWidget(pageImporter);
    pageImporter->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton à la méthode d'importation
    connect(importerPButton, &QPushButton::clicked, this, &testtenetre::importerParticipant);
}

void testtenetre::afficherPageExporterRDV() {
    // Créer l'interface pour exporter les rendez-vous
    auto pageExporterRDV = new QWidget(this);
    auto layoutExporterRDV = new QVBoxLayout(pageExporterRDV);

    // Ajouter des widgets pour l'exportation des rendez-vous dans un fichier texte
    nomLabelFichierExportRDV = new QLabel("Nom du fichier", this);
    nomLineEditFichierExportRDV = new QLineEdit(this);
    nomLineEditFichierExportRDV->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    exporterRButton = new QPushButton("Exporter", this);
    exporterRButton->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutExporterRDV->addWidget(nomLabelFichierExportRDV);
    layoutExporterRDV->addWidget(nomLineEditFichierExportRDV);
    layoutExporterRDV->addWidget(exporterRButton);

    // Appliquer le layout à la page d'exportation
    pageExporterRDV->setLayout(layoutExporterRDV);

    // Mettre à jour le widget central pour afficher la page d'exportation des RDV
    setCentralWidget(pageExporterRDV);
    pageExporterRDV->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton à la méthode d'exportation des RDV
    connect(exporterRButton, &QPushButton::clicked, this, &testtenetre::exporterRDV);
}


void testtenetre::afficherPageImporterRDV() {
    // Créer l'interface pour importer les rendez-vous
    auto pageImporterRDV = new QWidget(this);
    auto layoutImporterRDV = new QVBoxLayout(pageImporterRDV);

    // Ajouter des widgets pour l'importation des rendez-vous depuis un fichier texte
    nomLabelFichierImportRDV = new QLabel("Nom du fichier", this);
    nomLineEditFichierImportRDV = new QLineEdit(this);
    nomLineEditFichierImportRDV->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    importerRButton = new QPushButton("Importer", this);
    importerRButton->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutImporterRDV->addWidget(nomLabelFichierImportRDV);
    layoutImporterRDV->addWidget(nomLineEditFichierImportRDV);
    layoutImporterRDV->addWidget(importerRButton);

    // Appliquer le layout à la page d'importation
    pageImporterRDV->setLayout(layoutImporterRDV);

    // Mettre à jour le widget central pour afficher la page d'importation des RDV
    setCentralWidget(pageImporterRDV);
    pageImporterRDV->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton à la méthode d'importation des RDV
    connect(importerRButton, &QPushButton::clicked, this, &testtenetre::importerRDV);
}

void testtenetre::afficherPageAfficherTousRDVsPersonne() {
    // Créer l'interface pour afficher les rendez-vous d'un participant
    auto pageAfficherRsP = new QWidget(this);
    auto layoutAfficherRsP = new QVBoxLayout(pageAfficherRsP);

    // Ajouter des widgets pour l'affichage des rdv d'un participant
    nomLabelAffRsP = new QLabel("Nom personne : ", this);
    prenomLabelAffRsP = new QLabel("Prénom personne : ", this);
    nomLineEditAffRsP = new QLineEdit(this);
    nomLineEditAffRsP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    prenomLineEditAffRsP = new QLineEdit(this);
    prenomLineEditAffRsP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    afficherTousRDVsPersonne = new QPushButton("Afficher tous les rendez-vous d'un participant", this);
    afficherTousRDVsPersonne->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutAfficherRsP->addWidget(nomLabelAffRsP);
    layoutAfficherRsP->addWidget(nomLineEditAffRsP);
    layoutAfficherRsP->addWidget(prenomLabelAffRsP);
    layoutAfficherRsP->addWidget(prenomLineEditAffRsP);
    layoutAfficherRsP->addWidget(afficherTousRDVsPersonne);

    // Appliquer le layout à la page d'affichage des rendez-vous
    pageAfficherRsP->setLayout(layoutAfficherRsP);

    // Mettre à jour le widget central pour afficher la page d'affichage des rendez-vous d'un participant
    setCentralWidget(pageAfficherRsP);
    pageAfficherRsP->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton à la méthode qui affiche les rendez-vous du participant
    connect(afficherTousRDVsPersonne, &QPushButton::clicked, this, &testtenetre::afficherRDVsDUnParticipant);
}

void testtenetre::afficherPageAfficherDate() {
    // Créer l'interface pour afficher les rendez-vous pour une date donnée
    auto pageAfficherRD = new QWidget(this);
    auto layoutAfficherRD = new QVBoxLayout(pageAfficherRD);

    // Ajouter des widgets pour l'affichage des rdv pour une date donnée
    nomLabelAffRD = new QLabel("Date : ", this);
    objectifLineEditAffRD = new QLineEdit(this);
    objectifLineEditAffRD->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    afficherRDVDateButton = new QPushButton("Afficher les rendez-vous", this);
    afficherRDVDateButton->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter un calendrier pour sélectionner une date
    auto calendarAfficheRDVs = new QCalendarWidget(this);
    // Style CSS pour le QCalendarWidget
    QString styleCalendar = R"(
    QCalendarWidget {
        border: 2px solid #0088aa; /* Couleur du contour du calendrier */
        background-color: #f0f0f0; /* Couleur de fond du calendrier */
        font-size: 12px; /* Taille de la police des jours */
    }

    QCalendarWidget QAbstractItemView::item {
        border: 1px solid transparent; /* Bord transparent autour des jours */
        padding: 5px;
    }

    QCalendarWidget QAbstractItemView::item:selected {
        background-color: #0088aa; /* Couleur de fond du jour sélectionné */
        color: white; /* Couleur du texte du jour sélectionné */
    }

    QCalendarWidget QAbstractItemView::item:hover {
        background-color: #00b5cc; /* Couleur de fond au survol d'un jour */
    }

    QCalendarWidget QTableView::item {
        text-align: center;
    }
)";

    // Appliquer le style à l'objet QCalendarWidget
    calendarAfficheRDVs->setStyleSheet(styleCalendar);
    auto buttonAfficheRDVs = new QPushButton("Sélectionner", this);
    buttonAfficheRDVs->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutAfficherRD->addWidget(nomLabelAffRD);
    layoutAfficherRD->addWidget(calendarAfficheRDVs);
    layoutAfficherRD->addWidget(buttonAfficheRDVs);
    layoutAfficherRD->addWidget(objectifLineEditAffRD);
    layoutAfficherRD->addWidget(afficherRDVDateButton);

    // Connexion du bouton "Sélectionner" pour afficher la date sélectionnée
    connect(buttonAfficheRDVs, &QPushButton::clicked, this, [=]() {
        QDate selectedDate = calendarAfficheRDVs->selectedDate();
        QString dateStringM = selectedDate.toString("dd/MM/yyyy");
        objectifLineEditAffRD->setText(dateStringM);
    });

    // Connexion du bouton "Afficher les rendez-vous" à la méthode d'affichage des RDV
    connect(afficherRDVDateButton, &QPushButton::clicked, this, &testtenetre::afficherRDVsDate);

    // Appliquer le layout à la page
    pageAfficherRD->setLayout(layoutAfficherRD);

    // Mettre à jour le widget central pour afficher la page d'affichage des rendez-vous par date
    setCentralWidget(pageAfficherRD);
    pageAfficherRD->setStyleSheet("background-color: #0F0F1E; color: white;");
}

void testtenetre::afficherPageAfficherRechercherPRdv() {
    // Créer l'interface pour rechercher un participant pour un rdv donné
    auto pageRechercherRDV = new QWidget(this);
    auto layoutRechercher = new QVBoxLayout(pageRechercherRDV);

    // Ajouter des widgets pour la recherche d'un participant pour un rdv donné
    nomLabelRechercheP = new QLabel("Nom personne : ", this);
    objectifLineEditRechercheP = new QLineEdit(this);
    objectifLineEditRechercheP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    prenomLabelRechercheP = new QLabel("Prenom personne : ", this);
    prenomobjectifLineEditRechercheP = new QLineEdit(this);
    prenomobjectifLineEditRechercheP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    dateLabelRechercheP = new QLabel("Date : ", this);
    dateLineEditRechercheP = new QLineEdit(this);
    dateLineEditRechercheP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    hDebutLabelRechercheP = new QLabel("Heure de début en heure : ", this);
    hDebutLineEditRechercheP = new QLineEdit(this);
    hDebutLineEditRechercheP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    hFinLabelRechercheP = new QLabel("Heure de fin en heure : ", this);
    hFinLineEditRechercheP = new QLineEdit(this);
    hFinLineEditRechercheP->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    rehcercherRDVPButtonRR = new QPushButton("Rechercher", this);
    rehcercherRDVPButtonRR->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutRechercher->addWidget(nomLabelRechercheP);
    layoutRechercher->addWidget(objectifLineEditRechercheP);
    layoutRechercher->addWidget(prenomLabelRechercheP);
    layoutRechercher->addWidget(prenomobjectifLineEditRechercheP);
    layoutRechercher->addWidget(dateLabelRechercheP);

    // Ajouter un calendrier pour sélectionner une date
    auto calendarRecherche = new QCalendarWidget(this);
    // Style CSS pour le QCalendarWidget
    QString styleCalendar = R"(
    QCalendarWidget {
        border: 2px solid #0088aa; /* Couleur du contour du calendrier */
        background-color: #f0f0f0; /* Couleur de fond du calendrier */
        font-size: 12px; /* Taille de la police des jours */
    }

    QCalendarWidget QAbstractItemView::item {
        border: 1px solid transparent; /* Bord transparent autour des jours */
        padding: 5px;
    }

    QCalendarWidget QAbstractItemView::item:selected {
        background-color: #0088aa; /* Couleur de fond du jour sélectionné */
        color: white; /* Couleur du texte du jour sélectionné */
    }

    QCalendarWidget QAbstractItemView::item:hover {
        background-color: #00b5cc; /* Couleur de fond au survol d'un jour */
    }

    QCalendarWidget QTableView::item {
        text-align: center;
    }
)";

    // Appliquer le style à l'objet QCalendarWidget
    calendarRecherche->setStyleSheet(styleCalendar);
    layoutRechercher->addWidget(calendarRecherche);
    auto buttonRecherche = new QPushButton("Sélectionner", this);
    buttonRecherche->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");
    layoutRechercher->addWidget(buttonRecherche);
    layoutRechercher->addWidget(dateLineEditRechercheP);

    // Connexion pour mettre à jour la date dans le QLineEdit
    connect(buttonRecherche, &QPushButton::clicked, this, [=]() {
        QDate selectedDate = calendarRecherche->selectedDate();
        QString dateStringM = selectedDate.toString("dd/MM/yyyy");
        dateLineEditRechercheP->setText(dateStringM);
    });

    layoutRechercher->addWidget(hDebutLabelRechercheP);
    layoutRechercher->addWidget(hDebutLineEditRechercheP);
    layoutRechercher->addWidget(hFinLabelRechercheP);
    layoutRechercher->addWidget(hFinLineEditRechercheP);
    layoutRechercher->addWidget(rehcercherRDVPButtonRR);

    // Appliquer le layout à la page
    pageRechercherRDV->setLayout(layoutRechercher);

    // Mettre à jour le widget central pour afficher la page de recherche de RDV
    setCentralWidget(pageRechercherRDV);
    pageRechercherRDV->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton "Rechercher" à la méthode de recherche
    connect(rehcercherRDVPButtonRR, &QPushButton::clicked, this, &testtenetre::rechercher);
}


void testtenetre::afficherPageAfficherPRdv() {
    // Créer l'interface pour afficher les participants d'un rendez-vous
    auto pageAfficherPRD = new QWidget(this);
    auto layoutAfficherPRD = new QVBoxLayout(pageAfficherPRD);

    // Ajouter des widgets pour l'affichage des participants d'un rendez-vous
    nomLabelAffPR = new QLabel("Nom rendez-vous : ", this);
    nomLineEditAffPR = new QLineEdit(this);
    nomLineEditAffPR->setStyleSheet("background-color: #2E2E3E; color: white; border-radius: 5px; padding: 5px;");
    afficherParticipantsRDVButton = new QPushButton("Afficher les participants d'un rendez-vous", this);
    afficherParticipantsRDVButton->setStyleSheet("background-color: #3D4653; color: white; border-radius: 5px; padding: 5px;");

    // Ajouter les widgets au layout
    layoutAfficherPRD->addWidget(nomLabelAffPR);
    layoutAfficherPRD->addWidget(nomLineEditAffPR);
    layoutAfficherPRD->addWidget(afficherParticipantsRDVButton);

    // Appliquer le layout à la page
    pageAfficherPRD->setLayout(layoutAfficherPRD);

    // Mettre à jour le widget central pour afficher la page
    setCentralWidget(pageAfficherPRD);
    pageAfficherPRD->setStyleSheet("background-color: #0F0F1E; color: white;");

    // Connexion du bouton "Afficher les participants" à la méthode qui affiche les participants du RDV
    connect(afficherParticipantsRDVButton, &QPushButton::clicked, this, &testtenetre::afficherParticipantsDUnRDV);
}




void testtenetre::demarrer()
{
    qDebug() << "Début du chargement des participants...";
    listeP.chargerParticipantsDepuisFichier();
    listeRDV.chargerRdvDepuisFichier();
    qDebug() << "Fin du chargement des participants.";
}


void testtenetre::ajouterPersonne() {
    // Récupérez les informations de la personne saisies dans les champs de texte
    string nom = nomLineEditA->text().toStdString();
    string prenom = prenomLineEditA->text().toStdString();
    string email = emailLineEditA->text().toStdString();
    string tel = telephoneLineEditA->text().toStdString();
    participant pp;

    participant p{nom,prenom,tel,email};
    if(QString::fromStdString(p.retourneNom())=="" || QString::fromStdString(p.retournePrenom())=="" || QString::fromStdString(p.retourneTel())=="" || QString::fromStdString(p.retourneMail())=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs.");
        return;
    }


    if(listeP.trouverParticipant(nom,prenom,pp))
    {
        QMessageBox::critical(this, "Erreur", "La personne existe déjà dans la liste des participants.");
        return;
    }
    else
    {
        listeP.ajouterParticipant(p);
        QMessageBox::information(this, "Ajout réussi",
                                 QString("Nom : ") + QString::fromStdString(p.retourneNom()) + "\n" +
                                     QString("Prénom : ") + QString::fromStdString(p.retournePrenom()) + "\n" +
                                     QString("Téléphone : ") + QString::fromStdString(p.retourneTel()) + "\n" +
                                     QString("Email : ") + QString::fromStdString(p.retourneMail()));
    }
    // Effacez les champs de texte pour la saisie d'une nouvelle personne
    nomLineEditA->clear();
    prenomLineEditA->clear();
    emailLineEditA->clear();
    telephoneLineEditA->clear();
}


void testtenetre::modifierPersonne(participant p, QDialog* fenetreListe) {
    // Demander les nouvelles informations (email et téléphone) via une fenêtre modale
    bool ok;
    QString nouveauEmail = QInputDialog::getText(this, "Modification Email",
                                                 "Entrez le nouvel email:", QLineEdit::Normal,
                                                 "", &ok);
    if (!ok || nouveauEmail.isEmpty()) {
        QMessageBox::warning(this, "Attention", "L'email ne peut pas être vide.");
        return;
    }

    QString nouveauTel = QInputDialog::getText(this, "Modification Téléphone",
                                               "Entrez le nouveau téléphone:", QLineEdit::Normal,
                                               "", &ok);
    if (!ok || nouveauTel.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Le téléphone ne peut pas être vide.");
        return;
    }

    // Modification du participant avec les nouvelles informations
    listeP.modifierParticipant(p.retourneNom(), p.retournePrenom(), nouveauEmail.toStdString(), nouveauTel.toStdString());
    // Sauvegarder les modifications dans le fichier participants.log
    listeP.sauvegarderListe("participants.log");

    // Afficher un message de succès
    QMessageBox::information(this, "Modification réussie", "Les informations ont été modifiées avec succès !");

    // Fermer la fenêtre de la liste actuelle
    if(fenetreListe)
        fenetreListe->close();  // Fermer la fenêtre actuelle

    // Après la suppression, réactualiser la liste affichée
    afficherPersonnes();  // Réouvrir la fenêtre avec la nouvelle liste
}



void testtenetre::supprimerPersonne(participant p, QDialog* fenetreListe) {
    // Vérifier si la personne est associée à un rendez-vous
    if (listeRDV.estAssocieADesRDV(p)) {
        // Si la personne a des rendez-vous, afficher un message d'erreur
        QMessageBox::critical(this, "Erreur", "La personne est associée à un rendez-vous et ne peut pas être supprimée.");
        return;
    }

    // Si la personne n'est pas associée à un rendez-vous, la supprimer
    if (listeP.supprimerParticipant(p)) {
        // Afficher un message de succès
        QMessageBox::information(this, "Suppression réussie", "Participant supprimé avec succès !");

        // Fermer la fenêtre de la liste actuelle
        if (fenetreListe) {
            fenetreListe->close();
        }

        // Réafficher la liste des participants mise à jour
        afficherPersonnes();
    } else {
        // Afficher un message d'erreur si la suppression échoue
        QMessageBox::critical(this, "Erreur", "Le participant n'a pas pu être supprimé.");
    }
}




void testtenetre::afficherPersonnes() {
    // Créez une instance de la nouvelle fenêtre pour afficher la liste
    QDialog* fenetreListe = new QDialog(this);
    fenetreListe->setWindowTitle("Liste des participants");

    // Créer un QVBoxLayout pour la fenêtre modale
    QVBoxLayout* layout = new QVBoxLayout(fenetreListe);

    // Créer un QListWidget pour afficher les participants
    QListWidget* listWidget = new QListWidget(fenetreListe);
    layout->addWidget(listWidget);

    // Effacer les anciens éléments avant de réafficher la liste mise à jour
    listWidget->clear();

    // Parcourir les participants de la liste (chaînée)
    chainonParticipant* crt = listeP.getTete(); // Supposons que `getTete()` donne la tête de la liste
    while (crt != nullptr) {
        // Utiliser le getter pour obtenir le participant
        participant& p = crt->getParticipant();

        // Créer une chaîne formatée pour afficher les informations complètes du participant
        QString itemText = "Nom: " + QString::fromStdString(p.retourneNom()) + "\n" +
                           "Prénom: " + QString::fromStdString(p.retournePrenom()) + "\n" +
                           "Email: " + QString::fromStdString(p.retourneMail()) + "\n" +
                           "Téléphone: " + QString::fromStdString(p.retourneTel());

        // Créer un item pour chaque participant
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);

        // Ajouter un séparateur visuel entre les éléments
        QFrame* separator = new QFrame();
        separator->setFrameShape(QFrame::HLine);  // Ligne horizontale
        separator->setFrameShadow(QFrame::Sunken);
        listWidget->addItem(new QListWidgetItem());  // Ajout d'un espace pour la ligne de séparation
        listWidget->setItemWidget(listWidget->item(listWidget->count() - 1), separator);

        // Créer un widget contenant les boutons de modification, suppression et ajout au rendez-vous
        QWidget* itemWidget = new QWidget;
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);

        // Créer un spacer pour pousser les boutons à droite
        QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        itemLayout->addSpacerItem(spacer);  // Pousse les boutons vers la droite

        // Ajouter un bouton de modification avec un stylo
        QPushButton* editButton = new QPushButton(itemWidget);
        editButton->setFixedSize(40, 35);  // Taille fixe pour les boutons
        QIcon editIcon("../../icons/pencil_icon.png"); // Remplacer par le chemin vers l'icône du stylo
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(40, 20));  // Taille de l'icône
        connect(editButton, &QPushButton::clicked, this, [this, p, &fenetreListe]() {
            // Logique pour modifier le participant
            this->modifierPersonne(p, fenetreListe);
        });

        // Ajouter un bouton de suppression avec une corbeille
        QPushButton* deleteButton = new QPushButton(itemWidget);
        deleteButton->setFixedSize(40, 35);  // Taille fixe pour les boutons
        QIcon deleteIcon("../../icons/trash_icon.png"); // Remplacer par le chemin vers l'icône de la corbeille
        deleteButton->setIcon(deleteIcon);
        deleteButton->setIconSize(QSize(40, 20));  // Taille de l'icône
        connect(deleteButton, &QPushButton::clicked, this, [this, p, fenetreListe]() {
            // Logique pour supprimer le participant
            this->supprimerPersonne(p, fenetreListe);  // Passe le participant à supprimer
        });

        // Ajouter un bouton "Ajouter au RDV"
        QPushButton* addButton = new QPushButton(itemWidget);
        addButton->setFixedSize(40, 35);  // Taille fixe pour les boutons
        QIcon addIcon("../../icons/plus_icon.png"); // Remplacer par l'icône plus
        addButton->setIcon(addIcon);
        addButton->setIconSize(QSize(40, 20));  // Taille de l'icône
        connect(addButton, &QPushButton::clicked, this, [this, p, &fenetreListe]() {
            // Logique pour ajouter le participant au rendez-vous
            this->ajouterParticipantAUnRDV(p,fenetreListe);
        });


        // Ajouter un bouton "Ajouter au RDV"
        QPushButton* addButtonmoins = new QPushButton(itemWidget);
        addButtonmoins->setFixedSize(40, 35);  // Taille fixe pour les boutons
        QIcon moinsIcon("../../icons/moins_icon.png"); // Remplacer par l'icône plus
        addButtonmoins->setIcon(moinsIcon);
        addButtonmoins->setIconSize(QSize(40, 20));  // Taille de l'icône
        connect(addButtonmoins, &QPushButton::clicked, this, [this, p, &fenetreListe]() {
            // Logique pour ajouter le participant au rendez-vous
            this->retirer(p,fenetreListe);
        });

        // Ajouter les boutons au layout
        itemLayout->addWidget(editButton);
        itemLayout->addWidget(deleteButton);
        itemLayout->addWidget(addButton);
        itemLayout->addWidget(addButtonmoins);

        // Ajouter le widget contenant le texte et les boutons à l'item
        listWidget->setItemWidget(item, itemWidget);

        // Passer au maillon suivant dans la liste
        crt = crt->getSuiv(); // Passer au prochain participant dans la liste
    }

    // Ajouter la liste à la fenêtre modale
    fenetreListe->setLayout(layout);

    // Définir la taille de la fenêtre
    fenetreListe->resize(600, 400);  // Ajuste la taille de la fenêtre ici selon tes besoins
    fenetreListe->setMinimumSize(450,400);

    // Affichez la fenêtre
    fenetreListe->exec();
}











void testtenetre::ajouterRDV()
{
    string objectif = objectifLineEditAR->text().toStdString();
    QString date = dateLineEditAR->text();
    int hdebut = heureDebutLineEditAR->text().toInt();
    int hfin = heureFinLineEditAR->text().toInt();

    QStringList dateList = date.split("/");
    int jour = dateList[0].toInt();
    int mois = dateList[1].toInt();
    int an = dateList[2].toInt();

    if(QString::fromStdString(objectif)=="" || date.isEmpty() || QString::number(hdebut)=="" || QString::number(hfin)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs.");
        return;
    }

    RDV rr;
    RDV r{objectif,jour,mois,an,hdebut,hfin};

    if(QString::fromStdString(r.retourneNom())==""   || r.retourneHeureFin()<=r.retourneHeureDebut() || r.retourneJour()>30
        || r.retourneHeureDebut()<0 || r.retourneHeureDebut()>=24 || r.retourneHeureFin()<0 || r.retourneHeureFin()>=24)

    {
        QMessageBox::critical(this, "Erreur", "Veuillez saisir correctement tous les champs.");
        return;
    }

    if(listeRDV.chercheParNom(objectif,rr))
    {
        QMessageBox::critical(this, "Erreur","Un rendez-vous du même nom existe déjà");
    }
    else
    {
        listeRDV.ajtRdv(r);

        QMessageBox::information(this, "Ajout réussi",
                                 QString("Nom : ") + QString::fromStdString(r.retourneNom()) + "\n" +
                                     QString("Jour : ") + QString::number(r.retourneJour()) +"/" +QString::number(r.retourneMois())+"/"+QString::number(r.retourneAn())+ "\n" +
                                     QString("Heure de début : ") + QString::number(r.retourneHeureDebut()) + "\n" +
                                     QString("Heure de Fin : ") + QString::number(r.retourneHeureFin()));
    }
    objectifLineEditAR->clear();
    dateLineEditAR->clear();
    heureDebutLineEditAR->clear();
    heureFinLineEditAR->clear();

}


void testtenetre::modifierRDV(RDV r, QDialog* fenetreListe)
{
    // Créer un QDialog pour afficher la fenêtre de modification
    QDialog* modificationDialog = new QDialog(this);
    modificationDialog->setWindowTitle("Modifier le Rendez-vous");

    // Créer un QVBoxLayout pour la fenêtre modale
    QVBoxLayout* layout = new QVBoxLayout(modificationDialog);

    // Ajouter un QCalendarWidget pour sélectionner la nouvelle date
    QCalendarWidget* calendar = new QCalendarWidget(modificationDialog);
    calendar->setSelectedDate(QDate(r.retourneAn(), r.retourneMois(), r.retourneJour()));  // Date actuelle du RDV
    layout->addWidget(calendar);

    // Ajouter des champs de texte pour saisir les nouvelles heures
    QHBoxLayout* heureLayout = new QHBoxLayout;
    QLabel* heureDebutLabel = new QLabel("Heure de début:", modificationDialog);
    QSpinBox* heureDebutSpinBox = new QSpinBox(modificationDialog);
    heureDebutSpinBox->setRange(0, 23);
    heureDebutSpinBox->setValue(r.retourneHeureDebut());  // Heure actuelle du RDV

    QLabel* heureFinLabel = new QLabel("Heure de fin:", modificationDialog);
    QSpinBox* heureFinSpinBox = new QSpinBox(modificationDialog);
    heureFinSpinBox->setRange(0, 23);
    heureFinSpinBox->setValue(r.retourneHeureFin());  // Heure actuelle du RDV

    heureLayout->addWidget(heureDebutLabel);
    heureLayout->addWidget(heureDebutSpinBox);
    heureLayout->addWidget(heureFinLabel);
    heureLayout->addWidget(heureFinSpinBox);
    layout->addLayout(heureLayout);

    // Ajouter un bouton pour confirmer la modification
    QPushButton* modifierButton = new QPushButton("Modifier le RDV", modificationDialog);
    layout->addWidget(modifierButton);

    // Connecter le bouton de modification
    connect(modifierButton, &QPushButton::clicked, this, [this, &calendar, &r, &fenetreListe, heureDebutSpinBox, heureFinSpinBox, modificationDialog](){
        // Récupérer la date sélectionnée
        QDate selectedDate = calendar->selectedDate();
        int jour = selectedDate.day();
        int mois = selectedDate.month();
        int an = selectedDate.year();

        int heureDebut = heureDebutSpinBox->value();
        int heureFin = heureFinSpinBox->value();

        // Vérifier si l'heure de fin est après l'heure de début
        if (heureFin <= heureDebut) {
            QMessageBox::critical(this, "Erreur", "L'heure de fin doit être après l'heure de début.");
            return;
        }

        // Créer un nouvel objet RDV avec les nouvelles informations
        RDV nouveauRDV{r.retourneNom(), jour, mois, an, heureDebut, heureFin};

        // Vérifier si la date et l'heure sont valides
        if (jour <= 0 || jour > 31 || mois <= 0 || mois > 12 || an < 1000 || an > 9999) {
            QMessageBox::critical(this, "Erreur", "La date est invalide.");
            return;
        }

        // Vérification de la disponibilité des participants pour ce rendez-vous
        if (listeRDV.participantsDispo(r, nouveauRDV, listeP)) {
            // Si disponible, modifier le RDV
            listeRDV.modifierRdv(r.retourneNom(), heureDebut, heureFin, jour, mois, an);
            QMessageBox::information(this, "Modification réussie", "Le rendez-vous a été modifié avec succès.");
        } else {
            // Si pas disponible, afficher une erreur
            QMessageBox::critical(this, "Erreur", "Un ou plusieurs participants ne sont pas disponibles.");
        }

        // Fermer la fenêtre de la liste des rendez-vous
        if (fenetreListe) {
            fenetreListe->close();
        }

        // Réafficher la liste des rendez-vous mise à jour
        afficherRDVs();

        // Fermer la fenêtre de modification
        modificationDialog->close();  // Ferme directement la fenêtre modale après la modification
    });

    listeRDV.enregistrerDansFichier(r);
    // Afficher la fenêtre modale
    modificationDialog->exec();

}





/**void testtenetre::supprimerRDV(RDV r,QDialog* fenetreListe)
{
    string nom = r.retourneNom();
    if(QString::fromStdString(nom)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un rendez-vous.");
        return;
    }

    RDV rd;
    if(listeRDV.chercheParNom(nom,rd))
    {
        listeRDV.suprRdv(rd);
        listeP.suppRDV(rd);
        QMessageBox::information(this,"Suppression réussie" ,"Rendez-vous supprimé avec succès");

    }
    else
    {
        QMessageBox::critical(this, "Erreur","Le nom entré ne correspond à aucun rendez-vous");

    }

    // Fermer la fenêtre de la liste des rendez-vous
    if (fenetreListe) {
        fenetreListe->close();
    }

    // Réafficher la liste des rendez-vous mise à jour
    afficherRDVs();
    objectifLineEditSR->clear();
}*/
//test
void testtenetre::supprimerRDV(RDV r, QDialog* fenetreListe) {
    string nom = r.retourneNom();
    if(QString::fromStdString(nom)=="") {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un rendez-vous.");
        return;
    }

    RDV rd;
    if(listeRDV.chercheParNom(nom, rd)) {
        // Retirer le rendez-vous
        listeRDV.suprRdv(rd);
        listeP.suppRDV(rd);

        QMessageBox::information(this,"Suppression réussie", "Rendez-vous supprimé avec succès");

        // Fermer la fenêtre de la liste des rendez-vous si elle est ouverte
        if (fenetreListe) {
            fenetreListe->close();
        }

        // Réafficher la liste mise à jour
        afficherRDVs();
    } else {
        QMessageBox::critical(this, "Erreur", "Le nom entré ne correspond à aucun rendez-vous");
    }
}



void testtenetre::afficherRDVs() {
    // Créez une instance de la nouvelle fenêtre pour afficher la liste
    QDialog* fenetreListe = new QDialog(this);
    fenetreListe->setWindowTitle("Liste des rendez-vous");

    // Créer un QVBoxLayout pour la fenêtre modale
    QVBoxLayout* layout = new QVBoxLayout(fenetreListe);

    // Créer un QListWidget pour afficher les rendez-vous
    QListWidget* listWidget = new QListWidget(fenetreListe);
    layout->addWidget(listWidget);

    // Effacer les anciens éléments avant de réafficher la liste mise à jour
    listWidget->clear();

    // Parcourir les rendez-vous de la liste
    chainonRDV* crt = listeRDV.getTete(); // Supposons que `getTete()` donne la tête de la liste des rendez-vous
    while (crt != nullptr) {
        // Utiliser le getter pour obtenir le rendez-vous
        RDV& r = crt->getRDV();

        // Créer une chaîne formatée pour afficher les informations complètes du rendez-vous
        QString itemText = "Nom: " + QString::fromStdString(r.retourneNom()) + "\n" +
                           "Date: " + QString::number(r.retourneJour()) + "/" +
                           QString::number(r.retourneMois()) + "/" +
                           QString::number(r.retourneAn()) + "\n" +
                           "Heure début: " + QString::number(r.retourneHeureDebut()) + "\n" +
                           "Heure fin: " + QString::number(r.retourneHeureFin());

        // Créer un item pour chaque rendez-vous
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);

        // Ajouter un séparateur visuel entre les éléments
        QFrame* separator = new QFrame();
        separator->setFrameShape(QFrame::HLine);  // Ligne horizontale
        separator->setFrameShadow(QFrame::Sunken);
        listWidget->addItem(new QListWidgetItem());  // Ajout d'un espace pour la ligne de séparation
        listWidget->setItemWidget(listWidget->item(listWidget->count() - 1), separator);

        // Créer un widget contenant les boutons de modification, suppression et ajout au participant
        QWidget* itemWidget = new QWidget;
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);

        // Créer un spacer pour pousser les boutons à droite
        QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        itemLayout->addSpacerItem(spacer);  // Pousse les boutons vers la droite

        // Ajouter un bouton de modification avec un stylo
        QPushButton* editButton = new QPushButton(itemWidget);
        editButton->setFixedSize(40, 35);  // Taille fixe pour les boutons
        QIcon editIcon("../../icons/pencil_icon.png"); // Remplacer par le chemin vers l'icône du stylo
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(40, 20));  // Taille de l'icône
        connect(editButton, &QPushButton::clicked, this, [this, r, &fenetreListe]() {
            // Logique pour modifier le rendez-vous
            this->modifierRDV(r, fenetreListe);
        });

        // Ajouter un bouton de suppression avec une corbeille
        QPushButton* deleteButton = new QPushButton(itemWidget);
        deleteButton->setFixedSize(40, 35);  // Taille fixe pour les boutons
        QIcon deleteIcon("../../icons/trash_icon.png"); // Remplacer par le chemin vers l'icône de la corbeille
        deleteButton->setIcon(deleteIcon);
        deleteButton->setIconSize(QSize(40, 20));  // Taille de l'icône
        connect(deleteButton, &QPushButton::clicked, this, [this, r, fenetreListe]() {
            // Logique pour supprimer le rendez-vous
            this->supprimerRDV(r, fenetreListe);  // Passe le rendez-vous à supprimer
        });

        // Ajouter les boutons au layout
        itemLayout->addWidget(editButton);
        itemLayout->addWidget(deleteButton);

        // Ajouter le widget contenant le texte et les boutons à l'item
        listWidget->setItemWidget(item, itemWidget);

        // Passer au maillon suivant dans la liste des rendez-vous
        crt = crt->getSuiv(); // Passer au prochain rendez-vous dans la liste
    }

    // Ajouter la liste à la fenêtre modale
    fenetreListe->setLayout(layout);

    // Définir la taille de la fenêtre
    fenetreListe->resize(600, 400);  // Ajuste la taille de la fenêtre ici selon tes besoins
    fenetreListe->setMinimumSize(450, 400);

    // Afficher la fenêtre
    fenetreListe->exec();
}


/**void testtenetre::ajouterParticipantAUnRDV(participant p, QDialog* fenetreListe)
{
    // Demander le nom du rendez-vous auquel on veut ajouter le participant
    QString nomrdv = QInputDialog::getText(this, "Ajouter au rendez-vous",
                                           "Entrez le nom du rendez-vous:", QLineEdit::Normal,
                                           "", nullptr);
    if (nomrdv.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Le nom du rendez-vous ne peut pas être vide.");
        return;
    }

    RDV rr;
    if (!listeRDV.chercheParNom(nomrdv.toStdString(), rr)) {
        QMessageBox::critical(this, "Erreur", "Le rendez-vous n'existe pas.");
        return;
    }

    // Vérifier si le participant est déjà associé à ce rendez-vous
    if (listeP.chevaucheRDV(p, rr)) {
        QMessageBox::critical(this, "Erreur", "Le participant est déjà inscrit à ce rendez-vous ou a un autre rendez-vous qui a lieu au même moment.");
        return;
    }

    // Ajouter le participant au rendez-vous
    listeRDV.ajouterParticipantRdv(nomrdv.toStdString(), p);
    listeP.ajouterRDVP(p.retourneNom(), p.retournePrenom(), rr);

    // Afficher un message de succès
    QMessageBox::information(this, "Ajout réussi", "Le participant a été ajouté au rendez-vous avec succès.");

    // Fermer la fenêtre de la liste actuelle si elle est ouverte
    if(fenetreListe)
        fenetreListe->close();  // Fermer la fenêtre actuelle

    // Actualiser la liste des participants
    afficherPersonnes();  // Réouvrir la fenêtre avec la liste mise à jour
}*/
//test
void testtenetre::ajouterParticipantAUnRDV(participant p, QDialog* fenetreListe) {
    QString nomrdv = QInputDialog::getText(this, "Ajouter au rendez-vous",
                                           "Entrez le nom du rendez-vous:", QLineEdit::Normal,
                                           "", nullptr);
    if (nomrdv.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Le nom du rendez-vous ne peut pas être vide.");
        return;
    }

    RDV rr;
    if (!listeRDV.chercheParNom(nomrdv.toStdString(), rr)) {
        QMessageBox::critical(this, "Erreur", "Le rendez-vous n'existe pas.");
        return;
    }

    // Vérifier si le participant est déjà associé au rendez-vous
    if (listeP.chevaucheRDV(p, rr)) {
        QMessageBox::critical(this, "Erreur", "Le participant est déjà inscrit à ce rendez-vous ou a un autre rendez-vous qui a lieu au même moment.");
        return;
    }

    // Ajouter le participant au rendez-vous
    listeRDV.ajouterParticipantRdv(nomrdv.toStdString(), p);
    listeP.ajouterRDVP(p.retourneNom(), p.retournePrenom(), rr);

    // Afficher un message de succès
    QMessageBox::information(this, "Ajout réussi", "Le participant a été ajouté au rendez-vous avec succès.");

    // Fermer la fenêtre de la liste actuelle si elle est ouverte
    if(fenetreListe) {
        fenetreListe->close();  // Fermer la fenêtre actuelle
    }

    // Actualiser la liste des participants
    afficherPersonnes();  // Réouvrir la fenêtre avec la liste mise à jour
}


void testtenetre::afficherRDVsDUnParticipant()
{
    string nom = nomLineEditAffRsP->text().toStdString();
    string prenom = prenomLineEditAffRsP->text().toStdString();

    if(QString::fromStdString(nom)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un nom d'une personne.");
        return;
    }

    participant pp;
    if(listeP.trouverParticipant(nom,prenom,pp))
    {
        QString liste = QString::fromStdString(listeP.afficherRDVs(nom));
        // Créez une instance de la nouvelle fenêtre pour afficher la liste
        QDialog* fenetreListe = new QDialog(this);
        fenetreListe->setWindowTitle("Liste des rendez-vous d'un participant");

        // Créez une zone de texte pour afficher la liste
        QTextEdit* texteListe = new QTextEdit(fenetreListe);
        texteListe->setReadOnly(true);
        texteListe->setText( liste);


        // Ajoutez la zone de texte à la fenêtre
        QVBoxLayout* layout = new QVBoxLayout(fenetreListe);
        layout->addWidget(texteListe);
        fenetreListe->setLayout(layout);

        // Affichez la fenêtre
        fenetreListe->exec();

    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Le nom entré n'appartient à aucun participant");
    }

    nomLineEditAffRsP->clear();
    prenomLineEditAffRsP->clear();

}

void testtenetre::appliquerStyleBoutons()
{
    // Charger la police Poppins
    int id = QFontDatabase::addApplicationFont("../../fonts/Poppins-Regular.ttf");
    if (id != -1) {
        QStringList familles = QFontDatabase::applicationFontFamilies(id);
        if (!familles.isEmpty()) {
            QFont poppins(familles.at(0), 10);  // Taille par défaut : 10
            QApplication::setFont(poppins);
        }
    }

}







void testtenetre::afficherRDVsDate()
{
    QString date = objectifLineEditAffRD->text();
    QStringList dateList = date.split("/");
    int jour = dateList[0].toInt();
    int mois = dateList[1].toInt();
    int an = dateList[2].toInt();

    if(date.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer une date.");
        return;
    }

    QString liste = QString::fromStdString(listeRDV.afficherRdvDate(jour,mois,an));
    // Créez une instance de la nouvelle fenêtre pour afficher la liste
    QDialog* fenetreListe = new QDialog(this);
    fenetreListe->setWindowTitle("Liste des rendez-vous pour une date");

    // Créez une zone de texte pour afficher la liste
    QTextEdit* texteListe = new QTextEdit(fenetreListe);
    texteListe->setReadOnly(true);
    texteListe->setText( liste);


    // Ajoutez la zone de texte à la fenêtre
    QVBoxLayout* layout = new QVBoxLayout(fenetreListe);
    layout->addWidget(texteListe);
    fenetreListe->setLayout(layout);

    // Affichez la fenêtre
    fenetreListe->exec();



    objectifLineEditAffRD->clear();
}


void testtenetre::rechercher()
{

    string nom = objectifLineEditRechercheP->text().toStdString();
    string prenom = prenomobjectifLineEditRechercheP->text().toStdString();
    QString date = dateLineEditRechercheP->text();
    int hdebut = hDebutLineEditRechercheP->text().toInt();
    int hfin = hFinLineEditRechercheP->text().toInt();

    QStringList dateList = date.split("/");
    int jour = dateList[0].toInt();
    int mois = dateList[1].toInt();
    int an = dateList[2].toInt();

    if(date.isEmpty() || QString::fromStdString(nom)=="" || QString::fromStdString(prenom)=="" || QString::number(hdebut)=="" || QString::number(hfin)=="")
    {
        QMessageBox::critical(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }
    participant pp;
    if(listeP.trouverParticipant(nom,prenom,pp))
    {
        if(listeP.estOccupe(pp,jour,hdebut,hfin,mois,an))
        {
            QMessageBox::information(this,"Personne trouvée","La personne selectionnée a un RDV a la date entree.");

        }
        else
        {
            QMessageBox::warning(this,"Personne non trouvée","La personne selectionnée n'a aucun RDV a la date entree.");

        }

    }
    else
    {
        QMessageBox::critical(this, "Erreur","Le nom entré n'appartient à aucun participant");

    }
    objectifLineEditRechercheP->clear();
    dateLineEditRechercheP->clear();
    hDebutLineEditRechercheP->clear();
    hFinLineEditRechercheP->clear();
}

void testtenetre::afficherParticipantsDUnRDV()
{
    string nom = nomLineEditAffPR->text().toStdString();

    if(QString::fromStdString(nom)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un nom d'un rendez-vous.");
        return;
    }
    RDV rr;
    if(listeRDV.chercheParNom(nom,rr))
    {

        QString liste = QString::fromStdString(listeRDV.afficherParticipantsRDV(nom));
        // Créez une instance de la nouvelle fenêtre pour afficher la liste
        QDialog* fenetreListe = new QDialog(this);
        fenetreListe->setWindowTitle("Liste des participants d'un rendez-vous");

        // Créez une zone de texte pour afficher la liste
        QTextEdit* texteListe = new QTextEdit(fenetreListe);
        texteListe->setReadOnly(true);
        texteListe->setText( liste);


        // Ajoutez la zone de texte à la fenêtre
        QVBoxLayout* layout = new QVBoxLayout(fenetreListe);
        layout->addWidget(texteListe);
        fenetreListe->setLayout(layout);

        // Affichez la fenêtre
        fenetreListe->exec();

    }
    else{
        QMessageBox::critical(this, "Erreur","Le nom entré n'appartient à aucun rendez-vous");

    }
    nomLineEditAffPR->clear();
}

void testtenetre::on_calendarWidget_selectionChanged()
{

    int jour,mois, an;
    QCalendarWidget* calendar = qobject_cast<QCalendarWidget*>(sender());
    if (calendar)
    {
        QDate selectedDate = calendar->selectedDate();
        int jour = selectedDate.day();
        int  mois = selectedDate.month();
        int an = selectedDate.year();

    }
}

void testtenetre::exporterParticipant()
{
    nomFichier = nomLineEditFichierExportParticipants->text().toStdString();

    if(QString::fromStdString(nomFichier)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un nom d'un fichier.");
        return;
    }
    listeP.sauvegarderListe(nomFichier);
    QMessageBox::information(this,"Exportation réussie","Exportation faites avec succes");
    nomLineEditFichierExportParticipants->clear();
}

void testtenetre::importerParticipant()
{
    nomFichier = nomLineEditFichierImportParticipants->text().toStdString();
    if(QString::fromStdString(nomFichier)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un nom d'un fichier.");
        return;
    }
    bool ok=true;

    ifstream ifs;
    ifs.open(nomFichier);

    if(!ifs.is_open())
    {
        QMessageBox::critical(this, "Erreur","MAUVAIS FICHIER");

    }
    else
    {
        ifs>>nb;
        for(int i=0; i<nb; i++)
        {
            ifs>>nom>>prenom>>email>>tel;
            participant p{nom,prenom,tel,email};
            participant pp;
            if(!listeP.trouverParticipant(nom,prenom,pp))
            {
                listeP.ajouterParticipant(p);
            }
            else
                ok=false;
        }
        if(ok)
            QMessageBox::information(this,"Importation réussie","Importation faites avec succes");
        else
            QMessageBox::warning(this,"Importation faites partiellement","Echec de l'importation pour au moins un participants car le nom existe déjà dans la base de données");
        ifs.close();
    }

    nomLineEditFichierImportParticipants->clear();
}

void testtenetre::exporterRDV()
{
    nomFichier = nomLineEditFichierExportRDV->text().toStdString();
    if(QString::fromStdString(nomFichier)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un nom d'un fichier.");
        return;
    }

    listeRDV.sauvegardeListe(nomFichier);
    QMessageBox::information(this,"Exportation réussie","Exportation faites avec succes");
    nomLineEditFichierExportRDV->clear();
}


void testtenetre::exporterCarnet()
{

    listeP.sauvegarderRDVs();
    QMessageBox::information(this,"Exportation réussie","Exportation faites avec succes");

}


void testtenetre::importerRDV()
{
    nomFichier = nomLineEditFichierImportRDV->text().toStdString();
    string ponctuation;
    if(QString::fromStdString(nomFichier)=="")
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un nom d'un fichier.");
        return;
    }
    bool ok=true;

    ifstream ifs;
    ifs.open(nomFichier);

    if(!ifs.is_open())
    {
        QMessageBox::critical(this, "Erreur","MAUVAIS FICHIER");

    }
    else
    {
        ifs>>nb;
        for(int i=0; i<nb; i++)
        {
            ifs>>nom>>jour>>mois>>an>>hdebut>>hfin;
            RDV r{nom,jour,mois,an,hdebut,hfin};
            RDV rr;
            if(!listeRDV.chercheParNom(nom,rr))
            {
                listeRDV.ajtRdv(r);
            }
            else
            {
                ok=false;
            }
        }
        if(ok)
            QMessageBox::information(this,"Importation réussie","Importation faites avec succes");
        else
            QMessageBox::warning(this,"Importation faites partiellement","Echec de l'importation pour au moins 1 rdv car le nom existe déjà dans la base de données");
        ifs.close();
    }

    nomLineEditFichierImportRDV->clear();
}

void testtenetre::retirer(participant p, QDialog* fenetreListe)
{
    // Ouvrir une fenêtre modale pour demander le nom du rendez-vous
    bool ok;
    QString nomrdv = QInputDialog::getText(this, "Nom du Rendez-Vous",
                                           "Entrez le nom du rendez-vous:", QLineEdit::Normal,
                                           "", &ok);

    // Si l'utilisateur a annulé ou laissé le champ vide
    if (!ok || nomrdv.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Le nom du rendez-vous ne peut pas être vide.");
        return;
    }

    // Vérifier si le rendez-vous existe
    RDV rr;
    if (!listeRDV.chercheParNom(nomrdv.toStdString(), rr)) {
        QMessageBox::critical(this, "Erreur", "Le rendez-vous n'existe pas.");
        return;
    }

    // Vérifier si le participant est bien associé au rendez-vous
    if (!listeP.trouverParticipant(p.retourneNom(), p.retournePrenom(), p)) {
        QMessageBox::critical(this, "Erreur", "Le participant n'existe pas.");
        return;
    }

    // Vérifier si le participant est associé à ce rendez-vous
    bool associe = false;
    for (auto& rdvAssocie : rr.getParticipants()) {
        if (rdvAssocie == p) {
            associe = true;
            break;
        }
    }

    if (!associe) {
        QMessageBox::critical(this, "Erreur", "Le participant n'est pas associé à ce rendez-vous.");
        return;
    }

    // Supprimer le participant du rendez-vous
    listeP.supprimerRDVP(p.retourneNom(), p.retournePrenom(), rr);
    listeRDV.retirerParticipantRdv(nomrdv.toStdString(), p);

    // Afficher un message de succès
    QMessageBox::information(this, "Opération réussie", "Participant retiré du rendez-vous avec succès.");

    // Fermer la fenêtre de la liste actuelle si elle existe
    if (fenetreListe) {
        fenetreListe->close();
    }

    // Réactualiser la liste des participants
    afficherPersonnes();

    // Vider les champs de saisie
    nomRDVLineEditRetirerParticipant->clear();
}






void testtenetre::quitter()
{
    close();
}




