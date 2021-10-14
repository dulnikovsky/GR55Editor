#ifndef PATCH_H
#define PATCH_H

#include <QObject>
#include <QMap>

class Patch : public QObject
{
    Q_OBJECT
public:

    enum class SysExDataTypeID
    {
        Common,
        PCMTone
    };

    enum class SysExDataOffset
    {
        Common = 0x00,
        PcmTone1 = 0x1000,
        PcmTone2 = 0x1080
    };

    enum class SysExDataLength
    {
        Common = 0x80+0x80+0x4d,
        PCMTone = 0x17
    };

    explicit Patch(QObject *parent = nullptr);

    bool setData(unsigned int addressOffset, const QByteArray &newDataArr, bool byUserControl);
    bool setData(unsigned int addressOffset, quint16 val, bool byUserControl);
    bool setData(unsigned int addressOffset, quint8 val, bool byUserControl);

    char * data(unsigned int addressOffset, int length);

    static QList<QPair<quint32, quint32>> getOffsetLenghtList();

signals:
    void dataChanged(unsigned int adressOffset,int length, bool byUserControl);

private:
    static unsigned int addrOffsetToDataOffset(unsigned int addrOffset);

    struct SysExDataType
    {
        SysExDataType(SysExDataTypeID i, SysExDataLength l) : id(i), length(l) {}

        SysExDataTypeID id;
        SysExDataLength length;

        bool isVariable() const { return static_cast<unsigned int>(length) == 0;}
    };

    static const QMap<SysExDataOffset, SysExDataType> dataDefMap;

    QByteArray dataArr;
};

#endif // PATCH_H
