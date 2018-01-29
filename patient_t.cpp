#include "patient_t.h"
#include <QSharedPointer>

QDataStream& operator >>(QDataStream& in, patient_t& patient){
  in >> patient.card_number;
  in >> patient.name;
  in >> patient.mobile_number;
  in >> patient.pol;
  in >> patient.date;
  in >> patient.birthday;
  in >> patient.adress;
  in >> patient.allergia;
  in >> patient.visits;
  return in;
}

QDataStream& operator <<(QDataStream& out,const patient_t& patient){
  out << patient.card_number;
  out << patient.name;
  out << patient.mobile_number;
  out << patient.pol;
  out << patient.date;
  out << patient.birthday;
  out << patient.adress;
  out << patient.allergia;
  out << patient.visits;
  return out;
}

QDataStream &operator >>(QDataStream &in, visit_t& visit){
  in >> visit.date;
  in >> visit.manipulations;
  in >> visit.toothCard;
  return in;
}

QDataStream &operator <<(QDataStream &out, const visit_t &visit){
  out << visit.date;
  out << visit.manipulations;
  out << visit.toothCard;
  return out;
}
