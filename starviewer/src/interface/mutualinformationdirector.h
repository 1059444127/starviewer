/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
 
#ifndef MUTUALINFORMATIONDIRECTOR_H
#define MUTUALINFORMATIONDIRECTOR_H

#include "director.h"
#include "volume.h"
#include "mutualinformationviolawells.h"


namespace udg{

//Fordware declaration
class MutualInformationParameters;

/**
    Sub-classe de Director. En aquest cas fa de pont amb un m�tode de registre basat en mesures 
    d'informaci� m�tua.
    
    Aquesta inclou un punter a la classe de par�metres espec�fics del seu m�tode, la classe que 
    encapsula el m�tode de registre ( MutualInformationViolaWells )
    
    Totes les classes heredades de Director, inclouran un m�tode setParameters on l'indicarem el 
    punter a la classe de par�metres corresponent.
    Tamb� implementaran l'slot virtual apply, on es far� la feina m�s important de l'execuci� d'un 
    m�tode
    
*/

class MutualInformationDirector : public Director
{

 Q_OBJECT
 
public:

    // Com que el m�tode treballa amb dades itk, hem d'extreure el tipus itk
    // per declarar el m�tode que fa servir templates.
    typedef udg::Volume::ItkImageType ImageType;
    typedef udg::MutualInformationViolaWells< ImageType, ImageType > MethodType;    
    
    MutualInformationDirector( udg::QApplicationMainWindow* app, QObject *parent = 0, const char *name = 0 );

    ~MutualInformationDirector();

    
    void setParameters( MutualInformationParameters* parameters );
    
    
public slots:
    /**
        Aquest m�tode aplicar el m�tode de registre. Comprova primer si t� els par�metres
        necessaris, sin� retornar� fals. Slot p�blic heredat de Director.
    */
    void execute();

private:

    MethodType *m_method;
    MutualInformationParameters *m_parameters;
    
    udg::QApplicationMainWindow* m_applicationWindow;
    
    udg::Volume* getRegisteredImage(); 
};

}; // end namespace udg

#endif
