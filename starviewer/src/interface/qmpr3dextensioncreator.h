/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQMPR3DEXTENSIONCREATOR_H
#define UDGQMPR3DEXTENSIONCREATOR_H

#include "extensioncreator.h"

namespace udg {

/**
@author Grup de Gr�fics de Girona  ( GGG )
*/
class QMPR3DExtensionCreator : public ExtensionCreator{
Q_OBJECT
public:
    QMPR3DExtensionCreator(QObject *parent = 0, const char *name = 0);

    ~QMPR3DExtensionCreator();
    
    /// creador d'extensions. �s el m�tode del facory encarregat de proporcionar l'extensi� que demanem
    /// \TODO ara per ara retorna QWidget, per� �s possible que canviem a una altra classe que faci d'interf�cie per a les extensions [p.ex. ExtensionBase]
    /// aquest m�tode �s purament abstracte en el pare
    virtual QWidget *createExtension( QWidget *parent , QString name );
    /// inicialitzador d'extensions
    /// \TODO �dem anterior amb QWidget
    /// aquest m�tode �s purament abstracte en el pare
    virtual bool initializeExtension( QWidget *extension  /* , Resource *rsrc */);
};

};  //  end  namespace udg {

#endif
