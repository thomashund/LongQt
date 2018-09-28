#ifndef ADDSINGLECELLPVAR_H
#define ADDSINGLECELLPVAR_H

#include <QMap>
#include <QString>
#include <QDialog>
#include <pvarscell.h>
#include "protocol.h"

namespace Ui {
    class AddSingleCellPvar;
}

class AddSingleCellPvar : public QDialog
{
    Q_OBJECT

    public:
        explicit AddSingleCellPvar(shared_ptr<Protocol> proto, std::pair<QString, PvarsCell::IonChanParam*> parampair, QWidget *parent = 0);
        ~AddSingleCellPvar();

private:
        void updateIonChannelType();
        void setCurrentSelect(QString name, PvarsCell::IonChanParam *param);

        Ui::AddSingleCellPvar *ui;
        shared_ptr<Protocol> proto = 0;
        QMap<QString,QString> pvarsDescriptions;

    public slots:
        void changeProto(shared_ptr<Protocol> proto);
        void changeCell(shared_ptr<Cell> cell);

    signals:
        void pvarsChanged();

    private slots:
        void on_randomize_stateChanged(int state);
        void on_normalDist_toggled(bool checked);
        void on_addButton_clicked();
};

#endif // ADDSINGLECELLPVAR_H
