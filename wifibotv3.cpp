#include "wifibotv3.h"
#include <ui_wifibotv3.h>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QWebView>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTcpSocket>
#include <QTextEdit>
#include <QProgressBar>
#include <QSlider>
#include <QHBoxLayout>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QKeyEvent>

using namespace std;




// Déclaration du timer
QTimer *timer;
// Déclaration du socket TCP
QTcpSocket socket;
// Déclaration de la trame
QByteArray trame;
// Déclaration de l'adresse ip
QString ip = "192.168.1.106";
//Déclaration pour la caméra
QNetworkAccessManager *Camera;
// Déclaration des positions de la caméra
QString Haut,Droite,Gauche,Bas;
// Déclaration direction wifi
quint8 Goche, Droit;
// Déclaration du controle d'erreur CRC
quint16 crc;

quint16 VitesseL,VitesseR;

quint8 capteur1,capteur2;

// Constructeur par default
wifibotv3::wifibotv3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //Caméra

    Camera = new QNetworkAccessManager(this);
    Haut="/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200";
    Droite="/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200";
    Gauche="/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200";
    Bas="/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200";


    // Timer
    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer,SIGNAL(timeout()),this,SLOT(Vitesse()));


    ui->setupUi(this);
}


/**** Destructeur par default ****/

wifibotv3::~wifibotv3()
{
    delete ui;
}



/**** Connexion au robot ****/

void wifibotv3::connexionviaTCP(QString ip,int port)
{
     cout << "connexion au robot" << endl;
     socket.connectToHost(ip, port);
     connect(&socket, SIGNAL(connected()),this, SLOT(acquittement_de_la_connexion()));
     cout << "connecté" << endl;
}

/**** Gestion de la connexion ( ui ) ****/
void wifibotv3::on_connect_clicked()
{
    socket.connectToHost(ui->AdresseIp->text(),15020);
    connect(&socket, SIGNAL(connected()),this, SLOT(Connexion()));
    connect(&socket, SIGNAL(disconnected()),this, SLOT(Disconnexion()));
    timer->start();
}


/**** Si on est connecté,on active la camera + Annonce qu'il est connecté****/
void wifibotv3::Connexion()
{

    ui->AdresseIp->setText("Connecté !");
    QString webcam = "http://192.168.1.106:8080/javascript_simple.html";
    ui->cam->load(QUrl("http://192.168.1.106:8080/javascript_simple.html"));
    ui->cam->show();
    timer->start();

}

/**** Envoie un signal d'acquittement pour la connexion ****/
void wifibotv3::acquittement_de_la_connexion()
{
    emit vers_IHM_acquittement_connection();
    cout <<"connexion établie "<< endl;
}


/**** Bouton pour se déconnecter ****/
void wifibotv3::on_Disconnexion_clicked()
{
    socket.disconnectFromHost();
    cout << "deconnexion du robot" <<endl;
}


void wifibotv3::Disconnexion()
{
   ui->AdresseIp->setText("Déconnecté");
}


/**** Trame pour communiquer le déplacement du robot****/

void wifibotv3::Tramemove(quint8 Goche, quint8 Droit, int direction)
{

    trame.clear();
    trame.append((char)0xff);
    trame.append((char)0x07);
    trame.append((quint8)Goche);
    trame.append((char)(0x00));
    trame.append((quint8)Droit);
    trame.append((char)(0x00));
    if(direction==0){
        trame.append((char)0x00);
    }
    else if(direction==1){
        trame.append((char)80);
    }
    else if(direction==2){
        trame.append((char)64);
    }
    else if(direction==3){
        trame.append((char)16);
    }

   // bit 7 et 8 CRC
    quint16 crc = Crc16(&trame, 1);
    trame.append((char)crc);
    trame.append((char)(crc>>8));

}




void wifibotv3::Vitesse()
{
    socket.write(trame);

    QDataStream Donnees(&socket);
    Donnees.setByteOrder(QDataStream::LittleEndian);
    quint16 speedL,speedR;
    quint8 cpt1,cpt2;
    Donnees>>speedL>>speedR>>cpt1>>cpt2;
    VitesseL=speedL;
    VitesseR=speedR;
    capteur1=cpt1*3.3/255;
    capteur2=cpt2*3.3/255;
}

/**** Definition du CRC ****/
quint16 wifibotv3::Crc16(QByteArray* byteArray, int pos){
    unsigned char *data = (unsigned char* )byteArray->constData();
    quint16 crc = 0xFFFF;
    quint16 Polynome = 0xA001;
    quint16 Parity = 0;
    for(; pos < byteArray->length(); pos++){
        crc ^= *(data+pos);
        for (unsigned int CptBit = 0; CptBit <= 7 ; CptBit++){
            Parity= crc;
            crc >>= 1;
            if (Parity%2 == true) crc ^= Polynome;
        }
    }
    return crc;
}




/******** Gestion des boutons pour la direction du wifibot *********/

/******  Gestion mouvement de direction ( lorsqu'on reste appuyé ) ******/


void wifibotv3::on_BoutonHaut_pressed()
{
    if(capteur1==0)
    wifibotv3::Tramemove(ui->vitesse->value(),ui->vitesse->value(),1);
    else{
         wifibotv3::Tramemove(0,0,1);
    }
}

void wifibotv3::on_BoutonGauche_pressed()
{

    wifibotv3::Tramemove(ui->vitesse->value(),ui->vitesse->value(),3);
}

void wifibotv3::on_BoutonDroite_pressed()
{

    wifibotv3::Tramemove(ui->vitesse->value(),ui->vitesse->value(),2);
}


void wifibotv3::on_BoutonBas_pressed()
{
     if(capteur2==0)
    wifibotv3::Tramemove(ui->vitesse->value(),ui->vitesse->value(),0);
     else
     {
             wifibotv3::Tramemove(0,0,0);
     }
}


/****** Gestion mouvement de direction ( lorsqu'on relache ) ******/

void wifibotv3::on_BoutonHaut_released()
{
    wifibotv3::Tramemove(0,0,1);
}

void wifibotv3::on_BoutonDroite_released()
{
   wifibotv3::Tramemove(0,0,2);
}

void wifibotv3::on_BoutonGauche_released()
{
    wifibotv3::Tramemove(0,0,3);
}

void wifibotv3::on_BoutonBas_released()
{
    wifibotv3::Tramemove(0,0,0);
}



/******** Gestion des boutons pour la direction de la caméra *********/

void wifibotv3::on_camversleHaut_pressed()
{
    QUrl url("http://192.168.1.106:8080"+Haut);
            Camera->get(QNetworkRequest(url));
}

void wifibotv3::on_camversleBas_pressed()
{
    QUrl url("http://192.168.1.106:8080"+Bas);
            Camera->get(QNetworkRequest(url));
}

void wifibotv3::on_camverslaGauche_pressed()
{

    QUrl url("http://192.168.1.106:8080"+Gauche);
            Camera->get(QNetworkRequest(url));
}

void wifibotv3::on_camverslaDroite_pressed()
{
    QUrl url("http://192.168.1.106:8080"+Droite);
            Camera->get(QNetworkRequest(url));
}


/******* Gestion des boutons avec lettres ********/

void wifibotv3::keyPressEvent(QKeyEvent* event) {

    switch(event-> key()){
    case Qt::Key_Z:
        ui->BoutonHaut->animateClick();

        break;
    case Qt::Key_S:
        ui->BoutonBas->animateClick();

        break;
    case Qt::Key_Q:
        ui->BoutonGauche->animateClick();

        break;
    case Qt::Key_D:
        ui->BoutonDroite->animateClick();

        break;
    case Qt::Key_I:
        ui->camversleHaut->animateClick();
        break;
    case Qt::Key_J:
        ui->camverslaGauche->animateClick();
        break;
    case Qt::Key_K:
        ui->camversleBas->animateClick();
        break;
    case Qt::Key_L:
        ui->camverslaDroite->animateClick();
        break;

    }

}
