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
class ImageList;
class StudyMask;
class Study;

/** Aquesta classe permet llegir un dicomdir i consultar-ne els seus elements.
  * Accedint a trav�s de l'estructura d'arbres que representen els dicomdir Pacient/Estudi/Series/Imatges, accedim a la informaci� el Dicomdir per a realitzar cerques.
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
      * @param studyMask m�scara de cerca dels estudis a cercar dins el dicomdir
      * @return estat del m�tode
      */
    Status readStudies( StudyList &studyList , StudyMask studyMask );
    
    /** Retorna la llista de s�ries d'un estudi que estigui en el dicomdir
     * @param studyUID UID de l'estudi del qual es vol consultar les s�ries
     * @param seriesList llista amb les s�ries que cont� l'estudi
     * @return estat del m�tode
     */
    Status readSeries ( std::string studyUID , SeriesList  &serieList );

    /** Retorna la llista d'imatges que cont� un estudi
     * @param seriesUID UID de la serie que volem obtenir les imatges
     * @param imageList Llistat de les imatges que cont�
     * @return estat del m�tode
     */
    Status readImages( std::string seriesUID , ImageList &imageList );

    /** Retorna el path del dicomdir
     * @return path del dicomdir
     */
    std::string getDicomdirPath();

    ~ReadDicomdir();

private :

    DcmDicomDir *m_dicomdir;
    std::string m_dicomdirAbsolutePath;

    /** Comprova si un estudi compleix la m�scara, pels camps PatientId, StudyID, StudyDate, PatientName i AccessionNumber
     * @param study dades de l'estudi
     * @param studyMask m�scara de l'estudi
     * @return cert si l'estudi complei la m�scara
     */
    bool matchStudyMask( Study study , StudyMask studyMask );

    /** Comprova que els dos StudyId el de la m�scara i el de l'estudi siguin iguals. Si l'estudi Id de la m�scara est� buit, per defecte retorna cert
     * @param studyMaskStudyId studyId de la m�scara
     * @param studyStudyId studyId de l'estudi trobat al dicomdir
     * @return retorna cert si els dos studyID son iguals o studyMaskStudyID est� buit
     */
    bool matchStudyMaskStudyId( std::string studyMaskStudyId , std:: string studyStudyId );

    /** Comprova que els dos StudyUID el de la m�scara i el de l'estudi facin matching. Si l'estudi UID de la m�scara est� buit, per defecte retorna cert. En aquest cas fem wildcard matching 
     * @param studyMaskStudyUID studyUID de la m�scara
     * @param studyStudyUID studyUID de l'estudi trobat al dicomdir
     * @return retorna cert si els dos studyUID son iguals o studyMaskStudyUID est� buit
     */
    bool matchStudyMaskStudyUID( std::string studyMaskStudyUID , std:: string studyStudyUID );

    /** Comprova que els dos PatientId el de la m�scara i el de l'estudi facin matching. Si el Patient Id de la m�scara est� buit, per defecte retorna cert. En aquest cas fem wildcard matching 
     * @param studyMaskPatientId 
     * @param studyPatientId 
     * @return retorna cert si els dos patientId s�n iguals o studyMaskPatientId est� buit
     */
    bool matchStudyMaskPatientId( std::string studyMaskPatientId , std:: string studyPatientId );

    /** Comprova que la data de la m�scara i la de l'estudi facin matching. Si la studyMaskDate �s buida retorna cert per defecte
     * @param studyMaskDate M�scara de dates 
     * @param studyDate Data de l'estudi
     * @return retorna cert si es fa matching amb la data de la m�scara o studyMaskDate �s buit
     */
    bool matchStudyMaskDate( std::string studyMaskDate , std::string studyDate );

    /** Comprova que el nom del pacient de la m�scara i el de l'estudi facin matching. Si la studyMaskPatientName �s buida retorna cert per defecte. En aquest cas fem wildcard matching 
     * @param studyMaskPatienName M�scara de nom pacient
     * @param studyPatientName Nom del pacient
     * @return retorna cert si es fa matching amb el nom del pacient de la m�scara o studyMaskPatient �s buit
     */
    bool matchStudyMaskPatientName( std::string studyMaskPatientName , std::string studyPatientName );

    /** Comprova que el AccessionNumber de la m�scara i el de l'estudi facin matching. Si la studyMaskAccessionNumber �s buida retorna cert per defecte. En aquest cas fem wildcard matching 
     * @param studyMaskPatienName M�scara de AccessionNumber
     * @param studyPatientName AccessionNumber de l'estudi
     * @return retorna cert si es fa matching amb studyAccessionNumber de la m�scara o studyMaskAccessionNumber �s buit
     */
    bool matchStudyMaskAccessionNumber( std::string studyMaskAccessionNumber , std::string studyAccessionNumber );

    /** Converteix un string a maj�scules,
     *  Com el string distingeix entre maj�scules i min�scules, per fer els match, primer convertirem l'string a maj�scules, ja que el DICOM guardar la informaci� en maj�scules
     * @param original cadena original
     * @return cadena en maj�scules
     */
    std::string upperString( std:: string original );
    
    /** canvia les '\' per '/'. Aix� es degut a que les dcmtk retornen el path de la imatge en format Windows amb els directoris separats per '\'. En el cas de linux les hem de passar a '/'
     * @param original path original
     * @return path amb '/'
     */
    std::string replaceBarra( std::string original );
};

}

#endif
