#ifndef SAVEFEEDBACKCOMPONENT_H
#define SAVEFEEDBACKCOMPONENT_H

#include <QProgressDialog>

class SaveFeedbackComponent
{
private:
    QProgressDialog* myProgressBarFeedback = nullptr;

    void SetSaveOpenProgressFeedback(int _index, int _count, int _stepIndex, int _stepCount, const QString& _stepObjects);

public:
    SaveFeedbackComponent();

    void StartSaveOpenFeedback();
    void EndSaveOpenFeedback();

    void SetSaveLanguageProgress(int _languageCount);
    void SetSaveStringTableProgress(int tableIndex, int tableCount);
    void SetSaveEnumProgress(int enumIndex, int enumCount);
    void SetSaveStructTemplateProgress(int structIndex, int structCount);
    void SetSaveStructDataProgress(int structIndex, int structCount);
    void SetSaveProjectProgress();
    void SetSaveCompilationProgress(int fileIndex, int fileCount);

    void SetOpenDecompilationProgress(int fileIndex, int fileCount);
    void SetOpenProjectProgress();
    void SetOpenLanguageProgress();
    void SetOpenStringTableProgress(int tableIndex, int tableCount);
    void SetOpenEnumProgress();
    void SetOpenStructTemplateProgress(int structIndex, int structCount);
    void SetOpenStructTemplateDefaultProgress(int structIndex, int structCount);
    void SetOpenStructDataProgress(int structIndex, int structCount);
};

#endif // SAVEFEEDBACKCOMPONENT_H
