/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

 
#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <qobject.h>

namespace udg{
class QApplicationMainWindow;
}

/**
    Classe base Director. La seva funci� ser� coordinar les accions que l'usuari realitzi a trav�s de la interf�cie amb els par�metres interns de cada m�tode, interactuant amb la interf�cie.
    Aquesta classe fa de pont entre la interf�cie d'usuari i la implementaci� pura
    d'un m�tode.
    Aquesta �s una classe abstracta de la qual s'heredar� una sub-classe per cada m�tode implementat
    que proporciona la interf�cie comuna per a totes.
    
    Aquesta classe s'encarregar� per exemple d'actualitzar els par�metres d'un m�tode de segmentaci� que
    un usuari est� manipul�lant a trav�s d'un usuari, de dirigir la seva execuci� i validaci� dels 
    par�metres, l'apertura de noves finestres, mostrant resultats, vistes, etc.
    
    En el constructor li proporcionem un punter a la finestra principal perqu� pugui interactuar amb 
    ella.
    
    L'slot execute �s el que s'encarrega de l'exeucuci� d'un cert algorisme, mostrar finestres, etc.
    aqu� �s purament virtual, ja que cada sub-classe l'haur� d'implementar de diferent manera.
*/

namespace udg{

class Director : public QObject{

Q_OBJECT

public:
    
    Director( udg::QApplicationMainWindow* app, QObject *parent = 0, const char *name = 0 );
    virtual ~Director();
    
public slots:
    /** 
        Aquest slot s'encarrega d'executar totes les tasques del m�tode un cop tenim els par�metres
        a punt. Entre aquestes tasques tindrem l'execuci� de l'algorisme i la mostra de resultats.
        Aquest slot �s purament virtual i haur� de ser reimplementat en cadascuna de les subclasses
    */
    virtual void execute() = 0;

protected:
    udg::QApplicationMainWindow* m_applicationWindow;
    
};

}; // end namespace udg
#endif

