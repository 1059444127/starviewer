/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGPROCESSIMAGESINGLETON_H
#define UDGPROCESSIMAGESINGLETON_H

#define HAVE_CONFIG_H 1
#include <cond.h>
#include <list>
#include <semaphore.h>

namespace udg {

class ProcessImage;
class Image;
class string;
/** Aquesta �s una classe singleton que cont� una col.leci� d'objectes ProcessImage. La classe �s necessaria per poder descarregar estudis simult�niament ja que per cada estudi que �s descarrega, a trav�s del seu studyUID t� associat un ProcessImage, que �s que s'encarrega de dur a terme les accions pertinents despr�s de la desc�rrega de cada imatge.
 *
@author marc
 */
class ProcessImageSingleton{
public:
    
    /** retorna una instancia de l'objecte
     * @return instancia de l'objecte
     */
    static ProcessImageSingleton* getProcessImageSingleton();
 
    /** Afegeix un nou objecte ProcessImage, que s'encarregarr� de processar la informaci� de cada imatge descarregada
     * @param UID de l'estudi
     * @param Objecte processimage que tractar� la descarrega d'imatges
     */
    void addNewProcessImage( std::string , ProcessImage * );

    /** buscar l'objecte processimage que s'encarrega de gestionar la descarrega de l'estudi studyUID, per processar la imatge
     * @param UID de l'estudi que ha produit l'error
     * @param imatge descarregada
     */
    void process( std::string ,  Image *image ); 

    /** buscar l'objecte processimage que s'encarrega de gestionar la descarrega de l'estudi studyUID, per notificar l'error
     * @param UID de l'estudi que ha produit l'error
     */
    void setErrorRetrieving( std::string studyUID );

    /** esborra el ProcessImage de la llista
     * @param UID del Process Image de l'estudi a esborrar
     */
    bool delProcessImage( std::string );    

    /** Estableix el path on s'han de guardar les imatges de la cach�
     * @param path de la cache on es guarden les imatges
     */
    void setPath( std::string );

    /** retorna el path de la cach� on s'han de guardar les imatges
     * @return path de la cache
     */
    std::string getPath();
    
    /// destructor de la classe
    ~ProcessImageSingleton();    
        
private:

    static ProcessImageSingleton* pInstance;

    struct SingletonProcess
    {
        ProcessImage *imgProcess;//ha de ser un punter pq sino creari un objecte del pare, cridaria les funcions del pare i no dels fills
        std::string studyUID;
    };

    list<SingletonProcess>m_listProcess;     
    list<SingletonProcess>::iterator i;
    
    sem_t *m_semafor;
    std::string m_imagePath;
    
    /// Constructor de la classe
    ProcessImageSingleton();
};

};

#endif
