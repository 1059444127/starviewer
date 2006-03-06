/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGPROCESIMAGESINGLETON_H
#define UDGPROCESIMAGESINGLETON_H

#define HAVE_CONFIG_H 1
#include <cond.h>
#include <list>
#include "processimage.h"

namespace udg {

/** Classe singleton, que implementa una llista dels Objectes ProcessImage.Aquesta classe �s necessaria per poder fer m�s d'una descarrega alhora
  * ja que cont� els objectes ProcessImage, associats a cada descarrega, que indiquen quines accions s'han de portar a terme amb cada imatge descarregada, per
  * cada estudi
@author marc
*/
class ProcesImageSingleton{
public:
    ProcesImageSingleton();
    
    
    
    ~ProcesImageSingleton();

private:

struct SingletonProcess
    {
        ProcessImage process;
        string UID;
    };

    list<SingletonProcess>listProcess;     
    list<SingletonProcess>::iterator i;
   
};

};

#endif
