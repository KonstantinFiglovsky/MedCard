#include "form.h"
#include "ui_form.h"
#include <QPrinter>
#include <QPrintDialog>

Form::Form(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Form)
{
  ui->setupUi(this);
}

Form::~Form(){
  delete ui;
}

void Form::find(QSharedPointer<patient_t> patient){
  ui->textEdit->find("                   20");
  ui->textEdit->textCursor().insertText(patient->date.toString("dd.MM.yyyy"));
  ui->textEdit->find("$FIO$");
  ui->textEdit->textCursor().insertText(patient->name);
  ui->textEdit->find("$BIRTHDAY$");
  ui->textEdit->textCursor().insertText(patient->birthday.toString("dd.MM.yyyy"));
  ui->textEdit->find("$S$");
  ui->textEdit->textCursor().insertText(patient->pol);
  ui->textEdit->find("$PHONE$");
  ui->textEdit->textCursor().insertText(patient->mobile_number);
  ui->textEdit->find("$ADRES$");
  ui->textEdit->textCursor().insertText(patient->adress);
  ui->textEdit->find("$ALLR$");
  ui->textEdit->textCursor().insertText(patient->allergia);
}

QTextDocument* Form::getText(){
  return ui->textEdit->document();
}
