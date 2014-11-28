#include "signal.h"

// Constructeur
Signal::Signal(SampleType** uneMatrice,int taille,int colTemps,int colSignal,int tailleFenetreStats)
{
    _taille       = taille;
    _signal       = new SampleType[taille];
    _vecteurTemps = new SampleType[taille];
    _tailleFenetreStats = tailleFenetreStats;

    // Récupération des valeurs du signal temps et du signal tout court
    for (int i=0;i<taille;i++)
    {
        _vecteurTemps[i] = uneMatrice[colTemps][i];
        _signal[i]       = uneMatrice[colSignal][i];
    }

}

// Constructeur par copie
Signal::Signal(const Signal& unSignal)
{
    _taille       = unSignal._taille;
    _signal       = new SampleType[_taille];
    _vecteurTemps = new SampleType[_taille];
    _tailleFenetreStats = unSignal._tailleFenetreStats;


    // Recopie du temps et du signal
    for (int i=0;i<_taille;i++)
    {
        _vecteurTemps[i] = unSignal._vecteurTemps[i];
        _signal[i]       = unSignal._signal[i];
    }
    // Drapeaux d'intégration
    estIntegre      = unSignal.estIntegre;
    estDoubleIntegre= unSignal.estDoubleIntegre;
    // Recopie des signaux intégrés si le signal l'a été
    if (unSignal.estIntegre)
    {
        _signalIntegre = new SampleType[_taille];
        for (int i=0;i<_taille;i++)
        {
            _signalIntegre[i] = unSignal._signalIntegre[i];
        }
    }
    if (unSignal.estDoubleIntegre)
    {
        _signalDoubleIntegre = new SampleType[_taille];
        for (int i=0;i<_taille;i++)
        {
            _signalDoubleIntegre[i] = unSignal._signalDoubleIntegre[i];
        }
    }
    // Drapeau de statistiques & recopie éventuelle
    _estStatistique = unSignal._estStatistique;
    if (_estStatistique)
    {
        int delta       = (_tailleFenetreStats-1)/2;
        _signalMoyenne  = new SampleType[_taille-_tailleFenetreStats+1];
        _signalEcartType= new SampleType[_taille-_tailleFenetreStats+1];
        _signalAmplitude= new SampleType[_taille-_tailleFenetreStats+1];
        _signalMin      = new SampleType[_taille-_tailleFenetreStats+1];
        // Parcours du signal
        for (int i=0;i<_taille-2*delta;i++)
        {
            _signalMoyenne[i]   = unSignal._signalMoyenne[i];
            _signalEcartType[i] = unSignal._signalEcartType[i];
            _signalAmplitude[i] = unSignal._signalAmplitude[i];
            _signalMin[i]       = unSignal._signalMin[i];
        }
    }
}

Signal* Signal::operator+(Signal lautre)
{
    Signal *res = new Signal (*this);

    if (this->_taille == lautre._taille)
    {
        for (int i=0;i<_taille;i++)
        {
            res->_signal[i] += lautre._signal[i];
        }
    }
    // Si le signal est d'ores et déjà intégré, on réintègre apres soustraction
    if (this->estIntegre)
        res->integre();
    if (this->estDoubleIntegre)
        res->doubleIntegre();
    if (this->_estStatistique)
        res->calculStats();
    return res;
}


Signal* Signal::operator-(Signal lautre)
{
    Signal *res = new Signal (*this);
    if ((this->_taille == lautre._taille))
    {

        for (int i=0;i<_taille;i++)
        {
            res->_signal[i] -= lautre._signal[i];
        }
        // Si le signal est d'ores et déjà intégré, on réintègre apres soustraction
        if (this->estIntegre)
            res->integre();
        if (this->estDoubleIntegre)
            res->doubleIntegre();
        if (this->_estStatistique)
            res->calculStats();
    }
    return res;
}

Signal* Signal::operator*(Signal lautre)
{
    Signal *res = new Signal (*this);

    if (this->_taille == lautre._taille)
    {
        for (int i=0;i<_taille;i++)
        {
            res->_signal[i] *= lautre._signal[i];
        }
    }
    // Si le signal est d'ores et déjà intégré, on réintègre apres soustraction
    if (this->estIntegre)
        res->integre();
    if (this->estDoubleIntegre)
        res->doubleIntegre();
    if (this->_estStatistique)
        res->calculStats();
    return res;
}

SampleType Signal::getTemps(int i)
{

    return this->_vecteurTemps[i];
}

double  Signal::getSignal(int i)
{
    return this->_signal[i];


}

Signal::~Signal()
{
    delete _signal;
    delete _vecteurTemps;
    if (estIntegre)delete _signalIntegre;
    if (estDoubleIntegre)delete _signalDoubleIntegre;
    if (_estStatistique)
    {
        delete _signalMoyenne;
        delete _signalEcartType;
        delete _signalAmplitude;
        delete _signalMin;
    }
}


void Signal::passeBas(FrequencyType freqFiltre,FrequencyType freqEch,bool reEchantillone=false)
{

   if (reEchantillone)this->regulariseEchantillonage(freqEch);

    /* Pour le calcul de la transformée de Fourier, il est nécessaire que la taille du signal
       soit un multiple de 2
    */
    // Fabrication du signal temporaire avec taille en puiss de 2
    int taillePuiss2;
    SampleType * signalPuiss2=produitSignalMultiple2(&taillePuiss2);

    // Recuperation de la meilleure transformée adaptée à la taille
    auto fft = FftFactory::getFft(taillePuiss2);

    // Transformée de Fourier du signal
    SpectrumType spectre = fft->fft(signalPuiss2);

    // Génération du spectre filtre
    SpectrumType spectreFiltre(taillePuiss2);

    for (int i = 0; i < taillePuiss2; i++)
    {
        if (i < (taillePuiss2 * freqFiltre / freqEch))
        {
            // passband
            spectreFiltre[i] = 1.0;
        }
        else
        {
            // stopband
            spectreFiltre[i] = 0.0;
        }
    }

    // Multiplication des deux signaux
    std::transform(std::begin(spectre),std::end(spectre),std::begin(spectreFiltre),std::begin(spectre),[] (Aquila::ComplexType x, Aquila::ComplexType y) { return x * y; });

    // Fourier inverse ; retour dans l'espace temporel
    SampleType *resIfft = new SampleType[taillePuiss2];
    fft->ifft(spectre, resIfft);

    // Recopie des valeurs qui nous intéressent
    for (int i=0;i<_taille;i++)
        _signal[i] = resIfft[i];

}

/* Produit un nouveau signal (nouvSig) de taille puissance de 2 immédiatement supérieur à la taille du signal (nouvTaille) courant
    Les valeurs supplémentaires sont à zéro

*/
SampleType* Signal::produitSignalMultiple2(int* nouvTaille)
{
    // Identification de la taille t
    int t;
    for (t=2;t<_taille;t*=2);

    SampleType* nouvSig= new SampleType[t];

    for (int i=0;i<_taille;i++)// Recopie des valeurs du signal
        nouvSig[i] = _signal[i];
    for (int i=_taille;i<t;i++)// Le reste à zero
        nouvSig[i] = 0;

    *nouvTaille = t;

    return nouvSig;

}

// Régularise le pas d'échantillonage du signal
void Signal::regulariseEchantillonage(FrequencyType fEch)
{
    if ((_taille > 1) && (fEch != 0))
    {

        // Periode d'échantillonage en s
        SampleType tEch = 1/fEch;
        // Taille du tableau résultat
        int tailleTabRes = (int)((_vecteurTemps[_taille-1] - _vecteurTemps[0] )*fEch );

        SampleType *resTemps  = new SampleType[tailleTabRes];
        SampleType *resSignal = new SampleType[tailleTabRes];

        // Indice de parcours du signal origine
        int iSrc = 1;
        // Premieres valeurs du tableau résultat
        resSignal[0] = _signal[0];
        resTemps[0]  = _vecteurTemps[0];

        // Temps correspondant à l'indice courant dans le signal résultat
        SampleType tRes = _vecteurTemps[0]+tEch;

        // Parcours du signal résultat
        for (int iRes=1;iRes<tailleTabRes;iRes++)
        {
            // Il faut incrémenter iRes tant qu'on n'a pas atteint l'instant voulu (çad tRes)
            while ((_vecteurTemps[iSrc]<tRes) && (iSrc < _taille-1))
                iSrc++;
            // Temps et valeurs à gauche et droite de la valeur courante à remplir
            SampleType tGauche = _vecteurTemps[iSrc-1],tDroite = _vecteurTemps[iSrc];
            SampleType vGauche = _signal[iSrc-1],vDroite   = _signal[iSrc];

            // Interpolation des deux valeurs pour l'entree courante
            SampleType denomi   = tDroite-tGauche;
            resSignal[iRes] = (1 - ((tRes-tGauche)/denomi))*vGauche + (1 - ((tDroite-tRes)/denomi))*vDroite;
            resTemps[iRes]  = tRes;


            // Temps suivant...
            tRes    += tEch;
        }

        delete _signal;
        delete _vecteurTemps;

        _taille       = tailleTabRes;
        _signal       = resSignal;
        _vecteurTemps = resTemps;
    }
}

void Signal::changeRepere(Signal *srcX,Signal *srcY,Signal *srcZ, Signal orientationX,Signal orientationY,Signal orientationZ)
{
    int t = srcX->getTaille();
    // On vérifie que les six signaux ont la même taille
    if ((srcY->getTaille() == t) && (srcZ->getTaille() == t))
    {

        if ((orientationX.getTaille() == t) && (orientationY.getTaille() == t) && (orientationZ.getTaille() == t))
        {
            for (int i=0;i<t;i++)// Parcours des signaux source
            {
                double alpha = orientationX._signal[i];
                double teta  = orientationY._signal[i];
                double gamma = orientationZ._signal[i];

                // Rotations
                srcX->_signal[i] = cos(alpha)*cos(gamma)*srcX->_signal[i]-sin(gamma)*cos(alpha)*srcY->_signal[i]
                        +sin(alpha)*srcZ->_signal[i];

                srcY->_signal[i] = (sin(teta)*sin(alpha)*cos(gamma)+cos(teta)*sin(gamma))*srcX->_signal[i]
                        -(sin(gamma)*sin(teta)*sin(alpha)+cos(gamma)*cos(teta))*srcY->_signal[i]
                        -(sin(teta)*cos(alpha)) *srcZ->_signal[i];

                srcZ->_signal[i] = (cos(teta) * sin(alpha) * cos(gamma) - sin(teta)*sin(gamma))*srcX->_signal[i]
                        +(sin(gamma)*cos(teta)*sin(alpha)-cos(gamma)*sin(teta))*srcY->_signal[i]
                        +cos(teta)*cos(alpha)*srcZ->_signal[i];
            }
        }
    }
}


int Signal::getTaille()
{
    return _taille;

}


SampleType* Signal::integreUnSignal(SampleType* unTemps,SampleType *unSignal,int uneTaille)
{
    SampleType* res = new SampleType[uneTaille];
    res[0] = 0;
    for (int i=1;i<uneTaille;i++)
    {
        double deltaX = unTemps[i]-unTemps[i-1];
        double y1     = unSignal[i-1];
        double y2     = unSignal[i];
        double yMinAbs = (abs(y1)<abs(y2)) ? abs(y1):abs(y2);
        double deltaY = abs(y2-y1);
        double deltaIntegre = 0;
        // Cas 1 : aire triangle + aire rectangle
        if ((y1>0) && (y2>0))
        {
            deltaIntegre = deltaY*deltaX/2.0+deltaX*yMinAbs;
        }
        // Cas 2 : idem en négatif
        else if ((y1<0) && (y2<0))
        {
            deltaIntegre = -(deltaY*deltaX/2.0+deltaX*yMinAbs);
        }
        // Cas 3 : un y positif un autre négatif
        else
        {
            // Thales
            double a = (y2!=0) ? deltaX*abs(y1/y2) : deltaX;
            double b = (y1!=0) ? deltaX*abs(y2/y1) : deltaX;
            deltaIntegre = a*y1/2.0+b*y2/2.0;
        }

        res[i] = res[i-1] + deltaIntegre;

    }
    return res;
}


void Signal::integre()
{
    estIntegre = true;
    this->_signalIntegre= Signal::integreUnSignal(this->_vecteurTemps,this->_signal,this->getTaille());
}


void Signal::doubleIntegre()
{
    if (estIntegre == false)integre();
    estDoubleIntegre = true;
    this->_signalDoubleIntegre= Signal::integreUnSignal(this->_vecteurTemps,this->_signalIntegre,this->getTaille());
}


SampleType  Signal::getSignalIntegre(int i)
{
    return this->_signalIntegre[i];
}

SampleType  Signal::getSignalDoubleIntegre(int i)
{
    return this->_signalDoubleIntegre[i];
}



SampleType Signal::getMaxSignal ()
{
    SampleType max=0;

    for(int i=0;i<this->getTaille ();i++)
    {
        if (max<fabs(this->getSignal (i)))
            continue;
        else
            max=fabs(this->getSignal (i));
    }

    return max;
}

SampleType Signal::normalizeVector(SampleType val )
{
    SampleType result;

    result=val*9/this->getMaxSignal();

    return result;
}

void Signal::calculStats()
{
    _estStatistique = true;
    // Il faut que la taille du signal soit assez grande par rapport à la taille de la fenetre
    if (_taille > (_tailleFenetreStats+1))
    {
        _signalMoyenne  = new SampleType[_taille-_tailleFenetreStats+1];
        _signalEcartType= new SampleType[_taille-_tailleFenetreStats+1];
        _signalAmplitude= new SampleType[_taille-_tailleFenetreStats+1];
        _signalMin      = new SampleType[_taille-_tailleFenetreStats+1];

        int delta       = (_tailleFenetreStats-1)/2;

        // Parcours du signal
        for (int i=delta;i<_taille-delta;i++)
        {
            _signalMoyenne[i-delta]   = moyenne(i-delta,i+delta);
            _signalEcartType[i-delta] = ecartType(i-delta,i+delta,_signalMoyenne[i-delta]);
            _signalAmplitude[i-delta] = amplitude(i-delta,i+delta);
            _signalMin[i-delta]       = min(i-delta,i+delta);
        }
    }
}
// Calcul la moyenne du signal brut sur une fenetre donnée pour la classif
SampleType Signal::moyenne(int iDeb,int iFin)
{
    SampleType somme = 0;
    for (int i=iDeb;i<=iFin;i++)
        somme += _signal[i];
    return somme/_tailleFenetreStats;
}

SampleType Signal::ecartType(int iDeb,int iFin,SampleType moyenne)
{
    SampleType somme = 0;
    for (int i=iDeb;i<=iFin;i++)
    {
        somme += pow(_signal[i]-moyenne,2.0);
    }
    // somme/ taille => variance
    return sqrt(somme/_tailleFenetreStats);
}

SampleType Signal::amplitude(int iDeb,int iFin)
{
    SampleType min = INFINITY;
    SampleType max = -INFINITY;
    for (int i=iDeb;i<=iFin;i++)
    {
        if (_signal[i]<min)min=_signal[i];
        if (_signal[i]>max)max=_signal[i];
    }
    return max - min;
}

SampleType Signal::min(int iDeb,int iFin)
{
    SampleType min = INFINITY;
    for (int i=iDeb;i<=iFin;i++)
    {
        if (_signal[i]<min)min=_signal[i];
    }
    return min;
}

SampleType Signal::getEcartType(int i)
{
   int iStat = i-(_tailleFenetreStats-1)/2;
   if ((_estStatistique) && (iStat>0) && (iStat<_taille-_tailleFenetreStats+1))
        return _signalEcartType[iStat];
}

SampleType Signal::getMoyenne(int i)
{
    int iStat = i-(_tailleFenetreStats-1)/2;
    if ((_estStatistique) && (iStat>0) && (iStat<_taille-_tailleFenetreStats+1))
        return _signalMoyenne[iStat];
}

SampleType Signal::getMin(int i)
{
    int iStat = i-(_tailleFenetreStats-1)/2;
    if ((_estStatistique) && (iStat>0) && (iStat<_taille-_tailleFenetreStats+1))
        return _signalMin[iStat];
}

void Signal::setSignal(SampleType val,int i)
{
    if ((_estStatistique) && (i<_taille))_signal[i] = val;
}


