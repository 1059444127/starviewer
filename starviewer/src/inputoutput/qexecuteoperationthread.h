/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQEXECUTEOPERATIONTHREAD_H
#define UDGQEXECUTEOPERATIONTHREAD_H

#include <QThread>
#include "status.h"


namespace udg {

/** Aquest classe, s'encarrega d'anar executant objectes Operation. (operacions que s'han de dur a terme). Aquesta classe crea un thread quan hi ha alguna operacio i les executa. A m�s tamb� utilitza una cua, on es van guardant totes les operation pendents d'executar 
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

class Operation;

class QExecuteOperationThread :public QThread
{
Q_OBJECT

public:
    /** Constructor de la classe
      */
    QExecuteOperationThread( QObject *parent = 0 );

    /** afegeix una operacio a la cua, si la cua esta buida l'afegeix i crea el thread que l'executar� 
      *         @param Operation a executar
      */
    void queueOperation( Operation operation );
    
    /** Codi que s'executa en un nou thread, evalua l'operacio i decideix que fer
      */
    void run();
    
    /** Destructor de la classe
      */
    ~QExecuteOperationThread();

signals :
    void viewStudy( QString studyUID );
    
    void setStudyRetrieving( QString studyUID );
    void setStudyRetrieved( QString studyUID );
    void setErrorRetrieving( QString studyUID );
    
    void imageRetrieved( QString studyUID , int );
    void seriesRetrieved( QString );
    
    void notEnoughFreeSpace();
    void errorFreeingCacheSpace();

    /** signal que s'emete si s'ha produit algun error alhora de connectar amb algun pacs
     * @param  pacsID del pacs amb que s'ha produit error
     */
    void errorConnectingPacs( int );
    
private slots :

    /** Si la operacio es un view, emet un signal per indicar que ja es pot obrir la primera serie de l'estudi
      *     @param studyUID de l'estudi a descarregar
      */
     void firstSeriesRetrieved( QString );

    /** Emet un singnal cap al queryscreen per indicar que s'ha descarregat una imatge
      *     @param UID de l'estudi
      *     @param n�mero d'imatge
      */
    void imageRetrievedSlot( QString , int );
    
    /** Emet un singal per indicar que s'ha descarregat una s�rie
      *     @param studyUID de l'estudi a descarregar
      */
    void seriesRetrievedSlot( QString );
    

private :
    
    bool m_stop;//indica si el thread esta parat
    bool m_view;//indica si la operacio que s'esta realitzant �s la descarrega d'un estudi per visualitzar-lo    
        
    /** Descarrega un estudi, segons els par�metres a operation, si l'estudi s'ha de visualitzar 
      * captura els signals de l'objecte starviewersettings que es emes cada vegada que finalitza la descarrega d'una  serie
      *     @param operation a executar
      *     @param indica si l'estudi s'ha de visualitzar
      */
    void retrieveStudy(Operation operation,bool view);
    
    /** Indica si hi ha com a minim 1Gb d'espai lliure tant a la cache com al disc per descarregar el nou estudi. 
      * Si detecta,que no hi ha suficient a la cache o al disc, intenter� alliberar 2000 Mb, esborrant estudis vells, si no retornara que no hi ha suficient espai al disc
      *     @param retorna si hi ha suficient espai per descarregar l'estudi
      *     @return retorna l'estat de l'operacio
      */
    Status enoughFreeSpace( bool &enoughSpace);

};

}

#endif
