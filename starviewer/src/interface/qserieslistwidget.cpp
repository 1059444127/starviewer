/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#include "qserieslistwidget.h"

#include <QString>
#include <QIcon>

#include "starviewersettings.h"

namespace udg {

QSeriesListWidget::QSeriesListWidget(QWidget *parent )
 : QWidget( parent )
{
    setupUi( this );
    QSize size;    

    //Definim la mida de la imatge que mostrem
    size.setHeight( 100 );
    size.setWidth( 100 );
    m_seriesListWidget->setIconSize( size );
    
    createConnections();
}

void QSeriesListWidget::createConnections()
{
    connect( m_seriesListWidget , SIGNAL( itemClicked ( QListWidgetItem *) ) , SLOT( clicked( QListWidgetItem * ) ) );
    connect( m_seriesListWidget , SIGNAL( itemDoubleClicked ( QListWidgetItem * ) ), SLOT( view(QListWidgetItem * ) ) );
}

void QSeriesListWidget::insertSeries( Series *serie )
{
    QString text,num,pathImage,nameClass;
    StarviewerSettings settings;
    QListWidgetItem *item = new QListWidgetItem( m_seriesListWidget );
    QString statusTip;
    
    text.insert( 0 , tr( "Series " ) );
    text.append( serie->getSeriesNumber().c_str() );
    
    if ( serie->getProtocolName().length() > 0 )
    {//si hi ha descripci� la inserim
        text.append( " " );
        text.append (serie->getProtocolName().c_str() );
        text.append( '\n' );
    }
    else text.append( '\n' );
    
    if ( serie->getImageNumber() > 0 )
    {
        num.setNum( serie->getImageNumber() );
        text.append( num );
        text.append( tr(" images") );
        text.append( '\n' );
    }
    
    nameClass.insert( 0 , this->objectName() );
    if ( nameClass == "m_seriesListWidgetCache" )
    {
        pathImage.insert( 0 , settings.getCacheImagePath() );
        pathImage.append(serie->getStudyUID().c_str() );
        pathImage.append( "/" );
        pathImage.append( serie->getSeriesUID().c_str() );
        pathImage.append( "/scaled.jpeg" );
    }
    else
    {
        // \TODO lleig!!!!!!!!!!!!!!!!!
        pathImage = "/home/marc/starviewer-pacs/bin/images3.jpeg" ;
    }
    
    QIcon   icon(pathImage);

    item->setText(text);
    item->setIcon(icon);
    
    /* A l'status Tip de cada item es guarda la UID de la serie, ja que aquest camp no el vull mostrar i no tinc
   enlloc per amagar-lo, ho utilitzo per identificar la s�rie */
    item->setStatusTip(serie->getSeriesUID().c_str()); 
}

void QSeriesListWidget::clicked( QListWidgetItem *item )
{
     if ( item != NULL ) emit( selectedSeriesIcon( item->statusTip() ) );
}

void QSeriesListWidget::view( QListWidgetItem *item )
{
    if ( item != NULL ) emit( viewSeriesIcon() );
}

void QSeriesListWidget::selectedSeriesList( QString seriesUID )
{
    QList< QListWidgetItem * > qSeriesList( m_seriesListWidget->findItems( "*" , Qt::MatchWildcard ) );
    QListWidgetItem *item;
    
    for ( int i = 0;i < qSeriesList.count(); i++ )
    {
        item = qSeriesList.at( i );
        if ( item->statusTip() == seriesUID )
        {
            m_seriesListWidget->setItemSelected( item , true );
            m_seriesListWidget->setCurrentItem( item );
        }
        else m_seriesListWidget->setItemSelected( item , false );
    }  
}

void QSeriesListWidget::clear()
{
    m_seriesListWidget->clear();
}

void QSeriesListWidget::addSeries( Series *serie )
{
    insertSeries( serie );
}

void QSeriesListWidget::clearSeriesListWidget()
{
    clear();
}

QSeriesListWidget::~QSeriesListWidget()
{
}

};
