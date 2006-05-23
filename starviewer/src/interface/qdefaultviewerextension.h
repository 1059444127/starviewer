/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQDEFAULTVIEWEREXTENSION_H
#define UDGQDEFAULTVIEWEREXTENSION_H

#include "ui_qdefaultviewerextensionbase.h"

// FWD declarations
class QAction;
class QToolBar;

namespace udg {

// FWD declarations
class Volume;
class QCustomWindowLevelDialog;

/**
Extensi� que s'executar� per defecte a l'obrir un model

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class QDefaultViewerExtension : public QWidget , private Ui::QDefaultViewerExtensionBase {
Q_OBJECT
public:
    QDefaultViewerExtension( QWidget *parent = 0 );

    ~QDefaultViewerExtension();

    /// Li assigna el volum principal
    void setInput( Volume *input );

    /// Obtenim la ToolBar d'eines de l'extensi� \TODO 'pujar' al pare com a m�tode com� a Extensions?
    QToolBar *getToolsToolBar() const { return m_toolsToolBar; };

    /// Omple la ToolBar amb les eines de l'extensi� \TODO 'pujar' al pare com a m�tode com� a Extensions?
    void populateToolBar( QToolBar *toolbar );
    
public slots:
    /// Canvia a la vista axial, sagital o coronal
    void changeViewToAxial();
    void changeViewToSagital();
    void changeViewToCoronal();

    /// Permuta l'ajustament de window level
    void changeDefaultWindowLevel( int which );

    /// Li assigna el volum secundari. Aquest m�tode nom�s �s de conveni�ncia i �s temporal
    void setSecondInput( Volume *input );

    /// Canvia de vista simple a vista doble i viceversa
    void switchView();
    
private:
    /// Tipus de vistes que podem tenir
    enum ViewType{ Axial , Sagital , Coronal };

    /// canvia la vista actual
    void setView( ViewType view );
    
    /// La vista actual amb la que estem treballant
    ViewType m_currentView;
    
    /// El volum principal
    Volume *m_mainVolume;

    /// Membre temporal
    Volume *m_secondaryVolume;
    
    /// La ToolBar de les eines de l'extensi� \TODO 'pujar' al pare com a membre com� a Extensions? [hauria de ser protected]
    QToolBar *m_toolsToolBar;

    /// Accions
    QAction *m_axialViewAction;
    QAction *m_sagitalViewAction;
    QAction *m_coronalViewAction;

    /// crea les accions \TODO 'pujar' al pare com a m�tode virtual com� a Extensions? [hauria de ser protected]
    void createActions();
    
    /// Crea la ToolBar d'eines i altres si n'hi ha \TODO 'pujar' al pare com a m�tode virtual com� a Extensions? [hauria de ser protected]
    void createToolBars();
    
    /// Crea les connexions entre signals i slots
    void createConnections();

    /// El di�leg per escollir un window level ajustat per l'usuari
    QCustomWindowLevelDialog *m_customWindowLevelDialog;
    
private slots:
    /// Actua quan es canvia de p�gina, �s a dir es passa d'un determinat layout a un de simple, doble , triple , etc
    void pageChange( int index );

    /// sincronitza les llesques de les s�ries que es visualitzen
    void synchronizeSlices( bool ok );

    /// ens permet escollir una nova s�rie per a comparar
    void chooseNewSerie();

signals:
    /// Aquest senyal s'emetr� quan es vulgui canviar de s�rie per comparar
    void newSerie();
};

} // end namespace udg

#endif
