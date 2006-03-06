/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "extensionfactory.h"


namespace udg {

ExtensionFactory::ExtensionFactory(QObject *parent, const char *name)
 : QObject(parent, name)
{
}

ExtensionFactory::~ExtensionFactory()
{
}

bool ExtensionFactory::registerExtension( QString name , ExtensionCreator *extension )
{
    m_registerMap[ name ] = extension;
}

QWidget *ExtensionFactory::createExtension( QString name )
{
    RegisterMapType::const_iterator iterator = m_registerMap.find( name );
    if( iterator != m_registerMap.end() )
    {
        ExtensionCreator *creator = iterator->second;
        QWidget *extension = creator->createExtension( (QWidget*)this , name );
        creator->initializeExtension( extension );
        return extension;
    }
    else
    {
        return 0;
    }
}

};  // end namespace udg 
