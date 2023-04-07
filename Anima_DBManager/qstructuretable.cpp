#include "qstructuretable.h"
#include "qattribute.h"
#include <QHBoxLayout>

#include "db_manager.h"

QStructureTable::QStructureTable(StructureDB& _structureDB) :
    QTableWidget(nullptr),
    myStructureDB(_structureDB)
{
    UpdateTable();
    QObject::connect(this, &QTableWidget::currentCellChanged, this, &QStructureTable::OnSelectOrEditItem);
}

QStructureTable::~QStructureTable()
{
    DeleteAllChilds();
}


void QStructureTable::ExportStructsToCSV(const QString _directoryPath)
{
    Q_ASSERT(!_directoryPath.isEmpty());

    QString filePath = _directoryPath + "/DT_" + myStructureDB.GetTemplateName() + ".csv";
    qDebug() << "Export String table " << myStructureDB.GetTemplateName() << " to file : " << filePath;

    std::ofstream csvFile(filePath.toStdString());
    if (!csvFile)
    {
        qCritical() <<"ERROR EXPORT STRUCT TABLE : " << filePath << " couldn't be open";
        return;
    }

    myStructureDB.WriteValue_CSV_Table(csvFile);

    csvFile.close();
}

void QStructureTable::DeleteAllChilds()
{
    while (myChildWidgetsToDelete.size() > 0)
    {
        if (myChildWidgetsToDelete.back() != nullptr)
        {
            delete  myChildWidgetsToDelete.back();
        }
        myChildWidgetsToDelete.pop_back();
    }
}

void QStructureTable::UpdateTable()
{
    // Reset
    setRowCount(0);
    setColumnCount(0);
    DeleteAllChilds();

    // Column number & Headers
    const auto templ = myStructureDB.GetTemplate();

    const int templAttrCount = (int)(templ.GetAttributes().size());
    setColumnCount(templAttrCount);

    QStringList templAttrNames;
    for (int i = 0; i < templAttrCount; i++)
    {
        templAttrNames.append(templ.GetAttributeName(i));
        //setColumnWidth(i, GetColWidth(templ.GetAttributeType(i)));
    }
    setHorizontalHeaderLabels(templAttrNames);


    // Fill the table
    const int structureCount = myStructureDB.GetStructureCount();
    setRowCount(structureCount);
    myChildWidgetsToDelete.reserve(structureCount * templAttrCount);
    for (int row = 0; row < structureCount; row++)
    {
        const Structure* strct = myStructureDB.GetStructureAt(row);
        if (!strct)
            break;
        for (int col = 0; col < templAttrCount; col++)
        {
            QAttribute* qAttr = new QAttribute(this);
            setCellWidget(row, col, qAttr);
            qAttr->UpdateAttribute(strct->GetAttribute(col));
            myChildWidgetsToDelete.push_back(qAttr);
            QObject::connect(qAttr, &QAttribute::OnWidgetValueChanged, [this, row](){
                OnSelectOrEditItem(row);
            });
        }
    }
    resizeRowsToContents();
    //resizeColumnsToContents();
}


void QStructureTable::OnSelectOrEditItem(int _index)
{
    DB_Manager::GetDB_Manager().AskFocusOnStructPanel(myStructureDB.GetTemplateName(), _index);
}
