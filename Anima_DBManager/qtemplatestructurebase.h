#ifndef QTEMPLATESTRUCTUREBASE_H
#define QTEMPLATESTRUCTUREBASE_H

#include <QWidget>

#include "templatestructure.h"

#include <QList>
#include <QPushButton>
#include <QTabWidget>

class QtemplateStructureBase : public QWidget
{
    Q_OBJECT
protected:
    QTabWidget* myTabWidget;
    QList<QPushButton*> myPushBtns;

    virtual void AddAttribute(int _position, bool _duplicatePrevious = false) = 0;

public:
    QtemplateStructureBase(QWidget* _parent = nullptr);

signals:

public slots:
    virtual void UpdateContent() = 0;

    virtual void OnTabMoved(int _indexFrom, int _indexTo) = 0;
    virtual void OnAddBefore() = 0;
    virtual void OnAddAfter() = 0;
    virtual void OnDuplicate() = 0;
    virtual void OnRemove() = 0;
};


#endif // QTEMPLATESTRUCTUREBASE_H
