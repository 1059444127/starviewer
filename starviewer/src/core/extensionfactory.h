/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGEXTENSIONFACTORY_H
#define UDGEXTENSIONFACTORY_H

#include "genericfactory.h"
#include "singleton.h"

#include <QString>
#include <QWidget>

namespace udg {

/** \fn typedef Singleton<GenericFactory<QWidget, QString> > ExtensionFactory
    Typedef per la classe que serveix per crear una extensi� en temps d'execuci�. Aquesta �s d'�s intern a l'hora de registrar una extensi�.

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

typedef Singleton<GenericFactory<QWidget, QString> > ExtensionFactory;

}

#endif
