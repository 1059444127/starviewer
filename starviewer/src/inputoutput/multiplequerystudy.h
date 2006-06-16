/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGMULTQUERYSTUDY_H
#define UDGMULTQUERYSTUDY_H

#include <QObject>

#include "pacslist.h"
#include "studylistsingleton.h"
#include "studymask.h"

namespace udg {

class Status;
class PacsParameters;
/** Classe que permet fer diverses cerques simult�nies, a diversos pacs a la vegada mitjan�ant la utilitzacio de threads
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class MultipleQueryStudy : public QObject
{
Q_OBJECT
public:
    ///constructor de la classe
    MultipleQueryStudy( QObject *parent = 0 );
    
    ///destructor de la classe
    ~MultipleQueryStudy();
   
    /** Ens permet indicar quina m�scara utilitzarem per fer la query als PACS
     * @param StudyMask M�scara del estudis a cercar
     */
    void setMask( StudyMask );
    
    /** Estableix la llista de PACS als quals es far� la cerca
     * @param PacsList amb els pacs als quals es cercar�
     */
    void setPacsList( PacsList );
    
    /** Una vegada haguem especificat la m�scara, i tots els PACS als que volem realitzar la query, aquesta acci� iniciara el proc�s de cerca a tots els PACS
     * @return Estat del m�tode 
     */
    Status StartQueries();
    
    /** retorna un apuntador a la llist amb els estudis
     * @return  Llista amb els estudis trobats que complien amb la m�scara.
     */
    StudyListSingleton* getStudyList();

signals :

    /** signal que s'emete si s'ha produit algun error alhora de connectar amb algun pacs
     * @param  pacsID del pacs amb que s'ha produit error
     */
    void errorConnectingPacs( int );

    /** signal que s'envia per indicar que hi hagut un error cercant al PACS
     * @param ID del pacs que ha produit l'error 
     */    
    void errorQueringStudiesPacs( int pacsID );

public slots :

    /// Slot que s'activa pel signal de QQuerStudyThread, quan un thread acaba allibera un recurs del sem�for, perqu� es pugui iniciar una altre thread per continuar amb la cerca
    void threadFinished();
    
    /** slot que s'activa pel signal de QQueryStudThread, quan s'ha produ�t algun error al connectar amb el PACS
     * @param pacsID del pacs que ha produ�t l'error
     */   
    void slotErrorConnectingPacs( int );

    /** slot que s'activa pel signal de QQueryStudThread, quan s'ha produ�t algun error al fer la query amb el PACS
     * @param pacsID del pacs que ha produ�t l'error
     */   
    void slotErrorQueringStudiesPacs( int );


private :

    StudyMask m_searchMask;
    
    StudyListSingleton* m_studyListSingleton;
    PacsList m_pacsList;
    int m_maxThreads;//Nombre m�xim de threads que es poden executar a la vegada
   
};

}

#endif
