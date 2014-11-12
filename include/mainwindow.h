#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tableaudebord.h"
#include "principalcapteurgl.h"
#include <iostream>
#include <string>
#include "gyrograph.h"
#include"combobox.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent* event);

    ~MainWindow();

public slots :
    void majLCD();
    void setslidervalue();
    // Ajout Régis 12/11
    void timer_play();
    // Fin Régis 12/11
    void timer_stop();


private:
    Ui::MainWindow *ui;

    TableauDeBord *_pTdb;
    
    // Pointeur sur la fenetre d'affichage principale
    PrincipalCapteurGL *_pcGL;

    QTimer *_pTimer;
    // Pointeur sur Combobox
    Combobox *_pCombo;
};

#endif // MAINWINDOW_H
