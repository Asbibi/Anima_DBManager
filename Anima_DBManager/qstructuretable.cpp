#include "qstructuretable.h"
#include "qattributedisplay.h"
#include <QHBoxLayout>

#include "db_manager.h"

QStructureTable::QStructureTable(StructureDB& _structureDB) :
    QTableWidget(nullptr),
    myStructureDB(_structureDB)
{
    setItemPrototype(new QAttributeDisplay());
    QObject::connect(this, &QTableWidget::currentCellChanged, this, &QStructureTable::OnSelectItem);
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
    if (myCurrentAttributeEditor != nullptr)
    {
        setCellWidget(currentRow(), currentColumn(), nullptr);
        QObject::disconnect(myCurrentAttributeEditor);
        delete myCurrentAttributeEditor;
        myCurrentAttributeEditor = nullptr;
    }

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



void QStructureTable::OnSelectItem(int _currentRow, int _currentColumn, int _previousRow, int _previousColumn)
{
    if (myCurrentAttributeEditor != nullptr)
    {
        setCellWidget(_previousRow, _previousColumn, nullptr);
        QObject::disconnect(myCurrentAttributeEditor);
        delete myCurrentAttributeEditor;
        auto * attributeItem = new QAttributeDisplay();
        attributeItem->SetContentFromAttribute(myStructureDB.GetStructureAt(_previousRow)->GetAttribute(_previousColumn));
        setItem(_previousRow, _previousColumn, attributeItem);
    }

    myCurrentAttributeEditor = new QAttribute();
    myCurrentAttributeEditor->UpdateAttribute(myStructureDB.GetStructureAt(_currentRow)->GetAttribute(_currentColumn));
    QObject::connect(myCurrentAttributeEditor, &QAttribute::OnWidgetValueChanged, [this, _currentRow](){
                    OnSelectOrEditItem(_currentRow);
                });

    setItem(_currentRow, _currentColumn, nullptr);
    setCellWidget(_currentRow, _currentColumn, myCurrentAttributeEditor);

    OnSelectOrEditItem(_currentRow);
}

void QStructureTable::OnSelectOrEditItem(int _index)
{
    // Todo : QAttribute to add on cell, & delete previous
    DB_Manager::GetDB_Manager().AskFocusOnStructPanel(myStructureDB.GetTemplateName(), _index);
}
