/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGPATIENTSERIE_H
#define UDGPATIENTSERIE_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include "identifier.h"

namespace udg {

/**
Classe que encapsula la s�rie d'un pacient.

La classe cont� tot tipu d'informaci� relacionada amb la s�rie d'un pacient. Una s�rie equival a un volum per tant tindrem l'identificador del corresponent volum al repositori. Si l'id del volum �s nul voldr� dir que no est� carregat a mem�ria, en cas contrari estar� al repositori

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class PatientSerie : public QObject
{
Q_OBJECT
public:
    PatientSerie(QObject *parent = 0);

    ~PatientSerie();

    /// Assignar/Obtenir l'identificador universal de la s�rie
    void setUID( const char *uid );
    QString getUID() const { return m_serieUID; };

    /// Assignar/Obtenir l'identificador de la s�rie
    void setID( const char *id );
    QString getID() const { return m_serieID; };

    /// Assignar/Obtenir l'orientaci� del pacient
    void setPatientOrientation( const char *orientation );
    QString getPatientOrientation() const { return m_patientOrientation; };

    /// Assignar/Obtenir la modalitat de la s�rie
    void setModality( const char *modality );
    QString getModality() const { return m_modality; };

    /// Assignar/Obtenir la descripci� de la s�rie
    void setDescription( const char *description );
    QString getDescription() const { return m_description; };

    /// Assignar/Obtenir el protocol de la s�rie
    void setProtocolName( const char *protocolName );
    QString getProtocolName() const { return m_protocolName; };

    /// Assignar/Obtenir la data i hora d'adquisici� de la s�rie en format DD/MM/AAAA HH:MM. Retorna fals si hi ha algun error en el format
    bool setDateTime( int day , int month , int year , int hour , int minute );
    bool setDateTime( QString date , QString time );
    bool setDateTime( QString dateTime );
    QDateTime getDateTime() const { return m_dateTime; };
    QString getDateTimeAsString();
    bool setDate( int day , int month , int year );
    bool setDate( QString date );
    bool setTime( int hour , int minute );
    bool setTime( QString time );
    QDate getDate();
    QString getDateAsString();
    QTime getTime();
    QString getTimeAsString();

    /// Assignar/Obtenir nivells de contrast
    void setWindowLevel( double window , double level );
    double *getWindowLevel();
    void setWindow( double window );
    double getWindow() const { return m_window; }
    void setLevel( double level );
    double getLevel() const { return m_level; }

    /// Assignar/Obtenir dimensions de la s�rie
    void setDimensions( double dimensions[3] );
    void setDimensions( double x , double y , double z );
    double *getDimensions();
    void getDimensions( double dimensions[3] );
    
    /// Assignar/Obtenir espaiats de la s�rie
    void setSpacing( double spacing[3] );
    void setSpacing( double x , double y , double z );
    double *getSpacing();
    void getSpacing( double spacing[3] );
    
    /// Assignar/Obtenir identificador del volum al repositori corresponent a la s�rie
    void setVolumeIdentifier( Identifier id );
    Identifier getVolumeIdentifier() const { return m_volumeID; }
    
private:
    /// Identidicador universal de la s�rie
    QString m_serieUID;
    
    /// Identificador de la s�rie
    QString m_serieID;

    /// Orientaci� del pacient ( LR/AP/SI )
    QString m_patientOrientation;

    /// Modalitat d'imatge
    QString m_modality; 

    /// Descripci� de la s�rie
    QString m_description;

    /// Protocol que 'sha aplicat per obtenir la imatge
    QString m_protocolName;

    /// Data i hora en que s'ha adquirit la s�rie
    QDateTime m_dateTime;
    
    /// Window width i window level del contrast de la imatge
    double m_window;
    double m_level;

    /// Dimensions ( x , y , z [nombre de llesques] )
    double m_dimensions[3];

    /// Espaiat en cada dimensi�
    double m_spacing[3];

    /// Identificador del volum al repositori
    Identifier m_volumeID;
//     TransferFunctionList m_transferFunctionList;
};

}

#endif
