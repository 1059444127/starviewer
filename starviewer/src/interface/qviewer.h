/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQVIEWER_H
#define UDGQVIEWER_H

#include <QWidget>
#include "point.h"

// Fordward declarations
class QVTKWidget;
class vtkRenderer; 
class vtkRenderWindowInteractor;

namespace udg {

/**
Classe base per a totes les finestres de visualitzaci�

@author Grup de Gr�fics de Girona  ( GGG )
*/

// Fordward declarations
class Volume;

class QViewer : public QWidget{
Q_OBJECT
public:
    QViewer(QWidget *parent = 0);
    ~QViewer();

    /// Retorna l'interactor renderer
    virtual vtkRenderWindowInteractor *getInteractor() = 0;
    /// Retorna el renderer
    virtual vtkRenderer *getRenderer() = 0;
    /// Indiquem les dades d'entrada
    virtual void setInput(Volume* volume) = 0;
    /// Ens retorna el volum d'entrada
    virtual Volume* getInput( void ) { return m_mainVolume; }
    /// Ens retorna el punt del model que es correspon amb el punt de la finestra sobre el qual es troba el cursor desde l'�ltim event
    Point getModelPointFromCursor() const { return m_modelPointFromCursor; } 
    
    /// Retorna la posici� sobre la que es troba el cursor ( coordenades de mon )
    void getCurrentCursorPosition( double xyz[3] )
    {
        xyz[0] = m_currentCursorPosition[0];
        xyz[1] = m_currentCursorPosition[1];
        xyz[2] = m_currentCursorPosition[2];
    }
    /// Retorna el valor de la imatge que hi ha sota el cursor
    double getCurrentImageValue() const { return m_currentImageValue; }
    
    /// Passa coordenades de display a coordenades de m�n i viceversa \TODO aquest metode haurioa de ser virtual al pare
    static void computeDisplayToWorld( vtkRenderer *renderer , double x , double y , double z , double worldPoint[3] );    
    static void computeWorldToDisplay( vtkRenderer *renderer , double x , double y , double z , double displayPoint[3] );
    
public slots:
    /// For�a l'execuci� de la visualitzaci�
    virtual void render() = 0;
    
protected:
    /// El volum a visualitzar
    Volume* m_mainVolume;
    /// El widget per poder mostrar una finestra vtk amb qt
    QVTKWidget* m_vtkWidget;
    /// El punt del model que es correspon amb el punt de la finestra sobre el qual es troba el cursor desde l'�ltim event
    Point m_modelPointFromCursor;
    
    /// Posici� sobre la que es troba el ratol�
    double m_currentCursorPosition[3];
    /// Valor de la imatge corresponent a la posici� on es troba el ratol�. Quan la posici� est� fora del model li assignarem un valor "d'invalidesa" \TODO definir aquest valor, de moment fem servir -1 ( err�niament )
    double m_currentImageValue;

    
    
};

};  //  end  namespace udg {

#endif
