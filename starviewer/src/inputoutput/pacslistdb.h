/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGPACSLISTDB_H
#define UDGPACSLISTDB_H

#include <sqlite.h>
#include "databaseconnection.h"
#include "pacslist.h"

class string;
class PacsParameters;

namespace udg {

/** Aquesta classe implementa les accions necessaries per afegir nous pacs o modificar/consultar els par�metres dels PACS  que tenim disponible a l'aplicaci�, guardats a la base de dades local
@author marc
*/

class Status;

class PacsListDB{

public:

    ///Constructor de la classe
    PacsListDB( );
    
    ///Destructor de la classe
    ~PacsListDB( );

    /** Insereix els parametres d'un pacs a la base de dades, per a poder-hi cercar imatge. En un alta el camp PacsID, �s assignat autom�ticament per l'aplicaci�!
     * @param Objecte PacsParameters amb les dades del pacs
     * @return estat de l'operaci�  
     */
    Status insertPacs( PacsParameters *pacs );
    
    /** Retorna un objecte PacsList amb tots els Pacs que hi ha la taula PacsList odernats per AEtitle. Nomes selecciona els pacs vius, els que estan en estat de baixa no els selecciona
     * @param PacsList Cont� tots els Pacs de la taula PacsList 
     * @return estat de l'operaci�
     */
    Status queryPacsList( PacsList & );
    
    /** Permet actualitzar la informaci� d'un pacs, el PacsID camp clau no es pot canviar!
     * @param Objecte PAcsParameters ambles noves dades del PACS
     * @return estat de l'operaci�  
     */
    Status updatePacs( PacsParameters *pacs );
    
    /** Es donar� de baixa el Pacs de l'objecte PacsParameters passat. No es dona de baixa f�sicament, sin� que a la bd es posa en estat de baixa, ja que a la cach� podem tenir estudis que hi facin refer�ncia, De PacsParameters nom�s cal omplir el PacsID (camp clau= per esborrar el PACS
     * @param  Objecte PacsParamets amb el PacsID del pacs a esborrar
     * @return estat de l'operaci�  
     */
    Status deletePacs( PacsParameters *pacs );
    
    /** Cerca la informaci� d'un pacs en concret. 
     * @param Cont� la informaci� del pacs cercat
     * @param pacs a cercar 
     * @return estat de l'operaci�
     */
    Status queryPacs( PacsParameters *pacs , std::string AETitle );
    
    /** Cerca la informaci� d'un pacs en concret. 
     * @param Cont� la informaci� del pacs cercat
     * @param pacs a cercar 
     * @return estat de l'operaci�
     */
    Status queryPacs( PacsParameters *pacs , int pacsID );
    
private:
    
    DatabaseConnection *m_DBConnect;    
        
    /** Construeix l'estat en que ha finaltizat l'operaci� sol�licitada
     * @param Estat de sqlite
     * @return retorna l'estat de l'operaci�
     */
    Status constructState( int );
    
    /** Comprova si el pacs existeix en estat de baixa, comprovem si el AETitle est� en estat donat de baixa
     * @param Pacs a Trobar 
     * @return estat de l'operaci�
     */
    Status queryPacsDeleted( PacsParameters *pacs );
    
};

};

#endif
