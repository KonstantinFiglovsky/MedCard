#include "form2.h"
#include "ui_form2.h"
#include <QList>
#include <QComboBox>

Form2::Form2(const visit_t& current_visit,QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Form2)
{
  ui->setupUi(this);
  ui->textBrowser_2->setText(current_visit.manipulations);
  QList<QComboBox*> combo_boxes = findChildren<QComboBox *>();
  for (int i =0; i < 64; ++i){
    combo_boxes[i]->setCurrentIndex(current_visit.toothCard[i]);
  }
  ui->label_2->setText(current_visit.date.toString("dd.MM.yyyy"));
}

Form2::~Form2(){
  delete ui;
}
