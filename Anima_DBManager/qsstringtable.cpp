#include "qsstringtable.h"

#include "sstringhelper.h"
#include "db_manager.h"
#include <QCursor>
#include <QHeaderView>
#include <QMenu>
#include <QFile>
#include <QMessageBox>

QSStringTable::QSStringTable(int _strTableIndex, QWidget* _parent) :
    QTableWidget(_parent),
    myStringTableIndex(_strTableIndex)
{
    UpdateTableWithLanguages();

    QObject::connect(this, &QTableWidget::itemDoubleClicked, this, &QSStringTable::OnCellEdit);
    QObject::connect(this, &QTableWidget::cellChanged, this, &QSStringTable::OnCellEdited);
    QObject::connect(this, &QTableWidget::customContextMenuRequested, this, &QSStringTable::HandleContextMenu);

    DB_Manager* dbManager = &DB_Manager::GetDB_Manager();
    QObject::connect(dbManager, &DB_Manager::LanguagesChanged, this, &QSStringTable::UpdateTableWithLanguages);

    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
}

void QSStringTable::PrivateUpdate()
{
    UpdateTable();
    DB_Manager::GetDB_Manager().AskUpdateOnStringPanel(myStringTableIndex);
}



SStringTable& QSStringTable::GetTable()
{
    SStringTable* table = nullptr;
    if (myStringTableIndex < 0)
    {
        table = DB_Manager::GetDB_Manager().GetDictionary();
    }
    else
    {
        table = DB_Manager::GetDB_Manager().GetStringTable(myStringTableIndex);
    }
    Q_ASSERT(table != nullptr);
    return *table;
}

void QSStringTable::UpdateIndex(int _strTableIndex)
{
    myStringTableIndex = _strTableIndex;
}

void QSStringTable::ExportStringsToCSV(const QString& _filePathTemplate, int _languageIndex, QExportStringConflictPolicy _conflictPolicy)
{
    const auto& languages = DB_Manager::GetDB_Manager().GetLanguages();
    Q_ASSERT(!_filePathTemplate.isEmpty());
    Q_ASSERT(_languageIndex >= 0);
    Q_ASSERT(_languageIndex < languages.GetLanguageCount());
    SStringTable& stringTable = GetTable();
    const auto& language = languages.GetLanguage(_languageIndex);

    const QString& stringTableName = stringTable.GetTableName();
    QString filePath = _filePathTemplate.arg(language.GetAbbrev(), stringTableName);

    qDebug() << "Export String table " << stringTableName << " to file : " << filePath;

    auto fileToCreate = QFile(filePath);
    if (fileToCreate.exists())
    {
        switch(_conflictPolicy)
        {
            case QExportStringConflictPolicy::OVERWRITE:
            {
                qWarning() << "Conflict for table " << stringTableName << " [" << language.GetAbbrev() << "] | Policy set to overwrite : file saved anyway";
                break;
            }
            case QExportStringConflictPolicy::IGNORE:
            {
                qWarning() << "Conflict for table " << stringTableName << " [" << language.GetAbbrev() << "] | Policy set to ignore : table skipped";
                return;
            }
            default:
            case QExportStringConflictPolicy::ASK:
            {
                auto conflictDialogBoxResult = QMessageBox::warning(
                    this,
                    "String Table file already existing",
                    QString("The file \"%1\" already exists.\nSave and overwrite or ignore the String Table \"%2\" for the language %3 [%4] ?")
                        .arg(fileToCreate.fileName(), stringTableName, language.GetName(), language.GetAbbrev()),
                    QMessageBox::StandardButtons(QMessageBox::Save | QMessageBox::Ignore) );
                if (conflictDialogBoxResult != QMessageBox::Save)
                {
                    return;
                }
                break;
            }
        }
    }

    std::ofstream csvFile(filePath.toStdString());
    if (!csvFile)
    {
        qCritical() <<"ERROR EXPORT STRING TABLE : " << filePath << " couldn't be open";
        return;
    }

    csvFile << "Key,SourceString";
    stringTable.WriteValue_CSV(csvFile, _languageIndex, true);

    csvFile.close();
}



void QSStringTable::UpdateTableWithLanguages()
{
    const auto& languages = DB_Manager::GetDB_Manager().GetLanguages();
    const int languagesCount = languages.GetLanguageCount();
    setColumnCount(1 + languagesCount);

    QStringList colHeaderNames;
    colHeaderNames.append("Identifier");
    for (int language = 0; language < languagesCount; language++)
    {
        colHeaderNames.append(languages.GetLanguage(language).GetName());
        setColumnWidth(language+1, 300);
    }
    setHorizontalHeaderLabels(colHeaderNames);

    UpdateTable();
}
void QSStringTable::UpdateTable()
{
    const int languagesCount = DB_Manager::GetLanguagesCount();
    const int count = GetTable().GetStringItemCount();
    setRowCount(count);

    for (int i = 0; i < count; i++)
    {
        const auto& stringItem = GetTable().GetStringItem(i);
        QTableWidgetItem* item = new QTableWidgetItem(stringItem->GetIdentifier());
        item->setBackground(idBrush);
        setItem(i, 0, item);

        for (int language = 0; language < languagesCount; language++)
        {
            item = new QTableWidgetItem(stringItem->GetString(language));
            if (item->text().isEmpty())
                item->setBackground(emptyBrush);
            setItem(i, language + 1, item);
        }
    }
    verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}



void QSStringTable::OnCellEdit(QTableWidgetItem *item)
{
    scrollToItem(item);
    blockSignals(true);
    item->setText(item->text().replace('\n', "\\n"));
    blockSignals(false);
}
void QSStringTable::OnCellEdited(int row, int col)
{
    auto* changedItem = item(row,col);
    Q_ASSERT(changedItem != nullptr);

    const QString text = changedItem->text().replace("\\n", "\n");
    blockSignals(true);
    changedItem->setText(text);
    blockSignals(false);
    if (col == 0)
    {
        const QString oldId = GetTable().GetStringItem(row)->GetIdentifier();
        if (!GetTable().SetItemIdentifier(row, text))
            changedItem->setText(oldId);
        else
            emit IdentifierEdited(myStringTableIndex);
    }
    else
    {
        changedItem->setBackground(text.isEmpty() ? emptyBrush : defBrush);
        GetTable().SetItemString(row, col -1, text);
        verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    }
}


void QSStringTable::HandleContextMenu(const QPoint& point)
{
    QTableWidgetItem *item = itemAt(point);
    if (!item) {
        return;
    }

    const int rowClicked = row(item);
    qDebug() << rowClicked;
    QMenu menu(this);
    const auto* stringItem = GetTable().GetStringItem(rowClicked);
    if (!stringItem)
        return;

    const QString& rowId = stringItem->GetIdentifier();

    auto* action_AddB = menu.addAction("Add before '"+rowId+"'");
    auto* action_AddA = menu.addAction("Add after '"+rowId+"'");
    auto* action_Dupl = menu.addAction("Duplicate '"+rowId+"'");
    // Set verified/validated ?

    QObject::connect(action_AddB, &QAction::triggered, [this, rowClicked](){ GetTable().AddStringItem(rowClicked); PrivateUpdate(); });
    QObject::connect(action_AddA, &QAction::triggered, [this, rowClicked](){ GetTable().AddStringItem(rowClicked+1); PrivateUpdate(); });
    QObject::connect(action_Dupl, &QAction::triggered, [this, rowClicked, &stringItem](){
        const int languagesCount = DB_Manager::GetLanguagesCount();
        QString _texts[languagesCount];
        for (int i = 0; i < languagesCount; i++)
        {
            _texts[i] = stringItem->GetString(i);
        }

        GetTable().AddStringItemWithTexts(rowClicked+1, _texts, &stringItem->GetIdentifier());
        PrivateUpdate();
    });

    const int itemCount = GetTable().GetStringItemCount();
    if (itemCount > 1)
    {
        const int colClicked = column(item);

        menu.addSeparator();
        if (rowClicked != 0)
        {
            auto* action_MovF = menu.addAction("Move '"+rowId+"' first");
            auto* action_MovU = menu.addAction("Move '"+rowId+"' up");

            QObject::connect(action_MovF, &QAction::triggered, [this, rowClicked, colClicked](){
                GetTable().MoveStringItems(rowClicked, 0);
                setCurrentCell(0, colClicked);
                PrivateUpdate(); });
            QObject::connect(action_MovU, &QAction::triggered, [this, rowClicked, colClicked](){
                GetTable().SwapStringItems(rowClicked, rowClicked-1);
                setCurrentCell(rowClicked-1, colClicked);
                PrivateUpdate(); });
        }
        if (rowClicked != itemCount-1)
        {
            auto* action_MovD = menu.addAction("Move '"+rowId+"' down");
            auto* action_MovL = menu.addAction("Move '"+rowId+"' last");

            QObject::connect(action_MovD, &QAction::triggered, [this, rowClicked, colClicked](){
                GetTable().SwapStringItems(rowClicked, rowClicked+1);
                setCurrentCell(rowClicked+1, colClicked);
                PrivateUpdate(); });
            QObject::connect(action_MovL, &QAction::triggered, [this, rowClicked, colClicked, itemCount](){
                GetTable().MoveStringItems(rowClicked, itemCount-1);
                setCurrentCell(itemCount-1, colClicked);
                PrivateUpdate(); });
        }

        menu.addSeparator();
        auto* action_RMov = menu.addAction("Remove '"+rowId+"'");
        QObject::connect(action_RMov, &QAction::triggered, [this, rowClicked](){ GetTable().RemoveStringItem(rowClicked); PrivateUpdate(); });
    }

    menu.exec(QCursor::pos());
}
