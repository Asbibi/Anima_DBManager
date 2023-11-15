#include "qreflabel.h"

#include "qrefdialog.h"

QRefLabel::QRefLabel(QWidget* parent) :
    QLabel(parent)
{}

void QRefLabel::SetStructureDB(const StructureDB* structureDB)
{
    if (!structureDB)
        return;

    hisStructureDB = structureDB;
    myRefIndexInDB = 0;
}
void QRefLabel::SetValue(int index)
{
    if (!hisStructureDB)
    {

        myRefIndexInDB = -1;
        setText("<font color=\"red\">NULL DB</font>");
    }

    if (index < 0 || hisStructureDB->GetStructureCount() <= index)
    {
        myRefIndexInDB = -1;
        setText("<font color=\"darkred\">Invalid</font>");
        return;
    }

    myRefIndexInDB = index;
    setText(hisStructureDB->GetStructureRowName(index));
}
int QRefLabel::GetValue() const
{
    return hisStructureDB ? myRefIndexInDB : -1;
}

const QString QRefLabel::GetValueText()
{
    Q_ASSERT(hisStructureDB != nullptr);
    return hisStructureDB->GetStructureRowName(myRefIndexInDB);
}

void QRefLabel::EditValue()
{
    if (!hisStructureDB)
    {
        qCritical() << "Try to edit ref attribute value but structureDB is nullptr";
        return;
    }

    int outIndex = myRefIndexInDB;
    auto* dialog = new QRefDialog(*hisStructureDB, outIndex, this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {
        SetValue(outIndex);
        emit OnValueEdited();
    }
    delete dialog;
}
