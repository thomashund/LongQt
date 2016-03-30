#include "menu_object.h"
#include "varmenu.h"

class mvars_menu_object : public menu_object {
  public:
    mvars_menu_object(string name, Protocol* proto, QDir working_dir, QWidget* parent = 0) {
        this->proto = proto;
        this->name = name;
        this->working_dir = working_dir;
        this->parent = parent;
        this->menu = new mvarMenu(proto, working_dir, parent);
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
       this->menu = new mvarMenu(proto, working_dir, parent);
    }
    void setWorkingDir(QDir dir) {
        working_dir = dir;
        menu->setWorkingDir(dir);
    }
    void leave() {
        menu->write_file();
    }
    void changeProto(Protocol* proto) {
        this->proto = proto;
    }
  private:
    mvarMenu* menu;
    Protocol* proto;
    QDir working_dir;
    QWidget* parent;
};
