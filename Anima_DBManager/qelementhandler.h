#ifndef QELEMENTHANDLER_H
#define QELEMENTHANDLER_H

#include <QWidget>

#include <QLabel>
#include <QList>
#include <QPushButton>

class QElementHandler : public QWidget
{
    Q_OBJECT
private:
    int myElementIndex;
    QLabel* myCurrentLabel;
    QLabel* myCurrentTextPreview;
    QList<QPushButton*> myButtons;

public:
    explicit QElementHandler(QWidget *parent = nullptr);
    void OnSelectElement(int _index, const QString& _content);

signals:
    void SpinBoxSelected(int _index);
    void AddRequested(const int _index);
    void DuplicateRequested(const int _index, const int _originalIndex);
    void MoveRequested(const int _indexFrom, const int _indexTo);
    void RemoveRequested(const int _index);

public slots:
    void OnAddBefore();
    void OnAddAfter();
    void OnDuplicate();
    void OnRemove();

    void OnMoveFirst();
    void OnMoveUp();
    void OnMoveDown();
    void OnMoveLast();
};

#endif // QELEMENTHANDLER_H
