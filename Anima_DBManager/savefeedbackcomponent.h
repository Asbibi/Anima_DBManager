#ifndef SAVEFEEDBACKCOMPONENT_H
#define SAVEFEEDBACKCOMPONENT_H

#include <QProgressDialog>

class SaveFeedbackComponent
{
private:
    QProgressDialog* myProgressBarFeedback = nullptr;

    void SetSaveProgress(int _index, int _count, int _stepIndex, const QString& _stepObjects);

public:
    SaveFeedbackComponent();

    void StartSaveFeedback();
    void EndSaveFeedback();

    void SetSaveStringTableProgress(int tableIndex, int tableCount);
    void SetSaveEnumProgress(int enumIndex, int enumCount);
    void SetSaveStructTemplateProgress(int structIndex, int structCount);
    void SetSaveStructDataProgress(int structIndex, int structCount);
    void SetSaveProjectProgress();
    void SetSaveCompilationProgress(int fileIndex, int fileCount);
};

#endif // SAVEFEEDBACKCOMPONENT_H
