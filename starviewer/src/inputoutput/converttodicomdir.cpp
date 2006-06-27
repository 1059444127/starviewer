/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include <QString>
#include <QProgressDialog>

#include "converttodicomdir.h"
#include <QDir>
#include <QChar>

#include "logging.h"
#include "status.h"
#include "cacheseriesdal.h"
#include "cacheimagedal.h"
#include "seriesmask.h"
#include "imagemask.h"
#include "serieslist.h"
#include "imagelist.h"
#include "series.h"
#include "image.h"
#include "convertdicomtolittleendian.h"
#include "starviewersettings.h"
#include "createdicomdir.h"

namespace udg {

ConvertToDicomDir::ConvertToDicomDir()
{
    QDir dicomDir;

    m_study = 0;
    m_series = 0;
    m_image = 0;

    //Creem el dicomdir temporal al tmp del l'usuari
    m_dicomDirPath = dicomDir.tempPath();
    m_dicomDirPath.append( "/dicom" );
    dicomDir.mkdir( m_dicomDirPath );
}


Status ConvertToDicomDir::convert( QString studyUID )
{
    QDir studyDir;
    QChar fillChar = '0';    
    CacheSeriesDAL cacheSeriesDAL;
    CacheImageDAL cacheImageDAL;
    //creem el nom del directori de l'estudi el format �s STUXXXXX, on XXXXX �s el numero d'estudi dins el dicomdir
    QString studyName = QString( "/STU%1" ).arg( m_study , 5 , 10 , fillChar );
    SeriesList seriesList;
    SeriesMask seriesMask;
    Series series;
    ImageMask imageMask;
    Status state;
    int imageNumber;
    CreateDicomDir createDicomDir;

    //comptem el numero d'imatges pel progress de la barra
    imageMask.setStudyUID( studyUID.toAscii().constData() );
    state = cacheImageDAL.countImageNumber( imageMask , imageNumber );

    if ( !state.good() ) return state;

    m_progress = new QProgressDialog( tr( "Converting Images..." ) , "" , 0 , imageNumber );
    m_progress->setMinimumDuration( 0 );
    m_progress->setCancelButton( 0 );

    m_study++;
    m_series = 0;
    
    //Creem el directori on es guardar l'estudi en format DicomDir
    m_dicomDirStudyPath = m_dicomDirPath + studyName;
    studyDir.mkdir( m_dicomDirStudyPath );

    seriesMask.setStudyUID( studyUID.toAscii().constData() );
    
    state = cacheSeriesDAL.querySeries( seriesMask , seriesList );

    if ( !state.good() ) return state;
    
    seriesList.firstSeries();

    while ( !seriesList.end() )
    {
        state = convertSeries( seriesList.getSeries() );
        
        if ( !state.good() ) 
        {
            break;
        }
        else seriesList.nextSeries();
    }

    state = createDicomDir.create ( m_dicomDirPath.toAscii().constData() );
    
    DEBUG_LOG( state.text().c_str() );

    m_progress->close();
    
    return state;
}

Status ConvertToDicomDir::convertSeries( Series series )
{
    QDir seriesDir;
    QChar fillChar = '0';    
    CacheImageDAL cacheImageDAL;
    //creem el nom del directori de la s�rie, el format �s SERXXXXX, on XXXXX �s el numero de s�rie dins l'estudi
    QString seriesName = QString( "/SER%1" ).arg( m_series , 5 , 10 , fillChar ); 
    Image image;
    ImageMask imageMask;
    ImageList imageList;
    Status state;

    m_series++;
    m_image = 0;
    //Creem el directori on es guardar� la s�rie en format DicomDir
    m_dicomDirSeriesPath = m_dicomDirStudyPath + seriesName;
    seriesDir.mkdir( m_dicomDirSeriesPath );

    imageMask.setSeriesUID( series.getSeriesUID() );
    imageMask.setStudyUID( series.getStudyUID() );

    state = cacheImageDAL.queryImages( imageMask , imageList );

    if ( !state.good() ) return state;
    
    imageList.firstImage();

    while ( !imageList.end() )
    {
        state = convertImage( imageList.getImage() );
        
        if ( !state.good() )
        {
            break;
        }
        else imageList.nextImage();
    }
    
    return state;
}

Status ConvertToDicomDir::convertImage( Image image )
{
    QChar fillChar = '0';    
    //creem el nom del fitxer de l'imatge, el format �s IMGXXXXX, on XXXXX �s el numero d'imatge dins la s�rie
    QString  imageName = QString( "/IMG%1" ).arg( m_image , 5 , 10 , fillChar ) , imageInputPath , imageOutputPath; 
    ConvertDicomToLittleEndian convertDicom;
    StarviewerSettings settings;
    Series serie;
    Status state;    
    
    m_image++;

    imageInputPath.insert( 0 , settings.getCacheImagePath() );
    imageInputPath.append( image.getStudyUID().c_str() );
    imageInputPath.append( "/" );
    imageInputPath.append( image.getSeriesUID().c_str() );
    imageInputPath.append( "/" );
    imageInputPath.append( image.getImageName().c_str() );

    imageOutputPath = m_dicomDirSeriesPath + imageName;

    state = convertDicom.convert( imageInputPath.toAscii().constData() , imageOutputPath.toAscii().constData() );

     m_progress->setValue( m_progress->value() + 1 );
     m_progress->repaint();
    
    return state;

}

ConvertToDicomDir::~ConvertToDicomDir()
{
}


}
