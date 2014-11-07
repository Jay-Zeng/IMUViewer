
#include "tableaudebord.h"


TableauDeBord::TableauDeBord()
{   
    // Lecture du fichier des données centrale
    CSV donneesCentrale;
    double **donneesBrutes   = donneesCentrale.readCSV(fichierCsv);

    // Nombre de lignes du fichier
    _nbEch = donneesCentrale.getNbLines();
    // Cree un vecteur de signaux avec toutes les données
    creeVecteurSignaux(donneesBrutes,freqFiltre,freqEch,true);
    calculeFenetreCentrale();
}


TableauDeBord::~TableauDeBord()
{}

// Calcule la fenête d'évolution de la centrale en simulant son évolution
void TableauDeBord::calculeFenetreCentrale()
{
    double x,y,z=0.0;
    _coinInferieur.append(INFINITY);
    _coinInferieur.append(INFINITY);
    _coinInferieur.append(INFINITY);
    _coinSuperieur.append(-INFINITY);
    _coinSuperieur.append(-INFINITY);
    _coinSuperieur.append(-INFINITY);


    // Parcours de toutes les données
    for (int i=0;i<_nbEch-2;i++)
    {
        x+=_signaux[0]->getSignalDoubleIntegre(i);
        y+=_signaux[1]->getSignalDoubleIntegre(i);
        z+=_signaux[2]->getSignalDoubleIntegre(i);

        if (x<_coinInferieur[0])_coinInferieur[0]=x;
        if (y<_coinInferieur[1])_coinInferieur[1]=y;
        if (z<_coinInferieur[2])_coinInferieur[2]=z;

        if (x>_coinSuperieur[0])_coinSuperieur[0]=x;
        if (y>_coinSuperieur[1])_coinSuperieur[1]=y;
        if (z>_coinSuperieur[2])_coinSuperieur[2]=z;
    }
}

QVector<double> TableauDeBord::getCoinInferieur()
{
    return _coinInferieur;
}

QVector<double> TableauDeBord::getCoinSuperieur()
{
    return _coinSuperieur;
}

void TableauDeBord::creeVecteurSignaux(double** donneesBrutes,  FrequencyType uneFreqFiltre, FrequencyType uneFreqEch,bool reEchantillone)
{

    // Données de l'accéléromètre
    for (int i=2;i<=4;i++)
    {
        Signal *signalBrut = new Signal(donneesBrutes,_nbEch,0,i);
        //signalBrut->passeBas(uneFreqFiltre,uneFreqEch,reEchantillone);
        std::cout<<"passe bas ok"<<std::endl;
        signalBrut->doubleIntegre();
        std::cout<<"db integre ok"<<std::endl;
        _signaux.append(signalBrut);
    }

    // Données du gyroscope
    for (int i=6;i<=8;i++)
    {
        Signal *signalBrut = new Signal(donneesBrutes,_nbEch,0,i);
        //signalBrut->passeBas(uneFreqFiltre,uneFreqEch,reEchantillone);
        signalBrut->integre();
        _signaux.append(signalBrut);
    }
    // Données du magnéto
    for (int i=10;i<=12;i++)
    {
        Signal *signalBrut = new Signal(donneesBrutes,_nbEch,0,i);
        signalBrut->passeBas(uneFreqFiltre,uneFreqEch,reEchantillone);
        _signaux.append(signalBrut);
    }

}


// Mise à jour des données du capteur de la centrale
void TableauDeBord::majCentrale()
{
    // Acc
    _IMU._acc[0] = _signaux[0]->getSignal(iCourant);
    _IMU._accNorm[0]=_signaux[0]->normalizeVector (_signaux[0]->getSignal(iCourant));
    _IMU._acc[1] = _signaux[1]->getSignal(iCourant);
    _IMU._accNorm[1]=_signaux[1]->normalizeVector (_signaux[1]->getSignal(iCourant));
    _IMU._acc[2] = _signaux[2]->getSignal(iCourant);
    _IMU._accNorm[2]=_signaux[2]->normalizeVector (_signaux[2]->getSignal(iCourant));
    // Gyro
    _IMU._gyro[0]= _signaux[3]->getSignal(iCourant);
    _IMU._gyroNorm[0]=_signaux[3]->normalizeVector (_signaux[3]->getSignal(iCourant));
    _IMU._gyro[1]= _signaux[4]->getSignal(iCourant);
    _IMU._gyroNorm[1]=_signaux[4]->normalizeVector (_signaux[4]->getSignal(iCourant));
    _IMU._gyro[2]= _signaux[5]->getSignal(iCourant);
    _IMU._gyroNorm[2]=_signaux[5]->normalizeVector (_signaux[5]->getSignal(iCourant));
    // Magnétomètre
    _IMU._magn[0]=_signaux[6]->getSignal(iCourant);
    _IMU._magnNorm[0]=_signaux[6]->normalizeVector (_signaux[6]->getSignal(iCourant));
    _IMU._magn[1]=_signaux[7]->getSignal(iCourant);
    _IMU._magnNorm[1]=_signaux[7]->normalizeVector (_signaux[7]->getSignal(iCourant));
    _IMU._magn[2]=_signaux[8]->getSignal(iCourant);
    _IMU._magnNorm[0]=_signaux[8]->normalizeVector (_signaux[8]->getSignal(iCourant));

    //Position depuis l'accéléro
    _IMU._acc2I[0] = _signaux[0]->getSignalDoubleIntegre(iCourant);
    _IMU._acc2I[1] = _signaux[1]->getSignalDoubleIntegre(iCourant);
    _IMU._acc2I[2] = _signaux[2]->getSignalDoubleIntegre(iCourant);

    // Angle depuis le gyro
    _IMU._gyroI[0]= _signaux[3]->getSignalIntegre(iCourant);
    _IMU._gyroI[1]= _signaux[4]->getSignalIntegre(iCourant);
    _IMU._gyroI[2]= _signaux[5]->getSignalIntegre(iCourant);

    // Orientation depuis le gyro
    if (_IMU._orientation[0]>=360)
        _IMU._orientation[0]+= _IMU._gyroI[0] - 360;
    else
        _IMU._orientation[0]+= _IMU._gyroI[0];

    if (_IMU._orientation[1]>=360)
        _IMU._orientation[1]+= _IMU._gyroI[1] - 360;
    else
        _IMU._orientation[1]+= _IMU._gyroI[1];

    if (_IMU._orientation[2]>=360)
        _IMU._orientation[2]+= _IMU._gyroI[2] - 360;
    else
        _IMU._orientation[2]+= _IMU._gyroI[2];

    // Postion depuis l'acceleromètre
    _IMU._position[0]+= _IMU._acc2I[0];
    _IMU._position[1]+= _IMU._acc2I[1];
    _IMU._position[2]+= _IMU._acc2I[2];

    // Incrémentation de la distance totale parcourue
    _IMU._distance += sqrt(pow(_IMU._acc2I[0],2)+pow(_IMU._acc2I[1],2)+pow(_IMU._acc2I[2],2));

    // On ajoute le point courant de la centrale à la trajectoire
    _IMU._trajectoire.append(_IMU._position);

    // Incrémentation de l'indice de parcours des données
    iCourant = (iCourant < (_signaux.at(0)->getTaille())) ? iCourant+1:0;

    if (iCourant==0)
    {
        // Orientation depuis le gyro
        _IMU._orientation[0]=0;
        _IMU._orientation[1]=0;
        _IMU._orientation[2]=0;

        // Postion depuis l'acceleromètre
        _IMU._position[0]=0;
        _IMU._position[1]=0;
        _IMU._position[2]=0;

        // On vide la trajectoire
        _IMU._trajectoire.clear();
        _IMU._distance;

    }

}
QVector<Signal*> TableauDeBord::get_signaux()
{
    return this->_signaux;
}

int TableauDeBord::getiCourant()
{
    return this->iCourant;
}

