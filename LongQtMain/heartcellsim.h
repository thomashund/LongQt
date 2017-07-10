/*
 * the main window for LongQt contains all the pretinant widgets and maintains
 * the side pannel to switch between them
 */
#ifndef HEART_CELL_SIM_H
#define HEART_CELL_SIM_H

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QList>
#include <QProgressBar>
#include <QFuture>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QDir>

#include "protocol.h"

class Simulation : public QWidget {
    Q_OBJECT

  public:
    Simulation(QString simvarFile = "", QWidget* parent = 0);
    ~Simulation();

  private:
    QWidget* parent;
    shared_ptr<Protocol> proto;
    QString date_time;
    QList<QWidget*> menu_list;
//utility functions & classes
    void leave_current(int current);
//buttons
    QPushButton* next_button;
    QPushButton* cancel_button;
    QPushButton* about_button;
//organizational widgets
    QListWidget* menu_options;
    QStackedWidget* menu;
//layouts
    QGridLayout* main_layout;

  private slots:
    void next_button_aciton();
    void list_click_aciton (int next_row);
    void canceled();
    void finished();
    void running();
    void changeProto(shared_ptr<Protocol> proto);
  signals:
    void cellChanged(Cell*);
    void working_dir_changed(QDir& dir);
};

#endif // HEART_CELL_SIM_H