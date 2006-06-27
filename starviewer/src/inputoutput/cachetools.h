/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCACHETOOLS_H
#define UDGCACHETOOLS_H

class string;

namespace udg {

class Status;

/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class CacheTools{
public:
    CacheTools();

    /** Compacta la base de dades de la cache, per estalviar espai
     * @return estat del m�tode  
     */
    Status compactCachePacs();
    
    ~CacheTools();

};

}

#endif
