/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "identifier.h"
// QT
#include <qmainwindow.h>
#include <qstringlist.h>

#include "studyvolum.h"

// Forward declarations
class QAction;
class QDockWindow;
class QProgressDialog;

namespace udg{
// Forward declarations
class QueryScreen;
class VolumeRepository;
class Input;
class Output;
class ExtensionWorkspace;
class ExtensionHandler;

// ***************************************************************************
// Inici zona de codi editable [Forward Declarations]
// ***************************************************************************

// :: tot aix� anir� a parar a una mini aplicaci�
// Registre: Informaci� M�tua 
// class MutualInformationDirector;
// class MutualInformationParameters;
// class MutualInformationInputParametersForm;

// ***************************************************************************
// Fi zona de codi editable
// ***************************************************************************

class QApplicationMainWindow : public QMainWindow
{
Q_OBJECT
public:
    ///El constructor del Mainwindow. Aqu� es crea tot l'esquelet de l'interf�cie: Men�s, Barres d'eines, toolbox, working area, etc..
    QApplicationMainWindow(QWidget *parent = 0, const char *name = 0);
    
    ~QApplicationMainWindow();
    
    /// Ens retorna l'id del volum que ha obert
    Identifier getVolumeID() const { return m_volumeID; };
    /// Inserta l'acci� duna nova mini-aplicaci�
    enum OperationsType{ Segmentation , Registration , Clustering , Color  };
    void insertApplicationAction( QAction *action , OperationsType operation , bool toToolBar = false );
    
    
    /// L'�rea de mini-aplicacions
    ExtensionWorkspace *m_extensionWorkspace;
    
public slots:   
    /// mostra el progr�s d'un proc�s (ara mateix nom�s ser� obrir fitxers) amb un di�leg
    void showProgress( int value );
    ///Fa una petici� per obrir una finestra en la working area i es mostri el widget
    // void addWorkingAreaWidget( QWorkingAreaWidget *widget, const QString & title );
protected:
    ///Aquest event ocurreix quanes tanca la finestra. �s el moment en que es realitzen algunes tasques com desar la configuraci�
    void closeEvent(QCloseEvent *event);
private:
// :::::::::::::::::::::::::::
// membres
// :::::::::::::::::::::::::::
    /// screen de query del pacs
    QueryScreen *m_queryScreen;

    /// Gestor de serveis i miniaplicacions
    ExtensionHandler *m_extensionHandler;
    /// Tipus de fitxer a exportar
    enum ExportFiles{ JpegExport , MetaIOExport , TiffExport, PngExport , BmpExport };
    /// Punter a s� mateix \TODO, no se si �s gaire �til ni necessari, pot desapar�ixer facilment
    QApplicationMainWindow* m_self;    
    /// Indicar� l'id del volum que posse�m
    udg::Identifier m_volumeID; 
    /// Indica si hi ha hagut modificacions \TODO: �s gaire �til i/o necessari aix�? es podria eliminar
    bool m_modified;
    /// Llista d'arxius recents
    QStringList m_recentFiles;
    /// Arxiu actual que estem visualitzant
    QString m_currentFile;
    /// Filtres per als di�legs d'obrir/exportar arxius
    QString m_openFileFilters;
    QString m_exportFileFilters;
    QString m_exportToJpegFilter;
    QString m_exportToMetaIOFilter;
    QString m_exportToTiffFilter;
    QString m_exportToBmpFilter;
    QString m_exportToPngFilter;
    QString m_exportToDicomFilter;
    /// Path actual del directori de treball ( obrir fitxer )
    QString m_workingDirectory;
    ///Path del directori de treball ( exportaci� a altres formats )
    QString m_exportWorkingDirectory;
    /// Aquesta string indica el path del directori que cont� els arxius de traducci� de l'aplicaci�
    QString m_qmPath;
    /// Indica l'idioma per defecte amb el que s'inicialitza l'aplicaci�, tindr�em en_GB, en_US, ca_ES i es_ES
    QString m_defaultLocale;
    /// La llista de idiomes disponibles amb la que omplim el men� d'idiomes
    QStringList m_locales;
    /// \TODO no seria millor fer una const?
    enum { MaxRecentFiles = 5 }; 
    int m_recentFileIds[MaxRecentFiles];
    /// Men�s
    QPopupMenu *m_fileMenu;
    QPopupMenu *m_databaseMenu;
    QPopupMenu *m_segmentationMenu;
    QPopupMenu *m_clusteringMenu;
    QPopupMenu *m_registrationMenu;
    QPopupMenu *m_visualizationMenu;
    QPopupMenu *m_colorMenu;
    QPopupMenu *m_toolsMenu;
    QPopupMenu *m_optionsMenu;
    QPopupMenu *m_languageMenu;
    QPopupMenu *m_helpMenu;
    
    QPopupMenu *m_windowMenu;
    
    /// Sub men�s
    QPopupMenu *m_importFilesMenu;
    QPopupMenu *m_exportFilesMenu;
    QPopupMenu *m_recentFilesMenu;
    
    /// Barres d'eines
    QToolBar *m_extensionsToolBar;
    QToolBar *m_fileToolBar;
    QToolBar *m_databaseToolBar;
    QToolBar *m_segmentationToolBar;
    QToolBar *m_registrationToolBar;
    QToolBar *m_colorToolBar;
    QToolBar *m_clusteringToolBar;
    QToolBar *m_optionsToolBar;
    
    /// Accions
    QAction *m_mpr2DAction;
    QAction *m_mpr3DAction;
    QAction *m_mpr3D2DAction;
    QAction *m_pacsAction;
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_closeAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    QAction *m_aboutQtAction;
    QAction *m_exportToJpegAction;
    QAction *m_exportToMetaIOAction;
    QAction *m_exportToTiffAction;
    QAction *m_exportToPngAction;
    QAction *m_exportToBmpAction;
    /// Per indicar el proc�s al obrir-se un fitxer
    QProgressDialog *m_progressDialog;  
    /// El repositori de volums
    udg::VolumeRepository* m_volumeRepository;
    /// Lectura d'arxius
    udg::Input* m_inputReader;
    /// Escriptura/Exportaci� de formats d'arxius
    udg::Output* m_outputWriter;

// :::::::::::::::::::::::::::
// m�todes
// :::::::::::::::::::::::::::
    
    ///Crea i inicialitza les accions de l'aplicaci�
    void createActions();
    /// Crea els men�s.
    void createMenus();
    /// Crea la barra d'eines.
    void createToolBars();
    /// Crea la barra d'estat de l'aplicaci�
    void createStatusBar();
    /// Crea el men� des del qual podem escollir l'idioma
    void createLanguageMenu();
    /// Llegeix la configuraci� inicial amb la que engega el programa
    void readSettings();
    /// Escriu la configuraci� amb la que s'engegar� el programa el pr�xim cop. \TODO: s'hauria de cridar tamb� quan obrim una finestra nova?
    void writeSettings();
    ///Retorna el numero de finestres amb diferents models que tenim obertes, el num. de QApplicationMainWindow
    unsigned int getCountQApplicationMainWindow();
    /// Especifica el fitxer acctual que s'est� tractant \TODO \deprecated aquest m�tode esdevindr� antiquat en breu i no t� gaure sentit desar el nom del fitxer, el metode continua existint perqu� s'ha arrastrat des del principi
    void setCurrentFile( const QString &fileName );
    /// actualitza el llistat de noms de fitxers oberts recentment \TODO, s'hauria d'adaptar si cal quqan hi hagi lo del pacs
    void updateRecentFileItems();
    /// M�tode intern per extreure el nom del fitxer tal qual sense el path sencer
    QString strippedName(const QString &fullFileName);

private slots:
    /// es crida quan es demana un studi descarregat, es veui la pimera serie
    void viewStudy(StudyVolum study);
    /// Obre la finestra de query del pacs
    void pacsQueryScreen();
    /// Obre una nova finestra
    void newFile();
    /// Slot tancar volum
    void close();
    /// mostra el formulari d'about
    void about();
    /// obre un dels arxius recents::\TODO passar responsabilitat a la mini-app encarregada de fer aix�
    void openRecentFile(int param);
    /// Exporta la imatge a un format especificat
    void exportFile( int type );
    /// Exporta la imatge a JPEG
    void exportToJpeg();
    /// Exporta la imatge a MetaIO
    void exportToMetaIO();
    /// Exporta la imatge a TIFF
    void exportToTiff();
    /// Exporta la imatge a PNG
    void exportToPng();
    /// Exporta la imatge a BMP
    void exportToBmp();
    
signals:
    /// senyal emesa que indica si la finestra cont� un volum o no
    void containsVolume( bool );
};

}; // fi namespace udg

#endif
