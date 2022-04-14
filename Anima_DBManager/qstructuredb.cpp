#include "qstructuredb.h"


QStructureDB::QStructureDB(QWidget* parent) :
    QWidget(parent)
{
     myLayout = new QVBoxLayout(this);
}
QStructureDB::~QStructureDB()
{
    for(auto const& strcWidget : myStructureWidgets)
        delete(strcWidget);

    delete(myLayout);
}

void QStructureDB::Init(StructureDB& _structureDB) {}
void QStructureDB::UpdateRows() {}
void QStructureDB::UpdateCols() {}

//QTableView or QTableWidget ?
