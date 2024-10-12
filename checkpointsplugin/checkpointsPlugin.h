#ifndef CHECKPOINTSPLUGIN_H
#define CHECKPOINTSPLUGIN_H

#include <QMainWindow>
#include <qdltpluginmanager.h>
#include "plugininterface.h"
#include "form.h"
#include "sequences.h"

class CheckpointsPlugin : public QObject, QDLTPluginInterface, QDltPluginViewerInterface, QDltPluginControlInterface
{
    Q_OBJECT
    Q_INTERFACES(QDLTPluginInterface)
    Q_INTERFACES(QDltPluginViewerInterface)
    Q_INTERFACES(QDltPluginControlInterface)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "org.genivi.DLT.DltDbusPlugin")
#endif

public:
    CheckpointsPlugin();
    ~CheckpointsPlugin();

    /* QDLTPluginInterface interface */
    QString name();
    QString pluginVersion();
    QString pluginInterfaceVersion();
    QString description();
    QString error();
    bool loadConfig(QString filename);
    bool saveConfig(QString filename);
    QStringList infoConfig();

    /* QDltPluginViewerInterface */
    QWidget* initViewer();
    void initFileStart(QDltFile *file);
    void initFileFinish();
    void initMsg(int index, QDltMsg &msg);
    void initMsgDecoded(int index, QDltMsg &msg);
    void updateFileStart();
    void updateMsg(int index, QDltMsg &msg);
    void updateMsgDecoded(int index, QDltMsg &msg);
    void updateFileFinish();
    void selectedIdxMsg(int index, QDltMsg &msg);
    void selectedIdxMsgDecoded(int index, QDltMsg &msg);

    /* QDltPluginControlInterface */
    bool initControl(QDltControl *control);
    bool initConnections(QStringList list);
    bool controlMsg(int index, QDltMsg &msg);
    bool stateChanged(int index, QDltConnection::QDltConnectionState connectionState,QString hostname);
    bool autoscrollStateChanged(bool enabled);
    void initMessageDecoder(QDltMessageDecoder* pMessageDecoder);
    void initMainTableView(QTableView* pTableView);
    void configurationChanged();

    /* Plugin-Specific API */
    void addSequence(Sequence* s);
    void updateSequenceByName(Sequence* s);
    void clear();
    int findMessage(const QString& msg);
    void jumpToMsg(int index);
    void exportSequences();
    void importSequences();

    Sequence* getSequence(int id);
    chpsForm* form;

private:
    QDltControl *dltControl;
    QString errorText;
    Sequences* sequences_;
    int next_id {1};
    QDltFile *qFile;
    QDltPluginManager pluginManager;
};
#endif // CHECKPOINTSPLUGIN_H
