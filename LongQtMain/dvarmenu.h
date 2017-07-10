/*
 * Dvars are the output or tracked variables from a sim. They can be found in
 * cell->vars
 */
#ifndef DVARMENU_H
#define DVARMENU_H

#include <QWidget>
#include <QCheckBox>

#include "cell.h"

using namespace std;

class dvarMenu :public QWidget {
Q_OBJECT
  public:
    dvarMenu(Cell* cell, QWidget* parent = 0);
    void createMenu();
    ~dvarMenu();

  private:
	Cell* cell;

//Buttons & their labels
    QCheckBox** dvars;
//screen functions
    void update_menu(); //make menu match pars

  private slots:
    void update_datamap(string p, int state); //make a Protocol::pars entry match the screen
  public slots:
    void changeCell(Cell* cell);
    void reset();
};

#endif