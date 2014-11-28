#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _pTimer = new QTimer(this);
    Initmainwindow("ferdaousse_mixte_cheville.out");
    //Signal du chargement du fichier
    connect(ui->actionCharger_un_nouveau_fichier, SIGNAL(triggered(bool)),this , SLOT(loadfile()));

    // Affectation des Comboboxs aux widgets appropriés
    // Première Combobox (premier espace d'affichage)
    this->findChild<Combobox*>("comboBox_1")->addItem ("Acc. X (m/s2)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Acc. Y (m/s2)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Acc. Z (m/s2)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Gyro. X (rad/s)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Gyro. Y (rad/s)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Gyro. Z (m/s2)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Magn. X (uT)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Magn. Y (uT)");
    this->findChild<Combobox*>("comboBox_1")->addItem ("Magn. Z (uT)");
    this->findChild<Combobox*>("comboBox_1")->setgyrograph(this->findChild<gyrograph*>("glSignalAcc"));

    // Deuxième Combobox (deuxième espace d'affichage)
    this->findChild<Combobox*>("comboBox_2")->addItem ("Acc. X (m/s2)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Acc. Y (m/s2)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Acc. Z (m/s2)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Gyro. X (rad/s)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Gyro. Y (rad/s)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Gyro. Z (m/s2)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Magn. X (uT)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Magn. Y (uT)");
    this->findChild<Combobox*>("comboBox_2")->addItem ("Magn. Z (uT)");
    this->findChild<Combobox*>("comboBox_2")->setgyrograph(this->findChild<gyrograph*>("glSignalGyro"));

    // Troisième Combobox (troisième espace d'affichage)
    this->findChild<Combobox*>("comboBox_3")->addItem ("Acc. X (m/s2)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Acc. Y (m/s2)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Acc. Z (m/s2)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Gyro. X (rad/s)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Gyro. Y (rad/s)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Gyro. Z (m/s2)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Magn. X (uT)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Magn. Y (uT)");
    this->findChild<Combobox*>("comboBox_3")->addItem ("Magn. Z (uT)");
    this->findChild<Combobox*>("comboBox_3")->setgyrograph(this->findChild<gyrograph*>("glSignalGyro"));


    // Modifications sur les QLabel

   this->findChild<label*>("label_8")->setNum (this->findChild<gyrograph*>("glSignalAcc")->getpTDB ()->get_signaux ()[this->findChild<gyrograph*>("glSignalAcc")->getsignalIndex ()]->getMaxSignal ());
   this->findChild<label*>("label_9")->setNum (-this->findChild<gyrograph*>("glSignalAcc")->getpTDB ()->get_signaux ()[this->findChild<gyrograph*>("glSignalAcc")->getsignalIndex ()]->getMaxSignal ());
   this->findChild<label*>("label_10")->setNum (this->findChild<gyrograph*>("glSignalGyro")->getpTDB ()->get_signaux ()[this->findChild<gyrograph*>("glSignalGyro")->getsignalIndex ()]->getMaxSignal ());
   this->findChild<label*>("label_11")->setNum (this->findChild<gyrograph*>("glSignalGyro")->getpTDB ()->get_signaux ()[this->findChild<gyrograph*>("glSignalGyro")->getsignalIndex ()]->getMaxSignal ());
   this->findChild<label*>("label_12")->setNum (this->findChild<gyrograph*>("glSignalMagne")->getpTDB ()->get_signaux ()[this->findChild<gyrograph*>("glSignalMagne")->getsignalIndex ()]->getMaxSignal ());
   this->findChild<label*>("label_13")->setNum (this->findChild<gyrograph*>("glSignalMagne")->getpTDB ()->get_signaux ()[this->findChild<gyrograph*>("glSignalMagne")->getsignalIndex ()]->getMaxSignal ());

    ui->lcdNumber->setDigitCount(8);

    // Mise à jour de la centrale inertielle en suivant le _pTimer
    QObject::connect(_pTimer, SIGNAL(timeout()), _pTdb, SLOT(majCentrale()));
    // Mise à jour des widgets gl

    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<AccCapteurGL*>("glCapteurAcc"), SLOT(updateGL()));
    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<GyroCapteurGL*>("glCapteurGyro"), SLOT(updateGL()));
    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<MagneCapteurGL*>("glCapteurMagne"), SLOT(updateGL()));
    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<PrincipalCapteurGL*>("glPrincipal"), SLOT(updateGL()));
    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<gyrograph*>("glSignalAcc"), SLOT(updateGL()));
    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<gyrograph*>("glSignalGyro"), SLOT(updateGL()));
    QObject::connect(_pTimer, SIGNAL(timeout()), this->findChild<gyrograph*>("glSignalMagne"), SLOT(updateGL()));
    QObject::connect(this->findChild<Combobox*>("comboBox_1"), SIGNAL(currentIndexChanged(int)), this->findChild<gyrograph*>("glSignalAcc"), SLOT(setsignalIndex(int)));
    QObject::connect(this->findChild<Combobox*>("comboBox_2"), SIGNAL(currentIndexChanged(int)), this->findChild<gyrograph*>("glSignalGyro"), SLOT(setsignalIndex(int)));
    QObject::connect(this->findChild<Combobox*>("comboBox_3"), SIGNAL(currentIndexChanged(int)), this->findChild<gyrograph*>("glSignalMagne"), SLOT(setsignalIndex(int)));
    QObject::connect (this->findChild<Combobox*>("comboBox_1"), SIGNAL(currentIndexChanged(int)),this->findChild<gyrograph*>("glSignalAcc"), SLOT(updateLabel()));
    QObject::connect (this->findChild<Combobox*>("comboBox_2"), SIGNAL(currentIndexChanged(int)),this->findChild<gyrograph*>("glSignalGyro"), SLOT(updateLabel()));
    QObject::connect (this->findChild<Combobox*>("comboBox_3"), SIGNAL(currentIndexChanged(int)),this->findChild<gyrograph*>("glSignalMagne"), SLOT(updateLabel()));



    //Affichage en décimal dans l'écran LCD

     ui->lcdNumber->setDigitCount(8);

    QObject::connect(_pTimer, SIGNAL(timeout()), this, SLOT(majLCD()));

    // Evolution slider + boutons

    connect(_pTimer, SIGNAL(timeout()), this, SLOT(setslidervalue()));

    // Clic sur pause

    connect(ui->pushButton_3, SIGNAL(clicked()), _pTimer, SLOT(stop()));

    // Clic sur play

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicPlay()));

    // Clic sur stop

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clicStop()));

    //Faire glisser le slider

    connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(dragslidervalue()));
    connect(ui->horizontalSlider, SIGNAL(sliderPressed()), _pTimer, SLOT(stop()));
    connect(ui->actionQuitter, SIGNAL(triggered(bool)),this , SLOT(close()));

}

void MainWindow::Initmainwindow(const char* filename)
{

    _pTdb = new TableauDeBord(filename);
    int max=_pTdb->getnbEch();
    _pTdb->calculeFenetreCentrale ();

    _pcGL = this->findChild<PrincipalCapteurGL*>("glPrincipal");
    _pcGL->setCentrale(&_pTdb->_IMU);

    /*   // Mise à jour de la centrale inertielle en suivant le _pTimer
       QObject::connect(_pTimer, SIGNAL(timeout()), _pTdb, SLOT(majCentrale()));
    */   // Mise à jour des widgets gl
    // Affectation de la centrale du tableau de bord aux widgets capteurs
    this->findChild<AccCapteurGL*>("glCapteurAcc")->setCentrale(&_pTdb->_IMU);
    this->findChild<GyroCapteurGL*>("glCapteurGyro")->setCentrale(&_pTdb->_IMU);
    this->findChild<MagneCapteurGL*>("glCapteurMagne")->setCentrale(&_pTdb->_IMU);



    // Affectation de la centrale et du tableau de bord aux widgets signaux
    this->findChild<gyrograph*>("glSignalAcc")->setCentrale (&_pTdb->_IMU);
    this->findChild<gyrograph*>("glSignalAcc")->setTableauDeBord (_pTdb);
    this->findChild<gyrograph*>("glSignalAcc")->setsignalIndex (0);
    this->findChild<gyrograph*>("glSignalAcc")->setmaxLabel (this->findChild<label*>("label_8"));
    this->findChild<gyrograph*>("glSignalAcc")->setminLabel (this->findChild<label*>("label_9"));


    this->findChild<gyrograph*>("glSignalGyro")->setCentrale (&_pTdb->_IMU);
    this->findChild<gyrograph*>("glSignalGyro")->setTableauDeBord (_pTdb);
    this->findChild<gyrograph*>("glSignalGyro")->setsignalIndex (0);
    this->findChild<gyrograph*>("glSignalGyro")->setmaxLabel (this->findChild<label*>("label_10"));
    this->findChild<gyrograph*>("glSignalGyro")->setminLabel (this->findChild<label*>("label_11"));


    this->findChild<gyrograph*>("glSignalMagne")->setCentrale (&_pTdb->_IMU);
    this->findChild<gyrograph*>("glSignalMagne")->setTableauDeBord (_pTdb);
    this->findChild<gyrograph*>("glSignalMagne")->setsignalIndex (0);
    this->findChild<gyrograph*>("glSignalMagne")->setmaxLabel (this->findChild<label*>("label_12"));
    this->findChild<gyrograph*>("glSignalMagne")->setminLabel (this->findChild<label*>("label_13"));



 // Mise à jour de la centrale inertielle en suivant le _pTimer
 QObject::connect(_pTimer, SIGNAL(timeout()), _pTdb, SLOT(majCentrale()));

 // Maj de la classe de mouvement
 QObject::connect(_pTimer, SIGNAL(timeout()), this, SLOT(majClasse()));

 // + fenetre d'évolution de la centrale

 _pcGL->setFenetreEvolutionCentrale(_pTdb->getCoinInferieur(),_pTdb->getCoinSuperieur());

 //initialisation de la taille du slider

  ui->horizontalSlider->setRange(0,max);



}

//destructeur
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // On renvoit la gestion du clavier à la fenetre d'affichage principal
    _pcGL->keyPressEvent(event);
}

//Affichage dans l'écran LCD

void MainWindow::majLCD()
{

    std::ostringstream s;
    s<<(double)_pTdb->getiCourant()/(double)freqEch;
    std::string sttime=s.str();
    ui->lcdNumber->display(QString(sttime.c_str()));
}

void MainWindow::majClasse()
{
   // std::cout<< "CLASSE = " << _pTdb->getClasse(this->_pTdb->getiCourant())<<std::endl;

}

//Initialisation de la valeur du slider

void MainWindow::setslidervalue()
{

    ui->horizontalSlider->setValue(this->_pTdb->getiCourant());
    ui->horizontalSlider->setValue(this->_pTdb->getiCourant());
}

// PLAY
void MainWindow::clicPlay()
{
    // On réaffecte la mémoire de date à la date courante
    _pTdb->setLastTimeToCurrentTime();
    _pTimer->start();


}
// STOP
void MainWindow::clicStop()
{
  _pTimer->stop();
  _pTdb->reInitialiseCapteursCentraleEtProgressionSignal();
  ui->lcdNumber->display(0);
  ui->horizontalSlider->setValue(0);
}

//Faire glisser le slider
void MainWindow::dragslidervalue()
{
    this->_pTdb->setiCourant(ui->horizontalSlider->value());
    _pTimer->start();
    _pTdb->setLastTimeToCurrentTime();


}
//Chargement d'un fichier

void MainWindow::loadfile()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Text files (*.out)"));
    this->Initmainwindow(filename.toStdString().c_str());

}
