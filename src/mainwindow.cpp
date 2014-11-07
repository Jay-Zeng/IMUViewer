#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     QTimer *timer = new QTimer(this);
    _pTdb = new TableauDeBord();

    stp=false;
    int max=_pTdb->getnbEch();

    // Affectation de la centrale du tableau de bord aux widgets capteurs
    this->findChild<AccCapteurGL*>("glCapteurAcc")->setCentrale(&_pTdb->_IMU);
    this->findChild<GyroCapteurGL*>("glCapteurGyro")->setCentrale(&_pTdb->_IMU);
    this->findChild<MagneCapteurGL*>("glCapteurMagne")->setCentrale(&_pTdb->_IMU);

    _pcGL = this->findChild<PrincipalCapteurGL*>("glPrincipal");
    _pcGL->setCentrale(&_pTdb->_IMU);


    // Affectation de la centrale et du tableau de bord aux widgets signaux
    this->findChild<gyrograph*>("glSignalAcc")->setCentrale (&_pTdb->_IMU);
    this->findChild<gyrograph*>("glSignalAcc")->setTableauDeBord (_pTdb);
    this->findChild<gyrograph*>("glSignalAcc")->setsignalIndex (0);

    this->findChild<gyrograph*>("glSignalGyro")->setCentrale (&_pTdb->_IMU);
    this->findChild<gyrograph*>("glSignalGyro")->setTableauDeBord (_pTdb);
    this->findChild<gyrograph*>("glSignalGyro")->setsignalIndex (3);

    this->findChild<gyrograph*>("glSignalMagne")->setCentrale (&_pTdb->_IMU);
    this->findChild<gyrograph*>("glSignalMagne")->setTableauDeBord (_pTdb);
    this->findChild<gyrograph*>("glSignalMagne")->setsignalIndex (6);

    // + fenetre d'évolution de la centrale
    _pcGL->setFenetreEvolutionCentrale(_pTdb->getCoinInferieur(),_pTdb->getCoinSuperieur());

    ui->lcdNumber->setDigitCount(8);
    ui->horizontalSlider->setRange(0,max*1000);

    // Mise à jour de la centrale inertielle en suivant le timer
    QObject::connect(timer, SIGNAL(timeout()), _pTdb, SLOT(majCentrale()));
    // Mise à jour des widgets gl
    QObject::connect(timer, SIGNAL(timeout()), this->findChild<AccCapteurGL*>("glCapteurAcc"), SLOT(updateGL()));
    QObject::connect(timer, SIGNAL(timeout()), this->findChild<GyroCapteurGL*>("glCapteurGyro"), SLOT(updateGL()));
    QObject::connect(timer, SIGNAL(timeout()), this->findChild<MagneCapteurGL*>("glCapteurMagne"), SLOT(updateGL()));
    QObject::connect(timer, SIGNAL(timeout()), this->findChild<PrincipalCapteurGL*>("glPrincipal"), SLOT(updateGL()));
    QObject::connect(timer, SIGNAL(timeout()), this->findChild<gyrograph*>("glSignalAcc"), SLOT(updateGL()));

    // Mise à jour de l'écran LCD
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(majLCD()));

    QObject::connect(timer, SIGNAL(timeout()), this->findChild<gyrograph*>("glSignalGyro"), SLOT(updateGL()));
    QObject::connect(timer, SIGNAL(timeout()), this->findChild<gyrograph*>("glSignalMagne"), SLOT(updateGL()));

    // Evolution slider + boutons
    connect(timer, SIGNAL(timeout()), this, SLOT(setslidervalue()));
    connect(ui->pushButton_3, SIGNAL(clicked()), timer, SLOT(stop()));
    connect(ui->pushButton, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(timer_stop()));

    // timer calé sur la fréquence d'échantillonage des signaux
    //timer->start(1000/freqEch);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // On renvoit la gestion du clavier à la fenetre d'affichage principal
    _pcGL->keyPressEvent(event);
}


void MainWindow::majLCD()
{
    std::ostringstream s;
    s<<(double)_pTdb->getiCourant()/(double)freqEch;
    std::string sttime=s.str();
    ui->lcdNumber->display(QString(sttime.c_str()));
}

void MainWindow::setslidervalue()
{
    int valslider=this->_pTdb->getiCourant();
    int value = ui->horizontalSlider->value()+valslider;
    ui->horizontalSlider->setValue(value);

}

void MainWindow::timer_stop()
{
  timer->stop();
  ui->lcdNumber->display(0);
  ui->horizontalSlider->setValue(0);
  stp=true;

}

