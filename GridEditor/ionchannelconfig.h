/*
 * A widget for setting ion channels in a grid (or single cell)
 * ion channels are the *Factor in cell->pars
 */
#ifndef IONCHANNELCONFIG_H
#define IONCHANNELCONFIG_H

#include <QWidget>
#include <QTableView>

#include <gridProtocol.h>
#include <gridModel.h>

namespace Ui {
class IonChannelConfig;
}

class IonChannelConfig : public QWidget
{
    Q_OBJECT

public:
    explicit IonChannelConfig(QTableView* view, shared_ptr<GridProtocol> proto, QWidget *parent = 0);
    ~IonChannelConfig();
	void updateList();

private:
    Ui::IonChannelConfig *ui;
	GridModel* model = 0;
	shared_ptr<GridProtocol> proto = 0;
	QTableView* view = 0;

	set<pair<int,int>> getInitial();

public slots:
	void cellChanged(Cell*);

private slots:
	void on_randomize_stateChanged(int state);
	void on_multiple_stateChanged(int state);
	void on_normalDist_toggled(bool checked);
	void on_addButton_clicked();
    void on_actionDelete_triggered();
    void on_actionShow_Cells_triggered();
};

#endif // IONCHANNELCONFIG_H