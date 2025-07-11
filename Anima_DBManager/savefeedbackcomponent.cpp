#include "savefeedbackcomponent.h"

#include <QCoreApplication>

#define SAVE_STEP_COUNT 6
//#define WITH_THREAD_SLEEP

#ifdef WITH_THREAD_SLEEP
#include <QThread>
#endif


SaveFeedbackComponent::SaveFeedbackComponent() {}

void SaveFeedbackComponent::StartSaveFeedback()
{
    Q_ASSERT(myProgressBarFeedback == nullptr);
    myProgressBarFeedback = new QProgressDialog("Saving...", QString(), 0, 100);
    myProgressBarFeedback->setWindowModality(Qt::ApplicationModal);
    myProgressBarFeedback->setWindowFlags(myProgressBarFeedback->windowFlags() & ~Qt::WindowCloseButtonHint);
    myProgressBarFeedback->setAttribute(Qt::WA_DeleteOnClose);
    myProgressBarFeedback->setMinimumDuration(0);
    myProgressBarFeedback->show();
    QCoreApplication::processEvents();
}

void SaveFeedbackComponent::EndSaveFeedback()
{
    Q_ASSERT(myProgressBarFeedback != nullptr);
    myProgressBarFeedback->close();
    myProgressBarFeedback = nullptr;
    // delete handled by Qt::WA_DeleteOnClose attribute
}

void SaveFeedbackComponent::SetSaveProgress(int _index, int _count, int _stepIndex, const QString& _stepObjects)
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    Q_ASSERT(_count != 0);
    int progress = (_stepIndex * 100 / SAVE_STEP_COUNT) + ((100 * _index) / (SAVE_STEP_COUNT * _count));

    static QString textTemplate = QString("%3 (%1/%2)");
    QString labelText = textTemplate.arg(_index+1).arg(_count).arg(_stepObjects);
    myProgressBarFeedback->setLabelText(labelText);
    myProgressBarFeedback->setValue(progress);
#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetSaveStringTableProgress(int tableIndex, int tableCount)
{
    SetSaveProgress(tableIndex, tableCount, 0, "Saving String Tables");
}
void SaveFeedbackComponent::SetSaveEnumProgress(int enumIndex, int enumCount)
{
    SetSaveProgress(enumIndex, enumCount, 1, "Saving Enumerators");
}
void SaveFeedbackComponent::SetSaveStructTemplateProgress(int structIndex, int structCount)
{
    SetSaveProgress(structIndex, structCount, 2, "Saving Structure Templates");
}
void SaveFeedbackComponent::SetSaveStructDataProgress(int structIndex, int structCount)
{
    SetSaveProgress(structIndex, structCount, 3, "Saving Structure Data");
}
void SaveFeedbackComponent::SetSaveProjectProgress()
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    int progress = (5 * 100 / SAVE_STEP_COUNT);

    static QString text = QString("Saving Project properties...");
    myProgressBarFeedback->setLabelText(text);
    myProgressBarFeedback->setValue(progress);

#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetSaveCompilationProgress(int fileIndex, int fileCount)
{
    SetSaveProgress(fileIndex, fileCount, 5, "Compiling files");
}

