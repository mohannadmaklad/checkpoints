#include "addSequenceDialog.h"
#include "checkpointsPlugin.h"
#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QVariant>

chpsForm::chpsForm(CheckpointsPlugin *plugin, QWidget *parent)
    : QWidget(parent), ui(new Ui::CheckpointsForm), plugin_(plugin),
      dialog_(new ChpDialogForm(plugin_, parent)) {
  ui->setupUi(this);

  QAction *addAction = new QAction(QIcon(":/png/icon-add.png"), tr(""), this);
  ui->infoLabel->addAction(addAction);
  ui->chpsResults->addAction(addAction);

  QObject::connect(ui->chpsResults,
                   SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
                   SLOT(jumpToMsg(QListWidgetItem *)));

  menu = new QMenu(ui->chpsResults);
  auto deleteAction = new QAction("Delete Sequence");
  menu->addAction(deleteAction);

  connect(deleteAction, &QAction::triggered, this, [&]() {
    delete ui->chpsResults->takeTopLevelItem(
        ui->chpsResults->indexOfTopLevelItem(selectedItem));
  });
}

chpsForm::~chpsForm() { delete ui; }

void chpsForm::displayInfo(QString s) { ui->infoLabel->setText(s); }

QTreeWidgetItem *chpsForm::addListItem(QString s, int seq_id) {
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->chpsResults);
  item->setText(1, s);
  item->setIcon(0, QIcon(":/png/icon-unknown"));
  item->setData(1, Qt::UserRole, seq_id);
  return item;
}

QTreeWidgetItem *chpsForm::createListItem(QString s, int seq_id) {
  QTreeWidgetItem *item = new QTreeWidgetItem();
  item->setText(1, s);
  item->setIcon(0, QIcon(":/png/icon-unknown"));
  item->setData(1, Qt::UserRole, seq_id);
  return item;
}

QTreeWidgetItem *chpsForm::addSublistItem(QString s, QTreeWidgetItem *parent) {
  QTreeWidgetItem *item = new QTreeWidgetItem(parent);
  item->setText(3, s);
  item->setIcon(0, QIcon(":/png/icon-unknown"));

  return item;
}

QTreeWidgetItem *chpsForm::getSublistItem(int seq_id) {
  for (auto i = 0; i < ui->chpsResults->topLevelItemCount(); i++) {
    QTreeWidgetItem *cur_item = ui->chpsResults->topLevelItem(i);
    int cur_item_id = cur_item->data(1, Qt::UserRole).toInt();
    if (cur_item_id == seq_id) {
      return cur_item;
    }
  }

  return nullptr;
}

void chpsForm::updateSublistItem(int seq_id, QTreeWidgetItem *new_item) {
  QTreeWidgetItem *item = getSublistItem(seq_id);
  if (item == nullptr) {
    displayInfo("Couldn't update the item");
    return;
  }

  int item_index = ui->chpsResults->indexOfTopLevelItem(item);
  new_item->setData(1, Qt::UserRole, item->data(1, Qt::UserRole));

  // remove the old item
  delete ui->chpsResults->takeTopLevelItem(item_index);
  // add the new item
  ui->chpsResults->insertTopLevelItem(item_index, new_item);
  ui->chpsResults->update();
}

void chpsForm::jumpToMsg(QTreeWidgetItem *msg) {
  QVariant index = msg->data(2, Qt::UserRole);
  if (index == -1) {
    return;
  } else {
    plugin_->jumpToMsg(index.toInt());
  }
}

void chpsForm::on_add_chp_btn_clicked() {
  dialog_->editMode = false;
  dialog_->showDialog();
}

void chpsForm::on_exportBtn_clicked() { plugin_->exportSequences(); }

void chpsForm::on_chpsResults_itemDoubleClicked(QTreeWidgetItem *item, int) {
  if (ui->chpsResults->indexOfTopLevelItem(item) != -1) {
    dialog_->editMode = true;
    Sequence *s = plugin_->getSequence(item->data(1, Qt::UserRole).toInt());
    dialog_->showDialog(s);
  } else {
    jumpToMsg(item);
  }
}

void chpsForm::on_importBtn_clicked() { plugin_->importSequences(); }

void chpsForm::on_clearBtn_clicked() {
  plugin_->clear();
  ui->chpsResults->clear();
  dialog_->clear();
}

void chpsForm::on_verifyBtn_clicked() {
  const auto seq_count = ui->chpsResults->topLevelItemCount();
  for (auto i = 0; i < seq_count; i++) {
    bool seqFailed = false;
    QTreeWidgetItem *topItem = ui->chpsResults->topLevelItem(i);

    const auto chkpnts_count = topItem->childCount();
    for (auto j = 0; j < chkpnts_count; j++) {
      const QString search_text = topItem->child(j)->text(3);
      const int msg_id = plugin_->findMessage(search_text);
      if (msg_id != -1) {
        topItem->child(j)->setIcon(0, QIcon(":/png/icon-ok"));
        topItem->child(j)->setText(2, QString::number(msg_id));
        topItem->child(j)->setData(2, Qt::UserRole, msg_id);
      } else {
        topItem->child(j)->setIcon(0, QIcon(":/png/icon-bad"));
        topItem->child(j)->setData(2, Qt::UserRole, -1);
        seqFailed = true;
      }
    }

    if (seqFailed) {
      topItem->setIcon(0, QIcon(":/png/icon-bad"));
    } else {
      topItem->setIcon(0, QIcon(":/png/icon-ok"));
    }
  }
}

void chpsForm::contextMenuEvent(QContextMenuEvent *event) {
  selectedItem = ui->chpsResults->itemAt(
      ui->chpsResults->viewport()->mapFromGlobal(event->globalPos()));

  if (ui->chpsResults->indexOfTopLevelItem(selectedItem) != -1) {
    menu->exec(event->globalPos());
  }
}
