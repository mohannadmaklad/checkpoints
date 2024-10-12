#ifndef ADDSEQUENCEDIALOG_H
#define ADDSEQUENCEDIALOG_H

#include <QWidget>

#include "ui_addChpDialog.h"
#include "sequences.h"

class CheckpointsPlugin;
namespace Ui {
class addSequenceForm;
}

class ChpDialogForm: public QWidget {

    Q_OBJECT;

public:
    explicit ChpDialogForm(CheckpointsPlugin* plugin, QWidget* parent = 0);
    ~ChpDialogForm();

    void showDialog();
    void showDialog(Sequence* s);
    void clear();

public:
    bool editMode {false};

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_addBtn_clicked();
    void on_prevList_itemActivated(QListWidgetItem *item);
    void on_prevList_itemClicked(QListWidgetItem *item);

private:
    void showSequence(Sequence* s);
    void contextMenuEvent(QContextMenuEvent *event);
    Ui::addSequenceForm *ui;
    CheckpointsPlugin* plugin_;
    QMenu* menu;
    QListWidgetItem* selectedItem;
};

#endif // ADDSEQUENCEDIALOG_H
