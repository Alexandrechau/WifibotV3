#ifndef WIFIBOTV3_H
#define WIFIBOTV3_H
#include <QMainWindow>
#include <QString>


namespace Ui {
class MainWindow;
}

class wifibotv3 : public QMainWindow
{
    Q_OBJECT

public:
     wifibotv3(QWidget *parent = 0);
    ~wifibotv3();

    void connexionviaTCP(QString,int);
    quint16 Crc16(QByteArray*,int);

private slots:


    void Connexion();
    void acquittement_de_la_connexion();
    void Disconnexion();


    void Tramemove(quint8,quint8,int);
    void Vitesse();


    void on_BoutonHaut_released();
    void on_BoutonBas_released();
    void on_BoutonDroite_released();
    void on_BoutonGauche_released();


    void on_BoutonHaut_pressed();
    void on_BoutonGauche_pressed();
    void on_BoutonDroite_pressed();
    void on_BoutonBas_pressed();


    void on_camversleHaut_pressed();
    void on_camversleBas_pressed();
    void on_camverslaGauche_pressed();
    void on_camverslaDroite_pressed();


    void on_connect_clicked();
    void on_Disconnexion_clicked();

    void keyPressEvent(QKeyEvent* );


private:
    Ui::MainWindow *ui;

signals :

 void vers_IHM_acquittement_connection();


};



#endif // WIFIBOTV3_H
