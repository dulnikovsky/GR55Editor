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
    Q_PROPERTY(int octave READ getOctave WRITE setOctave NOTIFY octaveChanged)
    Q_PROPERTY(bool chromatic READ getChromatic WRITE setChromatic NOTIFY chromaticChanged)
    Q_PROPERTY(bool legato READ getLegato WRITE setLegato NOTIFY legatoChanged)
    Q_PROPERTY(bool nuance READ getNuance WRITE setNuance NOTIFY nuanceChanged)

public:

    enum Offsets
    {
        Waveform  = 0x01,
        ToneSwitch = 0x03,
        ToneLevel = 0x04,
        Octave = 0x05,
        Chromatic = 0x06,
        Legato = 0x07,
        Nuance = 0x08
    };

    explicit PatchPcmWrapper(QObject *parent = nullptr) : PatchDataWrapperBase(parent) {}

    static QStringList ToneList() { return toneNameList;}

    int getWaveform() const;
    void setWaveform(int val);

    int getLevel() const;
    void setLevel(int val);

    bool getToneSwitch() const;
    void setToneSwitch(bool toneswitch);

    int getOctave() const;
    void setOctave(int val);

    bool getChromatic() const;
    void setChromatic(bool val);

    bool getLegato() const;
    void setLegato(bool val);

    bool getNuance() const;
    void setNuance(bool val);

public slots:

signals:
    void waveformChanged(int);
    void levelChanged(int);
    void toneswitchChanged(bool);
    void octaveChanged(int);
    void chromaticChanged(bool);
    void legatoChanged(bool);
    void nuanceChanged(bool);

private slots:
    void onDataChanged(unsigned int offset, int lenght, bool byUserControl) override;

private:
    static const QStringList toneNameList;
};

#endif // PATCHPCMWRAPPER_H
