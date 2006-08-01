/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGVOLUMESOURCEINFORMATION_H
#define UDGVOLUMESOURCEINFORMATION_H

#include <QString>

namespace udg {

/**
Cont� informaci� relacionada amb el volum, com per exemple dades del pacient.

@author Grup de Gr�fics de Girona  ( GGG )
*/
class VolumeSourceInformation 
{
public:
    VolumeSourceInformation();

    ~VolumeSourceInformation();


    /** Indiquem l'string que defineix la orientaci� del pacient respecte el seu sistema de coordenades
        Per determinar la dreta o esquerra del pacient farem servir L, per LEFT i R per RIGHT.
        Per determinar la part davantera o del darrera farem servir A, per ANTERIOR , i P, per POSTERIOR.
        Per deteminar amunt i avall farem servir H/S, per HEAD/SUPERIOR, i F/I, per FEET/INFERIOR)

        En un scan t�pic de cervell tindr�em un string LPS, ja que l'eix X avan�a cap a l'esquerra del pacient, l'eix Y cap al darrera del pacient i les Z en direcci� cap al cap del pacient. Aix� doncs en les respectives visualitzacions tindr�em, comen�ant pel cant� esquerra les seg�ents refer�ncies i en sentit anti-horari:
        Axial: Left , Anterior , Right , Posterior
        Sagital: Posterior , Inferior , Anterior , Superior
        Coronal: Right , Inferior , Left , Superior

        En un scan tipu "radiografia" frontal ( com si ens fessin una foto de cares ) l'string seria LIP, ja que l'eix X avan�a cap a l'esquerra del pacient, l'eix Y avan�a cap als peus (part inferior) del pacient i l'eix Z avan�a cap a l'esquena del pacient

        M�s informaci� a l'standard DICOM cap�tols C.7.6.2.1.1 , C.7.6.1.1.1
        
        (
        especifica els "direction cosines" de la primera fila i de la primera columna respecte al pacient. Aquests atributs vindran en parells. El valor de la fila pels eixos x, y, i z respectivament seguits pels valors de la Columna per els eixos x, y, i z respectivament. La direcci� dels eixos est� definida completament per l'orientaci� del pacient.
         L'eix de les X incrementa cap a l'esquerra del pacient.
         L'eix de les Y incrementa cap a la cara posterior del pacient.
         L'eix de les Z incrementa cap al cap del pacient.
      El sistema de coordenades del pacient �s un sistema de m� dreta, �a dir, el producte vectorial de un vector unitari a trav�s de l'eix positiu de les X i el vector unitari sobre l'eix positiu de les Y �s = al vector unitari sobre l'eix positiu de les Z
      )

    */
    void setPatientOrientationString( const char* patientOrientationString )
    {
        m_patientOrientationString = QString::fromLatin1( patientOrientationString );
    };

    /// Retorna l'string d'orientaci� del pacient
    QString getPatientOrientationString( )
    {
        return m_patientOrientationString;
    };

    /// Retorna l'string d'orientaci� del pacient invertit, �s a dir L <=> R , P <=> A , S <=> I
    QString getRevertedPatientOrientationString();
    
    /// Assigna/Retorna nom del pacient
    void setPatientName( const char * patientName )
    {
        m_patientName = QString::fromLatin1( patientName );
    };
    QString getPatientName()
    {
        return m_patientName;
    };

    /// Assigna/Retorna l'id del pacient
    void setPatientID( const char * patientID )
    {
        m_patientID = QString::fromLatin1( patientID );
    };
    QString getPatientID()
    {
        return m_patientID;
    };

    /// Assigna/Retorna nom de la instituci�
    void setInstitutionName( const char * institution )
    {
        m_institution = QString::fromLatin1( institution );
    };
    QString getInstitutionName()
    {
        return m_institution;
    };

    /// Assigna/Retorna la data de l'estudi
    void setStudyDate( const char * studyDate )
    {
        m_studyDate = QString::fromLatin1( studyDate );
    };
    QString getStudyDate()
    {
        return m_studyDate;
    };

    /// Assigna/Retorna l'hora de l'estudi
    void setStudyTime( const char * studyTime )
    {
        m_studyTime = QString::fromLatin1( studyTime );
    };
    QString getStudyTime()
    {
        return m_studyTime;
    };

    /// Assigna/Retorna l'accession number
    void setAccessionNumber( const char * acc )
    {
        m_accessionNumber = QString::fromLatin1( acc );
    };
    QString getAccessionNumber()
    {
        return m_accessionNumber;
    };

    /// Assigna/ retorna el window/level que ens proporciona el DICOM
    void setWindowLevel( double window , double level )
    {
        m_windowLevel[0] = window;
        m_windowLevel[1] = level;
    }
    void setWindowLevel( double windowLevel[2] )
    {
        this->setWindowLevel( windowLevel[0] , windowLevel[1] );
    }
    void setWindow( double window )
    {
        m_windowLevel[0] = window;
    }
    void setLevel( double level )
    {
        m_windowLevel[1] = level;
    }
    double getWindow()
    {
        return m_windowLevel[0];
    }
    double getLevel()
    {
        return m_windowLevel[1];
    }
    double *getWindowLevel()
    {
        return m_windowLevel;
    }
    void getWindowLevel( double &window , double &level )
    {
        window = m_windowLevel[0];
        level = m_windowLevel[1];
    }
    void getWindowLevel( double windowLevel[2] )
    {
        windowLevel[0] = m_windowLevel[0];
        windowLevel[1] = m_windowLevel[1];
    }

    /// Assigna/Retorna el nom del protocol
    void setProtocolName( const char * protocol )
    {
        m_protocolName = QString::fromLatin1( protocol );
    };
    QString getProtocolName()
    {
        return m_protocolName;
    };
    
private:

    QString m_patientName;
    QString m_patientID;
    QString m_patientOrientationString;
    QString m_accessionNumber;
    QString m_institution;
    QString m_studyDate;
    QString m_studyTime;
    QString m_protocolName;
    double m_windowLevel[2];

};

};  //  end  namespace udg 

#endif
