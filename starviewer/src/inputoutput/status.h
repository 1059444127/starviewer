/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#ifndef STATUS
#define STATUS
#define HAVE_CONFIG_H 1

#include <ofcond.h> //provide the OFcondition structure and his members
#include <string>

#ifndef CONST
#include "const.h" 
#endif

/** Aquesta classe proporciona el control d'errors, dels m�todes invocats. 
 *Aquesta classe, est� basada seguin el patr� Abstract Factory, el que es tracta �s de separar el control d'errors del dctmk, de control
d'errors que utilitzarem nosaltres en la nostre aplicaci�, aixi si mai el control d'errors de dcmtk mai canvia, nom�s haurem de modificar aquesta classe i mai haurem de tocar les classes de la nostra aplicaci� que utilitzi el tractament d'errors. 
 * En aquesta classe encapsulem el tractament d'errors.
 *
 *Per englobar el tractament d'errors de tota l'aplicaci� hi podem tenir dos objectes d'error el OFCondition de dcmtk, i un altre que es propi format per un 
 *std::string que descriu l'error,un boolea que descriu si l'estat �s bo, i un enter que descriu el n�mero de l'error, d'aquesta manera, amb aquesta classe tamb�
 *podem encapsular els errors que no siguin de dctmk, en una sola classe per fer m�s f�cil la gesti� dels errors, ja que per l'aplicaci�, nom�s existir�
 *l'objecte status que controla l'error. Independentment si �s un error dcmtk o provinent d'altres fonts com la cach�. Nom�s podem inserir un dels dos tipus 
 *d'error!
 * 
 */
namespace udg{

class Status
{

public :

    Status();

	/** Retorna un text descrivint l'error o l'exit del proc�s
	 * @return text descriptiu de l'exit o l'error de la crida
	 */
    std::string text();

	/** per contruir l'objecte status
 	 * @param OFCodition resultant d'una crida
	 * @return retorna l'objecte status
	 */
    Status setStatus( const OFCondition );

	/** Crea un objecte Status
	 * @param descripci� de l'status
	 * @param indica si s'ha finalitzat amb �xit
	 * @param n�mero d'erro en cas d'haver-ni
	 * @return retorna l'objecte status  
 	 */
    Status setStatus( std::string , bool , int );

	/** Retorna si l'�xit de l'element cridat
	 * @return boolea indicant l'exit de la crida
	 */
    bool good();

	/** Retorna el codi d'estat
	 * @return codi d'estat 
  	 */
    int code();
    
private :

    std::string m_descText;
    bool m_success;
    int m_numberError;
};
}; // end namespace udg
#endif
