#include <iostream>
#include "wifibotv3.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    wifibotv3 fenetre;

    fenetre.show();
      return a.exec();
}
