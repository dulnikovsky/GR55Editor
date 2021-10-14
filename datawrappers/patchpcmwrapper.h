#ifndef PATCHPCMWRAPPER_H
#define PATCHPCMWRAPPER_H

#include "patchdatawrapperbase.h"
#include <QStringListModel>

class PatchPcmWrapper: public PatchDataWrapperBase
{
    Q_OBJECT

    Q_PROPERTY(QStringList toneList READ ToneList CONSTANT)

    Q_PROPERTY(int waveform READ getWaveform WRITE setWaveform NOTIFY waveformChanged)
    Q_PROPERTY(bool toneswitch READ getToneSwitch WRITE setToneSwitch NOTIFY toneswitchChanged)
    Q_PROPERTY(int level READ getLevel WRITE setLevel NOTIFY levelChanged)

public:

    enum Offsets
    {
        Waveform  = 0x01,
        ToneSwitch = 0x03,
        ToneLevel = 0x04
    };

    explicit PatchPcmWrapper(QObject *parent = nullptr) : PatchDataWrapperBase(parent) {}

    static QStringList ToneList() { return toneNameList;}

    int getWaveform() const;
    void setWaveform(int val);

    int getLevel() const;
    void setLevel(int val);

    bool getToneSwitch() const;
    void setToneSwitch(bool toneswitch);

public slots:

signals:
    void waveformChanged(int);
    void levelChanged(int);
    void toneswitchChanged(bool);

private slots:
    void onDataChanged(unsigned int offset, int lenght, bool byUserControl) override;

private:
    static const QStringList toneNameList;
};

#endif // PATCHPCMWRAPPER_H
