/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGEXTENSIONCREATOR_H
#define UDGEXTENSIONCREATOR_H

#include <QObject>
#include <QString>

class QWidget;

namespace udg {


/**
Creador d'extensions

@author Grup de Gr�fics de Girona  ( GGG )
*/
class ExtensionCreator : public QObject {
Q_OBJECT
public:
    ExtensionCreator( QObject *parent, const char *name );

    ~ExtensionCreator();
    
    /// creador d'extensions. �s el m�tode del factory encarregat de proporcionar l'extensi� que demanem
    /// \TODO ara per ara retorna QWidget, per� �s possible que canviem a una altra classe que faci d'interf�cie per a les extensions [p.ex. ExtensionBase]
    virtual QWidget *createExtension( QWidget *parent , QString name ) = 0;
    /// inicialitzador d'extensions
    /// \TODO �dem anterior amb QWidget
    virtual bool initializeExtension( QWidget *extension  /* , Resource *rsrc */) = 0;
    

};

};  //  end  namespace udg {

#endif
