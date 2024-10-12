#include "sequences.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileDialog>

void Sequence::addChp(QString s)
{
    sequence.append(s);
}

void Sequence::moveChp(int from_index, int to_index)
{
    sequence.move(from_index, to_index);
}

void Sequences::addSequence(Sequence *seq)
{
    sequences.append(seq);
}

Sequence *Sequences::getSequence(int id)
{
    auto sequence = std::find_if(sequences.begin(), sequences.end(),[&](const Sequence* s){
        return s->id == id;
    });

    if(sequence != sequences.end()){
        return *sequence;
    } else {
        return nullptr;
    }
}

void Sequences::updateSequenceByName(Sequence *s)
{
    QString name = s->name;
    std::for_each(sequences.begin(), sequences.end(),[&](Sequence* seq){
        if(seq->name == name){
            s->id = seq->id;
            *seq = *s;
            //TODO: use anoher approach to break the loop
        }
    });
}


QJsonArray Sequences::exportJson()
{
    QJsonArray sequencesArray;

    for (const Sequence* seq : sequences) {
        QJsonObject sequenceObject;
        sequenceObject["name"] = seq->name;

        QJsonArray payloadsArray;
        for (const QString &payload : seq->sequence) {
            payloadsArray.append(payload);
        }

        sequenceObject["payloads"] = payloadsArray;
        sequencesArray.append(sequenceObject);
    }

    return sequencesArray;
}

void Sequences::importJson()
{

}
