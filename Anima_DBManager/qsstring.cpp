#include "qsstring.h"

#include "db_manager.h"
#include "qsstringdialog.h"

QSString::QSString(QWidget* parent) :
    QLabel(parent)
{
    setMinimumWidth(110);
}


void QSString::SetValue(const QString& _tableId, const QString& _stringId)
{
    myTableName = _tableId;
    myStringIdentifier = _stringId;

    setText(DB_Manager::GetDB_Manager().GetStringForDisplay(myTableName, myStringIdentifier));
}

void QSString::GetValue(QString& _outTableId, QString& _outStringId) const
{
    _outTableId = myTableName;
    _outStringId = myStringIdentifier;
}

QString QSString::GetFormattedValue() const
{
    return "#" + myTableName + "#" + myStringIdentifier;
}


void QSString::EditValue()
{
    QString outTableName = myTableName;
    QString outStringIdentifier = myStringIdentifier;
    auto* dialog = new QSStringDialog(outTableName, outStringIdentifier, this);
    dialog->exec();//open();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {
        myTableName = outTableName;
        myStringIdentifier = outStringIdentifier;
        emit OnValueEdited();
    }
}
