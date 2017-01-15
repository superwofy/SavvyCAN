#ifndef DBC_CLASSES_H
#define DBC_CLASSES_H

#include <QColor>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "can_structs.h"

/*classes to encapsulate data from a DBC file. Really, the stuff of interest
  are the nodes, messages, signals, attributes, and comments.

  These things sort of form a hierarchy. Nodes send and receive messages.
  Messages are comprised of signals. Nodes, signals, and messages potentially have attribute values.
  All of them can have comments.
*/

enum DBC_SIG_VAL_TYPE
{
    UNSIGNED_INT,
    SIGNED_INT,
    SP_FLOAT,
    DP_FLOAT,
    STRING
};

enum DBC_ATTRIBUTE_VAL_TYPE
{
    HEX,
    QFLOAT,
    QSTRING,
    ENUM
};

enum DBC_ATTRIBUTE_TYPE
{
    GENERAL,
    NODE,
    MESSAGE,
    SIG
};

class DBC_ATTRIBUTE
{
public:
    QString name;
    DBC_ATTRIBUTE_VAL_TYPE valType;
    DBC_ATTRIBUTE_TYPE attrType;
    double upper, lower;
    QVariant defaultValue;
};

class DBC_ATTRIBUTE_VALUE
{
    QString attrName;
    QVariant value;
};

class DBC_VAL_ENUM_ENTRY
{
public:
    int value;
    QString descript;
};

class DBC_NODE
{
public:
    QString name;
    QString comment;
    QList<DBC_ATTRIBUTE_VALUE> attributes;
};

class DBC_MESSAGE; //forward reference so that DBC_SIGNAL can compile before we get to real definition of DBC_MESSAGE

class DBC_SIGNAL
{
public: //TODO: this is sloppy. It shouldn't all be public!
    QString name;
    int startBit;
    int signalSize;
    bool intelByteOrder; //true is obviously little endian. False is big endian
    bool isMultiplexor;
    bool isMultiplexed;
    int multiplexValue;
    DBC_SIG_VAL_TYPE valType;
    double factor;
    double bias;
    double min;
    double max;
    DBC_NODE *receiver; //it is fast to have a pointer but dangerous... Make sure to walk the whole tree and delete everything so nobody has stale references.
    DBC_MESSAGE *parentMessage;
    QString unitName;
    QString comment;
    QList<DBC_ATTRIBUTE_VALUE> attributes;
    QList<DBC_VAL_ENUM_ENTRY> valList;

    bool processAsText(const CANFrame &frame, QString &outString);
    bool processAsInt(const CANFrame &frame, int32_t &outValue);
    bool processAsDouble(const CANFrame &frame, double &outValue);
};

class DBCSignalHandler; //forward declaration to keep from having to include dbchandler.h in this file and thus create a loop

class DBC_MESSAGE
{
public:
    DBC_MESSAGE();

    uint32_t ID;
    QString name;
    QString comment;
    unsigned int len;
    DBC_NODE *sender;
    QColor bgColor;
    QColor fgColor;
    QList<DBC_ATTRIBUTE_VALUE> attributes;
    DBCSignalHandler *sigHandler;
    DBC_SIGNAL* multiplexorSignal;
};


#endif // DBC_CLASSES_H

