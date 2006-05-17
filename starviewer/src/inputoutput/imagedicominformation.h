/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�ics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGIMAGEDICOMINFORMATION_H
#define UDGIMAGEDICOMINFORMATION_H

#include <string>
#include <dcfilefo.h> //obre un fitxer dicom

namespace udg {

/** Aquesta classe retorna els tags DICOM sollicitats d'una imatge
	@author Grup de Gràfics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

class Status;

class ImageDicomInformation{
public:

    /// Constructor de la classe
    ImageDicomInformation();

    /** Obre el DICOM del qual s'ha d'obtenir la informaci�      
     *         @param ruta del fitxer
     */
    Status openDicomFile( std::string filePath );
    
    /** Retorna el study UID de l'imatge
     *         @param study UID de l'imatge
     */
    std::string getStudyUID();
    
    /** Retorna el ID de l'estudi de l'imatge
     *         @param study ID de l'estudi de l'imatge
     */
    std::string getStudyID();
    
    /** Retorna el series UID de l'imatge
     *         @param series UID de l'imatge
     */
    std::string getSeriesUID();
    
    /** Retorna el series number de l'imatge
     *         @param series number
     */
    std::string getSeriesNumber();
    
    /** Retorna la modalitat de la s�ie
     *         @param modalitat de la s�ie
     */
    std::string getSeriesModality();
    
    /** Retorna el nom del Procotol utilitzat
     *         @param Protocol utilitzat
     */
    std::string getSeriesProtocolName();
    
    /** Retorna la descripci�de la s�ie
     *         @param descripci�de la s�ie
     */
    std::string getSeriesDescription();
    
    /** Retorna la part del cos examinada
     *         @param part del cos examinada
     */
    std::string getSeriesBodyPartExamined();
    
    /** Retorna l'hora en que s'ha capturat la serie
     *         @return hora en que s'ha capturat la serie
     */
    std::string getSeriesTime();
    
    /** Retorna data en que s'ha capturat la serie
     *         @return data en que s'ha capturat la serie
     */
    std::string getSeriesDate();    
    
    /// Destructor de la classe
    ~ImageDicomInformation();
    
private :

    DcmFileFormat *m_dicomFile;
    DcmDataset *m_dataset;
};

}

#endif
