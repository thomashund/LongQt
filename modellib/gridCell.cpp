// 5/10/2016
//################################################
//cell type for a grid, used by grid protocol to
// run a simulaiton on a grid of cells
// class definitions
//################################################

#include "gridCell.h"

bool gridCell::setOutputfileConstants(string filename) {
    int i = 0;
    bool toReturn = true;
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++, i++) {
        int j = 0;
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++, j++) {
            sprintf(buffer,filename.c_str(),i,j);
            toReturn = toReturn&&(*iv)->cell->setOutputfileConstants(string(buffer));
        }
    }
    return toReturn;
}
bool gridCell::setOuputfileVariables(string filename) {
    int i = 0;
    bool toReturn = true;
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++, i++) {
        int j = 0;
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++, j++) {
            sprintf(buffer,filename.c_str(),i,j);
            toReturn = toReturn&&(*iv)->cell->setOuputfileVariables(string(buffer));
        }
    }
    return toReturn;
}
bool gridCell::setConstantSelection(set<string> new_selection) {
    bool toReturn = true;
    parsSelection = new_selection;
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            toReturn = toReturn&&(*iv)->cell->setConstantSelection(new_selection);
        }
    }
    return toReturn;
}
bool gridCell::setVariableSelection(set<string> new_selection) {
    bool toReturn = true;
    varsSelection = new_selection;
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            toReturn = toReturn&&(*iv)->cell->setVariableSelection(new_selection);
        }
    }
    return toReturn;
}
void gridCell::writeConstants() {
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            (*iv)->cell->writeConstants();
        }
    }
}
void gridCell::writeVariables() {
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            (*iv)->cell->writeVariables();
        }
    }
}
double gridCell::updateV() {
    int i,j;
    int cells = grid.fibery.size();
    int fibers = grid.fiber.size();
    if(tcount%2==0){
        for(i=0;i<fibers;i++){
            for(j=0;j<cells;j++){
                if(i>0&&i<(fibers-1))
                    grid.fiber[i].nodes[j]->cell->iTot-=grid.fibery[j].nodes[i]->B*(grid.fiber[i-1].nodes[j]->cell->vOld-grid.fiber[i].nodes[j]->cell->vOld)-grid.fibery[j].nodes[i+1]->B*(grid.fiber[i].nodes[j]->cell->vOld-grid.fiber[i+1].nodes[j]->cell->vOld);
                else if(i==0)
                    grid.fiber[i].nodes[j]->cell->iTot-=-grid.fibery[j].nodes[i+1]->B*(grid.fiber[i].nodes[j]->cell->vOld-grid.fiber[i+1].nodes[j]->cell->vOld);
                else if(i==fibers-1)
                    grid.fiber[i].nodes[j]->cell->iTot-=grid.fibery[j].nodes[i]->B*(grid.fiber[i-1].nodes[j]->cell->vOld-grid.fiber[i].nodes[j]->cell->vOld);
            }
            grid.fiber[i].updateVm(dt);
        }
    }
    else{
        for(i=0;i<cells;i++){
            for(j=0;j<fibers;j++){
                if(i>0&&i<(cells-1))
                    grid.fibery[i].nodes[j]->cell->iTot-=grid.fiber[j].nodes[i]->B*(grid.fibery[i-1].nodes[j]->cell->vOld-grid.fibery[i].nodes[j]->cell->vOld)-grid.fiber[j].nodes[i+1]->B*(grid.fibery[i].nodes[j]->cell->vOld-grid.fibery[i+1].nodes[j]->cell->vOld);
                else if(i==0)
                    grid.fibery[i].nodes[j]->cell->iTot-=-grid.fiber[j].nodes[i+1]->B*(grid.fibery[i].nodes[j]->cell->vOld-grid.fibery[i+1].nodes[j]->cell->vOld);
                else if(i==cells-1)
                    grid.fibery[i].nodes[j]->cell->iTot-=grid.fiber[j].nodes[i]->B*(grid.fibery[i-1].nodes[j]->cell->vOld-grid.fibery[i].nodes[j]->cell->vOld);
            }
            grid.fibery[i].updateVm(dt);
        }
    }
    return 0.0;
}
void gridCell::updateConc() {
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            (*iv)->cell->updateConc();
        }
    }
}
void gridCell::updateCurr() {
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            (*iv)->cell->updateCurr();
        }
    }
}
int gridCell::externalStim(double stimval) {
    for(auto it = grid.fiber.begin(); it != grid.fiber.end(); it++) {
        for(auto iv = it->nodes.begin(); iv != it->nodes.end(); iv++) {
            (*iv)->cell->externalStim(stimval);
        }
    }
    return 1;
}
double gridCell::tstep(double stimt)
{
    unsigned int i,j;
    int vmflag=0;

    tcount++;
    t=t+dt;
    for(i=0;i<grid.fiber.size();i++){
        for(j=0;j<grid.fibery.size();j++){
            grid.fiber[i].nodes[j]->cell->t=t;
            grid.fiber[i].nodes[j]->cell->dt = dt;      
                if(tcount%2==0){  // used to prevent time step change in middle of ADI	
                    if(grid.fiber[i].nodes[j]->cell->dVdt>1.0||(t>(stimt-1.0)&&t<stimt))
                        vmflag = 2;
                    else if(grid.fiber[i].nodes[j]->cell->vOld>(-50)&&vmflag!=2)
                        vmflag = 1;
                }
        }
    }
    if(tcount%2==0){
        if(vmflag==2)
	        dt = dtmin;
        else if(vmflag==1)
	        dt = dtmed;
        else  
	        dt = dtmax;
    }   
    return t;
}
