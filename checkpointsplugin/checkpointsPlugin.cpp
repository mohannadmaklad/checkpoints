#include "checkpointsPlugin.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <dltmessagematcher.h>
#include <qdltfile.h>
#include <qdltsettingsmanager.h>

namespace Ui {
class CheckpointsForm;
}

CheckpointsPlugin::CheckpointsPlugin() : sequences_(new Sequences) {}

CheckpointsPlugin::~CheckpointsPlugin() {}

/* QDLTPluginInterface interface */
QString CheckpointsPlugin::name() { return "Checkpoints"; }

QString CheckpointsPlugin::pluginVersion() { return "0.0.1"; }

QString CheckpointsPlugin::pluginInterfaceVersion() { return "1.0.1"; }

QString CheckpointsPlugin::description() {
  return "A simple plugin to verify that preset sequences exist in the dlt "
         "log.";
}

QString CheckpointsPlugin::error() { return errorText; }

bool CheckpointsPlugin::loadConfig([[maybe_unused]] QString filename) {
  return true;
}

bool CheckpointsPlugin::saveConfig([[maybe_unused]] QString filename) {
  return true;
}

QStringList CheckpointsPlugin::infoConfig() { return QStringList{}; }

/* QDltPluginViewerInterface */
QWidget *CheckpointsPlugin::initViewer() {
  form = new chpsForm(this);
  return form;
}

void CheckpointsPlugin::initFileStart(QDltFile *file) {
  qFile = file;
  form->displayInfo(file->getFileName());
}

void CheckpointsPlugin::initFileFinish() { form->displayInfo("Done"); }

void CheckpointsPlugin::initMsg(int, QDltMsg &) {}

void CheckpointsPlugin::initMsgDecoded(int, QDltMsg &) {}
void CheckpointsPlugin::updateFileStart() {}
void CheckpointsPlugin::updateMsg(int, QDltMsg &) {}

void CheckpointsPlugin::updateMsgDecoded(int, QDltMsg &) {}

void CheckpointsPlugin::updateFileFinish() {}

void CheckpointsPlugin::selectedIdxMsg(int, QDltMsg &) {}

void CheckpointsPlugin::selectedIdxMsgDecoded(int, QDltMsg &) {}

/* QDltPluginControlInterface */
bool CheckpointsPlugin::initControl(QDltControl *control) {
  dltControl = control;
  return true;
}

bool CheckpointsPlugin::initConnections(QStringList) { return false; }

bool CheckpointsPlugin::controlMsg(int, QDltMsg &) { return false; }

bool CheckpointsPlugin::stateChanged(int, QDltConnection::QDltConnectionState,
                                     QString) {
  return false;
}

bool CheckpointsPlugin::autoscrollStateChanged(bool) { return false; }

void CheckpointsPlugin::initMessageDecoder(QDltMessageDecoder *) {}

void CheckpointsPlugin::initMainTableView(QTableView *) {}

void CheckpointsPlugin::configurationChanged() {}

/* Plugin-Specific API */
void CheckpointsPlugin::addSequence(Sequence *s) {
  // assign it an id
  s->id = next_id++;
  // Update data
  sequences_->addSequence(s);
  // Update UI
  auto seq_item = form->addListItem(s->name, s->id);

  std::for_each(
      s->sequence.begin(), s->sequence.end(),
      [&](const QString &payload) { form->addSublistItem(payload, seq_item); });
}

void CheckpointsPlugin::updateSequenceByName(Sequence *s) {
  // Update data
  sequences_->updateSequenceByName(s);

  // Update UI
  QTreeWidgetItem *new_item = form->createListItem(s->name, s->id);
  form->updateSublistItem(s->id, new_item);
  std::for_each(
      s->sequence.begin(), s->sequence.end(),
      [&](const QString &payload) { form->addSublistItem(payload, new_item); });
}

void CheckpointsPlugin::exportSequences() {
  QJsonArray sequencesArray = sequences_->exportJson();

  QJsonObject rootObject;
  rootObject["sequences"] = sequencesArray;

  QJsonDocument jsonDoc(rootObject);

  QString file_name =
      QFileDialog::getSaveFileName(nullptr, "Save File", "", "*.json");
  if (file_name.isEmpty()) {
    form->displayInfo("No file selected for saving.");
    return;
  }

  QFile file(file_name);
  if (file.open(QFile::WriteOnly)) {
    file.write(jsonDoc.toJson());
    file.close();
    form->displayInfo("JSON file exported successfully.");
  } else {
    form->displayInfo("Failed to open file for writing.");
  }
}

void CheckpointsPlugin::importSequences() {
  QString file_name = QFileDialog::getOpenFileName(nullptr, "Open a file",
                                                   "directoryToOpen", "*.json");
  if (file_name.isEmpty()) {
    form->displayInfo("No file selected.");
    return;
  }

  QFile file(file_name);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    form->displayInfo("Failed to open file.");
    return;
  }

  QByteArray file_data = file.readAll();
  file.close();

  QJsonParseError json_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(file_data, &json_error);

  if (json_error.error != QJsonParseError::NoError) {
    form->displayInfo("Json parse error: " + json_error.errorString());
    return;
  }

  QJsonObject json_object = json_doc.object();
  QJsonArray sequences_array = json_object.value("sequences").toArray();

  // Step 5: Iterate through each sequence object and extract "name" and
  // "payloads"
  for (const QJsonValue &value : sequences_array) {
    QJsonObject sequence_object = value.toObject();

    // Extract the "name" field
    QString name = sequence_object.value("name").toString();
    // Extract the "payloads" array
    QJsonArray payloads_array = sequence_object.value("payloads").toArray();
    QStringList payloads;

    Sequence *s = new Sequence(name);
    for (const QJsonValue &payload_value : payloads_array) {
      payloads.append(payload_value.toString());
      s->addChp(payload_value.toString());
    }

    // add the sequence to data & UI
    addSequence(s);

    form->displayInfo("JSON file imported sucessfully.");
  }
}

void CheckpointsPlugin::clear() { sequences_->sequences.clear(); }

int CheckpointsPlugin::findMessage(const QString &search_msg) {
  int foundMsg = -1;
  Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;
  QByteArray buf;

  for (long int searchLine = 0; searchLine < qFile->sizeFilter();
       ++searchLine) {
    buf = qFile->getMsgFilter(searchLine);
    QDltMsg msg;
    msg.setMsg(buf);
    msg.setIndex(qFile->getMsgFilterPos(searchLine));

    if (QDltSettingsManager::getInstance()
            ->value("startup/pluginsEnabled", true)
            .toBool()) {
      pluginManager.decodeMsg(msg, true);
    }

    const QString payload = msg.toStringPayload();

    if (payload.contains(search_msg)) {
      foundMsg = searchLine;
      break;
    }
  }

  return foundMsg;
}

void CheckpointsPlugin::jumpToMsg(int index) { dltControl->jumpToMsg(index); }

Sequence *CheckpointsPlugin::getSequence(int id) {
  return sequences_->getSequence(id);
}
