/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#include "qconfigurationscreen.h"

#include <math.h>

#include <QString>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>
#include <QCursor>
#include <QFileDialog>
#include <QLabel>
#include <QTreeView>

#include "pacsparameters.h"
#include "status.h"
#include "pacslistdb.h"
#include "pacslist.h"
#include "cachepool.h"
#include "starviewersettings.h"
#include "status.h"

#include "cachelayer.h"
#include "logging.h"
#include "pacsserver.h"
#include "pacsnetwork.h"
#include "cachetools.h"

namespace udg {


QConfigurationScreen::QConfigurationScreen( QWidget *parent )
 : QDialog( parent )
{
    setupUi( this );
    fillPacsListView(); //emplena el listview amb les dades dels pacs, que tenim configurats

    m_PacsID = 0;
    
    loadCacheDefaults();
    loadPacsDefaults();
    loadInstitutionInformation();
    m_buttonApplyCache->setEnabled(false);
    m_buttonApplyPacs->setEnabled(false);
    
    m_configurationChanged = false;
    m_createDatabase = false;
    
    createConnections();
	setIconButtons();

}

void QConfigurationScreen::createConnections()
{
    //connecta el boto examinar de la cache amb el dialog per escollir el path de la base de dades
    connect( m_buttonExaminateDataBase , SIGNAL( clicked() ) , this , SLOT( examinateDataBaseRoot() ) );
    
    //connecta el boto examinar de la cache amb el dialog per escollir el path de la base de dades
    connect( m_buttonExaminateCacheImage , SIGNAL( clicked() ) , this, SLOT( examinateCacheImagePath() ) );
    
    //connecta el boto acceptar de la cache amb l'slot accept
    connect( m_buttonAcceptCache , SIGNAL( clicked() ) , this , SLOT( acceptChanges() ) );
    
    //connecta el boto cancelar de la cache amb l'slot cancel
    connect( m_buttonCancelCache , SIGNAL( clicked()) , this , SLOT( cancelChanges( ) ) );
    
    //connecta el boto aplicar de la cache amb l'slot apply
    connect( m_buttonApplyCache , SIGNAL( clicked() ) , this ,  SLOT( applyChanges() ) );
    
    //connecta el boto acceptar del pacs amb l'slot accept
    connect( m_buttonAcceptPacs , SIGNAL( clicked() ), this ,  SLOT( acceptChanges() ) );
    
    //connecta el boto cancelar del pacs amb l'slot cancel
    connect( m_buttonCancelPacs , SIGNAL( clicked() ) , this ,  SLOT( cancelChanges() ) );
    
    //connecta el boto aplicar del Pacs amb l'slot apply
    connect( m_buttonApplyPacs , SIGNAL( clicked() ) , this ,  SLOT( applyChanges() ) );
    
    //connecta el boto acceptar de l'informaci� de l'instituci� amb l'slot accept
    connect( m_buttonAcceptInstitution , SIGNAL( clicked() ), this ,  SLOT( acceptChanges() ) );
    
    //connecta el boto cancelar de l'informaci� de l'instituci� amb l'slot cancel
    connect( m_buttonCancelInstitution , SIGNAL( clicked() ) , this ,  SLOT( cancelChanges() ) );
    
    //connecta el boto aplicar de l'informaci� de l'instituci� amb l'slot apply
    connect( m_buttonApplyInstitution , SIGNAL( clicked() ) , this ,  SLOT( applyChanges() ) );
    
    //connecta el boto aplicar de l'informaci� de l'instituci� amb l'slot apply
    connect( m_buttonCreateDatabase , SIGNAL( clicked() ) , this ,  SLOT( createDatabase() ) );
    
    //activen el boto apply quant canvia el seu valor
    connect( m_textDatabaseRoot , SIGNAL( textChanged(const QString &) ), this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textCacheImagePath , SIGNAL( textChanged(const QString &) ), this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textPoolSize , SIGNAL( textChanged(const QString &) ) , this, SLOT( configurationChanged( const QString& ) ) );
    connect( m_textAETitleMachine , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textTimeout , SIGNAL( textChanged(const QString &) ), this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textDatabaseRoot , SIGNAL( textChanged(const QString &) ), this , SLOT ( configurationChangedDatabaseRoot( const QString& ) ) );
    connect( m_textLocalPort , SIGNAL( textChanged(const QString &) ), this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textMaxConnections , SIGNAL( textChanged(const QString &) ), this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_comboLanguage , SIGNAL( editTextChanged(const QString &) ), this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionName , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionAddress , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionTown , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionZipCode , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionCountry , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionPhoneNumber , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textInstitutionEmail , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );
    connect( m_textMaximumDaysNotViewed , SIGNAL( textChanged(const QString &) ) , this , SLOT( configurationChanged( const QString& ) ) );    
    
    //mateniment base de dades
    connect( m_buttonDeleteStudies , SIGNAL( clicked() ) , this , SLOT( deleteStudies() ) );
    connect( m_buttonCompactDatabase , SIGNAL( clicked() ), this , SLOT( compactCache() ) );
    
    //afegeix la / al final del Path de la cache d'imatges
    connect( m_textCacheImagePath , SIGNAL( editingFinished() ) , this , SLOT( cacheImagePathEditingFinish() ) );
    
    //manteniment PACS
    connect( m_buttonAddPacs , SIGNAL( clicked() ) , this , SLOT( addPacs() ) );
    connect( m_buttonDeletePacs , SIGNAL( clicked() ) , this , SLOT( deletePacs() ) );
    connect( m_buttonUpdatePacs , SIGNAL( clicked() ), this , SLOT( updatePacs() ) );
    connect( m_buttonClear , SIGNAL( clicked() ) , this , SLOT( clear() ) );
    connect( m_buttonTestPacs , SIGNAL( clicked() ) , this , SLOT( test() ) );
    connect( m_PacsTreeView , SIGNAL( itemClicked ( QTreeWidgetItem * , int) ) , this , SLOT( selectedPacs( QTreeWidgetItem * , int ) ) );
}

void QConfigurationScreen::setIconButtons()
{
    m_buttonAcceptCache->setIcon( QIcon( ":images/button_ok.png" ) );
    m_buttonAcceptPacs->setIcon( QIcon( ":images/button_ok.png" ) );
    m_buttonAcceptInstitution->setIcon( QIcon( ":images/button_ok.png" ) );
	
    m_buttonApplyPacs->setIcon( QIcon( ":images/apply.png" ) );
    m_buttonApplyCache->setIcon( QIcon( ":images/apply.png" ) );
    m_buttonApplyInstitution->setIcon( QIcon( ":images/apply.png" ) );	
    
    m_buttonCancelPacs->setIcon( QIcon( ":images/cancel.png" ) );
    m_buttonCancelCache->setIcon( QIcon( ":images/cancel.png" ) );
    m_buttonCancelInstitution->setIcon( QIcon( ":images/cancel.png" ) );
}

void QConfigurationScreen::loadCacheDefaults()
{
    StarviewerSettings settings;

    m_textDatabaseRoot->setText( settings.getDatabasePath() );
    m_textCacheImagePath->setText( settings.getCacheImagePath() );
    m_textMaximumDaysNotViewed->setText( settings.getMaximumDaysNotViewedStudy() );
    
    loadCachePoolDefaults();
}

void QConfigurationScreen::loadCachePoolDefaults()
{
    Status state;
    unsigned int space,used;
    float result;
    QString text;
    CachePool pool;
    
    //accemdim a la cach� a agafar les dades del Pool
    state = pool.getPoolTotalSize( space );   
    if ( !state.good() )
    {
        databaseError( &state );
        return;
    }
    
    state = pool.getPoolUsedSpace( used );
    if ( !state.good() )
    {
        databaseError( &state );
        return;
    }
    
    m_textPoolSize->setText( text.setNum( space / 1024 , 10 ) );
    
    result = used;
    result = result / 1024; //passem Mb a Gb;
    text.setNum( result , 'f' , 2 );
    text.append( " Gb" );
    m_PoolUsed->setText( text );
    
    //espai lliure
    result = space - used;
    result = result / 1024;
    text.setNum( result , 'f' , 2 );
    text.append( " Gb" );
    m_PoolFree->setText( text );
    
    //Percentatges
    result = used;
    result = result / space * 100;
    text.setNum( result , 'f' , 1 );
    text.append( " %" );
    m_PoolUsedPerc->setText( text );
    
    result = ( space - used );
    result = result / space * 100;
    text.setNum( result , 'f' , 1 );
    text.append( " %" );
    m_PoolFreePerc->setText( text );
}

void QConfigurationScreen::loadPacsDefaults()
{
    QString result;
    StarviewerSettings settings;

    m_textAETitleMachine->setText( settings.getAETitleMachine() );
    m_textLocalPort->setText( settings.getLocalPort() );
    m_textTimeout->setText( settings.getTimeout() );
    m_textMaxConnections->setText( settings.getMaxConnections() );
}

void QConfigurationScreen::loadInstitutionInformation()
{
    StarviewerSettings settings;
    
    m_textInstitutionName->setText( settings.getInstitutionName() );
    m_textInstitutionAddress->setText( settings.getInstitutionAddress() );
    m_textInstitutionTown->setText( settings.getInstitutionTown() );
    m_textInstitutionZipCode->setText( settings.getInstitutionZipCode() );
    m_textInstitutionCountry->setText( settings.getInstitutionCountry() );
    m_textInstitutionPhoneNumber->setText( settings.getInstitutionPhoneNumber() );
    m_textInstitutionEmail->setText( settings.getInstitutionEmail() );
}

/************************************************************************************************************************/
/*                                              PACS DEVICE                                                             */
/************************************************************************************************************************/

void QConfigurationScreen:: clear()
{
    m_textAETitle->setText( "" );
    m_textAddress->setText( "" );
    m_textPort->setText( "" );
    m_textInstitution->setText( "" );
    m_textLocation->setText( "" );
    m_textDescription->setText( "" );
    m_checkDefault->setChecked( false );
    m_PacsID = 0;
    
}

void QConfigurationScreen::addPacs()
{
    PacsParameters pacs;
    Status state;
    PacsListDB pacsList;
	QString logMessage;
    
    if (validatePacsParameters())
    {
        pacs.setAEPacs( m_textAETitle->text().toStdString() );
        pacs.setPacsPort( m_textPort->text().toStdString() );
        pacs.setPacsAdr( m_textAddress->text().toStdString() );
        pacs.setInstitution( m_textInstitution->text().toStdString() );
        pacs.setLocation( m_textLocation->text().toStdString() );
        pacs.setDescription( m_textDescription->text().toStdString() );
        if ( m_checkDefault->isChecked() )
        {
            pacs.setDefault( "S" );
        }
        else pacs.setDefault( "N" );
       
		logMessage = "Afegir pacs ";
		logMessage.append( m_textAETitle->text() );
		INFO_LOG ( logMessage.toAscii().constData() );
        
        state =  pacsList.insertPacs( &pacs );

        if ( !state.good() )
        {
            if ( state.code() == 2019 )
            {
            
                QMessageBox::warning( this , tr("StarViewer") , tr("AETitle ") + pacs.getAEPacs().c_str() + tr(" exists") + "\n" );
            }else databaseError( &state );

        }
        else 
        {
            fillPacsListView();
            clear();
            emit( pacsListChanged() ); 
        }
    }
}

void QConfigurationScreen::selectedPacs( QTreeWidgetItem * item , int )
{
    PacsList list;
    PacsParameters pacs;
    Status state;
    PacsListDB pacsList;

    if ( item != NULL )
    {
        state = pacsList.queryPacsList( list );        
        
        if ( !state.good() )
        {
            databaseError( &state );
            return;
        }
                
        if (list.findPacs( item->text(0).toStdString() ) ) //busquem les dades del PACS
        {   
            pacs = list.getPacs();
            
            //emplenem els textots
            m_textAETitle->setText( pacs.getAEPacs().c_str() );
            m_textPort->setText( pacs.getPacsPort().c_str() );
            m_textAddress->setText( pacs.getPacsAdr().c_str() );
            m_textInstitution->setText( pacs.getInstitution().c_str() );
            m_textLocation->setText( pacs.getLocation().c_str() );
            m_textDescription->setText( pacs.getDescription().c_str() );
            m_PacsID = pacs.getPacsID();
            if ( pacs.getDefault() == "S" )
            {
                m_checkDefault->setChecked( true );
            }
            else m_checkDefault->setChecked( false );
       }
    }
}

void QConfigurationScreen::updatePacs()
{
    PacsParameters pacs;
    Status state;
    PacsListDB pacsList;
	QString logMessage;
    
    if ( m_PacsID == 0 )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Select a Pacs for update" ) );
        return;
    }
   
    if ( validatePacsParameters() )
    {
        pacs.setAEPacs( m_textAETitle->text().toStdString() );
        pacs.setPacsPort( m_textPort->text().toStdString() );
        pacs.setPacsAdr( m_textAddress->text().toStdString() );
        pacs.setInstitution( m_textInstitution->text().toStdString() );
        pacs.setLocation( m_textLocation->text().toStdString() );
        pacs.setDescription( m_textDescription->text().toStdString() );
        pacs.setPacsID( m_PacsID );
        if ( m_checkDefault->isChecked() )
        {
            pacs.setDefault( "S" );
        }
        else pacs.setDefault( "N" );
       
        logMessage = "Actualitzant dades del pacs ";
        logMessage.append( m_textAETitle->text() );
        INFO_LOG ( logMessage.toAscii().constData() );

        state = pacsList.updatePacs( &pacs );
         
        if ( !state.good() )
        {
            databaseError( &state );
        }
        else 
        {
            fillPacsListView();
            clear();
            emit( pacsListChanged() ); 
        }
    }
}

void QConfigurationScreen::deletePacs()
{
    Status state;
    PacsParameters pacs;
    PacsListDB pacsList;
    QString logMessage;
    
    if ( m_PacsID == 0 )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Select a Pacs for delete" ) );
        return;
    }    
    
    pacs.setPacsID( m_PacsID );//per donar de baixa n'hi prou amb el camp clau    

    logMessage = "Esborrant el pacs ";
    logMessage.append( m_textAETitle->text() );
    INFO_LOG ( logMessage.toAscii().constData() );

    state = pacsList.deletePacs( &pacs );
    
    if ( !state.good() )
    {
        databaseError( &state );
    }
    else
    {
        fillPacsListView();
        clear();
        emit( pacsListChanged() ); 
    }
}

void QConfigurationScreen::fillPacsListView()
{
    Status state;
    PacsParameters pacs;
    PacsList list;
    PacsListDB pacsList;
    
    m_PacsTreeView->clear();
    
    state = pacsList.queryPacsList( list );
    
    if ( state.good() )
    {
        list.firstPacs();
        
        while ( !list.end() )
        {
            QTreeWidgetItem* item = new QTreeWidgetItem( m_PacsTreeView );            
            pacs = list.getPacs();
            item->setText( 0 , pacs.getAEPacs().c_str() );
            item->setText( 1 , pacs.getPacsAdr().c_str() );
            item->setText( 2 , pacs.getPacsPort().c_str() );
            item->setText( 3 , pacs.getInstitution().c_str() );
            item->setText( 4 , pacs.getLocation().c_str() );
            item->setText( 5 , pacs.getDescription().c_str() );
            
            if ( pacs.getDefault() == "S" )
            {
                item->setText( 6 , tr( "&Yes" ) );
            }
            else item->setText( 6 , tr( "&No" ) );
            
            list.nextPacs();
        }
    }
}

void QConfigurationScreen::test()
{
    Status state;
    PacsParameters pacs;
    PacsListDB pacsList;
    PacsServer pacsServer;
    QString message , logMessage;
    StarviewerSettings settings;

    //Agafem les dades del PACS que estan el textbox per testejar
    pacs.setAEPacs( m_textAETitle->text().toStdString() );
    pacs.setPacsPort( m_textPort->text().toStdString() );
    pacs.setPacsAdr( m_textAddress->text().toStdString() );
    pacs.setAELocal( settings.getAETitleMachine().toAscii().constData() );
    pacsServer.setPacs( pacs );
    
    state = pacsServer.connect( PacsServer::echoPacs , PacsServer::studyLevel );
    
    if ( !state.good() )
    {
        message.insert( 0 , tr( " Pacs " ) );
        message.append( pacs.getAEPacs().c_str() );
        message.append( tr ( " doesn't responds " ) );
        message.append( '\n' );
        message.append( tr( " Be sure that the IP and AETitle of the PACS is correct " ) ); 
        QMessageBox::warning( this , tr("StarViewer") , message );
        
        logMessage.insert( 0 , "Doing echo pacs " );
        logMessage.append( pacs.getAEPacs().c_str() );
        logMessage.append( " doesn't responds. PACS ERROR : " );
        logMessage.append( state.text().c_str() );
        INFO_LOG ( logMessage.toAscii().constData() );
    }
    else
    {
        state = pacsServer.echo();
        
        if ( state.good() )
        {
            message.insert( 0 , tr( " Test of Pacs " ) );
            message.append( pacs.getAEPacs().c_str() );
            message.append( tr ( " is correct " ) );
            QMessageBox::information( this , tr("StarViewer") , message );
    
            logMessage.insert( 0 , "Test of Pacs " );
            logMessage.append( pacs.getAEPacs().c_str() );
            logMessage.append( " is correct " );
            INFO_LOG ( logMessage.toAscii().constData() );
        }
        else
        {
            message.insert( 0 , tr( " Pacs " ) );
            message.append( pacs.getAEPacs().c_str() );
            message.append( tr ( " doesn't responds correctly" ) );
            message.append( '\n' );
            message.append( tr( " Be sure that the IP and AETitle of the PACS is correct " ) ); 
            QMessageBox::warning( this , tr("StarViewer") , message );
            
            logMessage.insert( 0 , "Doing echo pacs " );
            logMessage.append( pacs.getAEPacs().c_str() );
            logMessage.append( " doesn't responds correctly. PACS ERROR : " );
            logMessage.append( state.text().c_str() );
            INFO_LOG ( logMessage.toAscii().constData() );
        }
    }
    
}

bool QConfigurationScreen::validatePacsParameters()
{
    QString text;
    
    //Per for�a tot els pacs han de tenir algun AETitle
    text = m_textAETitle->text();
    if ( text.length() == 0 )
    {
         QMessageBox::warning( this , tr("StarViewer") , tr("AETitle field can't be empty") );
        return false;
    }
    
    //adre�a del pacs no pot estar en blanc
    text = m_textAddress->text();
    if ( text.length() == 0 )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr ( "Incorrect address server" ) );
        return false;
    }   
    
    //el port ha d'estar entre 0 i 65535
    text = m_textPort->text();
    if ( !( text.toInt( NULL , 10 ) >= 0 && text.toInt( NULL , 10 ) <= 65535 ) || text.length() ==0 )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "PACS Port has to be between 0 and 65535" ) );
        return false;    
    }
    
    //la instituci� no pot estar en blanc
    text = m_textInstitution->text();
    if ( text.length() == 0 )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Institution field can't be empty" ) );
        return false;
    }   
    
    return true;
}

bool QConfigurationScreen::validateChanges()
{
    QDir dir;
	unsigned int usedSpace;
	CachePool pool;

    if ( m_textLocalPort->isModified() )
    {
        if ( m_textLocalPort->text().toInt( NULL , 10 ) < 0 || m_textLocalPort->text().toInt( NULL , 10 ) > 65535 )
        {
            QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Local Port has to be between 0 and 65535" ) );
            return false;        
        }
    }
    
    if ( m_textMaxConnections->isModified() )
    {
        if ( m_textMaxConnections->text().toInt( NULL , 10 ) < 1 || m_textMaxConnections->text().toInt( NULL , 10 ) > 15 )
        {
            QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Maximum simultaenious connections has to be between 1 and 15" ) );
            return false;        
        }
    }    
    
    if ( m_textDatabaseRoot->isModified() )
    {
        if ( !dir.exists(m_textDatabaseRoot->text() ) && m_createDatabase == false ) // si el fitxer indicat no existeix i no s'ha demanat que es crei una nova base de dades, el path �s inv�lid
        {
            QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Invalid database path" ) );
            return false;             
        }
    }

    if ( m_textCacheImagePath->isModified() )
    {
        if ( !dir.exists(m_textCacheImagePath->text() ) )
        {
            switch ( QMessageBox::question( this ,
                    tr( "Create directory ?" ) ,
                    tr( "The cache image directory doesn't exists. Do you want to create it ?" ) ,
                    tr( "&Yes" ) , tr( "&No" ) , 0 , 1 ) )
            {
                case 0:
                    if ( !dir.mkpath( m_textCacheImagePath->text() ) )
                    {
                        QMessageBox::critical( this , tr( "StarViewer" ) , tr( "Can't create the directory. Please check users permission" ) );
                        return false;
                    }
                    else return true;
                    break;
                case 1: 
                    return false;
                    break;
            }
        }
    }    

	if ( m_textPoolSize->isModified() )
	{
		//hem de comprova que si canviem el tamany de la cache, si el reduim que no sigui mes petit que l'espia usat
		pool.getPoolUsedSpace( usedSpace );

		if ( m_textPoolSize->text().toUInt( NULL , 10 )* 1024 < usedSpace )
		{
            QMessageBox::warning( this , tr( "StarViewer" ) , tr( "Pool space can't be less than used space" ) );		
			return false;
		}
	}

    return true;
}

void QConfigurationScreen::acceptChanges()
{
    if ( m_configurationChanged )        
    {
        if ( applyChanges() ) this->hide();
    }
    else this->hide();
}

void QConfigurationScreen::cancelChanges()
{   
    this->hide();
}

bool QConfigurationScreen::applyChanges()
{

    if (validateChanges())
    {
        applyChangesPacs();
        applyChangesCache();
        loadCachePoolDefaults();
        applyChangesInstitution();
        
        if ( m_textDatabaseRoot->isModified() && m_createDatabase == false ) // nom�s s'ha de reiniciar en el cas que que s'hagi canviat el path de la base de dades, per una ja existent. En el cas que la base de dades no existeixi, a l'usuari al fer click al bot� crear base de dades, ja se li haur� informat que s'havia de reiniciar l'aplicaci�
        {
            QMessageBox::warning( this , tr( "StarViewer" ) , tr( "The application has to be restart to apply the changes" ) );
        }

        m_configurationChanged = false;
        
        return true;
    }
    else return false;
}

void QConfigurationScreen::applyChangesPacs()
{
    StarviewerSettings settings;
    QString logMessage;
    
    if ( m_textAETitleMachine->isModified() )
    {
        logMessage = "Modificaci� del AETitle de la m�quina ";
        logMessage.append( m_textAETitleMachine->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        settings.setAETitleMachine(m_textAETitleMachine->text());
    }
    
    if ( m_textTimeout->isModified() )
    {
        logMessage = "Modificaci� del valor del timeout ";
        logMessage.append( m_textTimeout->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        settings.setTimeout(m_textTimeout->text());
    }
    
    if ( m_textLocalPort->isModified() )
    {
        logMessage = "Modificaci� del Port d'entrada dels estudis";
        logMessage.append( m_textLocalPort->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        settings.setLocalPort( m_textLocalPort->text() );
    }
    
    if ( m_textMaxConnections->isModified() )
    {
        logMessage = "Modificaci� del nombre m�xim de connexions ";
        logMessage.append( m_textMaxConnections->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        settings.setMaxConnections( m_textMaxConnections->text() );
    }
    
    m_buttonApplyPacs->setEnabled( false );
}

void QConfigurationScreen::configurationChanged ( const QString& )
{
    m_buttonApplyPacs->setEnabled( true );
    m_buttonApplyCache->setEnabled( true );
    m_buttonApplyInstitution->setEnabled( true );    
    m_configurationChanged = true;
}

void QConfigurationScreen::configurationChangedDatabaseRoot ( const QString& )
{
    m_createDatabase= false; //indiquem no s'ha demanat que es cre� la base de dades indicada a m_textDatabaseRoot
    configurationChanged( "" );
}


void QConfigurationScreen::examinateDataBaseRoot()
{
      //a la p�gina de QT indica que en el cas que nomes deixem seleccionar un fitxer, agafar el primer element de la llista i punt, no hi ha cap m�tode que te retornin directament el fitxer selccionat
      
    QFileDialog *dlg = new QFileDialog( 0 , QFileDialog::tr( "Open" ) , "./" , "Starviewer Database (*.sdb)" );

    dlg->setFileMode( QFileDialog::ExistingFile );
    
    if ( dlg->exec() == QDialog::Accepted ) 
    {
        if ( !dlg->selectedFiles().empty() ) m_textDatabaseRoot->setText( dlg->selectedFiles().takeFirst() );
    }
    
    delete dlg;
}

void QConfigurationScreen::examinateCacheImagePath()
{
    QFileDialog *dlg = new QFileDialog( 0 , QFileDialog::tr( "Open" ) , "./" , tr( "Cache Directory" ) );
    QString path;
    
    dlg->setFileMode( QFileDialog::DirectoryOnly );
    
    if ( dlg->exec() == QDialog::Accepted )
    {
        if ( !dlg->selectedFiles().empty() ) m_textCacheImagePath->setText( dlg->selectedFiles().takeFirst() );
        cacheImagePathEditingFinish();//afegeix la '/' al final
    }
       
    delete dlg;
}

void QConfigurationScreen::applyChangesCache()
{
    StarviewerSettings settings;
    CachePool pool;
    Status state;
    QString logMessage;
    
    //Aquest els guardem sempre 
    settings.setCacheImagePath( m_textCacheImagePath->text() );
    settings.setDatabasePath( m_textDatabaseRoot->text() );
    
    if ( m_textPoolSize->isModified() )
    {   
        logMessage = "Es modificar� la mida de la cache ";
        logMessage.append( m_textPoolSize->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        state = pool.updatePoolTotalSize( m_textPoolSize->text().toInt( NULL , 10 )* 1024 );//Passem l'espai a Mb
        databaseError( &state );
    }
    
    if ( m_textCacheImagePath->isModified() )
    {
        logMessage = "Es modificar� el directori de la cache d'imatges ";
        logMessage.append( m_textCacheImagePath->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        settings.setCacheImagePath( m_textCacheImagePath->text() );
    }
    
    if ( m_textMaximumDaysNotViewed->isModified() )
    {
        logMessage = "Es modificar� el nombre maxim de dies d'un estudi a la cache";
        logMessage.append( m_textMaximumDaysNotViewed->text() );
        INFO_LOG( logMessage.toAscii().constData() );
        
        settings.setMaximumDaysNotViewedStudy( m_textMaximumDaysNotViewed->text() );
    }
    
    m_buttonApplyCache->setEnabled( false );
}

void QConfigurationScreen::deleteStudies()
{
    Status state;
    CacheLayer cacheLayer;
   
   switch( QMessageBox::information( this , tr( "Starviewer" ),
				      tr ( "Are you sure you want to delete all Studies of the cache ?" ),
				      tr( "&Yes" ) , tr( "&No" ) ,
				      0 , 1 ) ) 
    {
    case 0:
        
        INFO_LOG ( "Neteja de la cache" );
    
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            
        state =  cacheLayer.clearCache();
       
        QApplication::restoreOverrideCursor();
 
        if ( !state.good() )
        {
            databaseError( &state );
        }
        
        loadCachePoolDefaults();
        emit( cacheCleared() );
       
        break;
    }
}

void QConfigurationScreen::compactCache()
{
    CacheTools cacheTools;
    Status state;
    
    INFO_LOG( "Compactacio de la cache" );
    
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    state = cacheTools.compactCachePacs();

    QApplication::restoreOverrideCursor();
    
    if ( !state.good() )
    {
        databaseError( &state );
    }
	
}

void QConfigurationScreen::cacheImagePathEditingFinish()
{
    QString path;
    
    if ( !m_textCacheImagePath->text().endsWith( '/' , Qt::CaseInsensitive ) )
    {
        path = m_textCacheImagePath->text();
        path.append( "/" );
        m_textCacheImagePath->setText( path );
    }
}

void QConfigurationScreen::applyChangesInstitution()
{
    StarviewerSettings settings;

    if ( m_textInstitutionName->isModified() ) settings.setInstitutionName( m_textInstitutionName->text() );
    
    if ( m_textInstitutionAddress->isModified() ) settings.setInstitutionAddress( m_textInstitutionAddress->text() );
    
    if ( m_textInstitutionTown->isModified() ) settings.setInstitutionTown( m_textInstitutionTown->text() );
    
    if ( m_textInstitutionZipCode->isModified() ) settings.setInstitutionZipCode( m_textInstitutionZipCode->text() );
    
    if ( m_textInstitutionCountry->isModified() ) settings.setInstitutionCountry( m_textInstitutionCountry->text() );
    
    if ( m_textInstitutionPhoneNumber->isModified() ) settings.setInstitutionPhoneNumber( m_textInstitutionPhoneNumber->text() );
    
    if ( m_textInstitutionEmail->isModified() ) settings.setInstitutionEmail( m_textInstitutionEmail->text() );
    
    m_buttonApplyInstitution->setEnabled( false );
}

void QConfigurationScreen::createDatabase()
{
    StarviewerSettings settings;
    QFile databaseFile;
    QString stringDatabasePath;
    
    if ( m_textDatabaseRoot->text().right(4) != ".sdb" )
    {
        QMessageBox::warning( this , tr( "StarViewer" ) , "The extension of the database has to be '.sdb'" );        
    }
    else
    {
        if ( databaseFile.exists( m_textDatabaseRoot->text() ) )
        {
            QMessageBox::warning( this , tr( "StarViewer" ) , "Starviewer can't create the database because, a database with the same name exists in the directory" );                  
        }
        else
        {
            settings.setDatabasePath( m_textDatabaseRoot->text() );
            QMessageBox::warning( this , tr( "StarViewer" ) , "The application has to be restart to apply the changes" );        
            m_createDatabase = true;
        }
    }
}

void QConfigurationScreen::databaseError(Status *state)
{
    QString text , code;
    if ( !state->good() )
    {
        switch( state->code() )
        {  case 2001 : text.insert( 0, tr("Database is corrupted or SQL syntax error") );
                        text.append( "\n" );
                        text.append( tr( "Error Number : " ) );
                        code.setNum( state->code() , 10 );
                        text.append( code );
                        break;
            case 2005 : text.insert( 0 , tr("Database is locked") );
                        text.append( "\n" );
                        text.append( tr( "To solve this error restart the user session" ) );
                        text.append( "\n" );
                        text.append( tr( "Error Number : " ) );
                        code.setNum( state->code() , 10 );
                        text.append( code );
                        break;
            case 2011 : text.insert( 0 , tr( "Database is corrupted." ) );
                        text.append( "\n" );
                        text.append( tr( "Error Number : " ) );
                        code.setNum( state->code() , 10 );
                        text.append( code );
                        break;
            case 2019 : text.insert( 0 , tr( "Register duplicated." ) );
                        text.append( "\n" );
                        text.append( tr( "Error Number : " ) );
                        code.setNum( state->code() , 10 );
                        text.append( code );
                        break;
            case 2050 : text.insert( 0 , "Not Connected to database" );
                        text.append( "\n" );
                        text.append( tr( "Error Number : " ) );
                        code.setNum( state->code() , 10 );
                        text.append( code );
                        break;            
            default :   text.insert( 0 , tr( "Internal Database error" ) );
                        text.append( "\n" );
                        text.append( tr( "Error Number : " ) );
                        code.setNum( state->code() , 10 );
                        text.append( code );
        }
        QMessageBox::critical( this , tr( "StarViewer" ) , text );
    }    
}

QConfigurationScreen::~QConfigurationScreen()
{
}

};
