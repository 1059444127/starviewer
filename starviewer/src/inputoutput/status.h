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
#include <list.h> 

#ifndef CONST
#include "const.h" 
#endif



/** Aquesta classe, est� basada seguin el patr� Abstract Factory, el que es tracta �s de separar el control d'errors del dctmk, de control
 *d'errors que utilitzarem nosaltres en la nostre aplicaci�, aixi si mai el control d'errors de dcmtk mai canvia, nom�s haurem de modificar 
 *aquesta classe i mai haurem de tocar les classes de la nostra aplicaci� que utilitzi el tractament d'errors. 
 * En aquesta classe encapsulem el tractament d'errors.
 *
 *Per englobar el tractament d'errors de tota l'aplicaci� hi podem tenir dos objectes d'error el OFCondition de dcmtk, i un altre que es propi format per un 
 *std::string que descriu l'error,un boolea que descriu si l'estat �s bo, i un enter que descriu el n�mero de l'error, d'aquesta manera, amb aquesta classe tamb�
 *podem encapsular els errors que no siguin de dctmk, en una sola classe per fer m�s f�cil la gesti� dels errors, ja que per l'aplicaci�, nom�s existir�
 *l'objecte status que controla l'error. Independentment si �s un error dcmtk o provinent d'altres fonts com la cach�. Nom�s podem inserir un dels dos tipus 
 *d'error!
 * 
 *Aquesta Classe tamb� cont� una llista d'errors, per� nom�s per Errors OFCondition, en tots els casos nom�s tindr� un element la llista, per tant l'haurem de   tractar com si no fos una llista
 *exceptuant en el cas que fem una query a m�ltiples PACS, en aquest cas com podem tenir un error per cada PACS, aquesta classe si que es converteix
 *en una aut�ntica llista. Pel reste de casos podem ignorar les accions de llistat com nextStatus, firstStatus, etc..
 *
 */
namespace udg{
class Status{

public :

    Status();
    std::string text();
    Status setStatus(const OFCondition);
    Status setStatus(std::string,bool,int);
    bool good();
    int code();
    void firstStatus();
    void nextStatus();
    bool end();
    
private :

    
    list<OFCondition>m_listStatus;     
    list<OFCondition>::iterator i;
    
    
    std::string m_descText;
    bool m_success;
    int m_numberError;
    bool m_dcmtkError; // descriu la font de l'error, si prove de les dctmk=True  altres fons com la cach�, �s false
};
};
#endif
