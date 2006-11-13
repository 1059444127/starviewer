/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                 *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "qlogviewer.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTextCodec>
#include "logging.h"

namespace udg {

QLogViewer::QLogViewer(QWidget *parent)
 : QDialog(parent)
{
    setupUi( this );
    // carreguem l'arxiu de log
    // \TODO aquest directori s'hauria de guardar en alguna mena de qsettings o similar
    QFile logFile( QDir::homePath() + "/.starviewer/log/starviewer.log" );
    if ( !logFile.open( QFile::ReadOnly | QFile::Text) )
    {
        ERROR_LOG( "No s'ha pogut obrir l'arxiu de logs" );
        m_logBrowser->setPlainText( tr("ERROR: No Log file found!") );
    }
    else
    {
        INFO_LOG( "S'ha obert amb �xit l'arxiu de logs" );
        QByteArray data = logFile.readAll();
        QTextCodec *codec = Qt::codecForHtml( data );
        QString str = codec->toUnicode( data );
        //     str = QString::fromLocal8Bit( data );
        m_logBrowser->setReadOnly( true );
        m_logBrowser->setPlainText( str );
    }
    createConnections();
}

QLogViewer::~QLogViewer()
{
}

void QLogViewer::createConnections()
{
    connect( m_closeButton , SIGNAL( clicked() ) , this , SLOT( close() ) );
}

}
