/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCACHEIMAGEDAL_H
#define UDGCACHEIMAGEDAL_H

#include "imagelist.h"

class string;


namespace udg {

class ImageMask;
class Status;
class Image;
/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class CacheImageDAL
{
public:
    CacheImageDAL();

    /** Insereix la informaci� d'una imatge a la cach�. I actualitza l'espai ocupat de la pool, com s'ha de fer un insert i un update aquests dos operacions es fan dins el marc d'una transaccio, per mantenir coherent l'espai de la pool ocupat
     * @param dades de la imatge 
     * @return retorna estat del m�tode
     */
    Status insertImage( Image * );
    
    /** Cerca les imatges demanades a la m�scara. Important! Aquesta acci� nom�s t� en compte l'StudyUID i el SeriesUID de la m�scara per fer la cerca, els altres caps de la m�scara els ignorar�!
     * @param  mascara de les imatges a cercar
     * @param llistat amb les imatges trobades
     * @return retorna estat del m�tode
     */
    Status queryImages( ImageMask mask , ImageList &list );

    /** compta les imatges d'una s�rie 
     * @param imageMask mascar� de les imatges a comptar les images. Las m�scara ha de contenir el UID de l'estudi i opcionalment el UID de la s�rie 
     * @param imageNumber conte el nombre d'imatges
     * @return retorna estat del m�tode  
     */
    Status countImageNumber( ImageMask mask , int &imageNumber );
    
    /** Esborra les imatges que tinguin el studyUID passat per par�metre
      * @param studyUID StudyUID de les imatges a esborrar
      */
    Status deleteImages( std::string studyUID );
    
    ~CacheImageDAL();

private :


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


};

}

#endif
