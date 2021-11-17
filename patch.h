#ifndef PATCH_H
#define PATCH_H

#include <QObject>
#include <QMap>

constexpr quint32 sevenBitToUint32( const unsigned char msb, const unsigned char mcleft, const unsigned char mcright, const unsigned char lsb)
{
    return (msb<<(24-3)) | (mcleft<<(16-2)) | (mcright<<(8-1)) | (lsb<<0);
}

class Patch : public QObject
{
    Q_OBJECT
public:

    enum class SysExDataTypeID
    {
        Common,
        MFX,
        ChorDlyRevEq,
        Preamp,
        Modeling,
        PCMTone,
        PCMFilter
    };

    enum class SysExDataOffset
    {
        Common = 0x00,
        MFX =           sevenBitToUint32( 0x00, 0x00, 0x03, 0x00),
        ChorDlyRevEq =  sevenBitToUint32( 0x00, 0x00, 0x06, 0x00),
        Preamp =        sevenBitToUint32( 0x00, 0x00, 0x07, 0x00),
        Modeling= sevenBitToUint32( 0x00, 0x00, 0x10, 0x00),
        PcmTone1 =      sevenBitToUint32( 0x00, 0x00, 0x20, 0x00),
        PcmTone2 =      sevenBitToUint32( 0x00, 0x00, 0x21, 0x00),
        PcmFilter1 =    sevenBitToUint32( 0x00, 0x00, 0x30, 0x00),
        PcmFilter2 =    sevenBitToUint32( 0x00, 0x00, 0x31, 0x00)
    };

    enum class SysExDataLength
    {
        Common = 0x80+0x80+0x4d,
        MFX = 0x80+0x72,
        ChorDlyRevEq = 0x1E,
        Preamp = 0x5D,
        Modeling = 0x80 + 0x4A,
        PCMTone = 0x17,
        PCMFilter = 0x34
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
