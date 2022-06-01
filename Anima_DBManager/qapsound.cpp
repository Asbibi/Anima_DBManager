#include "qapsound.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QAudioDevice>

float QAPSound::ourVolume = 1;

QAPSound::QAPSound(QWidget *parent) :
    QAssetPreview(parent)
{
    auto* vLayout = layout();

    QWidget* audioWidget = new QWidget();
    QHBoxLayout* audioLayout = new QHBoxLayout();

    QSize btnSize = QSize(80,40);

    myPlayBtn = new QPushButton("Play");
    myPlayBtn->setMinimumSize(btnSize);
    myPlayBtn->setMaximumSize(btnSize);
    QObject::connect(myPlayBtn, &QPushButton::clicked, this, &QAPSound::PlaySoundMedia);

    myStopBtn = new QPushButton("Stop");
    myStopBtn->setMinimumSize(btnSize);
    myStopBtn->setMaximumSize(btnSize);
    myStopBtn->hide();
    QObject::connect(myStopBtn, &QPushButton::clicked, this, &QAPSound::StopSoundMedia);

    myDurationLabel = new QLabel();

    QSlider* volumeSlider = new QSlider(Qt::Orientation::Horizontal);
    volumeSlider->setMaximum(100);
    volumeSlider->setValue(ourVolume * 100);
    QObject::connect(volumeSlider, &QSlider::valueChanged, this, &QAPSound::VolumeChanged);

    audioLayout->addWidget(myPlayBtn);
    audioLayout->addWidget(myStopBtn);
    audioLayout->addItem(new QSpacerItem(10,3));
    audioLayout->addWidget(myDurationLabel);
    audioLayout->addItem(new QSpacerItem(10,3));
    audioLayout->addWidget(volumeSlider);
    audioWidget->setLayout(audioLayout);
    vLayout->addWidget(audioWidget);

    mySound = new QMediaPlayer();
    myOuput = new QAudioOutput();
    myOuput->setVolume(ourVolume);
    mySound->setAudioOutput(myOuput);
    QObject::connect(mySound, &QMediaPlayer::durationChanged, this, &QAPSound::SoundDurationChanged);
}


QAPSound::~QAPSound()
{
    mySound->stop();
    delete mySound;
}



void QAPSound::SetFilePath(const QString& _filePath)
{
    QAssetPreview::SetFilePath(_filePath);
    mySound->setSource(QUrl::fromLocalFile(_filePath));
}

void QAPSound::PlaySoundMedia()
{
    mySound->play();

    myPlayBtn->hide();
    myStopBtn->show();
}

void QAPSound::StopSoundMedia()
{
    mySound->stop();

    myStopBtn->hide();
    myPlayBtn->show();
}

void QAPSound::SoundDurationChanged()
{
    myDurationLabel->setText(QString("%1 sec").arg(mySound->duration() / 1000));
}

void QAPSound::VolumeChanged(int value)
{
    ourVolume = ((float)value)/100;
    myOuput->setVolume(ourVolume);
}
