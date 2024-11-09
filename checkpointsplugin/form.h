#ifndef FORM_H
#define FORM_H

#include <QString>
#include <QTreeWidgetItem>
#include <QWidget>

#include "addSequenceDialog.h"
#include "ui_form.h"

class CheckpointsPlugin;
namespace Ui
{
class CheckpointsForm;
}

class chpsForm : public QWidget
{

    Q_OBJECT;

  public:
    explicit chpsForm(CheckpointsPlugin* plugin, QWidget* parent = 0);
    ~chpsForm();

    void displayInfo(QString s);
    QTreeWidgetItem* addListItem(QString s, int seq_id);
    QTreeWidgetItem* createListItem(QString s, int seq_id);
    QTreeWidgetItem* addSublistItem(QString s, QTreeWidgetItem* parent);
    QTreeWidgetItem* getSublistItem(int seq_id);
    void updateSublistItem(int seq_id, QTreeWidgetItem* new_item);
  private slots:
    void on_add_chp_btn_clicked();
    void on_exportBtn_clicked();
    void jumpToMsg(QTreeWidgetItem* msg);
    void on_chpsResults_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_importBtn_clicked();
    void on_clearBtn_clicked();
    void on_verifyBtn_clicked();

  private:
    void contextMenuEvent(QContextMenuEvent* event);
    void processTopItem(QTreeWidgetItem* topItem, bool& seqFailed);

  private:
    Ui::CheckpointsForm* ui;
    CheckpointsPlugin* plugin_;
    ChpDialogForm* dialog_;
    QMenu* menu;
    QTreeWidgetItem* selectedItem;
};

#endif // FORM_H
