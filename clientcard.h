#ifndef CLIENTCARD_H
#define CLIENTCARD_H

#include <QMainWindow>
#include <QComboBox>
#include <QListWidgetItem>
#include "patient_t.h"

namespace Ui {
class clientCard;
}

class ClientCard : public QMainWindow {
    Q_OBJECT

public:
    explicit ClientCard(QWidget *parent = 0);
    ~ClientCard();
signals:
    void openMainWindow();  // Сигнал для первого окна на открытие
    void saveData(QString);
public slots:
    void getPatient(QSharedPointer<patient_t> patient);
private slots:
    void on_pushButton_clicked();
    void on_newVisitButton_clicked();
    void on_saveButton_clicked();
    void on_visits_currentRowChanged(int currentRow);
    void on_pushButton_4_clicked();
    void on_deleteButton_clicked();
    void on_visits_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::clientCard *ui;
    QString html_text;
    QList<QComboBox *> combo_boxes;
    QSharedPointer<patient_t> current_patient;
    int current_row;
    bool item_deleted;
};

#endif // CLIENTCARD_H

