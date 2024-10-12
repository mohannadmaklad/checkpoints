#ifndef SEQUENCES_H
#define SEQUENCES_H

#include <QVector>
#include <QString>

class QJsonArray;

struct Sequence {

    QString name{};
    int id{};
    QVector<QString> sequence{};

    Sequence(QString name): name(name) {}
    ~Sequence() = default;

    void addChp(QString s);
    void moveChp(int from_index, int to_index);
};

struct Sequences
{
    QVector<Sequence*> sequences;

    Sequences() = default;
    ~Sequences() = default;
    void addSequence(Sequence* seq);
    Sequence *getSequence(int id);
    void updateSequenceByName(Sequence *s);

    QJsonArray exportJson();
    void importJson();
};

#endif // SEQUENCES_H
