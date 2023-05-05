#include "qstructuretable.h"
#include "qattribute.h"
#include "qattributedisplay.h"
#include <QHBoxLayout>

#include "db_manager.h"

QStructureTable::QStructureTable(StructureDB& _structureDB) :
    QTableWidget(nullptr),
    myStructureDB(_structureDB)
{
    setItemPrototype(new QAttributeDisplay());
    //QObject::connect(this, &QTableWidget::currentCellChanged, this, &QStructureTable::OnSelectOrEditItem);
}

QStructureTable::~QStructureTable()
{}


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

void QStructureTable::UpdateTable()
{
    // Reset
        // delete current QAttribute

    // Column number & Headers
    const auto& templ = myStructureDB.GetTemplate();
    const int templAttrCount = (int)(templ.GetAttributes().size());
    setColumnCount(templAttrCount);

    QStringList templAttrNames;
    for (int i = 0; i < templAttrCount; i++)
    {
        templAttrNames.append(templ.GetAttributeName(i));
    }
    setHorizontalHeaderLabels(templAttrNames);


    // Fill the table
    const int structureCount = myStructureDB.GetStructureCount();
    setRowCount(structureCount);

    for (int row = 0; row < structureCount; row++)
    {
        const Structure* strct = myStructureDB.GetStructureAt(row);
        Q_ASSERT(strct != nullptr);
        for (int col = 0; col < templAttrCount; col++)
        {
            const auto* attribute = strct->GetAttribute(col);

            QAttributeDisplay* attributeItem;
            auto* baseItem = item(row,col);
            bool needCreateItem = true;
            if (baseItem != nullptr)
            {
                attributeItem = dynamic_cast<QAttributeDisplay*>(baseItem);
                if (attributeItem->IsRepresentingABool() && attribute->GetType() != AttributeTypeHelper::Type::Bool)
                {
                    delete takeItem(row, col);
                }
                else
                {
                    needCreateItem = false;
                }
            }

            if (needCreateItem)
            {
                attributeItem = new QAttributeDisplay();
                setItem(row, col, attributeItem);
            }
            Q_ASSERT(attributeItem);
            attributeItem->SetContentFromAttribute(attribute);
        }
    }
}


void QStructureTable::OnSelectOrEditItem(int _index)
{
    // Todo : QAttribute to add on cell, & delete previous
    DB_Manager::GetDB_Manager().AskFocusOnStructPanel(myStructureDB.GetTemplateName(), _index);
}
