#include "clientcard.h"
#include "ui_clientcard.h"
#include <QInputDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include "form2.h"

ClientCard::ClientCard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::clientCard)
{
  ui->setupUi(this);
  html_text = ui->manipulations->toHtml();
  combo_boxes = findChildren<QComboBox *>();
  current_row = -1;
  item_deleted = false;
  setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );
}


ClientCard::~ClientCard(){
  delete ui;
}

void ClientCard::getPatient(QSharedPointer<patient_t> patient){
  current_patient = patient;
  ui->fio->setText(patient->name);
  ui->visits->clear();
  for (auto i = patient->visits.begin(); i!= patient->visits.end(); ++i ){
    ui->visits->addItem(i->date.toString("dd.MM.yyyy"));
  }
  ui->visits->setCurrentRow(0);
  for (int i = 0; i < 64; ++i){
    combo_boxes[i]->setCurrentIndex(current_patient->visits[0].toothCard[i]);
  }
  ui->manipulations->setHtml(current_patient->visits[0].manipulations);
  show();
}


void ClientCard::on_pushButton_clicked(){
  current_patient->visits[ui->visits->currentRow()].manipulations = ui->manipulations->toHtml();
  this->close();
  emit openMainWindow();
}

void ClientCard::on_newVisitButton_clicked(){
  bool bOk;
  QString str = QInputDialog::getText( 0, "Ввод Даты", "Дата", QLineEdit::Normal,
                                       QDate::currentDate().toString("dd.MM.yyyy"),&bOk);
  if (bOk){
    ui->visits->addItem(str);
    visit_t visit;
    visit.date = QDate::fromString(str,"dd.MM.yyyy");
    visit.manipulations = HTML_TEMPLATE;
    visit.toothCard.insert(0,64,0);
    current_patient->visits.push_back(visit);
  }
}

void ClientCard::on_saveButton_clicked(){
  if (!current_patient->visits.empty()){
    for (int i = 0; i < 64; ++i){
      current_patient->visits[current_row].toothCard[i] = combo_boxes[i]->currentIndex();
    }
    current_patient->visits[ui->visits->currentRow()].manipulations = ui->manipulations->toHtml();
    emit saveData("saves.mk");
  }
}

void ClientCard::on_visits_currentRowChanged(int nextRow){
  if (item_deleted){
    delete ui->visits->item(current_row);
    item_deleted = false;
  } else if (current_row != -1) {
    for (int i = 0; i < 64; ++i){
      current_patient->visits[current_row].toothCard[i] = combo_boxes[i]->currentIndex();
    }
    current_patient->visits[current_row].manipulations = ui->manipulations->toHtml();
  }
  if (nextRow != -1){
    for (int i = 0; i < 64; ++i){
      combo_boxes[i]->setCurrentIndex(current_patient->visits[nextRow].toothCard[i]);
    }
    ui->manipulations->setHtml(current_patient->visits[nextRow].manipulations);
  }
  current_row = nextRow;
}


void ClientCard::on_pushButton_4_clicked(){
  Form2* form = new Form2(current_patient->visits[current_row]);
  QPixmap pixmap(form->size());
  form->render(&pixmap);
  QPrinter printer(QPrinter::HighResolution);
  QPrintDialog printDialog(&printer, form);
  if (printDialog.exec()) {
    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = pixmap.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow (pixmap.rect());
    painter.drawPixmap(0, 0, pixmap);
  }
}


void ClientCard::on_deleteButton_clicked(){
  if (current_row != -1){
    current_patient->visits.remove(current_row);
    item_deleted = true;
    ui->visits->setCurrentRow(current_row-1);
  }
}

void ClientCard::on_visits_itemDoubleClicked(QListWidgetItem *item){
  bool bOk;
  QString str = QInputDialog::getText(0, "Ввод Даты", "Дата", QLineEdit::Normal,
                                      QDate::currentDate().toString("dd.MM.yyyy"), &bOk);
  if (bOk){
    current_patient->visits[current_row].date.fromString(str,"dd.MM.yyyy");
    item->setText(str);
  }
}

