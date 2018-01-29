#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QRect>
#include <QDesktopWidget>
#include <QPrinter>
#include <QPrintDialog>
#include "form.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  patients_(),
  number_of_patients_(0)
{
  QString fileName("saves.mk");
  QFile file(fileName);
  file.close();
  if (!file.exists()) {
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Book"), "",
                                            tr("medical cards (*.abk)"));
    file.setFileName(fileName);
  }
  if (!fileName.isEmpty()) {
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::information(this, tr("Unable to open file"), file.errorString());
      return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_7);
    patients_.empty();
    patients_.clear();
    quint32 n;
    in >> n;
    patients_.detach();
    for (quint32 i = 0; i < n; ++i) {
      if (in.status() != QDataStream::Ok){
        break;
      }
      QString key;
      patient_t value;
      in >> key >> value;
      QSharedPointer<patient_t> valueptr(new patient_t(value));
      patients_.insertMulti(key, valueptr);
    }
    in >> number_of_patients_;
    file.close();
  }
  connect(this,&MainWindow::saveData,this,&MainWindow::on_saveData);
  ui->setupUi(this);
  for (auto i = patients_.begin(); i != patients_.end(); ++i){
    ui->pacient_list->addItem(i.key());
  }
  if (!patients_.empty()){
    ui->pacient_list->setCurrentRow(0);
    ui->pacient_list->setFocus();
  }
  QSharedPointer<patient_t> newpatient(new patient_t{});
  newpatient->visits.push_back({QDate(1,1,1),{},{}}); patients_.insert("Новый пациент",newpatient); client_card_ = new ClientCard();
  connect(client_card_,&ClientCard::openMainWindow, this, &MainWindow::show);
  connect(this,&MainWindow::openCard,client_card_,&ClientCard::getPatient);
  connect(client_card_,&ClientCard::saveData,this,&MainWindow::on_saveData);
  QRect rect = frameGeometry();
  rect.moveCenter(QDesktopWidget().availableGeometry().center());
  move(rect.topLeft()); client_card_->move(rect.topLeft());
}

MainWindow::~MainWindow()
{
  QString fileName = "saves.mk";
  QFile file(fileName);
  patients_.remove(QString("Новый пациент"));
  if (!file.exists()) {
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save Book"), "",
                                            tr("Medical cards (*.mk)"));
  }
  if (!fileName.isEmpty()) {
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
      QMessageBox::information(this, tr("Unable to open file"),
                               file.errorString());
      delete ui;
      delete client_card_;
      return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint32(patients_.size());
    typename QMap<QString, QSharedPointer<patient_t>>::ConstIterator it = patients_.end();
    typename QMap<QString, QSharedPointer<patient_t>>::ConstIterator begin = patients_.begin();
    while (it != begin) {
      --it;
      patient_t value = *it.value();
      out << it.key() << value;
    }
    out << number_of_patients_;
    file.close();
  }
  delete ui;
  delete client_card_;
}

void MainWindow::on_saveData(QString fileName)
{
  QFile file(fileName);
  if (!file.exists()) {
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save Book"), "",
                                            tr("Medical cards (*.mk)"));
  }
  if (!fileName.isEmpty()) {
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
      QMessageBox::information(this, tr("Unable to open file"),
                               file.errorString());
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint32(patients_.size());
    typename QMap<QString, QSharedPointer<patient_t>>::ConstIterator it = patients_.end()-1;
    typename QMap<QString, QSharedPointer<patient_t>>::ConstIterator begin = patients_.begin();
    while (it != begin) {
      --it;
      patient_t value = *it.value();
      out << it.key() << value;
    }
    out << number_of_patients_;
    file.close();
  }
}


void MainWindow::on_find_line_textChanged(const QString &arg1)
{
  QString arg2(". "+arg1);
  auto found = ui->pacient_list->findItems(arg2, Qt::MatchContains);
  if (!found.empty()){
    ui->pacient_list->setCurrentItem(found.front());
  }
}

void MainWindow::on_pacient_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
  if (current != 0){
    auto patient = *patients_.find(current->text());
    if (patient->card_number != "") ui->cardNumber->setText(patient->card_number);
    ui->adres->setText(patient->adress);
    ui->birthday->setDate(patient->birthday);
    ui->fio->setText(patient->name);
    ui->date->setDate(patient->date);
    ui->mobile->setText(patient->mobile_number);
    if (patient->pol == "М"){
      ui->pol->setCurrentIndex(0);
    } else {
      ui->pol->setCurrentIndex(1);
    }
    ui->allergies->setText(patient->allergia);
    if (!patient->visits.empty()){
      ui->last_visit->setDate(patient->visits.back().date);
    }
  }
  ui->newPacientButton->setDisabled(false);
  ui->openButton->setDisabled(false);
}

void MainWindow::on_saveButton_clicked()
{
  if (ui->pacient_list->currentRow() != -1){
    if (ui->pacient_list->currentItem()->text() != "Новый пациент"){
      auto iterator = patients_.find(ui->pacient_list->currentItem()->text());
      auto patient = iterator.value();
      patients_.erase(iterator);
      patient->card_number = ui->cardNumber->text();
      patient->name = ui->fio->text();
      patient->mobile_number = ui->mobile->text();
      patient->pol = ui->pol->currentText();
      patient->date = ui->date->date();
      patient->birthday = ui->birthday->date();
      patient->adress = ui->adres->text();
      patient->allergia = ui->allergies->text();
      patients_.insert(patient->card_number + ". " + patient->name,patient);
    } else {
      QSharedPointer<patient_t> patient(new patient_t{ui->cardNumber->text(),ui->fio->text(),
                                                      ui->mobile->text(),ui->pol->currentText(),
                                                      ui->date->date(),ui->birthday->date(),
                                                      ui->adres->text(),ui->allergies->text(),{{ui->date->date(),{},{}}}});
      patient->visits[0].toothCard.insert(0,64,0);
      patient->visits[0].manipulations = HTML_TEMPLATE;
      patients_.insert(patient->card_number + ". " + patient->name,patient);
      ui->newPacientButton->setDisabled(false);
      ui->openButton->setDisabled(false);
      ui->pacient_list->setFocus();
    }
    ui->pacient_list->currentItem()->setText(ui->cardNumber->text() + ". " + ui->fio->text());
    emit saveData("saves.mk");
  }
}

void MainWindow::on_deleteButton_clicked()
{
  if (ui->pacient_list->currentRow()!= -1){
    QListWidget* list = ui->pacient_list;
    if (list->currentItem()->text() != "Новый пациент")
      patients_.erase(patients_.find(list->currentItem()->text()));
    delete list->currentItem();
  }
}

void MainWindow::on_newPacientButton_clicked()
{
  ui->date->setFocus();
  ++number_of_patients_;
  QString cardNumber(QString::number(number_of_patients_));
  ui->cardNumber->setText(cardNumber);
  ui->pacient_list->addItem("Новый пациент");
  ui->pacient_list->setCurrentRow(ui->pacient_list->count()-1);
  ui->newPacientButton->setDisabled(true);
  ui->openButton->setDisabled(true);
}

void MainWindow::on_fio_returnPressed()
{
  ui->birthday->setFocus();
}

void MainWindow::on_allergies_returnPressed()
{
  ui->mobile->setFocus();
}

void MainWindow::on_mobile_returnPressed()
{
  ui->adres->setFocus();
}

void MainWindow::on_adres_returnPressed()
{
  ui->saveButton->click();
}

void MainWindow::on_date_editingFinished()
{
  ui->fio->setFocus();
}

void MainWindow::on_birthday_editingFinished()
{
  ui->allergies->setFocus();
}

void MainWindow::on_openButton_clicked()
{
  if (ui->pacient_list->currentRow() != -1){
    emit openCard(patients_.find(ui->pacient_list->currentItem()->text()).value());
    this->close();
  }
}

void MainWindow::on_exitButton_clicked()
{
  close();
}

void MainWindow::on_pacient_list_doubleClicked(const QModelIndex &)
{
  emit ui->openButton->clicked();
}

void MainWindow::on_pushButton_clicked()
{
  if (ui->pacient_list->currentRow() != -1){
    QPrinter printer(QPrinter::HighResolution);
    Form form;
    QSharedPointer<patient_t> current_patient = patients_.find(ui->pacient_list->currentItem()->text()).value();
    form.find(current_patient);
    printer.setPageMargins(5,5,5,5, QPrinter::Millimeter);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    dlg->setWindowTitle(tr("Печать"));
    if (dlg->exec() == QDialog::Accepted)
    {
      QTextDocument* td = form.getText();
      td->print(&printer);
    }
    delete dlg;
  }
}


void MainWindow::on_save_action_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save Book"), "",
                                                  tr("medical cards (*.mk)"));
  patients_.remove(QString("Новый пациент"));
  if (!fileName.isEmpty()){
    emit saveData(fileName);
  }
}

void MainWindow::on_open_action_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open Book"), "",
                                                  tr("medical cards (*.mk)"));
  if (!fileName.isEmpty()) {

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::information(this, tr("Unable to open file"),
                               file.errorString());
      return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_7);
    patients_.empty();
    patients_.clear();
    quint32 n;
    in >> n;
    patients_.detach();
    for (quint32 i = 0; i < n; ++i) {
      if (in.status() != QDataStream::Ok)
        break;
      QString key;
      patient_t value;
      in >> key >> value;
      QSharedPointer<patient_t> valueptr(new patient_t(value));
      patients_.insertMulti(key, valueptr);
    }
    in >> number_of_patients_;
    ui->pacient_list->clear();
    for (auto i = patients_.begin(); i != patients_.end(); ++i){
      ui->pacient_list->addItem(i.key());
    }
    QSharedPointer<patient_t> newpatient(new patient_t{});
    newpatient->visits.push_back({QDate(1,1,1),{},{}});
    if (!patients_.empty()){
      ui->pacient_list->setCurrentRow(0);
      ui->pacient_list->setFocus();
    }
  }
}

