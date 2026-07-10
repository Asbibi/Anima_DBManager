#include "savefeedbackcomponent.h"

#include <QCoreApplication>

#define SAVE_STEP_COUNT 7
#define OPEN_STEP_COUNT 8

#define WITH_THREAD_SLEEP

#ifdef WITH_THREAD_SLEEP
#include <QThread>
#endif


SaveFeedbackComponent::SaveFeedbackComponent() {}

void SaveFeedbackComponent::StartSaveOpenFeedback()
{
    Q_ASSERT(myProgressBarFeedback == nullptr);
    myProgressBarFeedback = new QProgressDialog("Saving/Opening...", QString(), 0, 100);
    myProgressBarFeedback->setWindowModality(Qt::ApplicationModal);
    myProgressBarFeedback->setWindowFlags(myProgressBarFeedback->windowFlags() & ~Qt::WindowCloseButtonHint);
    myProgressBarFeedback->setAttribute(Qt::WA_DeleteOnClose);
    myProgressBarFeedback->setMinimumDuration(0);
    myProgressBarFeedback->show();
    QCoreApplication::processEvents();
}

void SaveFeedbackComponent::EndSaveOpenFeedback()
{
    Q_ASSERT(myProgressBarFeedback != nullptr);
    myProgressBarFeedback->close();
    myProgressBarFeedback = nullptr;
    // delete handled by Qt::WA_DeleteOnClose attribute
}

void SaveFeedbackComponent::SetSaveOpenProgressFeedback(int _index, int _count, int _stepIndex, int _stepCount, const QString& _stepObjects)
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    Q_ASSERT(_count != 0);
    int progress = (_stepIndex * 100 / _stepCount) + ((100 * _index) / (_stepCount * _count));

    static QString textTemplate = QString("%3 (%1/%2)");
    QString labelText = textTemplate.arg(_index+1).arg(_count < 0 ? '?' : _count).arg(_stepObjects);
    myProgressBarFeedback->setLabelText(labelText);
    myProgressBarFeedback->setValue(progress);
#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetSaveLanguageProgress(int _languageCount)
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    int progress = (1 * 100 / SAVE_STEP_COUNT);

    static QString textTemplate = QString("Saving %1 languages...");
    QString labelText = textTemplate.arg(_languageCount);
    myProgressBarFeedback->setLabelText(labelText);
    myProgressBarFeedback->setValue(progress);

#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetSaveStringTableProgress(int tableIndex, int tableCount)
{
    SetSaveOpenProgressFeedback(tableIndex, tableCount, 1, SAVE_STEP_COUNT, "Saving String Tables");
}
void SaveFeedbackComponent::SetSaveEnumProgress(int enumIndex, int enumCount)
{
    SetSaveOpenProgressFeedback(enumIndex, enumCount, 2, SAVE_STEP_COUNT, "Saving Enumerators");
}
void SaveFeedbackComponent::SetSaveStructTemplateProgress(int structIndex, int structCount)
{
    SetSaveOpenProgressFeedback(structIndex, structCount, 3, SAVE_STEP_COUNT, "Saving Structure Templates");
}
void SaveFeedbackComponent::SetSaveStructDataProgress(int structIndex, int structCount)
{
    SetSaveOpenProgressFeedback(structIndex, structCount, 4, SAVE_STEP_COUNT, "Saving Structure Data");
}
void SaveFeedbackComponent::SetSaveProjectProgress()
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    int progress = (6 * 100 / SAVE_STEP_COUNT);

    static QString text = QString("Saving Project properties...");
    myProgressBarFeedback->setLabelText(text);
    myProgressBarFeedback->setValue(progress);

#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetSaveCompilationProgress(int fileIndex, int fileCount)
{
    SetSaveOpenProgressFeedback(fileIndex, fileCount, 6, SAVE_STEP_COUNT, "Compiling temporary files");
}


void SaveFeedbackComponent::SetOpenDecompilationProgress(int fileIndex, int fileCount)
{
    SetSaveOpenProgressFeedback(fileIndex, fileCount, 0, OPEN_STEP_COUNT, "Reading save file parts");
}
void SaveFeedbackComponent::SetOpenProjectProgress()
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    int progress = (2*100 / OPEN_STEP_COUNT);

    static QString text = QString("Loading Project properties...");
    myProgressBarFeedback->setLabelText(text);
    myProgressBarFeedback->setValue(progress);

#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetOpenLanguageProgress()
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    int progress = (3*100 / OPEN_STEP_COUNT);

    static QString text = QString("Loading Languages...");
    myProgressBarFeedback->setLabelText(text);
    myProgressBarFeedback->setValue(progress);

#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetOpenStringTableProgress(int tableIndex, int tableCount)
{
    SetSaveOpenProgressFeedback(tableIndex, tableCount, 3, OPEN_STEP_COUNT, "Loading String Tables");
}
void SaveFeedbackComponent::SetOpenEnumProgress()
{
    if (myProgressBarFeedback == nullptr)
    {
        return;
    }

    int progress = (5*100 / OPEN_STEP_COUNT);

    static QString text = QString("Loading Enumerators...");
    myProgressBarFeedback->setLabelText(text);
    myProgressBarFeedback->setValue(progress);

#ifdef WITH_THREAD_SLEEP
    QThread::msleep(250);
#endif
}
void SaveFeedbackComponent::SetOpenStructTemplateProgress(int structIndex, int structCount)
{
    SetSaveOpenProgressFeedback(structIndex, structCount, 5, OPEN_STEP_COUNT, "Loading Structure Templates");
}
void SaveFeedbackComponent::SetOpenStructTemplateDefaultProgress(int structIndex, int structCount)
{
    SetSaveOpenProgressFeedback(structIndex, structCount, 6, OPEN_STEP_COUNT, "Loading Structure default values");
}
void SaveFeedbackComponent::SetOpenStructDataProgress(int structIndex, int structCount)
{
    SetSaveOpenProgressFeedback(structIndex, structCount, 7, OPEN_STEP_COUNT, "Loading Structure Data");
}
