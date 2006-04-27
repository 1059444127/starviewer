/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQDEFAULTVIEWEREXTENSIONCREATOR_H
#define UDGQDEFAULTVIEWEREXTENSIONCREATOR_H

#include "extensioncreator.h"

namespace udg {

/**
Creador de l'extensi� QDefaultViewerExtension

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class QDefaultViewerExtensionCreator : public ExtensionCreator
{
Q_OBJECT
public:
    QDefaultViewerExtensionCreator( QObject* parent = 0, const char* name = 0 );

    ~QDefaultViewerExtensionCreator();

    /// creador d'extensions. �s el m�tode del facory encarregat de proporcionar l'extensi� que demanem
    /// \TODO ara per ara retorna QWidget, per� �s possible que canviem a una altra classe que faci d'interf�cie per a les extensions [p.ex. ExtensionBase]
    /// aquest m�tode �s purament abstracte en el pare
    virtual QWidget *createExtension( QWidget *parent , QString name );
    /// inicialitzador d'extensions
    /// \TODO �dem anterior amb QWidget
    /// aquest m�tode �s purament abstracte en el pare
    virtual bool initializeExtension( QWidget *extension  /* , Resource *rsrc */);
};

}

#endif
