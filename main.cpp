#include "fenetrecarnet.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    fenetrecarnet w;
    w.demarrer();
    w.setWindowTitle("Carnet de rendez-vous");
    w.setWindowIcon(QIcon("rdvImage.jpg"));
    w.show();
    return a.exec();
}
