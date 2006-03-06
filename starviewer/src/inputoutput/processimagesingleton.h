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
// #include "processimage.h"
//#include "starviewerprocessimage.h"
#include <string> 
#include <semaphore.h>

namespace udg {

class ProcessImage;
class Image;
class string;
/** Aquesta �s una classe singleton que cont� una col.leci� d'objectes ProcessImage. La classe �s necessaria per poder descarregar estudis simult�niament
  * ja que per cada estudi que �s descarrega, a trav�s del seu studyUID t� associat un ProcessImage, que �s que s'encarrega de dur a terme les accions pertinents
  * despr�s de la desc�rrega de cada imatge.
  *
@author marc
  */
class ProcessImageSingleton{
public:
    
    static ProcessImageSingleton* getProcessImageSingleton();
 
    void addNewProcessImage(std::string,ProcessImage *);
    void process(std::string,Image *image); 
    void setErrorRetrieving(std::string studyUID);
    bool delProcessImage(std::string);    

    void setPath(std::string);
    std::string getPath();
        
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

    ProcessImageSingleton();
    
    ~ProcessImageSingleton();
    
};

};

#endif
