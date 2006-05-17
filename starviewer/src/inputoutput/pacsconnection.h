/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#ifndef PACSCONNECTION
#define PACSCONNECTION
#define HAVE_CONFIG_H 1

#include <dimse.h> // provide the structure DcmDataSet
#include <dcdeftag.h> //provide the information for the tags
#include <ofcond.h> //provide the OFcondition structure and his members
#ifndef CONST
#include "const.h"    
#endif

/** Aquesta classe encapusa el objecte connexi�, que hauran d'utilitzar les classes per fer les seves crides al PACS. Aquesta classe segueix el patr� Abstract   *Factory. A�llem l'objecte connexi� de les dctmk, d'aquesta manera si mai canvia l'objecte de connexi� de les dcmtk, nom�s haurem de tocar aquesta classe 
 */
namespace udg{
class PacsConnection{

public :

	/** retorna l'objecte connexi� de les dcmtk, perqu� altres classes les puguin utilitzar per connectar-se amb el PACS i fer-li les crides, pertitents, com la classe QueryStudy,QuerySeries 
     * @return retorna una connexi� dctmk
     */
    T_ASC_Association* getPacsConnection();

	/** Inserim a l'objecte, la connexi� dcmtk que utilitzarem per connectar-nos en el pacs
     * @param estableix la connexio a utilitzar
     */
    void setPacsConnection( T_ASC_Association * );

private :
    
    T_ASC_Association *  m_assoc;

};
};
#endif
