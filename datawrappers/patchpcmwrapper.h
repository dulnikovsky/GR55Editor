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
    Q_PROPERTY(int pan READ getPan WRITE setPan NOTIFY panChanged)
    Q_PROPERTY(int pitchshift READ getPitchShift WRITE setPitchShift NOTIFY pitchshiftChanged)
    Q_PROPERTY(int pitchfine READ getPitchFine WRITE setPitchFine NOTIFY pitchfineChanged)
    Q_PROPERTY(int portamento READ getPortamento WRITE setPortamento NOTIFY portamentoChanged)
    Q_PROPERTY(int portamentotime READ getPortamentoTime WRITE setPortamentoTime NOTIFY portamentotimeChanged)

public:

    enum Offsets
    {
        Waveform  = 0x01,
        ToneSwitch = 0x03,
        ToneLevel = 0x04,
        Octave = 0x05,
        Chromatic = 0x06,
        Legato = 0x07,
        Nuance = 0x08,
        Pan = 0x09,
        PitchShift = 0x0A,
        PitchFine = 0x0B,
        Portamento = 0x0C,
        PortamentoTime = 0x0D,
        TVAReleaseMode = 0x0F,
        String1Level = 0x10,
        String2Level = 0x11,
        String3Level = 0x12,
        String4Level = 0x13,
        String5Level = 0x14,
        String6Level = 0x15,
        LineRoute = 0x16
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

    int getPan() const;
    void setPan(int val);

    int getPitchShift() const;
    void setPitchShift(int val);

    int getPitchFine() const;
    void setPitchFine(int val);

    int getPortamento() const;
    void setPortamento(int val);

    int getPortamentoTime() const;
    void setPortamentoTime(int val);

public slots:

signals:
    void waveformChanged(int);
    void levelChanged(int);
    void toneswitchChanged(bool);
    void octaveChanged(int);
    void chromaticChanged(bool);
    void legatoChanged(bool);
    void nuanceChanged(bool);
    void panChanged(int);
    void pitchshiftChanged(int);
    void pitchfineChanged(int);
    void portamentoChanged(int);
    void portamentotimeChanged(int);

private slots:
    void onDataChanged(unsigned int offset, int lenght, bool byUserControl) override;

private:
    static const QStringList toneNameList;
};

#endif // PATCHPCMWRAPPER_H
