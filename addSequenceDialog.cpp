#include "addSequenceDialog.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <memory.h>
#include "checkpointsPlugin.h"

ChpDialogForm::ChpDialogForm(CheckpointsPlugin* plugin, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::addSequenceForm),
    plugin_(plugin),
    selectedItem(nullptr)
{
    ui->setupUi(this);

    menu = new QMenu(ui->prevList);
    auto deleteAction = new QAction("Delete Message");
    menu->addAction(deleteAction);

    connect(deleteAction, &QAction::triggered,this, [&](){
        ui->prevList->removeItemWidget(selectedItem);
        delete selectedItem;
    });
}

ChpDialogForm::~ChpDialogForm(){
    delete ui;
}

void ChpDialogForm::showDialog()
{
    if(editMode){
        setWindowTitle("Edit Sequence");
    } else {
        setWindowTitle("Add Sequence");
    }

    this->show();
}

void ChpDialogForm::showDialog(Sequence *s)
{
    if(editMode){
        setWindowTitle("Edit Sequence");
    } else {
        setWindowTitle("Add Sequence");
    }

    clear();
    showSequence(s);
    show();
}

void ChpDialogForm::clear()
{
    ui->seqName->clear();
    ui->payload->clear();
    ui->prevList->clear();
}


void ChpDialogForm::on_buttonBox_accepted()
{
    Sequence* s = new Sequence{ui->seqName->text()};

    for(auto i=0; i<ui->prevList->count();i++){
        s->sequence.append(ui->prevList->item(i)->text());
    }

    if(editMode){
        plugin_->updateSequenceByName(s);
    } else {
        plugin_->addSequence(s);
    }
    ui->prevList->clear();
    ui->seqName->clear();
    this->hide();
}


void ChpDialogForm::on_buttonBox_rejected()
{
    // @TODO: add temp list
    this->hide();
    ui->prevList->clear();
    ui->seqName->clear();
    ui->payload->clear();
}


void ChpDialogForm::on_addBtn_clicked()
{
    const QString payload = ui->payload->text();
    if(payload.isEmpty())
    {
        // @TODO display error message
        return;
    }
    ui->prevList->addItem(payload);
    ui->payload->clear();
}


void ChpDialogForm::on_prevList_itemActivated(QListWidgetItem *item)
{
    ui->payload->setText(item->text());
}


void ChpDialogForm::contextMenuEvent(QContextMenuEvent *event)
{
    selectedItem = ui->prevList->itemAt(ui->prevList->viewport()->mapFromGlobal(event->globalPos()));

    if(selectedItem){
        menu->exec(event->globalPos());
    }
}


void ChpDialogForm::on_prevList_itemClicked(QListWidgetItem *item)
{
    ui->payload->setText(item->text());
}

void ChpDialogForm::showSequence(Sequence *s)
{
    ui->seqName->setText(s->name);
    ui->payload->setText("");
    std::for_each(s->sequence.begin(), s->sequence.end(), [&](const QString& seq){
        ui->prevList->addItem(seq);
    });
    ui->payload->clear();

}

