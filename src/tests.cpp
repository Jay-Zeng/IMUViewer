#include "tests.h"

/////////////Tests à écrire dans cette classe (méthodes statiques)


Tests::Tests()
{
}


// Methode à finir
bool Tests::testQuaternion()
{
    bool resB = true;
    Quaternion q(1,1,1,1);

    Quaternion resq(1,1,1,1);


    float *rotations;

    rotations=new float[3];

    rotations[0]=10; // pitch
    rotations[1]=20; // yaw
    rotations[2]=15; // roll

    // le Quaternion qu'on obtient à partir des angles d'eulers précédemment définisss
    resq=q.FromEuler(rotations[0],rotations[1],rotations[2]);

    //////////////////////::/TESTS


    srand (time(NULL));

    int val1 = rand()%1000-500;
    int val2 = rand()%1000-500;
    int val3 = rand()%1000-500;
    int val4 = rand()%1000-500;
    int val5 = rand()%1000-500;
    int val6 = rand()%1000-500;
    int val7 = rand()%1000-500;
    int val8 = rand()%1000-500;


    cout << "Jean Michel val 1 = " << val1 <<"val 2 = " << val2 <<"val 3 = " << val3 <<"val 4 = " << val4 << endl;
    cout << "Tarik val 5 = " << val5 <<"val 6 = " << val6 <<"val 7 = " << val7 <<"val 8 = " << val8 << endl;


    Quaternion JeanMichel(val1,val2,val3,val4);
    Quaternion Tarik(val5,val6,val7,val8);

    //////////////////////::/GET

    float *res = JeanMichel.getQ();

    if ((res[0] == val1) && (res[1] == val2) && (res[2] == val3) && (res[3] == val4))
            cout << " GET OK " << endl;
    else
    {
        cout << " GET PB " << endl;
        resB = false;
    }

    //delete res;

    //////////////////////::/ADDITION
    Quaternion Khaled = JeanMichel.add(JeanMichel,Tarik);

    float *res2 = Khaled.getQ();

    cout << "res Khaled = " << res2[0]  << " " << res2[1] << " " << res2[2] << " " << res2[3] << endl;

    if ((res2[0] == (val1+val5)) && (res2[1] == (val2+val6)) && (res2[2] == (val3+val7)) && (res2[3] == (val4+val8)))
        cout << " ADDITION OK " << endl;
    else
    {
        cout << "ADDITION PB" << endl;
        resB = false;
    }
    //delete res2;

    //////////////////////::/MULTIPLICATION
    Quaternion Marcel = JeanMichel.multipQuaternion(JeanMichel,Tarik);

    float *res3 = Marcel.getQ();

    cout << "res Khaled = " << res3[0]  << " " << res3[1] << " " << res3[2] << " " << res3[3] << endl;

    if ((res3[0] == (val1*val5-val2*val6-val3*val7-val4*val8)) && (res3[1] == (val1*val6+val2*val5+val3*val8-val4*val7)) && (res3[2] == (val1*val7+val3*val5-val2*val8+val4*val6)) && (res3[3] == (val4*val5+val1*val8+val2*val7-val3*val6)))
        cout << " MULTIP OK " << endl;
    else
    {
        cout << "MULTIP PB" << endl;
        resB = false;
    }

    //////////////////////::/INVERSE
    Quaternion Qinv = JeanMichel.inverseQuaternion ();

    float *res51 = Qinv.getQ();

    cout << " res Qinv= "<< res51[0] << " " << res51[1] << " " << res51[2] << " " << res51[3]<<endl;


    float tmp= val1/(val1*val1+val2*val2+val3*val3+val4*val4);

    cout << tmp <<endl;

    if ((res51[0] == (val1/(val1*val1+val2*val2+val3*val3+val4*val4))) && (res51[1] == (-val2/(val1*val1+val2*val2+val3*val3+val4*val4))) && (res51[2] == (-val3/(val1*val1+val2*val2+val3*val3+val4*val4))) && (res51[3] == (-val4/(val1*val1+val2*val2+val3*val3+val4*val4))))
        cout << " INV OK " << endl;
    else
    {
        cout << " INV PB " << endl;
        resB = false;
    }

    //////////////////////::/CONJUGATE
    Quaternion conjugate=JeanMichel.conjugateQuaternion ();

    float *res10 = conjugate.getQ ();

    if ((res10[0] == val1) && (res10[1] == (-val2)) && (res10[2] == (-val3)) && (res10[3] == (-val4)))
        cout << " CONJ OK " << endl;
    else
    {
        cout << " CONJ PB " << endl;
        resB = false;
    }

    //////////////////////::/SET
    JeanMichel.setQ(Tarik.getQ());
    float *res4 = JeanMichel.getQ();

    if((res4[0] == val5) && (res4[1] == val6) && (res4[2] == val7)  && (res4[3] == val8))
            cout << " SET OK "<< endl;
    else
    {
          cout << " SET PB " << endl;
          resB = false;
    }
    return resB;
}

void Tests::TdSReechantilonnage()
{
    CSV donneesCentrale;
    double **donnees   = donneesCentrale.readCSV("amjad_marche_cheville.out");
    int nbEch = donneesCentrale.getNbLines();

    std::cout << "Données avant echantilonge"<< std::endl;

    for (int i=0;i<nbEch;i++)
        printf("%f;%f\n",donnees[0][i],donnees[4][i]);;//std::cout<<donnees[0][i]<< " "<<donnees[4][i]<<std::endl;

    // Creation de l'objet signal pour la composante x de l'accélérometre
    Signal tdS(donnees,nbEch,0,4);

    //tdS.passeBas(FrequencyType freqFiltre,FrequencyType freqEch);

    std::cout << "***************"<< std::endl;
    // Test rééchantillonage 50Hz
    tdS.regulariseEchantillonage(50.0);

    const SampleType *signalReechantillone = tdS.getSignal();
    const SampleType *temps = tdS.getTemps();
    int taille = tdS.getTaille();

    std::cout << "TAILLE AVANT = "<< nbEch << std::endl;
    std::cout << "TAILLE APRES = "<< taille << std::endl;

    std::cout << "Données apres echantilonge"<< std::endl;

    for (int i=0;i<taille;i++)
        printf("%lf;%lf\n",temps[i],signalReechantillone[i]);

}

void Tests::TdSPasseBas()
{
    CSV donneesCentrale;
    double **donnees   = donneesCentrale.readCSV("amjad_marche_cheville.out");
    Signal tdS(donnees,donneesCentrale.getNbLines(),0,4);
    // filtrage passe bas à 5 Hz apres rééchantillonnage du signal
    tdS.passeBas(5,50,true);
}

bool Tests::tout()
{
    bool res = Tests::testQuaternion();
    Tests::TdSReechantilonnage();
    Tests::TdSPasseBas();
    // etc...
    return res;
}


// Gather all tds data in a matrix

double** Tests::TDS_matix(int n,double** sens,  FrequencyType freqFiltre, FrequencyType freqEch,bool reEchantillone)
{


    double** TDSmatix=new double*[4];

    Signal saccx(sens,n,0,2);
    saccx.passeBas(freqFiltre,freqEch,reEchantillone);

    Signal saccy(sens,n,0,3);
    saccy.passeBas(freqFiltre,freqEch,reEchantillone);

    Signal saccz(sens,n,0,4);
    saccz.passeBas(freqFiltre,freqEch,reEchantillone);

    SampleType *tmps = new SampleType[saccx.getTaille()];
    tmps=saccx.getTemps();
    for (int col=0;col<4;col++)
        TDSmatix[col] = new double[saccx.getTaille()];

    for (int i=0;i<saccz.getTaille();i++)
    {
       TDSmatix[0][i]= tmps[i];

       TDSmatix[1][i]= saccx.getSignal(i);

       TDSmatix[2][i]= 0;

       TDSmatix[3][i]= saccz.getSignal(i);

    }

    return TDSmatix;

}
bool Tests::testItegration()
{
/*

    double **sensors=new double*[13];
    double **tdsmatrix=new double*[4];
    CSV fchr;
    sensors=fchr.readCSV("amjad_controle.csv");
    int n=fchr.getNbLines();
    float* X=new float [n-2];
    float* Y=new float [n-2];
    float* Z=new float [n-2];
    float* angX=new float [n-1];
    float* angY=new float [n-1];
    float* angZ=new float [n-1];
    Integration I(sensors,n);
    integration_gyro IG(sensors,n);
    X=I.Integration_accelo_X(n);
    Y=I.Integration_accelo_Y(n);
    Z=I.Integration_accelo_Z(n);
    angX=IG.Integration_gyro_X(n);
    angY=IG.Integration_gyro_Y(n);
    angZ=IG.Integration_gyro_Z(n);
    tdsmatrix=Tests::TDS_matix(n,sensors,3,3,true);
*/
    return false;
}
