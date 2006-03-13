/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQ2DVIEWER_H
#define UDGQ2DVIEWER_H

#include "qviewer.h"

// Fordward declarations
// qt
class QEvent;
class QAction;
// vtk
class vtkImageViewer2;
class vtkCellPicker;
class vtkTextActor;
class vtkTextMapper;
class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkCornerAnnotation;

namespace udg {

// Forward delcarations
class DistanceTool;

/**

Classe base per als visualitzadors 2D.
El mode d'operaci� habitual ser� el de visualitar un sol volum.
Normalment per poder visualitzar un volum farem el seg�ent

Q2DViewer* visor = new Q2DViewer();
visor->setInput( volum );
visor->setView( Q2DViewer::Axial );
visor->render();

En el cas que desitjem solapar dos volums haurem d'indicar el volum solapat mab el m�tode setOverlayInput().
Quan solapem volums tenim 3 maneres de solapar aquests volums, amb un patr� de checkerboard, aplicant un blending o un rectilinearWipe, en aquest cas haur�em de fer servir el m�tode setOverlay() indicant una de les opcions, Blend, CheckerBoard o RectilinearWipe
\TODO acabar la doc sobre solapament 

Per defecte el visualitzador mostra la llesca central de la vista seleccionada, si volem canviar al llesca farem servir el m�tode setSlice().

@author Grup de Gr�fics de Girona  ( GGG )
*/

// Fordward declarations
class Volume;

class Q2DViewer  : public QViewer{
Q_OBJECT
public:

    // Axial: XY, Coronal: XZ, Sagittal: YZ
    enum ViewType{ Axial, Coronal, Sagittal , None };
    // tipus de fusi� dels models
    enum OverlayType{ Blend , CheckerBoard , RectilinearWipe };


    /// Tools que proporciona... NotSuported �s una Tool fictica que indica que la tool en �s a 'aplicaci� no �s aplicable a aquest visor, per tant podr�em mostrar un cursor amb signe de prohibici� que indiqui que no podem fer res amb aquella tool
    enum Tools{ Zoom , Rotate , Move , Pick , Distance , Cursor , Custom , NotSuported , NoTool };
    
    Q2DViewer( QWidget *parent = 0 );
    ~Q2DViewer();
    
    /// Li indiquem quina vista volem del volum: Axial, Coronal o Sagital
    void setView( ViewType view );
    void setViewToAxial(){ setView( Q2DViewer::Axial ); }
    void setViewToCoronal(){ setView( Q2DViewer::Coronal ); }
    void setViewToSagittal(){ setView( Q2DViewer::Sagittal ); }
    /// ens retorna la vista que tenim en aquells moments del volum
    ViewType getView() const { return m_lastView; }

    /// Ens retorna l'ImageViewer
    vtkImageViewer2* getImageViewer(){ return m_viewer; }
    
    virtual vtkRenderer *getRenderer();
    virtual vtkRenderWindowInteractor *getInteractor();            
    virtual void setInput( Volume* volume );
    
    /// Afegim el volum solapat
    void setOverlayInput( Volume* volume );    
    
    // M�todes espec�fics checkerboard
    /// Obtenim el nombre de divisions
    int * getDivisions();
    void getDivisions( int data[3] );
    /// Indiquem el nombre de divisions del checkerboard
    void setDivisions( int data[3] );
    
    /// Retorna la tool que s'est� fent servir en aquell moment
    Tools getCurrentTool(){ return m_currentTool; };

public slots:  

    // Temporal per proves, veurem quins events es criden
    void eventHandler( vtkObject * obj, unsigned long event, void * client_data, void* call_data, vtkCommand * command );
    
    /// Indiquem si volem veure la informaci� del volum per pantalla
    void displayInformationText( bool display );
    
    /// Li indica la tool que es fa servir en aquell moment
    bool setCurrentTool( Tools toolName ){ m_currentTool = toolName; return true; };
    ///M�todes alternatius per activar Tools
    void setCurrentToolToZoom(){ m_currentTool = Zoom; };
    void setCurrentToolToRotate(){ m_currentTool = Rotate; };
    void setCurrentToolToMove(){ m_currentTool = Move; };
    void setCurrentToolToCustom(){ m_currentTool = Custom; };
    void setCurrentToolToPick(){ m_currentTool = Pick; };
    void setCurrentToolToDistance(){ m_currentTool = Distance; };
    
    /// Fa que una determinada tool quedi des/activada \TODO fer m�tode espec�fic per cadascuna
    bool enableTool( Tools toolName , bool enable = true ){ return true; };
    // void enableZoomTool(bool enable = true);
    // etc...
    
    /// retorna la llesca actual
    int getSlice( void ){ return m_currentSlice; }
    /// canvia la llesca que veiem de la vista actual
    void setSlice(int value);
    /// indica el tipu de solapament dels volums, per defecte checkerboard
    void setOverlay( OverlayType overlay ){ m_overlay = overlay; }
    void setOverlayToBlend(){ setOverlay( Q2DViewer::Blend ); };
    void setOverlayToCheckerBoard(){ setOverlay( Q2DViewer::CheckerBoard ); };
    void setOverlayToRectilinearWipe(){ setOverlay( Q2DViewer::RectilinearWipe ); };
    
    /// Actualitza els valors on apunta el cursor
    void updateCursor( double x, double y , double z , double value )
    {
        m_currentCursorPosition[0] = x;
        m_currentCursorPosition[1] = y;
        m_currentCursorPosition[2] = z;

        m_currentImageValue = value;
    }
    /// Actualitza els valors de window level
    void updateWindowLevel();
    
    virtual void render();
    
    // M�todes espec�fics checkerboard
    /// Indiquem el nombre de divisions del checkerboard
    void setDivisions( int x , int y , int z );

    /// Fa reset dels par�metres inicials \TODO:  no es del tot complet el reset
    void reset()
    {    
        updateView();
    }
    void contextMenuRelease( vtkObject *object, unsigned long event, void *client_data, vtkCommand * command);
    
private slots:  
    /// refrescar� les dades del texte que es mostrar� en pantalla
    void updateInformationText();

protected:
    vtkEventQtSlotConnect *m_vtkQtConnections;
    /// Aquest m�tode es fa servir perqu� es mostri un men� contextual
    //virtual void contextMenuEvent( QContextMenuEvent *event );
    
    /// Acci� temporal de proves. Simplement fa un reset mostrant la llesca central de nou
    QAction *m_resetAction;
    /// per a les vistes 2D farem servir el vtkImageViewer2
    vtkImageViewer2 *m_viewer;
    /// conserva la vista actual
    ViewType m_lastView;    
    /// La llesca actual que estem visualitzant
    int m_currentSlice;
    /// Aquest �s el segon volum afegit a solapar
    Volume* m_overlayVolume;
    /// El nombre de divisions per cada dimensi�
    int m_divisions[3];
    /// El picker per anotar punts de la imatge
    vtkCellPicker *m_cellPicker;
    
    
    /// Textes informatius de l'image actor , ens estalviar� molta feina
    vtkCornerAnnotation *m_textAnnotation;
    
    /// Calcula el window/level inicial per defecte de la imatge
    void resetWindowLevel();
    /// Actualitza la vista en el rendering
    void updateView();

    // Processem l'event de resize de la finestra Qt
    virtual void resizeEvent( QResizeEvent* resize );

private:

    
    /// inicialitza els strings d'informaci� que es mostraran a la pantalla
    void initInformationText();
    /// accions que es fan quan es mou el ratol� (es crida dins d'event handler)
    void onMouseMove();
    /// La tool de dist�ncia
    DistanceTool *m_distanceTool;
    /// Indica la Tool que s'est� fent servir en aquell moment
    Tools m_currentTool;
    /// Tipu de solapament dels volums en cas que en tinguem m�s d'un
    OverlayType m_overlay;
    
    /// configuraci� de la interacci� amb l'usuari
    void setupInteraction();
    
    /// Crea i inicialitza totes les accions d'aquest widget
    void createActions();
    /// Crea i inicialitza totes les tools d'aquest widget
    void createTools();
    
    /// S'encarrega de fer la feina per defecte que cal fer cada cop que s'invoca qualsevol event, com per exemple registrar el punt sobre el qual es troba el mouse
    void anyEvent();

    /// Els strings amb els textes de cada part de la imatge
    QString m_lowerLeftText, m_lowerRightText, m_upperLeftText, m_upperRightText;
    /// El format de cada text ->\TODO aix� acabar� deprecated \DEPRECATED
//     QString m_formatedUpperLeftString, m_formatedUpperLeftOffImageString,  m_formatedUpperRightString , m_formatedLowerLeftString , m_formatedLowerRightString;
    
    
signals:
    /// envia la nova llesca en la que ens trobem
    void sliceChanged(int);
    /// indica que alguna de la informaci� que s'ha de mostrar per pantall ha canviat
    void infoChanged( void );
    
    
};



};  //  end  namespace udg {

#endif
