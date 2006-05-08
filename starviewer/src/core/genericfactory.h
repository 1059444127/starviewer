/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGGENERICFACTORY_H
#define UDGGENERICFACTORY_H

#include <map>
#include <QObject>

namespace udg {

/**
    Classe que serveix com a base per implementar el pattern Factory.
    L'�s d'aquesta classe �s intern a la plataforma i no s'hauria d'utilitzar excepte si s'est� desenvolupant per el core.
    La seva utilitat, juntament amb GenericFactoryRegister, �s la de prove�r una implementaci� gen�rica del pattern Factory.

    Aquesta classe ens �s �til a l'hora de generar classes d'una mateixa jerarquia de classes. Per exemple, si tinguessim
    la classe Vehicle com a classe abstracta i les classes Camio, Cotxe, Motocicleta com a classes que hereden de Vehicle,
    podr�em utilitzar la GenericFactory per instanciar objectes del tipus Camio, Cotxe o Motocicleta per� que fossin retornats
    com a Vehicles.

    L'implementaci� actual pressuposa que tots els objectes que es voldran crear heredaran de QObject i, per tant, el seu constructor
    t� un par�metre que �s el parent d'aquest.

    Exemple d'utilitzaci�:
    @code
     //Creem una Factory de Vehicles que seran identificats per una string. Vehicles i els seus fills s�n subclasses de QObject
     typedef GenericFactory<Vehicle, std::string> VehicleFactory;

     VehicleFactory vehicles;
     
     //.. Aqu� haur�em de registrar les diferents classes amb el Factory.
     //   Veure GenericFactoryRegister per tenir una manera de fer-ho senzillament
     
     Vehicle* vehicle = vehicles->create("cotxe");
     Vehicle* vehicle2 = vehicles->create("motocicleta");

     std::cout<< "Total de rodes = " << vehicle->getNumeroRodes() + vehicle2->getNumeroRodes() << std::endl;

     //.. Aix� imprimiria "Total de rodes = 6" suposant que cotxe retornes 4 i motocicleta 2.
    @endcode
    Tot i que en l'exemple no es faci, caldria mirar si l'objecte retornat �s NULL o no ho �s.
    
    Aquesta classe s'utilitzar�, la majoria de les vegades, amb un singleton per facilitar-ne el registre i l'acc�s per� no t� perqu�.
    
    @TODO Si s'utilitza aquesta classe conjuntament amb un singleton nom�s es podr� tenir un objecte de cada tipus.
    @TODO En cas que fos necessari s'hauria de fer l'implementaci� m�s gen�rica per permetre de 0 a n par�metres en el constructor i no
          no obligar a que els objectes creat heretin de QObject.
    @author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
    @see GenericSingletonFactoryRegister
    @see ExtensionFactory
*/

template <class BaseClass, typename ClassIdentifier>
class GenericFactory
{
    typedef BaseClass* (*BaseClassCreateFunction)(QObject*);
    typedef std::map<ClassIdentifier, BaseClassCreateFunction> FunctionRegistry;

public:
    ///Constructor de la classe
    GenericFactory(){}

    /**
     * M�tode que serveix per registrar una funci� de creaci� d'una classe.
     * Aquest m�tode �s el que s'ha de fer servir per tal de poder registrar una determinada classe en el factory.
     * @param className Nom de la classe que es vol registrar.
     * @param function Funci� del tipus BaseClassCreateFunction que ens retorna un objecte de la classe className.
     *                 Aquest m�tode el dona, autom�ticament, la classe GenericFactoryRegister.
     */
    void registerCreateFunction(const ClassIdentifier &className, BaseClassCreateFunction function)
    {
        m_registry[className] = function;
    }

    /**
     * M�tode que ens crea l'objecte que vingui definit per l'identificador. El retorna del tipus BaseClass.
     * @param className Nom de la classe que volem que faci l'objecte
     * @param parent QObject pare de l'objecte que es crear�.
     * @return Retorna l'objecte convertit a la classe base BaseClass. En cas que no trobi l'objecte o error retornar� NULL.
     */
    BaseClass* create(const ClassIdentifier &className, QObject* parent = 0) const
    {
        BaseClass* theObject = NULL;
        
        typename FunctionRegistry::const_iterator regEntry = m_registry.find(className);

        if (regEntry != m_registry.end()) 
        {
            try
            {
                theObject = regEntry->second(parent);
            }
            catch (std::bad_alloc)
            {
                theObject = NULL;
            }
        }
        return theObject;
    }

private:
    FunctionRegistry m_registry;
};

}

#endif
