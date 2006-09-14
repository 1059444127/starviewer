/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/


#ifndef UDGQINPUTPARAMETERS_H
#define UDGQINPUTPARAMETERS_H

#include <QWidget>

namespace udg {

/**
    Classe base per als widgets destinats a introdu�r par�metres.
    Els par�metres es poden introdu�r ja de forma simple i cl�ssica com pot ser amb 
    caixes de text i similar o de formes m�s complexes com amb histogrames o d'altres.
    
    Aquesta ofereix la interf�cie comuna i b�sica de signals o slots perqu� les dades
    es mantinguin en sincronisme amb la resta de la interf�cie.

@author Grup de Gr�fics de Girona  ( GGG )
*/

class QInputParameters : public QWidget{
Q_OBJECT
public:
    QInputParameters( QWidget *parent = 0 );

    virtual ~QInputParameters();

    /**
        Activa/desactiva l'emissi� de la senyal "parameterChanged(int)"
    */
    void disableIndividualSincronization();
    void enableIndividualSincronization();
    bool isIndividualSincronizationEnabled() const { return m_individualSincronization; };
    
public slots:
    /**
        Slot que ens serveix per indicar que hem d'actualitzar el parametre que ens diguin mitjan�ant 
        un identificador (que, en realitat, ser� un enum). Serveix per canviar els valors a partir 
        d'una classe Parameters
    */
    virtual void readParameter(int index) /*= 0*/{}; 
    /**
        Escriu tots els valors de par�metres que t� actualment al Parameters associat
    */
    virtual void writeAllParameters() /*= 0*/{};

private:
    /// indicar� si els par�metres s'actualitzen 'en viu' o d'una tacada
    bool m_individualSincronization;
};

};  

#endif
