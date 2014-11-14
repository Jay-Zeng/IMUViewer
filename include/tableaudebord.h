#ifndef TABLEAUDEBORD_H
#define TABLEAUDEBORD_H

// Mode test
//#define TESTS_MODE

// Mode run
#define RUN_MODE



// Fréquence du filtre passe-bas permettant de ressortir le signal gravité en Hz
#define freqFiltreGravite 0.18
// Fréquence de ré-échantillonage du signal en Hz
#define freqEch 100

//#define fichierCsv "amjad_marche_cheville.out"

//#define fichierCsv "ferdaousse_mixte_cheville.out"


#include <QThread>
#include <QTimer>
#include "signal.h"
#include "csv.h"
#include <QVector>
#include "centrale.h"
#include <QTime>


class TableauDeBord : public QObject
{
    Q_OBJECT
    public:

        TableauDeBord(const char* fichiercsv);
        ~TableauDeBord();

        Centrale _IMU;

        QVector<double> getCoinInferieur();
        QVector<double> getCoinSuperieur();
        QVector<Signal*> get_signaux();
        int getiCourant();
        int getnbEch();
        // Affecte current time au last time
        void setLastTimeToCurrentTime();
        void setiCourant(int i);
        // Réinitilisation de tous les capteurs
        void reInitialiseCapteursCentraleEtProgressionSignal();
        //mise en place de la centrale
        void miseenplace(int i);

    public slots:

        void majCentrale();

   protected :


    private:

        //const char* _fichiercsv;
        // 0-1-2 : accelero
        // 3-4-5 : gyro
        // 6 7 8 : magnéto
        QVector<Signal*> _signaux;

        // Coordonnées du point inférieur et supérieur de la fenetre d'évolution de la centrale
        QVector<double> _coinInferieur;
        QVector<double> _coinSuperieur;

        // Nb de lignes dans le fichier de départ
        int _nbEch;
        // Représente l'indice courant dans le tableau de données
        int iCourant;


        // Enregistre le QTime de la dernière itération pour identifier un éventuel décalage entre le temps
        // reel et le temps du signal
        QTime _lastTime;

        int incrementeICourant();

        void creeVecteurSignaux(double**,  FrequencyType, FrequencyType);

        void calculeFenetreCentrale();


};

#endif // TABLEAUDEBORD_H
