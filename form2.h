#ifndef FORM2_H
#define FORM2_H

#include <QWidget>
#include "patient_t.h"

namespace Ui {
class Form2;
}

class Form2 : public QWidget
{
    Q_OBJECT
public:
    explicit Form2(const visit_t& current_visit,QWidget *parent = 0);
    ~Form2();

private:
    Ui::Form2 *ui;
};

#endif // FORM2_H
