#ifndef PATIENT_T_H
#define PATIENT_T_H
#include <QString>
#include <QDataStream>
#include <QDate>
#include <QVector>

static const QString HTML_TEMPLATE = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Зуб:</span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Диагноз:</span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Лечение:</p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Цена:</span></p>\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>";

struct visit_t{
  QDate date;
  QString manipulations;
  QVector<int> toothCard;
};

struct patient_t{
    QString card_number,name,mobile_number,pol;
    QDate date,birthday;
    QString adress,allergia;
    QVector<visit_t> visits;
};

QDataStream& operator >>(QDataStream& in, patient_t& patient);

QDataStream& operator <<(QDataStream& out,const patient_t& patient);

QDataStream& operator >>(QDataStream& in, visit_t& visit);

QDataStream& operator <<(QDataStream& out,const visit_t& visit);

#endif // PATIENT_T_H
