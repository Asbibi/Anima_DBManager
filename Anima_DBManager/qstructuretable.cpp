#include "qstructuretable.h"
#include "qattributedisplay.h"
#include <QHBoxLayout>

#include "db_manager.h"
#include <QFile>
#include <QJsonDocument>

QStructureTable::QStructureTable(StructureDB& _structureDB) :
    QTableWidget(nullptr),
    myStructureDB(_structureDB)
{
    setItemPrototype(new QAttributeDisplay());
    setSelectionMode(QAbstractItemView::SingleSelection);
    QObject::connect(this, &QTableWidget::itemSelectionChanged, this, &QStructureTable::OnSelectionChanged);
    QObject::connect(this, &QTableWidget::currentCellChanged, this, &QStructureTable::OnSelectItem);
}

QStructureTable::~QStructureTable()
{}


void QStructureTable::ExportStructsToJSON(const QString _directoryPath)
{
    Q_ASSERT(!_directoryPath.isEmpty());
    QString filePath = _directoryPath + "/DT_" + myStructureDB.GetTemplateName() + ".json";
    qDebug() << "Export String table " << myStructureDB.GetTemplateName() << " to file : " << filePath;

    QFile file = QFile(filePath);
    if(!file.open(QIODevice::ReadWrite))
    {
        qCritical() <<"ERROR EXPORT STRUCT TABLE : " << filePath << " couldn't be open";
        return;
    }
    file.resize(0); // Clear file content

    file.write(QJsonDocument(myStructureDB.WriteValue_JSON_Table()).toJson());

    file.close();
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
void QStructureTable::UnselectCurrent()
{
    Unselect(currentRow(), currentColumn());
}
void QStructureTable::Unselect(int _row, int _col)
{
    if (myCurrentAttributeEditor == nullptr)
    {
        return;
    }

    Q_ASSERT(myCurrentAttributeEditor == cellWidget(_row, _col));

    // Clean previous QAttribute
    setCellWidget(_row, _col, nullptr);
    QObject::disconnect(myCurrentAttributeEditor);
    delete myCurrentAttributeEditor;
    myCurrentAttributeEditor = nullptr;

    // "Show" the corresponding item
    QAttributeDisplay* attributeItem = dynamic_cast<QAttributeDisplay*>(item(_row, _col));
    Q_ASSERT(attributeItem != nullptr);
    attributeItem->SetContentFromAttribute(myStructureDB.GetStructureAt(_row)->GetAttribute(_col));
    setItem(_row, _col, attributeItem);
}



void QStructureTable::UpdateTable()
{
    // Reset
    clearSelection();
    UnselectCurrent();

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
    // Unselect previous cell
    if (_previousRow != -1 && _previousColumn != -1)
    {
        Unselect(_previousRow, _previousColumn);
    }


    // "Hiding" without deleting the item (need to be != nullptr for OnSelectionChanged
    QAttributeDisplay* attributeItem = dynamic_cast<QAttributeDisplay*>(item(_currentRow, _currentColumn));
    Q_ASSERT(attributeItem != nullptr);
    attributeItem->SetContentFromAttribute(nullptr);


    // Creating a QAttribute to edit the cell
    myCurrentAttributeEditor = new QAttribute();
    myCurrentAttributeEditor->UpdateAttribute(myStructureDB.GetStructureAt(_currentRow)->GetAttribute(_currentColumn));
    QObject::connect(myCurrentAttributeEditor, &QAttribute::OnWidgetValueChanged, this, [this, _currentRow](){
                    OnSelectOrEditItem(_currentRow);
                });
    setCellWidget(_currentRow, _currentColumn, myCurrentAttributeEditor);


    OnSelectOrEditItem(_currentRow);
}

void QStructureTable::OnSelectOrEditItem(int _index)
{
    // Todo : QAttribute to add on cell, & delete previous
    DB_Manager::GetDB_Manager().AskFocusOnStructPanel(myStructureDB.GetTemplateName(), _index);
}

void QStructureTable::OnSelectionChanged()
{
    if (selectedItems().count() == 0)
    {
        UnselectCurrent();
    }
    else if (myCurrentAttributeEditor == nullptr)
    {
        OnSelectItem(currentRow(), currentColumn(), -1, -1);
    }
}
