#include "qstructure.h"

QStructure::QStructure(QWidget *parent) :
    QWidget(parent)
{

}

QStructure::~QStructure()
{
    for(auto const& strcWidget : myStructureWidgets)
        delete(strcWidget);

    delete(myLayout);
}

void QStructure::UpdateAttributes(const Structure& _structure)
{
    //...
}
