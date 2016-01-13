#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QCheckBox>
#include <QDate>
#include <QCloseEvent>
#include <limits>
#include <QHBoxLayout>
#include <QListWidget>

#include "varmenu.h"
#include "proto.h"


/*########################
  begin simvarMenu class
##########################*/
simvarMenu::simvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> pars = proto->pars;
    map<string,double*>::iterator it;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
    QHBoxLayout* button_group;
//initialize buttons &lables
    simvars = (QDoubleSpinBox**)malloc(pars.size()*sizeof(QDoubleSpinBox*));
    simvar_names = (QLabel**)malloc(pars.size()*sizeof(QLabel*));
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    close_button = new QPushButton("Save and Exit", this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
//do all the work for simvars setup
    for(it = pars.begin(), i = 0; it!=pars.end(); it++,i++) {
        simvars[i] = new QDoubleSpinBox(this);
        simvar_names[i] = new QLabel(*(new QString((it->first).c_str())),this);
        string name = it->first;
        simvars[i]->setMaximum(std::numeric_limits<double>::max());
        button_group = new QHBoxLayout;
        button_group->addWidget(simvar_names[i]);
        button_group->addWidget(simvars[i]);
        central_layout->addLayout(button_group, (2*i)/row_len+1, (2*i)%row_len, 1, 2);
        connect(simvars[i], static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] (double value) {update_pvars(pair<string,double>(name, value));});
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, i/row_len, 2*row_len); 
    main_layout->addWidget(close_button, (2*i)*row_len, 2*row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Simulation Variables Menu"));
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_simvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
    
}

simvarMenu::~simvarMenu(){}

void simvarMenu::update_menu() {
    map<string,double*>::iterator it;
    unsigned int i; 
    for(it = proto->pars.begin(), i = 0; it != proto->pars.end(); it++, i++)     {  
        simvars[i]->setValue(*(it->second));
    }
}

void simvarMenu::closeEvent(QCloseEvent* event){
    if(write_close) {
        !(bool)proto->writepars(proto->pars, string("simvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
    }
    event->accept();
}

bool simvarMenu::read_simvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->simvarfile = fileName.toStdString();
    ret = !(bool)proto->readpars(proto->pars, proto->simvarfile);
    }
    update_menu();
    return ret;
}

bool simvarMenu::write_simvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->simvarfile = fileName.toStdString();
    ret = !(bool)proto->writepars(proto->pars, proto->simvarfile);
    }
    return ret;

}

void simvarMenu::update_pvars(pair<string,double> p){
    *(proto->pars[p.first]) = p.second;
}

void simvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}

/*#################################
    begin dvarMenu class
###################################*/
dvarMenu::dvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> vars = proto->cell->vars;
    map<string,double*>::iterator it;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    dvars = (QCheckBox**)malloc(vars.size()*sizeof(QCheckBox*));
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    close_button = new QPushButton(tr("Save and Exit"), this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
//do all the work for dvars setup
    for(it = vars.begin(), i = 0; it!=vars.end(); it++,i++) {
        pair<string,double*> p (it->first, it->second);
        dvars[i] = new QCheckBox(*(new QString((it->first).c_str())), this);
        central_layout->addWidget(dvars[i], i/row_len, i%row_len);
        connect(dvars[i], &QCheckBox::stateChanged, [=] (int state) {update_datamap(p, state);});
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, i/row_len, row_len); 
    main_layout->addWidget(close_button, (2*i)*row_len, 2*row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Output Variables Menu"));
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_dvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
    
}

dvarMenu::~dvarMenu(){}

void dvarMenu::update_menu() {
    map<string,double*>::iterator it;
    unsigned int i; 
    for(it = proto->cell->vars.begin(), i = 0; it != proto->cell->vars.end(); it++, i++)     {  
        if(proto->datamap.count(it->first) > 0) {
            dvars[i]->setChecked(true);
        } else {
            dvars[i]->setChecked(false);
        }
    }
}

void dvarMenu::closeEvent(QCloseEvent* event){
    if(write_close) {
        !(bool)proto->writedvars(proto->datamap, string("dvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
    }
    event->accept();
}

bool dvarMenu::read_dvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->dvarfile = fileName.toStdString();
        ret = !(bool)proto->resizemap(proto->cell->vars, proto->dvarfile, &(proto->datamap));  // use names in dvars.txt to resize datamap
    }
    update_menu();
    return ret;
}

bool dvarMenu::write_dvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->dvarfile = fileName.toStdString();
    ret = !(bool)proto->writedvars(proto->datamap, proto->dvarfile);
    }
    return ret;

}

void dvarMenu::update_datamap(pair<string,double*> p, int state){
    if((state = 0)) {
        proto->datamap.erase(p.first);
    } else {
        proto->datamap.insert(p);
    }
}

void dvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}

/*#################################
    begin mvarMenu class
###################################*/
mvarMenu::mvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
    vars_list = new QMap<QString,QStringList>();
//setup useful constants and aliases
    unsigned int row_len = 6;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    close_button = new QPushButton(tr("Save and Exit"), this);
    vars_view = new QListWidget(this);
    meas_view = new QListWidget(this);
    meas_list_label = new QLabel("Variables to Track",this);
    vars_list_label = new QLabel("Aspects to Measure", this);
    addto_meas_list_button = new QPushButton("+", this);
    removefr_meas_list_button = new QPushButton("-", this);
    addto_vars_list_button = new QPushButton("+", this);
    removefr_vars_list_button = new QPushButton("-", this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
//central_layout
    central_layout->addWidget(vars_list_label, 0,0);
    central_layout->addWidget(meas_list_label, 0,2);
    central_layout->addWidget(vars_view, 1,0,1,2);
    central_layout->addWidget(meas_view, 1,2,1,2);
    central_layout->addWidget(addto_vars_list_button,2,0);
    central_layout->addWidget(removefr_vars_list_button,2,1);
    central_layout->addWidget(addto_meas_list_button,2,2);
    central_layout->addWidget(removefr_meas_list_button,2,3);
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, row_len, row_len); 
    main_layout->addWidget(close_button, row_len +1, row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Output Variables Menu"));
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_mvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
}

mvarMenu::~mvarMenu(){}

void mvarMenu::update_menu() {
    unsigned int i,j;
    for(i = 0; i < proto->mvnames.size(); i++) {
        QString ith_mvnames = proto->mvnames[i].c_str();
        if(!vars_list->contains(ith_mvnames)) {
            vars_list->insert(ith_mvnames,QStringList());
        }
        QStringList* next_meas = &((*vars_list)[ith_mvnames]);
        for(j = 0; j < proto->mpnames[i].size(); j++){
            if(!next_meas->contains(QString(proto->mpnames[i][j].c_str()))) {
                next_meas->append(QString(proto->mpnames[i][j].c_str()));
            }
        }
    }
    vars_view->addItems(QStringList(vars_list->keys()));
    if(!vars_list->empty()){
        meas_view->addItems(vars_list->first());
    }
}

void mvarMenu::closeEvent(QCloseEvent* event){
    if(write_close) {
        !(bool)proto->writemvars( proto->mvnames, proto->mpnames, string("mvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
    }
    event->accept();
}

bool mvarMenu::read_mvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->measfile = fileName.toStdString();
        ret = !(bool)proto->parse2Dmap(proto->cell->vars,measures().varmap, proto->measfile, &proto->mvnames, &proto->mpnames);
//        ret = !(bool)proto->initializeMeasure(int(proto->maxmeassize));//create measure from mvarfile
   }
    update_menu();
    return ret;
}

bool mvarMenu::write_mvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->measfile = fileName.toStdString();
        ret = !(bool)proto->writemvars(proto->mvnames, proto->mpnames, proto->measfile);
    }
    return ret;

}

void mvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}

void mvarMenu::addto_meas_list(){};

void mvarMenu::removefr_meas_list(){};

void mvarMenu::addto_vars_list(){};

void mvarMenu::removefr_vars_list(){};

