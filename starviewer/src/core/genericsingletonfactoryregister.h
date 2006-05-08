/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGGENERICSINGLETONFACTORYREGISTER_H
#define UDGGENERICSINGLETONFACTORYREGISTER_H

namespace udg {

/**
    Classe que ens facilita el registrar una classe en una GenericFactory que sigui singleton. Aquest template ens facilita la feina
    de registrar una classe en un GenericFactory. L'�nica condici� �s que el GenericFactory sigui Singleton. En comptes de crear
    una macroe s'ha escollit fer-ho amb un template ja que aix� ens estalviem els problemes de fer servir macros.
    La manera de registrar una classe �s declarar una variable del tipus GenericSingletonFactoryRegister.

    Tot i aix�, aquesta classe s'hauria d'heredar i nom�s fer servir les heredades d'aquesta. Com a exemple es t� ExtensionFactoryRegister.
    Aix� doncs, aquesta classe queda nom�s reservada per quan es vulgui implementar un nou factory i no per utilitzar-la directament en el
    registre de factory's.

    @author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

template <class AncestorType, class BaseClass, typename ClassIdentifier, class SingletonRegistry>
class GenericSingletonFactoryRegister
{
public:
    
    /**
     * M�tode constructor que ens serveix per registrar una classe amb un id concret en un Factory.
     * @param id Identificador de la classe que es vol registrar
     * @return 
     */
    GenericSingletonFactoryRegister(const ClassIdentifier &id)
    {
        SingletonRegistry::instance()->registerCreateFunction(id, createInstance);
    }

    ///M�tode auxiliar i que no s'hauria d'utilitzar 
    static AncestorType* createInstance(QObject* parent)
    {
        return dynamic_cast<AncestorType*>( new BaseClass(parent) );
    }
};

}

#endif
