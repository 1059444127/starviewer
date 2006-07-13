/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGREADDICOMDIR_H
#define UDGREADDICOMDIR_H

#define HAVE_CONFIG_H 1
#include "dcdicdir.h" //llegeix el dicom dir

class string;
class DcmDicomDir;

namespace udg {

class Status;
class StudyList;
class SeriesList;

/** Aquesta classe permet llegir un dicomdir i consultar-ne els seus elements
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class ReadDicomdir{
public:

    ReadDicomdir();

    /** Obre un directori domcidr
     * @param dicomdirPath directori on es troba el dicomdir
     * @return estat del m�tode
     */
    Status open( std::string dicomdirPath );
    
    /** Retorna la llista d'estudis que cont� el dicomdir
      * @param studyList llista amb els estudis que cont� el dicomdir
      * @return estat del m�tode
      */
    Status readStudies( StudyList &studyList );
    
    /** Retorna la llista de s�ries d'un estudi que estigui en el dicomdir
     * @param studyUID UID de l'estudi del qual es vol consultar les s�ries
     * @param seriesList llista amb les s�ries que cont� l'estudi
     * @return estat del m�tode
     */
    Status readSeries ( std::string studyUID , SeriesList  &serieList );

    ~ReadDicomdir();

private :

    DcmDicomDir *dicomdir;

};

}

#endif
