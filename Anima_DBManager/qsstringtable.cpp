#include "qsstringtable.h"

#include "sstringhelper.h"
#include <QMenu>
#include <QCursor>

QSStringTable::QSStringTable(SStringTable& _stringTable, QWidget* _parent) :
    QTableWidget(_parent),
    myStringTable(_stringTable)
{
    setColumnCount(1 + SStringHelper::SStringLanguages::Count);

    QStringList colHeaderNames;
    colHeaderNames.append("Identifier");
    for (int language = 0; language < SStringHelper::SStringLanguages::Count; language++)
    {
        colHeaderNames.append(SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)language));
        setColumnWidth(language+1, 300);
    }
    setHorizontalHeaderLabels(colHeaderNames);

    UpdateTable();

    QObject::connect(this, &QTableWidget::cellChanged, this, &QSStringTable::OnCellEdited);
    QObject::connect(this, &QTableWidget::customContextMenuRequested, this, &QSStringTable::HandleContextMenu);
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
}


void QSStringTable::UpdateTable()
{
    const int count = myStringTable.GetStringItemCount();
    setRowCount(count);

    for (int i = 0; i < count; i++)
    {
        const auto& stringItem = myStringTable.GetStringItem(i);
        QTableWidgetItem* item = new QTableWidgetItem(stringItem->GetIdentifier());
        item->setBackground(idBrush);
        setItem(i, 0, item);

        for (int language = 0; language < SStringHelper::SStringLanguages::Count; language++)
        {
            item = new QTableWidgetItem(stringItem->GetString((SStringHelper::SStringLanguages)language));
            if (item->text().isEmpty())
                item->setBackground(emptyBrush);
            setItem(i, language + 1, item);
        }

        //QTableWidgetItem* header = verticalHeaderItem(i);
    }
}

void QSStringTable::OnCellEdited(int row, int col)
{
    auto* changedItem = item(row,col);
    if (!changedItem)
    {
        qFatal("Invalid item changed");
    }

    const QString text = changedItem->text();
    if (col == 0)
    {
        const QString oldId = myStringTable.GetStringItem(row)->GetIdentifier();
        if (!myStringTable.SetItemIdentifier(row, text))
            changedItem->setText(oldId);
    }
    else
    {
        changedItem->setBackground(text.isEmpty() ? emptyBrush : defBrush);
        myStringTable.SetItemString(row, (SStringHelper::SStringLanguages)(col -1), text);
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
    const auto* stringItem = myStringTable.GetStringItem(rowClicked);
    if (!stringItem)
        return;

    const QString& rowId = stringItem->GetIdentifier();

    auto* action_AddB = menu.addAction("Add before '"+rowId+"'");
    auto* action_AddA = menu.addAction("Add after '"+rowId+"'");
    auto* action_Dupl = menu.addAction("Duplicate '"+rowId+"'");
    // Set verified/validated ?

    QObject::connect(action_AddB, &QAction::triggered, [this, rowClicked](){ myStringTable.AddStringItem(rowClicked); UpdateTable(); });
    QObject::connect(action_AddA, &QAction::triggered, [this, rowClicked](){ myStringTable.AddStringItem(rowClicked+1); UpdateTable(); });
    QObject::connect(action_Dupl, &QAction::triggered, [this, rowClicked, stringItem](){
        QString _texts[SStringHelper::SStringLanguages::Count];
        for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
        {
            _texts[i] = stringItem->GetString((SStringHelper::SStringLanguages)i);
        }

        myStringTable.AddStringItemWithTexts(rowClicked+1, _texts, &stringItem->GetIdentifier());
        UpdateTable();
    });

    const int itemCount = myStringTable.GetStringItemCount();
    if (itemCount > 1)
    {
        const int colClicked = column(item);

        menu.addSeparator();
        if (rowClicked != 0)
        {
            auto* action_MovF = menu.addAction("Move '"+rowId+"' first");
            auto* action_MovU = menu.addAction("Move '"+rowId+"' up");

            QObject::connect(action_MovF, &QAction::triggered, [this, rowClicked, colClicked](){
                myStringTable.MoveStringItems(rowClicked, 0);
                setCurrentCell(0, colClicked);
                UpdateTable(); });
            QObject::connect(action_MovU, &QAction::triggered, [this, rowClicked, colClicked](){
                myStringTable.SwapStringItems(rowClicked, rowClicked-1);
                setCurrentCell(rowClicked-1, colClicked);
                UpdateTable(); });
        }
        if (rowClicked != itemCount-1)
        {
            auto* action_MovD = menu.addAction("Move '"+rowId+"' down");
            auto* action_MovL = menu.addAction("Move '"+rowId+"' last");

            QObject::connect(action_MovD, &QAction::triggered, [this, rowClicked, colClicked](){
                myStringTable.SwapStringItems(rowClicked, rowClicked+1);
                setCurrentCell(rowClicked+1, colClicked);
                UpdateTable(); });
            QObject::connect(action_MovL, &QAction::triggered, [this, rowClicked, colClicked, itemCount](){
                myStringTable.MoveStringItems(rowClicked, itemCount-1);
                setCurrentCell(itemCount-1, colClicked);
                UpdateTable(); });
        }

        menu.addSeparator();
        auto* action_RMov = menu.addAction("Remove '"+rowId+"'");
        QObject::connect(action_RMov, &QAction::triggered, [this, rowClicked](){ myStringTable.RemoveStringItem(rowClicked); UpdateTable(); });
    }

    menu.exec(QCursor::pos());
}
