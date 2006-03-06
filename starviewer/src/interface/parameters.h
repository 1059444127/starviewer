/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

 
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <qobject.h>

/**
    Classe base Parameters. Aquesta classe s'encarrega d'encapsular els par�metres d'un m�tode
    en concret. Aquesta proporciona la interf�cie comuna per a totes les que heredin d'aquesta.
    
    Proporciona un signal changed(int) el qual indicar� quin par�metre s'ha actualitzat. El par�metre 
    d'aquest signal ser� un �ndex que identifica quin par�metre ha canviat. Les sub-classes tindran
    un tipus enumerat que far� d'�ndex per cada par�metre
*/

namespace udg{

class Parameters : public QObject{
Q_OBJECT
public:
    Parameters(QObject *parent = 0, const char *name = 0);

    virtual ~Parameters();

signals:
    /// indica a les interf�cies que un dels seus par�metres s'ha actualitzat
    /// El par�metre �s un enter que identifica el par�metre
    void changed( int ); 

};

}; // end namespace udg
#endif
// fi de #endif
