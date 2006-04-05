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
class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkCornerAnnotation;
class vtkAxisActor2D;
class vtkWindowToImageFilter;
class vtkLookupTable;

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

Podem escollir quines annotacions textuals i de refer�ncia apareixeran en la vista 2D a trav�s dels flags "AnnotationFlags" definits com enums. Aquests flags es poden passar en el constructor o els podem modificar a trav�s dels m�todes \c addAnnotation() o \c removeAnnotation() que faran visible o invisible l'anotaci� indicada. Per defecte el flag �s \c AllAnnotation i per tant es veuen totes les anotacions per defecte.

@author Grup de Gr�fics de Girona  ( GGG )
*/

// Fordward declarations
class Volume;

class Q2DViewer  : public QViewer{
Q_OBJECT
public:
    /// Actualitzaci� d'anotacions v�ries
    void updateWindowLevelAnnotation();
    
    /// Axial: XY, Coronal: XZ, Sagittal: YZ
    enum ViewType{ Axial, Coronal, Sagittal , None };

    /// tipus de fusi� dels models
    enum OverlayType{ Blend , CheckerBoard , RectilinearWipe };

    /// Botons del mouse
    enum Buttons{ LeftButton , MiddleButton , RightButton };

    /// tipus d'accions associables als botons
    enum Actions{ CursorAction , SliceMotionAction , WindowLevelAction };

    /// Aquests flags els farem servir per decidir quines anotacions seran visibles i quines no
    enum AnnotationFlags{ NoAnnotation = 0x0 , WindowLevelAnnotation = 0x2 , ReferenceAnnotation = 0x4 , AllAnnotation = 0x6 };
    
    /// Tools que proporciona... NotSuported �s una Tool fictica que indica que la tool en �s a 'aplicaci� no �s aplicable a aquest visor, per tant podr�em mostrar un cursor amb signe de prohibici� que indiqui que no podem fer res amb aquella tool
    enum Tools{ Zoom , Rotate , Move , Pick , Distance , Cursor , Custom , NotSuported , NoTool , Manipulate };

    enum ManipulateState{ Ready , Picked };
    ManipulateState m_manipulateState;

    Q2DViewer( QWidget *parent = 0 , unsigned int annotations = Q2DViewer::AllAnnotation );
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

    /// Desa totes les llesques que es veuen al visor amb el nom de fitxer base \c baseName i en format especificat per \c extension
    void saveAll( const char *baseName , FileType extension );

    /// Desa la vista actual del visor amb el nom de fitxer base \c baseName i en format especificat per \c extension
    void saveCurrent( const char *baseName , FileType extension );

    /// Assigna la LUT en format vtk
    void setVtkLUT( vtkLookupTable * lut );

    /// Retorna la LUT en format vtk
    vtkLookupTable *getVtkLUT();
    
public slots:  

    /// Temporal per proves, veurem quins events es criden
    void eventHandler( vtkObject * obj, unsigned long event, void * client_data, vtkCommand * command );
    
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
    void setCurrentToolToManipulate(){ m_currentTool = Manipulate; };
    
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
    
    /// Afegir o treure la visibilitat d'una anotaci� textual
    void addAnnotation( AnnotationFlags annotation )
    {
        m_enabledAnnotations = m_enabledAnnotations | annotation;
        updateAnnotations();
    }
    void removeAnnotation( AnnotationFlags annotation )
    {
        // si existeix, llavors la "restem", sin� no cal fer res
        if( m_enabledAnnotations & annotation )
        {
            m_enabledAnnotations -= annotation;
            updateAnnotations();
        }
        
    }

    /// Ajusta el window/level
    void setWindowLevel( double window , double level );

    /// M�todes per donar diversos window level per defecte
    void resetWindowLevelToDefault();
    void resetWindowLevelToBone();
    void resetWindowLevelToEmphysema();
    void resetWindowLevelToSoftTissuesNonContrast();
    void resetWindowLevelToLiverNonContrast();
    void resetWindowLevelToSoftTissuesContrastMedium();
    void resetWindowLevelToLiverContrastMedium();
    void resetWindowLevelToNeckContrastMedium();
    void resetWindowLevelToAngiography();
    void resetWindowLevelToOsteoporosis();
    void resetWindowLevelToPetrousBone();
    void resetWindowLevelToLung();
    
protected:
    /// asscociaci� de botons amb accions
    int m_leftButtonAction , m_middleButtonAction , m_rightButtonAction;

    /// �ltim bot� que s'ha clicat
    int m_lastButtonPressed;

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

    /// actor que agafem de l'escena
    vtkAxisActor2D *m_pickedAxisActor;

    /// Posicions de l'actor agafat
    double* m_pickedActorPosition;
    int m_initialPickedActorPositionX , m_initialPickedActorPositionY;
    int m_initialPickX, m_initialPickY;
    
    /// Textes informatius de l'image actor , ens estalviar� molta feina
    vtkCornerAnnotation *m_textAnnotation;
    
    /// Actualitza la vista en el rendering
    void updateView();

    // Processem l'event de resize de la finestra Qt
    virtual void resizeEvent( QResizeEvent* resize );

private:
    /// flag que ens indica quines anotacions es veuran per la finestra
    unsigned int m_enabledAnnotations;

    /// Refresca les anotacions que s'han de veure i les que no
    void updateAnnotations();
    
    /// inicialitza els strings d'informaci� que es mostraran a la pantalla
    void initInformationText();

    /// accions associades a interacci� amb el ratol�
    void onMouseMove();
    void onLeftButtonUp();
    void onLeftButtonDown();
    void onMiddleButtonUp();
    void onMiddleButtonDown();
    void onRightButtonUp();
    void onRightButtonDown();

    /// Accions
    void startCursor();
    void startSliceMotion();
    void startWindowLevel();
    void stopCursor();
    void stopSliceMotion();
    void stopWindowLevel();
    
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

    /// Crea i inicialitza totes les anotacions que apareixeran per pantalla
    void createAnnotations();
    
    /// S'encarrega de fer la feina per defecte que cal fer cada cop que s'invoca qualsevol event, com per exemple registrar el punt sobre el qual es troba el mouse
    void anyEvent();

    /// Els strings amb els textes de cada part de la imatge
    QString m_lowerLeftText, m_lowerRightText, m_upperLeftText, m_upperRightText;

    /// Aquest string indica les anotacions que ens donen les refer�ncies del pacient ( Right,Left,Posterior,Anterior,Inferior,Superior)
    QString m_patientOrientationText[4];

    /// A partir de l'string d'orientaci� del pacient mapeja les anotacions correctes segons com estem mirant el model. A aix� li afecta tamb� si la vista �s axial, sagital o coronal
    void mapOrientationStringToAnnotation();
    
    /// Marcadors que indicaran on est� l'esquerra/dreta, abaix/amunt en refer�ncia al model
    vtkAxisActor2D *m_sideOrientationMarker , *m_bottomOrientationMarker;
    
    /// Textes adicionals d'anotoaci�
    vtkTextActor *m_patientOrientationTextActor[4];

    /// Actualitzaci� d'anotacions v�ries
//     void updateWindowLevelAnnotation();
    void updateSliceAnnotation();
    void updateWindowSizeAnnotation();

    /// Mida de voxels en cada direcci�
    int m_size[3];

    /// Valors dels window level per defecte. Pot venir donat pel DICOM o assignat per nosaltres a un valor est�ndar de constrast
    double m_defaultWindow, m_defaultLevel;

    
signals:
    /// envia la nova llesca en la que ens trobem
    void sliceChanged(int);

    /// indica el nou window level
    void windowLevelChanged( double window , double level );

    /// informem del punt que hem clicat, coordenades de m�n
    void leftButtonDown( double x , double y );

    /// informem del punt sobre el que movem el mouse, coordenades de m�n
    void mouseMove( double x, double y );

    /// informem del punt que hem deixat de clicar, coordenades de m�n
    void leftButtonUp( double x , double y );
    
    
};



};  //  end  namespace udg {

#endif
