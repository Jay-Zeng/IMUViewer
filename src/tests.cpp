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

    float *res5 = Qinv.getQ();

    cout << " res Qinv= "<< res5[0] << " " << res5[1] << " " << res5[2] << " " << res5[3]<<endl;


    float tmp= val1/(val1*val1+val2*val2+val3*val3+val4*val4);

    cout << tmp <<endl;

    if ((res5[0] == (val1/(val1*val1+val2*val2+val3*val3+val4*val4))) && (res5[1] == (-val2/(val1*val1+val2*val2+val3*val3+val4*val4))) && (res5[2] == (-val3/(val1*val1+val2*val2+val3*val3+val4*val4))) && (res5[3] == (-val4/(val1*val1+val2*val2+val3*val3+val4*val4))))
        cout << " INV OK " << endl;
    else
    {
        cout << " INV PB " << endl;
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

bool Tests::TdS()
{

    CSV donneesCentrale;
    float **donnees   = donneesCentrale.readCSV("amjad_marche_cheville.out");
    int nbEch = donneesCentrale.getNbLines();


    for (int i=0;i<nbEch;i++)
        std::cout<<i<< " "<<donnees[4][i]<<std::endl;

    Signal tdS ((SampleType *)donnees[0],(SampleType *)donnees[4],nbEch);


    //tdS.passeBas(FrequencyType freqFiltre,FrequencyType freqEch);

    // Test rééchantillonage
    //tdS.regulariseEchantillonage(   );

}

bool Tests::tout()
{
    bool res = Tests::testQuaternion();
    res &= Tests::TdS();
    // etc...
    return res;
}
