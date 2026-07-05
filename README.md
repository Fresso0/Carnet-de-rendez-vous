# 📅 Carnet de Rendez-vous

Application de bureau de gestion de rendez-vous et de participants, développée en **C++ / Qt Widgets** dans le cadre d'un projet de Licence 2 Informatique.

## 👥 Équipe

| Membre | Rôle |
|---|---|
| **Fresnel Dossou** | Chef de projet & Développeur |
| **Fatoumata Sall** | Développeuse |

## 📖 Description

Le Carnet de Rendez-vous est une application graphique permettant de gérer un carnet de contacts (participants) et un agenda de rendez-vous. Chaque rendez-vous possède un nom, une date, un horaire (heure de début et de fin) et une liste de participants. L'application détecte automatiquement les **chevauchements de rendez-vous** et vérifie la **disponibilité des participants** avant toute modification.

## ✨ Fonctionnalités

### Gestion des participants
- Ajouter, modifier et supprimer un participant (nom, prénom, téléphone, e-mail)
- Afficher la liste des participants
- Afficher tous les rendez-vous d'un participant

### Gestion des rendez-vous
- Créer, modifier et supprimer un rendez-vous
- Ajouter ou retirer des participants à un rendez-vous
- Afficher les participants d'un rendez-vous
- Afficher les rendez-vous d'une date donnée (via un calendrier interactif `QCalendarWidget`)
- Détection des conflits d'horaires (chevauchements) et vérification de la disponibilité des participants

### Recherche & Import / Export
- Recherche dans le carnet
- Import / export de participants
- Import / export de rendez-vous
- Export complet du carnet
- Sauvegarde et chargement des données depuis des fichiers

## 🏗️ Architecture

Le projet sépare la logique métier de l'interface graphique :

| Fichier | Rôle |
|---|---|
| `rdv.h / rdv.cpp` | Classe `RDV` : un rendez-vous (nom, date, horaires, participants), surcharges d'opérateurs, détection de chevauchement |
| `participant.h / participant.cpp` | Classe `participant` : un contact et ses rendez-vous |
| `ldrdv.h / ldrdv.cpp` | Liste doublement chaînée de rendez-vous (`chainonRDV` + `ldrdv`), avec sauvegarde/chargement fichier |
| `ldpersonne.h / ldpersonne.cpp` | Liste doublement chaînée de participants (`chainonParticipant` + `ldpersonne`) |
| `fenetrecarnet.h / fenetrecarnet.cpp` | Interface graphique principale (`QMainWindow`) : formulaires, tableaux, calendrier, dialogues |
| `main.cpp` | Point d'entrée de l'application |

Les listes de participants et de rendez-vous sont implémentées **manuellement sous forme de listes doublement chaînées**, dans un objectif pédagogique (structures de données).

## 🛠️ Technologies

- **Langage :** C++17
- **Framework :** Qt 6 (modules `core`, `gui`, `widgets`)
- **Build :** qmake (`Carnet.pro`)
- **IDE recommandé :** Qt Creator

## 🚀 Compilation et exécution

### Avec Qt Creator (recommandé)
1. Ouvrir `Carnet.pro` dans Qt Creator
2. Configurer un kit Qt 6 (testé avec Qt 6.5.3 MinGW 64-bit)
3. Compiler et exécuter (Ctrl+R)

### En ligne de commande
```bash
qmake Carnet.pro
make        # ou mingw32-make sous Windows
./Carnet
```

## 📂 Sauvegarde des données

Les participants et les rendez-vous peuvent être sauvegardés et rechargés depuis des fichiers texte via les fonctions d'import/export de l'interface.

---

*Projet réalisé en L2 Informatique — Programmation orientée objet & interfaces graphiques Qt.*
