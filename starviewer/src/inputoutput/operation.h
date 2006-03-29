/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGOPERATION_H
#define UDGOPERATION_H

#include "studymask.h"
#include "pacsparameters.h"
#include <QString>

namespace udg {

/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class Operation{

public:
    Operation();
    
    /** Especifica la m�scara de l'estudi per aquella operacio,
      *         @param m�scara de l'estudi
      */
    void setStudyMask(StudyMask mask);
    
    /** Estableix la prioritat de l'aplicaci� si no s'especifica, el setOperation automaticament l'assigna en funci� del tipus d'operacio
      *     @param prioritat
      *         High = 0
      *         Normal = 50
      *         Low = 100
      */
    void setPriority(int priority);
    
    /** Especifica quina operacio es portara a terma en aquesta operacio, si no s'ha assignat prioritat, aquest m�tode l'assigna en funci� de l'operaci�
      *             Retrieve = 1
      *             Print = 2
      *             Move = 3
      *         @param operacio a realitzar
      *
      */   
    void setOperation(int operation);
    
    
    /** Estableix a quin PACS es dura a terme la operacio
      *     @param Pacs al qual es dura a terme l'operacio
      */
    void setPacsParameters(PacsParameters parameters);
        

   /** Retorna la m�scara de l'estudi
     *         @return m�scara de l'estudi
     */        
    StudyMask getStudyMask();
    
    /** Retorna la prioritat de l'operaci�
      *         @return prioritat de l'operaci�
      */    
    int getPriority();
    
    /**  Retorna la operacio a realitzar
      *         @return operacio a realitzar
      */
    int getOperation();
    
    /** Retorna el pacsParameters de l'objecte operation
      *     @return PacsParameters de l'objecte operation
      */
    PacsParameters getPacsParameters();
    
    /** Operador per odernar per prioritats
      */
    bool operator<(Operation) const;

    ~Operation();

private :

    StudyMask m_mask;
    int m_priority;
    int m_operation;
    PacsParameters m_pacsParameters;
    QString m_text;    
};

}

#endif
