#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTextDocument>
#include "patient_t.h"

namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void find(QSharedPointer<patient_t>);
    QTextDocument* getText();

private:
    Ui::Form *ui;
};

#endif // FORM_H
