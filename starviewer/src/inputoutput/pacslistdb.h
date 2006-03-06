/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGPACSLISTDB_H
#define UDGPACSLISTDB_H

#include <sqlite.h>
#include "study.h"
#include <string> 
#include "studymask.h"
#include "seriesmask.h"
#include "series.h"
#include "imagemask.h"
#include "image.h"
#include "studylist.h"
#include "serieslist.h"
#include "imagelist.h"
#include "databaseconnection.h"
#include "pacsparameters.h"
#include "pacslist.h"

#include <string>

class Status;

namespace udg {

/** Aquesta classe implementa les accions necessaries per afegir nous pacs o modificar/consultar els par�metres dels PACS  que tenim disponibles
  * a l'aplicaci�, guardats a la base de dades local
@author marc
*/
class PacsListDB{

private:
    
    DatabaseConnection *m_DBConnect;    
        
    Status constructState(int);
    Status queryPacsDeleted(PacsParameters *pacs);
    
public:

    PacsListDB();
    ~PacsListDB();

    Status insertPacs(PacsParameters *pacs);
    Status queryPacsList(PacsList &);
    Status updatePacs(PacsParameters *pacs);
    Status deletePacs(PacsParameters *pacs);
    Status queryPacs(PacsParameters *pacs,std::string AETitle);
};

};

#endif
