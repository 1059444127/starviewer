/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCONVERTTODICOMDIR_H
#define UDGCONVERTTODICOMDIR_H
#include <QProgressDialog>
#include <QObject>

class QString;
class QStringList;

namespace udg {

class Status;
class Series;
class Image;

/** Converteix un estudi a DICOMDIR, invocant el m�todes i classes necess�ries
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class ConvertToDicomdir : public QObject 
{
public:

    ConvertToDicomdir( );

    
    /** Afegeix un estudi a la llista per convertir-se a dicomsdir
     * @param studyUID UID de l'estudi a convertir a dicomdir
     */
    void addStudy ( QString studyUID );

    /** Converteix l'estudi amb el UID passat per par�metre a dicomDir
     * @param studyUID UID de l'estudi a convertir a DicomDir
     * @return estat del m�tode
     */
    Status convert( QString studyUID );

    ~ConvertToDicomdir();

private :
    QProgressDialog *m_progress;

    QStringList m_studiesToConvert;

    QString m_dicomDirPath;
    QString m_dicomDirStudyPath;
    QString m_dicomDirSeriesPath;

    int m_study;
    int m_series;
    int m_image;

    Status convertSeries( Series series );
    Status convertImage( Image image );    
};

}

#endif
