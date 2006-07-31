/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "qapplicationmainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QDir>
#include "logging.h"
#include "extensions.h"

void configureLogging()
{
    // \TODO donem per fet que l'arxiu es diu aix� i es troba a la localitzaci� que indiquem. S'hauria de fer una mica m�s flexible o gen�ric; est� aix� perqu� de moment volem anar per feina i no entretenir-nos per� s'ha de fer b�.
    QString configurationFile = "/etc/starviewer/log.conf";
    QDir logFile( QDir::homePath() + "/.starviewer/log" );

    if (!logFile.exists())
    {
        logFile.mkpath( QDir::homePath() + "/.starviewer/log" );
    }
    LOGGER_INIT( configurationFile.toStdString() );
}

int main(int argc, char *argv[])
{ 
    QApplication app(argc, argv);
    
    app.setOrganizationName("GGG");
    app.setOrganizationDomain("ima.udg.es");
    app.setApplicationName("Starviewer");
     
    configureLogging();
    // translation
    QSettings settings("GGG", "StarViewer-Core");
    settings.beginGroup("StarViewer-Language");
    QString m_defaultLocale = settings.value( "languageLocale", "interface_" + QLocale::system().name() ).toString();
    settings.endGroup();

    QTranslator m_applicationTranslator;
    m_applicationTranslator.load( QString(":/translations/") + m_defaultLocale );
    app.installTranslator( &m_applicationTranslator );
    
    QSplashScreen *splash = new QSplashScreen( QPixmap(":/images/splash.png") );
    splash->show();
    
    udg::QApplicationMainWindow *mainWin = new udg::QApplicationMainWindow;
    INFO_LOG("Creada finestra principal")
    mainWin->show();
    
    QObject::connect( &app, SIGNAL( lastWindowClosed() ),
                      &app, SLOT( quit() )); 
    splash->finish( mainWin );
    delete splash;
    
    return app.exec();
}
