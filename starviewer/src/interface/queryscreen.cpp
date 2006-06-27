/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#include "queryscreen.h"

#include <QMessageBox>
#include <QDesktopWidget> 
#include <QCloseEvent>
#include <string>
#include <QDateTime>

#include "starviewerprocessimage.h"
#include "processimagesingleton.h"
#include "pacsparameters.h"
#include "pacsconnection.h"
#include "multiplequerystudy.h"
#include "studylist.h"
#include "qstudytreewidget.h"
#include "series.h"
#include "seriesmask.h"
#include "queryseries.h"
#include "pacsparameters.h"
#include "pacsserver.h"
#include "imagemask.h"
#include "queryimagenumber.h"
#include "qserieslistwidget.h"
#include "retrieveimages.h"
#include "qconfigurationscreen.h"
#include "pacslist.h"
#include "qpacslist.h"
#include "starviewersettings.h"
#include "cachepool.h"
#include "scalestudy.h"
#include "queueoperationlist.h"
#include "operation.h"
#include "cachelayer.h"
#include "pacslistdb.h"
#include "logging.h"
#include "status.h"
#include "cachestudydal.h"
#include "cacheseriesdal.h"
#include "cacheimagedal.h"
#include "seriesmask.h"

namespace udg {

QueryScreen::QueryScreen( QWidget *parent )
 : QWidget(parent )
{
    setupUi( this );

    Status state;
    QString path;
    StarviewerSettings settings;
    CacheStudyDAL cacheStudyDal;
           
    m_retrieveScreen = new udg::QRetrieveScreen;
	
    initialize();//inicialitzem les variables necess�ries

    //connectem signals i slots
    connectSignalsAndSlots();
    
    state = cacheStudyDal.delNotRetrievedStudies();//Esborrem els estudis en estat 'PENDING' o 'RETRIEVING'
    if ( !state.good() ) 
    {
        databaseError( &state );
    } //si no hi ha error a la base de dades, podem esborrar els estudis vells
    else deleteOldStudies();
        
    //carreguem el processImageSingleton    
    m_piSingleton = ProcessImageSingleton::getProcessImageSingleton();
    m_piSingleton->setPath( settings.getCacheImagePath().toStdString() );

    //Instanciem els llistats
    m_seriesListSingleton = SeriesListSingleton::getSeriesListSingleton();
    m_studyListSingleton = StudyListSingleton::getStudyListSingleton();
   
    centerWindow(); //centrem la finestra
    
    m_textPatientID->setFocus();
}

void QueryScreen::initialize()
{
    QDate currentDate;

    //indiquem que la llista de Pacs no es mostra
    m_PacsListShow = false;

    setEnabledModalityChecks( true );

    qPacsList->setMaximumSize( 1 , 1 );//amaguem al finestra del QPacsList

    m_textFrom->setDate( currentDate.currentDate() );
    m_textTo->setDate( currentDate.currentDate() );
}

void QueryScreen::deleteOldStudies()
{  
    Status state;
    CacheLayer cacheLayer;
        
    state = cacheLayer.deleteOldStudies();
    
    if ( !state.good() ) 
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Error deleting old studies" ) );
        databaseError( &state );
    }
}

void QueryScreen::connectSignalsAndSlots()
{
    //connectem els butons
    connect( m_buttonConfig , SIGNAL( clicked() ) , this , SLOT( config() ) );
    connect( m_buttonSearch , SIGNAL( clicked() ) , this , SLOT( search() ) );
    connect( m_buttonToday , SIGNAL( clicked() ) , this , SLOT( searchToday() ) );
    connect( m_buttonYesterday , SIGNAL( clicked() ) , this , SLOT( searchYesterday() ) );
    connect( m_buttonClear , SIGNAL( clicked() ) , this , SLOT( clearTexts() ) );
    connect( m_buttonRetrieve , SIGNAL( clicked() ) , this , SLOT( retrieve() ) );
    connect( m_buttonRetrieveList , SIGNAL( clicked() ) , this , SLOT( showRetrieveScreen() ) );
    connect( m_buttonShowPacsList , SIGNAL( clicked() ) , this , SLOT( showPacsList() ) );
    connect( m_buttonView , SIGNAL( clicked() ) , this , SLOT( view() ) );
    
    //connectem Slots dels StudyList amb la interficie
    connect( m_studyTreeWidgetPacs , SIGNAL( expand( QString , QString ) ) , this , SLOT( searchSeries( QString , QString ) ) );
    connect( m_studyTreeWidgetCache,  SIGNAL( expand( QString , QString ) ) , this , SLOT( searchSeries( QString , QString ) ) );
    
    //es canvia de pestanya del TAB
    connect( m_tab , SIGNAL( currentChanged( int ) ) , this , SLOT( tabChanged( int ) ) );
    
    //connectem els checkbox de les dates
    connect( m_checkFrom, SIGNAL( stateChanged( int ) ) , this , SLOT( setEnabledTextFrom( int ) ) );
    connect( m_checkTo, SIGNAL( stateChanged( int ) ) , this , SLOT( setEnabledTextTo( int ) ) );
    
    //lineedit al fer entrer que cerquin    
    connect( m_textPatientID , SIGNAL( returnPressed() ) , this , SLOT( search() ) );
    connect( m_textLastName , SIGNAL( returnPressed() ) , this , SLOT( search() ) );
    connect( m_textFirstName , SIGNAL( returnPressed() ) , this , SLOT( search() ) );
    connect( m_textStudyID , SIGNAL( returnPressed() ) , this , SLOT( search() ) );
    connect( m_textAccessionNumber , SIGNAL( returnPressed() ) , this , SLOT( search() ) );
    
    //checkbox
    connect( m_checkAll, SIGNAL( clicked() ) , this , SLOT( clearCheckedModality() ) );
    connect( m_checkCT, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkCR, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkDX, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkES, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkMG, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkMR, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkNM, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkDT, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkPT, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkRF, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkSC, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkUS, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    connect( m_checkXA, SIGNAL( clicked() ) , this , SLOT( setCheckAll() ) );
    
    //conectem els sinals dels TreeView
    
    connect( m_studyTreeWidgetCache , SIGNAL( delStudy() ) , this , SLOT( deleteStudyCache() ) );
    connect( m_studyTreeWidgetCache , SIGNAL( view() ) , this , SLOT( view() ) );
    
    //connectem els signes del SeriesIconView StudyListView
    connect( m_studyTreeWidgetCache , SIGNAL( addSeries(Series * ) ) , m_seriesListWidgetCache , SLOT( addSeries(Series *) ) );
    connect( m_studyTreeWidgetCache , SIGNAL( clearSeriesListWidget() ) , m_seriesListWidgetCache , SLOT( clearSeriesListWidget() ) );    
    connect( m_seriesListWidgetCache , SIGNAL( selectedSeriesIcon( QString) ) , m_studyTreeWidgetCache , SLOT( selectedSeriesIcon( QString) ) ); 
    connect( m_seriesListWidgetCache , SIGNAL( viewSeriesIcon() ) , m_studyTreeWidgetCache , SLOT( viewStudy() ) ); 
    connect( m_studyTreeWidgetCache , SIGNAL( selectedSeriesList( QString) ) , m_seriesListWidgetCache , SLOT( selectedSeriesList( QString) ) ); 
    
    //per netejar la QSeriesIconView quant s'esborrar un estudi
    connect(this , SIGNAL( clearSeriesListWidget() ) , m_seriesListWidgetCache , SLOT( clearSeriesListWidget() ) );
    
    //per poder descarregar i veure un estudi amb el menu contextual dels del QStudyList del PACS
    connect( m_studyTreeWidgetPacs , SIGNAL( view() ) , this , SLOT( view() ) );
    connect( m_studyTreeWidgetPacs , SIGNAL( retrieve() ) , this , SLOT( retrieve() ) );
    
    //connecta el signal que emiteix qexecuteoperationthread, per visualitzar un estudi amb aquesta classe
     QObject::connect( &m_qexecuteOperationThread , SIGNAL( viewStudy( QString) ) , this , SLOT( studyRetrievedView( QString) ) , Qt::QueuedConnection );
    
    //connecta els signals el qexecute operation thread amb els de qretrievescreen, per coneixer quant s'ha descarregat una imatge, serie, estudi, si hi ha error, etc..
    connect( &m_qexecuteOperationThread , SIGNAL(  setErrorRetrieving( QString ) ) , m_retrieveScreen, SLOT(  setErrorRetrieving( QString ) ) );
    connect( &m_qexecuteOperationThread , SIGNAL(  setStudyRetrieved( QString ) ) , m_retrieveScreen, SLOT(  setRetrievedFinished( QString ) ) ); 
    connect( &m_qexecuteOperationThread , SIGNAL(  setStudyRetrieving( QString ) ) , m_retrieveScreen, SLOT(  setRetrieving( QString ) ) );
    connect( &m_qexecuteOperationThread , SIGNAL(  imageRetrieved( QString , int) ) , m_retrieveScreen , SLOT(  imageRetrieved( QString , int ) ) );
    connect( &m_qexecuteOperationThread , SIGNAL(  seriesRetrieved( QString ) ) ,  m_retrieveScreen , SLOT(  setSeriesRetrieved( QString ) ) );
    
    //connecta el signal de que no hi ha suficient espai de disc
    connect( &m_qexecuteOperationThread , SIGNAL(  notEnoughFreeSpace() ) , this , SLOT(  notEnoughFreeSpace() ) );
    //error alliberant espai de la cache
    connect( &m_qexecuteOperationThread , SIGNAL(  errorFreeingCacheSpace() ) , this , SLOT(  errorFreeingCacheSpace() ) );
    
    //connect tracta els errors de connexi� al PACS
    connect ( &multipleQueryStudy , SIGNAL ( errorConnectingPacs( int ) ) , this , SLOT(  errorConnectingPacs( int ) ) );

    connect ( &multipleQueryStudy , SIGNAL ( errorQueringStudiesPacs( int ) ) , this , SLOT(  errorQueringStudiesPacs( int ) ) );


    //connect tracta els errors de connexi� al PACS, al descarregar imatges 
    connect ( &m_qexecuteOperationThread , SIGNAL ( errorConnectingPacs( int ) ) , this , SLOT(  errorConnectingPacs( int ) ) );

    connect( &m_qexecuteOperationThread , SIGNAL(  setStudyRetrieved( QString ) ) , this, SLOT(  studyRetrieveFinished ( QString ) ) ); 

    connect( m_studyTreeWidgetCache , SIGNAL ( convertToDicomDir( QString ) ) , this , SLOT ( convertToDicomDir( QString ) ) );

}

void QueryScreen::centerWindow()
{
    QDesktopWidget *d = QApplication::desktop();
        
    move( ( d->width() - frameGeometry().width() ) / 2 , (d->height() - frameGeometry().height() ) / 2 );
}

void QueryScreen::clearTexts()
{
    m_textStudyID->setText( "" );
    m_textPatientID->setText( "" );
    m_textFirstName->setText( "" );
    m_textLastName->setText( "" );
    m_textAccessionNumber->setText( "" );
    m_checkTo->setChecked( false );
    m_checkFrom->setChecked( false );
    
    setEnabledTextTo( m_checkTo->isChecked() );
    setEnabledTextFrom( m_checkFrom->isChecked() );
}

void QueryScreen::setEnabledTextTo( int value )
{
    m_textTo->setEnabled( value );
}

void QueryScreen::setEnabledTextFrom( int value )
{
    m_textFrom->setEnabled( value );
}

void QueryScreen::setEnabledModalityChecks( bool enabled )
{
    m_checkAll->setEnabled( enabled );
    m_checkCR->setEnabled( enabled );
    m_checkCT->setEnabled( enabled );
    m_checkDX->setEnabled( enabled );
    m_checkES->setEnabled( enabled );
    m_checkMG->setEnabled( enabled );
    m_checkMR->setEnabled( enabled );
    m_checkNM->setEnabled( enabled );
    m_checkDT->setEnabled( enabled );
    m_checkPT->setEnabled( enabled );
    m_checkRF->setEnabled( enabled );
    m_checkSC->setEnabled( enabled );
    m_checkUS->setEnabled( enabled );
    m_checkXA->setEnabled( enabled );
    
    m_buttonGroupModality->setEnabled( enabled );
}

void QueryScreen::clearCheckedModality()
{
    m_checkAll->setChecked( true );
    m_checkCR->setChecked( false );
    m_checkCT->setChecked( false );
    m_checkDX->setChecked( false );
    m_checkES->setChecked( false );
    m_checkMG->setChecked( false );
    m_checkMR->setChecked( false );
    m_checkNM->setChecked( false );
    m_checkDT->setChecked( false );
    m_checkPT->setChecked( false );
    m_checkRF->setChecked( false );
    m_checkSC->setChecked( false );
    m_checkUS->setChecked( false );
    m_checkXA->setChecked( false );
}

void QueryScreen::setCheckAll()
{
    if ( m_checkCR->isChecked() ||
        m_checkCT->isChecked() ||
        m_checkDX->isChecked() ||
        m_checkES->isChecked() ||
        m_checkMG->isChecked() ||
        m_checkMR->isChecked() ||
        m_checkNM->isChecked() ||
        m_checkDT->isChecked() ||
        m_checkPT->isChecked() ||
        m_checkRF->isChecked() ||
        m_checkSC->isChecked() ||
        m_checkUS->isChecked() ||
        m_checkXA->isChecked() )
     {
         m_checkAll->setChecked( false );
     } 
     else m_checkAll->setChecked( true );
}

void QueryScreen::dateFromChanged( const QDate &data )
{
    m_textTo->setDate( data );
}

void QueryScreen::setEnabledDates( bool enabled )
{
    m_textFrom->setEnabled( enabled );
    m_checkFrom->setEnabled( enabled );
    m_checkFrom->setChecked( enabled );
    
    m_textTo->setEnabled( enabled );
    m_checkTo->setEnabled( enabled );
    m_checkTo->setChecked( enabled );
}

void QueryScreen::searchToday()
{
    QDate today;

    setEnabledDates( true );
        
    m_textTo->setDate(today.currentDate() );
    m_textFrom->setDate(today.currentDate() );
    search();
}

void QueryScreen::searchYesterday()
{
    QDate today;
    QDate yesterday( today.currentDate() );
    
    setEnabledDates( true );
    yesterday = yesterday.addDays( -1 );//calcula la data d'ahir
    
    m_textFrom->setDate( yesterday );
    m_textTo->setDate( yesterday );
    search();
}

void QueryScreen::search()
{
    QString logMessage;
    
    if ( m_tab->currentIndex() == 1 )
    {
        if ( !validateNoEmptyMask() )
        {
            switch( QMessageBox::information( this , tr( "Starviewer" ) ,
                                        tr( "You have not specified any filter. This query could take a long time. Do you want to continue ?" ) ,
                                        tr( "&Yes" ) , tr( "&No" ) ,
                                        0 , 1 ) ) 
            {
				
                case 0: 
                        logMessage = "Cerca d'estudis als PACS amb par�metres " + logQueryStudy();
                        INFO_LOG ( logMessage.toAscii().constData() ); 
                        queryStudyPacs();
                        QApplication::restoreOverrideCursor();
                        break;
            }    
        }
        else 
        {	
            logMessage = "Cerca d'estudis als PACS amb par�metres " + logQueryStudy();
            INFO_LOG ( logMessage.toAscii().constData() ); 
            queryStudyPacs(); 
        }
    }
    else
    {
        logMessage = "Cerca d'estudis a la cache amb par�metres " + logQueryStudy();
        INFO_LOG ( logMessage.toAscii().constData() ); 
        queryStudyCache();
    }
}

bool QueryScreen::validateNoEmptyMask()
{
    if ( m_textPatientID->text().length() == 0 &&
         m_textFirstName->text().length() == 0 &&
         m_textLastName->text().length() == 0 &&
         m_textStudyID->text().length() == 0 &&
         m_textAccessionNumber->text().length() == 0 &&
         !m_checkFrom->isChecked()  &&
         !m_checkTo->isChecked() )
    {
        return false;
    }
    else return true;
}

void QueryScreen::queryStudyPacs()
{
    PacsList pacsList;
    PacsParameters pa;
    QString result;
    StarviewerSettings settings;
    
    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
	
    pacsList.clear(); //netejem el pacsLIST
    qPacsList->getSelectedPacs( &pacsList ); //Emplemen el pacsList amb les pacs seleccionats al QPacsList
    
    pacsList.firstPacs();
    
    if ( pacsList.end() ) //es comprova que hi hagi pacs seleccionats
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Please select a PACS to query" ) );
        return;      
    }
    
    multipleQueryStudy.setPacsList( pacsList ); //indiquem a quins Pacs Cercar
    multipleQueryStudy.setMask( buildMask() ); //construim la mascara
    if ( !multipleQueryStudy.StartQueries().good() )  //fem la query
    {
        m_studyTreeWidgetPacs->clear();
	QApplication::restoreOverrideCursor();
        QMessageBox::information( this , tr( "StarViewer" ) , tr( "ERROR QUERING!." ) );
        return;  
    }
     
    m_studyListSingleton->firstStudy();
       
    if ( m_studyListSingleton->end() )
    {
        m_studyTreeWidgetPacs->clear();        
	QApplication::restoreOverrideCursor();
        QMessageBox::information( this , tr( "StarViewer" ) , tr( "No study match found." ) );
        return;
    }
    m_studyTreeWidgetPacs->insertStudyList( m_studyListSingleton ); //fem que es visualitzi l'studyView seleccionat
    m_studyTreeWidgetPacs->setSortColumn( 2 );//ordenem pel nom
    
    QApplication::restoreOverrideCursor();
    
}

void QueryScreen::queryStudyCache()
{
    CacheStudyDAL cacheStudyDAL;
    Status state;
    
    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

    m_seriesListWidgetCache->clear();
    
    m_studyListCache.clear();
    
    state= cacheStudyDAL.queryStudy( buildMask() , m_studyListCache ); //busquem els estudis a la cache

    if ( !state.good() ) 
    {
        m_studyTreeWidgetCache->clear();
	QApplication::restoreOverrideCursor();
        databaseError( &state );
        return;
    }
    
    m_studyListCache.firstStudy();

    if ( m_studyListCache.end() ) //no hi ha estudis
    {
        m_studyTreeWidgetCache->clear();
	QApplication::restoreOverrideCursor();
        QMessageBox::information( this , tr( "StarViewer" ) , tr( "No study match found." ) );
        return;
    }
    
    m_studyTreeWidgetCache->insertStudyList( &m_studyListCache );//es mostra la llista d'estudis
    
    m_studyTreeWidgetCache->setSortColumn( 2 ); //ordenem pel nom

    QApplication::restoreOverrideCursor();
}

/* AQUESTA ACCIO ES CRIDADA DES DEL STUDYLISTVIEW*/
void QueryScreen::searchSeries( QString studyUID , QString pacsAETitle )
{   
    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
    
    if ( m_tab->currentIndex() == 1 ) //si estem la pestanya del PACS fem query al Pacs
    {
        QuerySeriesPacs( studyUID , pacsAETitle , true );
    }
    else if ( m_tab->currentIndex() == 0 ) // si estem a la pestanya de la cache
    {
        QuerySeriesCache( studyUID );
    }

    QApplication::restoreOverrideCursor();
}

void QueryScreen::QuerySeriesPacs( QString studyUID , QString pacsAETitle , bool show )
{
    Series serie;
    ImageMask imagem;
    QString text,timeoutPacs;
    PacsParameters pacs;
    PacsListDB pacsListDB;
    Status state;   
    StarviewerSettings settings;
    QString logMessage;
	
    logMessage = "Cercant informacio de les s�ries de l'estudi" + studyUID + " del PACS " + pacsAETitle;    
    INFO_LOG( logMessage.toAscii().constData() );
    
    state = pacsListDB.queryPacs( &pacs, pacsAETitle.toAscii().constData() );//cerquem els par�metres del Pacs al qual s'han de cercar les dades
    if ( !state.good() )
    {
        databaseError( &state );
        return;
    }

    pacs.setAELocal( settings.getAETitleMachine().toStdString() ); //especifiquem el nostres AE
    pacs.setTimeOut( settings.getTimeout().toInt( NULL , 10 ) ); //li especifiquem el TimeOut

    m_seriesListSingleton->clear();
      
    PacsServer pacsConnection( pacs );
    
    state = pacsConnection.connect(PacsServer::query,PacsServer::seriesLevel);
    if ( !state.good() )
    {//Error al connectar
        logMessage = "Error al connectar al pacs ";    
        logMessage.append( pacsAETitle );
        logMessage.append( ". PACS ERROR : " );
        logMessage.append( state.text().c_str() );
        ERROR_LOG( logMessage.toAscii().constData() );		
			
        errorConnectingPacs ( pacs.getPacsID() );
        return;         
    }
    
    QuerySeries querySeries( pacsConnection.getConnection() , buildSeriesMask( studyUID ) );
    
    state = querySeries.find();
    if ( !state.good() )
    {//Error a la query
        logMessage = "QueryScreen::QuerySeriesPacs : Error cercant les s�ries al PACS ";
        logMessage.append( pacsAETitle );
        logMessage.append( ". PACS ERROR : " );
        logMessage.append( state.text().c_str() );
        ERROR_LOG( logMessage.toAscii().constData() );		

        text.insert( 0 , tr( "Error! Can't query studies in PACS : " ) );
        text.append( pacsAETitle);
        QMessageBox::warning( this , tr( "StarViewer" ) , text );
        return;         
    }
    
    m_seriesListSingleton->firstSeries();
    if ( m_seriesListSingleton->end() )
    {
        QMessageBox::information( this , tr( "StarViewer" ) , tr( "No series match for this study.\n" ) );
        return;
    }
    
    m_seriesListSingleton->firstSeries();
    
    while ( !m_seriesListSingleton->end() )
    {
        serie = m_seriesListSingleton->getSeries();
        
        if ( show )
        {
            m_studyTreeWidgetPacs->insertSeries( &serie );//inserim la informaci� de les imatges al formulari
 
        }
        m_seriesListSingleton->nextSeries();
    }
    pacsConnection.disconnect();
}

void QueryScreen::QuerySeriesCache( QString studyUID )
{
    Series serie;
    ImageMask imageMask;
    CacheSeriesDAL cacheSeriesDAL;
    CacheImageDAL cacheImageDAL;
    int imagesNumber;
    Status state;
    QString logMessage;
    
    logMessage = "Cerca de s�ries a la cach� de l'estudi " + studyUID;
    INFO_LOG( logMessage.toAscii().constData() );
 
    m_seriesListCache.clear();//preparem la llista de series
    
    //preparem la mascara i cerquem les series a la cache
    state=cacheSeriesDAL.querySeries( buildSeriesMask( studyUID ) , m_seriesListCache );
    
    if ( !state.good() )
    {
        databaseError( &state );
        return;
    }
    
    m_seriesListCache.firstSeries();
    if ( m_seriesListCache.end() )
    {
        QMessageBox::information( this , tr( "StarViewer" ) , tr( "No series match for this study.\n" ) );
        return;
    }
    
    m_seriesListCache.firstSeries();
    m_seriesListWidgetCache->clear();
    
    while ( !m_seriesListCache.end() )
    {
        serie= m_seriesListCache.getSeries();
        
        //preparem per fer la cerca d'imatges
        imageMask.setStudyUID( serie.getStudyUID() );
        imageMask.setSeriesUID( serie.getSeriesUID() );
        
        state = cacheImageDAL.countImageNumber( imageMask , imagesNumber );
        serie.setImageNumber( imagesNumber );
        if ( !state.good() )
        {
            databaseError( &state );
            return;
        }
        m_studyTreeWidgetCache->insertSeries( &serie );//inserim la informaci� de les imatges al formulari
        m_seriesListCache.nextSeries();
    }   
}

void QueryScreen::retrieve()
{
    switch( QMessageBox::information( this , tr( "Starviewer" ) ,
				      tr( "Are you sure you want to retrieve this Study ?" ) ,
				      tr( "&Yes" ) , tr( "&No" ) ,
				      0, 1 ) ) 
    {
    case 0:
        retrievePacs( false );
    }
}
   
void QueryScreen::retrievePacs( bool view )
{
    StudyMask mask;
    QString studyUID,defaultSeriesUID;
    Status state;
    Operation operation;
    PacsParameters pacs;      
    PacsListDB pacsListDB; 
    StarviewerSettings settings;
    
    QApplication::setOverrideCursor( QCursor ( Qt::WaitCursor ) );

    if ( m_studyTreeWidgetPacs->getSelectedStudyUID() == "" )
    {
        QApplication::restoreOverrideCursor();
        if ( view)
        {
            QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Select a study to view " ) );
        }
        else QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Select a study to download " ) );
        return;
    }
    studyUID.insert(0 , m_studyTreeWidgetPacs->getSelectedStudyUID().toAscii().constData() );

    //Tenim l'informaci� de l'estudi a descarregar a la llista d'estudis, el busquem a la llista
    if ( !m_studyListSingleton->findStudy( studyUID.toAscii().constData() ) )
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Internal Error : " ) );
        return;
    }
   
    //Inserim l'informaci� de l'estudi a la cach�!
    state = insertStudyCache( m_studyListSingleton->getStudy() );
    
    if (  !state.good() )
    {   
        QApplication::restoreOverrideCursor();
        if (  state.code() == 2019 ) //l'estudi ja existeix
        {   
            if (  view )  //si es vol visualitzar no donem missatge de que ja esta descarregat, obrim l'estudi
            {
                studyRetrievedView( studyUID ); 
            }
            else QMessageBox::warning( this , tr( "StarViewer" ) , tr( "The study has been retrieved or is retrieving." ) );
        }
        else databaseError( &state );
        
        return;
    }   
    
    mask.setStudyUID( studyUID.toAscii().constData() );//definim la m�scara per descarregar l'estudi
    
    //busquem els par�metres del pacs del qual volem descarregar l'estudi
    state = pacsListDB.queryPacs( &pacs , m_studyTreeWidgetPacs->getSelectedStudyPacsAETitle().toAscii().constData() );
    
    if ( !state.good() )
    {
        QApplication::restoreOverrideCursor();
        databaseError( &state );
        return;
    }

    //inserim a la pantalla de retrieve que iniciem la descarrega
    m_retrieveScreen->insertNewRetrieve( &m_studyListSingleton->getStudy() );      

    //emplanem els parametres amb dades del starviewersettings
    pacs.setAELocal( settings.getAETitleMachine().toAscii().constData() );
    pacs.setTimeOut( settings.getTimeout().toInt( NULL , 10 ) );
    pacs.setLocalPort( settings.getLocalPort().toAscii().constData() );
    
    //definim l'operacio
    operation.setPacsParameters( pacs );
    operation.setStudyMask( mask );
    if ( view )
    {
        operation.setOperation( operationView );
    }
    else operation.setOperation( operationRetrieve );
    
    m_qexecuteOperationThread.queueOperation( operation );
    
    QApplication::restoreOverrideCursor();
}

Status QueryScreen::insertStudyCache( Study stu )
{
    std::string absPath;
    Status state;
    Study study = stu;
    StarviewerSettings settings;
    CacheStudyDAL cacheStudyDAL;
       
    //creem el path absolut de l'estudi
    absPath.insert( 0 , settings.getCacheImagePath().toAscii().constData() );
    absPath.append( study.getStudyUID() );
    absPath.append( "/" );
    study.setAbsPath(absPath);
    //inserim l'estudi a la cach�
    state = cacheStudyDAL.insertStudy( &study ); 
   
    return state;
}
 
void QueryScreen::studyRetrievedView( QString studyUID )
{
    QString series = "";
    
    retrieveCache( studyUID , series );
}

void QueryScreen::tabChanged( int index )
{
    if ( index != 0 )
    {
        setEnabledModalityChecks( false );//desactivem el grup button de modalitat
        m_buttonRetrieve->setEnabled( true );//activem el boto retrieve
        m_buttonView->setEnabled( true );
        m_buttonShowPacsList->setEnabled( true );//activem el boto d'ensenyar la llista de pacs
    }
    else 
    {
        setEnabledModalityChecks( true );//activem el grup button de motalitat
        m_buttonRetrieve->setEnabled( false );//desactivem el boto retrieve
        m_buttonView->setEnabled( true );
        m_buttonShowPacsList->setEnabled( false );//activem el boto d'ensenyar la llista de pacs
    }
    
    if (  m_PacsListShow ) resizePacsList();
}

void QueryScreen::view()
{
    if ( m_tab->currentIndex() == 1)
    {
        switch( QMessageBox::information( this , tr( "Starviewer" ) ,
				      tr( "Are you sure you want to view this Study ?" ) ,
				      tr( "&Yes" ) , tr( "&No" ) ,
				      0 , 1 ) ) 
        {
        case 0:
            retrievePacs( true );
        }
    }
    else if ( m_tab->currentIndex() == 0)
    {
        
        retrieveCache( m_studyTreeWidgetCache->getSelectedStudyUID() , m_studyTreeWidgetCache->getSelectedSeriesUID() );
    }
}


void QueryScreen::retrieveCache( QString studyUID , QString seriesUID )
{
    CacheStudyDAL cacheStudyDAL;
    CacheSeriesDAL cacheSeriesDAL;
    CacheImageDAL cacheImageDAL;
    Status state;
    StudyList stuList;
    Study stu;
    SeriesMask mask;
    SeriesList seriesList;
    Series series;
    ImageMask imageMask;
    ImageList imageList;
    QString absSeriesPath;
    StarviewerSettings settings;
    StudyVolum volum;
    QString logMessage;
        
    if ( studyUID == "" )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Select a study to view " ) );
        return;
    }    
    
    logMessage = "Es visualitza l'estudi " + studyUID;
    INFO_LOG ( logMessage.toAscii().constData() );
	
    state = cacheStudyDAL.queryStudy( studyUID.toAscii().constData() , stu ); //cerquem la informaci� de l'estudi
    if ( !state.good() )
    {   
        databaseError( &state );
        return;
    }
    
    volum.setPatientAge( stu.getPatientAge() );
    volum.setPatientId( stu.getPatientId() );
    volum.setPatientName( stu.getPatientName() );
    volum.setStudyDate( stu.getStudyDate() );
    volum.setStudyId( stu.getStudyId() );
    volum.setStudyTime( stu.getStudyTime() );
    volum.setStudyUID( stu.getStudyUID() );    

    mask.setStudyUID( stu.getStudyUID().c_str() );

    cacheSeriesDAL.querySeries( mask ,seriesList );
    if ( !state.good() )
    {
        databaseError( &state );
        return;
    }
        
    seriesList.firstSeries();
    
    //si es buit indiquem que per defecte es visualitza la primera serie
    if ( seriesUID == "" )
    {
        volum.setDefaultSeriesUID( seriesList.getSeries().getSeriesUID() );
    }
    else volum.setDefaultSeriesUID( seriesUID.toStdString() );
    
    while ( !seriesList.end() )
    {
        SeriesVolum seriesVol;
        series = seriesList.getSeries();
        
        absSeriesPath = settings.getCacheImagePath();
        absSeriesPath += series.getSeriesPath().c_str();
        seriesVol.setSeriesUID(series.getSeriesUID() );
        seriesVol.setStudyId( stu.getStudyId() );
        seriesVol.setStudyUID( stu.getStudyUID() );
        seriesVol.setSeriesPath( absSeriesPath.toStdString() );
        
        imageMask.setSeriesUID(series.getSeriesUID().c_str() );
        imageMask.setStudyUID( stu.getStudyUID().c_str() );
        
        imageList.clear();
        state= cacheImageDAL.queryImages(imageMask , imageList);

        if ( !state.good() )
        {
            databaseError( &state );
            return;
        }       
  
        imageList.firstImage();
        
        while ( !imageList.end() )
        {
            seriesVol.addImage( imageList.getImage().getImagePath().c_str() );
            imageList.nextImage();
        }
        
        volum.addSeriesVolum( seriesVol );
        seriesList.nextSeries();
    }
    
    cacheStudyDAL.updateStudyAccTime( studyUID.toStdString() );
    
    this->close();//s'amaga per poder visualitzar la serie
    if ( m_retrieveScreen->isVisible() )
    {
        m_retrieveScreen->close();//s'amaga per poder visualitzar la serie
    }
    emit( viewStudy(volum) );
}

void QueryScreen::deleteStudyCache()
{
    Status state;
    CacheStudyDAL cacheStudyDAL;
    QString studyUID;
    QString logMessage;
    
    studyUID = m_studyTreeWidgetCache->getSelectedStudyUID();
    
    if ( studyUID == "" )
    {
        QMessageBox::information( this , tr( "StarViewer" ) , tr( "Please select a study to delete" ) );
        return;    
    }
    
    switch( QMessageBox::information( this , tr( "Starviewer" ) ,
				      tr( "Are you sure you want to delete this Study ?" ) ,
				      tr( "&Yes" ) , tr( "&No" ) ,
				      0, 1 ) ) 
    {
        case 0:
            logMessage = "S'esborra de la cache l'estudi " + studyUID;
            INFO_LOG( logMessage.toAscii().constData() );
            
            state = cacheStudyDAL.delStudy( studyUID.toAscii().constData() );   
            
            if ( state.good() )
            {
                m_studyTreeWidgetCache->removeStudy( studyUID );
                emit( clearSeriesListWidget() );//Aquest signal es recollit per qSeriesIconView
            }
            else
            {
                databaseError( &state );
            }
    }
}

void QueryScreen::studyRetrieveFinished( QString studyUID ) 
{
    Study study;
    CacheStudyDAL cacheStudyDAL;
    Status state;
    
    state = cacheStudyDAL.queryStudy( studyUID.toAscii().constData() , study );

    if ( state.good() )
    {
        m_studyTreeWidgetCache->insertStudy( &study );
        m_studyTreeWidgetCache->sort();
    }
    else databaseError( &state );

} 

void QueryScreen::closeEvent( QCloseEvent* ce )
{
	//ce->ignore();
	m_studyTreeWidgetPacs->saveColumnsWidth();
	m_studyTreeWidgetCache->saveColumnsWidth();
        ce->accept();        
}

void QueryScreen::showRetrieveScreen()
{
    //el ActiveWindow no funciona, no enfoca la finestra el setWindowState tampoc, �s un bug de QT ? a la docu posa que en certes ocasions el Qt::WindowActive pot ser ignorat! Per aixo s'ha de tornar la finestra invisble i tornar-la a fer visible per visualitzar-la, sin� no s'enfoca la finestra
    m_retrieveScreen->setVisible( false );
    m_retrieveScreen->setVisible( true );
}


void QueryScreen::config()
{
    udg::QConfigurationScreen *configScreen = new udg::QConfigurationScreen;
    
    INFO_LOG( "S'obre la finestra de configuraci�" );

    connect( configScreen , SIGNAL(  pacsListChanged() ) , qPacsList , SLOT(  refresh()  ) );
    connect( configScreen , SIGNAL(  cacheCleared() ) , m_studyTreeWidgetCache , SLOT(  clear() ) );
    connect( configScreen , SIGNAL(  cacheCleared() ) , m_seriesListWidgetCache , SLOT(  clearSeriesListWidget() ) );	

    configScreen->setModal( true );
    configScreen->show();
}

void QueryScreen::showPacsList()
{
    if ( !m_PacsListShow )
    {   
        m_PacsListShow = true;
        m_buttonShowPacsList->setText( tr( "Hide Pacs List" ) );
    }
    else
    {
        m_buttonShowPacsList->setText( tr( "Show Pacs List" ) );
        m_PacsListShow = false;
    }
    
    resizePacsList();
}

void QueryScreen::resizePacsList()
{
    int mida;
    
    //si es fals i estem al tab del Pacs s'ha de mostrar la llistat de PACS
    if (  m_PacsListShow && m_tab->currentWidget()->objectName() == "m_tabPacs" )
    {   qPacsList->setFixedSize( 200 ,240 );
        mida = 200 + 20;
    }
    else
    {
        qPacsList->setFixedSize( 1 , 1 );
        mida = -( 200 + 20 );
    }
    
   this->resize( this->width() + mida, this->height() );
}

void QueryScreen::convertToDicomDir( QString studyUID )
{
    m_convert.convert( studyUID );
}

void QueryScreen::notEnoughFreeSpace()
{
    QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Not enough space to retrieve studies. Please free space" ) );
}

void QueryScreen::errorFreeingCacheSpace()
{
    QMessageBox::critical( this , tr( "StarViewer" ) , tr( "Error Freeing Space. The study couldn't be retrieved" ) );
}

void QueryScreen::errorConnectingPacs( int PacsID )
{
    PacsListDB pacsListDB;
    PacsParameters errorPacs;
    QString errorMessage;
    
    pacsListDB.queryPacs( &errorPacs, PacsID );
    
    errorMessage.insert( 0 , tr( " Can't connect to PACS " ) );
    errorMessage.append( errorPacs.getAEPacs().c_str() );
    errorMessage.append( tr ( " of " ) );
    errorMessage.append( errorPacs.getInstitution().c_str() );
    errorMessage.append( '\n' );
    errorMessage.append( tr( " Be sure that the IP and AETitle of the PACS is correct " ) ); 
    
    QMessageBox::critical( this , tr( "StarViewer" ) , errorMessage );
}

void QueryScreen::errorQueringStudiesPacs( int PacsID )
{
    PacsListDB pacsListDB;
    PacsParameters errorPacs;
    QString errorMessage;
    
    pacsListDB.queryPacs( &errorPacs, PacsID );
    
    errorMessage.insert( 0 , tr( " Can't query studies to PACS " ) );
    errorMessage.append( errorPacs.getAEPacs().c_str() );
    errorMessage.append( tr ( " of " ) );
    errorMessage.append( errorPacs.getInstitution().c_str() );
    errorMessage.append( '\n' );
    errorMessage.append( tr( " Be sure that the IP and AETitle of the PACS is correct " ) ); 
    
    QMessageBox::critical( this , tr( "StarViewer" ) , errorMessage );
}

SeriesMask QueryScreen::buildSeriesMask( QString studyUID)
{
    SeriesMask mask;

    mask.setStudyUID( studyUID.toAscii().constData() );
    mask.setSeriesDate( "" );
    mask.setSeriesTime( "" );
    mask.setSeriesModality( "" );
    mask.setSeriesNumber( "" );
    mask.setSeriesBodyPartExaminated( "" );
    mask.setSeriesUID( "" );
    
    return mask;
}

QString QueryScreen::buildPatientName()
{
    QString patientName;
     
    if ( m_textFirstName->text().length() == 0 && m_textLastName->text().length() == 0)
    {
        patientName.append( "*" );
    }    
    else
    {
        patientName.append( m_textLastName->text() );
        //posem * per si nom�s posem el primer cognom
        patientName.append( "*" );//posem * per si no posa el nom complet, i espai per separar del nom
        if ( m_textFirstName->text().length() > 0)
        {
            patientName.append( m_textFirstName->text() );
            patientName.append( "*" );//per si no posa el nom complet
        }
        else patientName.append( "*" );
    }
    
    return patientName;
}

QString QueryScreen::buildStudyDates()
{
    QString date;
    
    if ( m_checkFrom->isChecked() && m_checkTo->isChecked() )
    {
        date.append( m_textFrom->date().toString( "yyyyMMdd" ) );
        date.append( "-" );
        date.append( m_textTo->date().toString( "yyyyMMdd" ) );
    }
    else 
    {
        if ( m_checkFrom->isChecked() )
        {
            date.append( m_textFrom->date().toString( "yyyyMMdd" ) );
            date.append( "-" ); // indiquem que volem buscar tots els estudis d'aquella data en endavant 
        }
        else if ( m_checkTo->isChecked() )
        {
            date.append( "-" ); //indiquem que volem buscar tots els estudis que no superin aquesta data
            date.append( m_textTo->date().toString( "yyyyMMdd" ) );
        }
    }
    
    return date;
}

StudyMask QueryScreen::buildMask()
{
    /*Per fer cerques entre valors consultat el cap�tol 4 de DICOM punt C.2.2.2.5*/
    /*Per defecte si passem un valor buit a la m�scara,far� una cerca per tots els els valor d'aquella camp*/    
    /*En aqu� hem de fer un set a tots els camps que volem cercar */
    StudyMask mask;
    QString modalityMask;
    
    mask.setPatientId(m_textPatientID->text().toStdString() );
    mask.setPatientName(buildPatientName().toStdString() );
    mask.setStudyId(m_textStudyID->text().toStdString() );
    mask.setStudyDate(buildStudyDates().toStdString() );
    mask.setStudyDescription( "" );
    mask.setStudyTime( "" );
    mask.setStudyUID( "" );
    mask.setInstitutionName( "" );
    mask.setStudyModality( "" );
    mask.setPatientAge( "" );
    mask.setAccessionNumber( m_textAccessionNumber->text().toStdString() );
    
    /*Aquesta mascara nom�s serveix per la cach�, no serveix pel pacs!!!!!!!!!!!!!!!!!!!1*/
    if ( !m_checkAll->isChecked() )
    { //es crea una sentencia per poder fer un in       
        if ( m_checkCT->isChecked() )
        {
            modalityMask.append( ",'CT'" );
        }
        if ( m_checkCR->isChecked() )
        {
            modalityMask.append( ",'CR'" );
        }        
        if ( m_checkDX->isChecked() )
        {
            modalityMask.append( ",'DX'" );
        }        
        if ( m_checkES->isChecked() )
        {
            modalityMask.append( ",'ES'" );
        }        
        if ( m_checkMG->isChecked() )
        {
            modalityMask.append( ",'MG'" );
        }        
        if ( m_checkMR->isChecked() )
        {
            modalityMask.append( ",'MR'" );
        }        
        if ( m_checkNM->isChecked() )
        {
            modalityMask.append( ",'NM'" );
        }        
        if ( m_checkDT->isChecked() )
        {
            modalityMask.append( ",'DT'" );
        }        
        if ( m_checkPT->isChecked() )
        {
            modalityMask.append( ",'PT'" );
        }        
        if ( m_checkRF->isChecked() )
        {
            modalityMask.append( ",'RF'" );
        }
        if ( m_checkSC->isChecked() )
        {
            modalityMask.append( ",'SC'" );
        }        
        if ( m_checkUS->isChecked() )
        {
            modalityMask.append( ",'US'" );
        }        
        if ( m_checkXA->isChecked() )
        {
            modalityMask.append( ",'XA'" );
        }        
        if  (modalityMask.length()> 0) 
        {
            modalityMask = modalityMask.replace( 0 , 1 , "(" );
            modalityMask.append( ")" );
            mask.setStudyModality(modalityMask.toAscii().constData() );
        }
    }
    
    return mask;
}

QString QueryScreen::logQueryStudy()
{
	QString logMessage;
		
    logMessage.insert( 0 , m_textPatientID->text() );
    logMessage.append(  ";" );
    logMessage.append(  buildPatientName() );
    logMessage.append(  ";" );
    logMessage.append(  m_textStudyID->text() );
    logMessage.append(  ";" );
    logMessage.append(  buildStudyDates() );
    logMessage.append(  ";" );
    logMessage.append(  m_textAccessionNumber->text() );
    logMessage.append(  ";" );

    return logMessage;
}

QueryScreen::~QueryScreen()
{
}

};

