#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSharedPointer>
#include "patient_t.h"
#include "clientcard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void openCard(QSharedPointer<patient_t> patient);
    void saveData(QString file_name);
private slots:
    void on_saveData(QString);
    void on_find_line_textChanged(const QString &arg1);
    void on_pacient_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_saveButton_clicked();
    void on_deleteButton_clicked();
    void on_newPacientButton_clicked();
    void on_fio_returnPressed();
    void on_allergies_returnPressed();
    void on_mobile_returnPressed();
    void on_adres_returnPressed();
    void on_date_editingFinished();
    void on_birthday_editingFinished();
    void on_openButton_clicked();
    void on_exitButton_clicked();
    void on_pacient_list_doubleClicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_save_action_triggered();
    void on_open_action_triggered();

private:
    Ui::MainWindow *ui;
    ClientCard *client_card_;
    QMap<QString, QSharedPointer<patient_t>> patients_;
    unsigned int number_of_patients_;
};

#endif // MAINWINDOW_H
