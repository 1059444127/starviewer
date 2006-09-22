/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCACHEPOOL_H
#define UDGCACHEPOOL_H

#include "databaseconnection.h"

namespace udg {

class Status;
class string;

/** Classe que s'encarrega de gestionar l'spool de la cach�
@author Grup de Gr�fics de Girona  ( GGG )
*/

class CachePool
{
public:
    
    static const unsigned int MinimumMBytesOfDiskSpaceRequired = 1000;///<Espai m�nim lliure requerit al disc dur
    static const unsigned int MBytesToEraseWhenDiskOrCacheFull = 2000;///<Si la cache esta plena, s'allibera aquesta quanttiat de Mb en estudis vells

    /// Constructor de la classe
    CachePool();

    /** Esborra un estudi de l'spool de l'aplicaci�
     * @param path absolut de l'estudi
     */
    void removeStudy( std::string studyUID );    

    /** actualitza l'espai utiltizat de la cache a 0 bytes
     * @return estat el m�tode
     */
    Status resetPoolSpace();
    
    /** Actualitza l'espai utilitzat de cach�, amb la quantitat de bytes passats per par�metre
     * @param mida a actualitzar
     * @return retorna estat del m�tode
     */
    Status updatePoolSpace( int );
    
    /** Retorna l'espai que estem utilitzan de l'spool en Mb
     * @param  Espai ocupat del Pool (la cach�) actualment en Mb
     * @return estat el m�tode
     */
    Status getPoolUsedSpace( unsigned int &usedSpace );
    
    /** Retorna l'espai m�xim que pot ocupar el Pool(Tamany de la cach�)
     * @param  Espai assignat en Mb que pot ocupar el Pool (la cach�)
     * @return estat el m�tode
     */
    Status getPoolTotalSize( unsigned int &totalSize );
    
    /** actualitza l'espai m�xim disponible per a la cach�
     * @param  Espai en Mb que pot ocupar la cach�
     * @return estat el m�tode
     */
    Status updatePoolTotalSize( int );    
    
    /** Calcula l'espai lliure disponible a la Pool
     * @param space  Espai lliure en Mb de la Pool (la cach�)
     * @return estat el m�tode
     */
    Status getPoolFreeSpace( unsigned int &freeSpace );

    ///Destructor de classe
    ~CachePool();

private:
    
    DatabaseConnection *m_DBConnect;
    
};
};

#endif
