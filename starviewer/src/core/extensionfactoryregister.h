/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGEXTENSIONFACTORYREGISTER_H
#define UDGEXTENSIONFACTORYREGISTER_H

#include "extensionfactory.h"

namespace udg {

/**
    Classe que ens permet registrar una Extension en el ExtensionFactory. Per tal de poder registrar una extensi� hem de declarar
    un objecte del tipus ExtensionFactoryRegister.
    Exemple:
    @code
    ExtensionFactoryRegister<ExtensionName> registerAs("Extension Identifier");
    @endcode
    Amb aquesta simple l�nia de codi ja tenim registrada la nostra extensi�.

    @author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
template <class FactoryType>
class ExtensionFactoryRegister: public GenericSingletonFactoryRegister<QWidget, FactoryType, QString, ExtensionFactory>
{
public:
    ///M�tode
    ExtensionFactoryRegister(QString identifier)
        :GenericSingletonFactoryRegister<QWidget, FactoryType, QString, ExtensionFactory>( identifier )
    {
    }
};

}

#endif
