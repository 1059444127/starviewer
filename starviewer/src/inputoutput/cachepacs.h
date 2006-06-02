/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGCACHEPACS_H
#define UDGCACHEPACS_H

#include <sqlite.h>
#include "studylist.h"
#include "serieslist.h"
#include "imagelist.h"
#include "databaseconnection.h"

class string;

namespace udg {

/** Classe que gestiona la cach� de l'aplicaci�, i fa els inserts,queries i deletre
@author marc
*/

class Study;
class Series;
class Image;
class StudyMask;
class SeriesMask;
class ImageMask;
class Status;

class CachePacs
{

private:
    
    DatabaseConnection *m_DBConnect;

    ///Constructor de la classe        
    CachePacs();
    
    ///Destructor de la classe
    ~CachePacs();

        
   /** Construeix l'estat en que ha finaltizat l'operaci� sol�licitada
    * @param  Estat de sqlite
    * @return retorna l'estat de l'operaci�
    */
    Status constructState(int error);

    /** Construeix la sent�ncia sql per fer la query de l'estudi en funci� dels parametres de cerca
     * @param mascara de cerca
     * @return retorna estat del m�tode
     */
    std::string buildSqlQueryStudy( StudyMask *studyMask );

    /** Construeix la sent�ncia per buscar les s�ries d'un estudi
     * @param mask m�scara de cerca
     * @return sent�ncia sql
     */    
	std::string buildSqlQuerySeries( SeriesMask *seriesMask );
    
    /** Construeix la sent�ncia per buscar les imatges d'un estudi
     * @param mask m�scara de cerca
     * @return sent�ncia sql
     */    
	std::string buildSqlQueryImages( ImageMask *imageMask );
    
    /** Construiex la sent�ncia sql per comptar el nombre d'imatges de la s�rie d'un estudi
     * @param mask 
     * @return retorna la sent�ncia Sql
     */
	std::string buildSqlCountImageNumber( ImageMask *imageMask );    
	            
    /** retorna l'hora del sistema
     * @return retorna l'hora del sistema en format HHMM
     */
    int getTime();
    
	/** retorna la data del sistema
     *    @return retorna la data del sistema en format yyyymmdd
     */
    int getDate();
    
    /** Converteix l'asterisc, que conte el tag origen per %, per transformar-lo a la sintaxis de sql
     * @param string original
     * @return retorna l'string original, havent canviat els '*' per '%'
     */
    std::string replaceAsterisk( std::string );

public:
 
     static CachePacs* getCachePacs()
     {
         static CachePacs cache;
         return &cache; 
     }
   
    /** Afegeix un nou estudi i pacient a la bd local, quant comencem a descarregar un nou estudi.
     *   La informaci� que insereix �s :
     *        Si el pacient no existeix - PatientId
     *                                  - PatientName
     *                                  - PatientBirthDate
     *                                  - PatientSex  
     *
     *       Si l'estudi no existeix    - PatientID
     *                                  - StudyUID
     *                                  - StudyDate
     *                                  - StudyTime
     *                                  - StudyID
     *                                  - AccessionNumber
     *                                  - StudyDescription
     *                                  - Status
     *  La resta d'informaci� no estar� disponible fins que les imatges estiguin descarregades, 
     *                    
     * @param Study[in]  Informaci� de l'estudi 
     * @return retorna l'estat de l'inserci�                                    
     */
    Status insertStudy(Study *); 

    /** Insereix una s�rie a la cach�
     * @param series Dades de la s�rie
     * @return retorna l'estat de la inserci�
     */
    Status insertSeries(Series *); 

    /** Insereix la informaci� d'una imatge a la cach�. I actualitza l'espai ocupat de la pool, com s'ha de fer un insert i un update aquests dos operacions es fan dins el marc d'una transaccio, per mantenir coherent l'espai de la pool ocupat
     * @param dades de la imatge 
     * @return retorna estat del m�tode
     */
    Status insertImage(Image *);

    /** Cerca els estudis que compleixen la m�scara a la cach�
     * @param M�scara de la cerca
     * @param StudyList amb els resultats
     * @return retorna estat del m�tode
     */
    Status queryStudy(StudyMask mask,StudyList &list);

    /** Cerca l'estudi que compleix amb la m�scara de cerca. Cerca ens els estudis que estan en estat Retrived o Retrieving
     * @param    M�scara de  la cerca
     * @param    StudyList amb els resultats
     * @return retorna estat del m�tode
     */
    Status queryStudy(std::string studyUID,Study &study);

    /** Selecciona els estudis vells que no han estat visualitzats des de una data inferior a la que es passa per parametre
     * @param  Data a partir de la qual es seleccionaran els estudis vells
     * @param  StudyList amb els resultats dels estudis, que l'ultima vegada visualitzats es una data inferior a la passa per par�metre
     * @return retorna estat del m�tode
     */
    Status queryOldStudies( std::string , StudyList &list );

    /** Cerca les s�ries demanades a la m�scara. Important! Aquesta acci� nom�s t� en compte l'StudyUID de la m�scara per fer la cerca, els altres camps de la m�scara els ignorar�!
     * @param  mascar� de la serie amb l' sstudiUID a buscar
     * @param  retorna la llista amb la s�ries trobades
     * @return retorna estat del m�tode
     */
    Status querySeries(SeriesMask mask,SeriesList &list);

    /** Cerca les imatges demanades a la m�scara. Important! Aquesta acci� nom�s t� en compte l'StudyUID i el SeriesUID de la m�scara per fer la cerca, els altres caps de la m�scara els ignorar�!
     * @param  mascara de les imatges a cercar
     * @param llistat amb les imatges trobades
     * @return retorna estat del m�tode
     */
    Status queryImages(ImageMask mask,ImageList &list);

    /** compta les imatges d'una s�rie 
     * @param series [in] mascar� de la serie a comptar les images. Las m�scara ha de contenir el UID de l'estudi i de la s�rie
     * @param imageNumber [out] conte el nombre d'imatges
     * @return retorna estat del m�tode  
     */
    Status countImageNumber(ImageMask mask,int &imageNumber);
    
    /** actualitza l'�ltima vegada que un estudi ha estat visualitzat, d'aquesta manera quant haguem d'esborrar estudis
     * autom�ticament per falta d'espai, esborarrem els que fa m�s temps que no s'han visualitzat
     * @param UID de l'estudi
     * @param hora de visualitzaci� de l'estudi format 'hhmm'
     * @param data visualitzaci� de l'estudi format 'yyyymmdd'
     * @return estat el m�tode
     */
    Status updateStudyAccTime(std::string studyUID);
    
    /** Updata un estudi a Retrieved
     * @param Uid de l'estudi a actualitzar
     * @return retorna estat del m�tode
     */
    Status setStudyRetrieved(std::string studyUID);
    
    /** Updata la modalitat d'un estudi
     * @param Uid de l'estudi a actualitzar
     * @param Modalitat de l'estudi
     * @return retorna estat del m�tode
     */
    Status setStudyModality(std::string studyUID,std::string modality);
    
    /** Updata un estudi PENDING a RETRIEVING, per indicar que l'estudi s'esta descarregant
     * @param Uid de l'estudi a actualitzar
     * @return retorna estat del m�tode
     */
    Status setStudyRetrieving(std::string studyUID);
    
    /** Esborra un estudi de la cache, l'esborra la taula estudi,series, i image, i si el pacient d'aquell estudi, no t� cap altre estudi a la cache local tambe esborrem el pacient
     * @param std::string [in] UID de l'estudi
     * @return estat de l'operaci�
     */
    Status delStudy(std::string);
    
    /** Aquesta acci� es per mantenir la coherencia de la base de dades, si ens trobem estudis al iniciar l'aplicaci� que tenen l'estat pendent o descarregant vol dir que l'aplicaci� en l'anterior execussi� ha finalitzat an�malament, per tant aquest estudis en estat pendents, les seves s�rie i imatges han de ser borrades perqu� es puguin tornar a descarregar. Aquesta acci� �s simplement per seguretat!
     * @return estat de l'operaci�
     */
    Status delNotRetrievedStudies();    
    
    /** Compacta la base de dades de la cache, per estalviar espai
     * @return estat del m�tode  
     */
    Status compactCachePacs();
    
};

};

#endif
