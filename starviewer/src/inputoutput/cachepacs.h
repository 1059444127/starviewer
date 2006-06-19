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

public:
 
     static CachePacs* getCachePacs()
     {
         static CachePacs cache;
         return &cache; 
     }

    /** Insereix la informaci� d'una imatge a la cach�. I actualitza l'espai ocupat de la pool, com s'ha de fer un insert i un update aquests dos operacions es fan dins el marc d'una transaccio, per mantenir coherent l'espai de la pool ocupat
     * @param dades de la imatge 
     * @return retorna estat del m�tode
     */
    Status insertImage(Image *);

    /** Selecciona els estudis vells que no han estat visualitzats des de una data inferior a la que es passa per parametre
     * @param  Data a partir de la qual es seleccionaran els estudis vells
     * @param  StudyList amb els resultats dels estudis, que l'ultima vegada visualitzats es una data inferior a la passa per par�metre
     * @return retorna estat del m�tode
     */
    Status queryOldStudies( std::string , StudyList &list );
    
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
