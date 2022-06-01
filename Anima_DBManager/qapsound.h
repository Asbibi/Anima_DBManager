#ifndef QAPSOUND_H
#define QAPSOUND_H

#include "qassetpreview.h"

#include <QMediaPlayer>
#include <QAudioOutput>

#include <QLabel>
#include <QPushButton>

class QAPSound : public QAssetPreview
{
    Q_OBJECT

private:
    QAudioOutput* myOuput;
    QMediaPlayer* mySound;

    QLabel* myDurationLabel;
    QPushButton* myPlayBtn;
    QPushButton* myStopBtn;

    static float ourVolume;

public:
    explicit QAPSound(QWidget *parent = nullptr);
    ~QAPSound();

public slots:
    virtual void SetFilePath(const QString& _filePath) override;
    void PlaySoundMedia();
    void StopSoundMedia();
    void SoundDurationChanged();
    void VolumeChanged(int value);
};

#endif // QAPSOUND_H
