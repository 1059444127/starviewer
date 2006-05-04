/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGPATIENT_H
#define UDGPATIENT_H

#include <QObject>
#include <QString>
#include <QDate>
#include <list>
#include "patientstudy.h"

namespace udg {

/**
Classe que representa la unitat central de treball de l'aplicaci�: el Pacient. Guardar� tota la informaci� relacionada amb aquests ( Estudis, S�ries , etc )

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class Patient : public QObject
{
Q_OBJECT
public:
    Patient( QObject *parent = 0 );

    ~Patient();

    /// Assigna/Obt� nom del pacient
    void setName( const char *name );
    void setName( QString name );
    QString getName();

    /// Assigna/Obt� cognom del pacient
    void setSurname( const char *surname );
    void setSurname( QString surname );
    QString getSurname();
    
    /// Assigna/Obt� data de naixement
    void setDateOfBirth( int day , int month , int year );
    QString getDateOfBirth();
    int getDayOfBirth();
    int getMonthOfBirth();
    int getYearOfBirth();

    /// Obt� edat del pacient
    int getAge();

    /// Assigna/Obt� sexe del pacient
    inline char getSex() const { return m_sex; };
    
    /// Assignar/Obtenir el pes del pacient
    void setWeight( double weight );
    double getWeight() const { return m_weight; };

    /// Assignar/Obtenir l'acl�ada del pacient
    void setHeight( double height );
    double getHeight() const { return m_height; };
    
private:
    /// Identificador de pacient \TODO equival al tag DICOM ???? 
    QString m_patientID;

    /// Nom del pacient
    QString m_name;

    /// Cognom del pacient
    QString m_surname;

    /// Data de naixement
    QDate m_dateOfBirth;

    /// Sexe
    char m_sex;

    /// Pes ( en Kg. ) del pacient
    double m_weight;

    /// Al�ada ( en metres ) del pacient
    double m_height;

    /// Llista d'estudis \TODO aix� podria ser un map o un hash, per determinar
    std::list< PatientStudy > m_studiesList;
};

}

#endif
