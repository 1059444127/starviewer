/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGINPUT_H
#define UDGINPUT_H

#include <QObject>
#include <iostream>

#include "volume.h"
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>

#include <itkCommand.h>
#include <itkSmartPointer.h>

#include "itkQtAdaptor.h"

namespace udg {


/*
    Classe auxiliar per monitorejar el progr�s de la lectura del fitxer
*/
class ProgressCommand : public itk::Command 
{
public:
    typedef  ProgressCommand   Self;
    typedef  itk::Command             Superclass;
    typedef  itk::SmartPointer<Self>  Pointer;
    itkNewMacro( Self );
protected:
    ProgressCommand() {};
public:

    typedef Volume::ItkImageType ImageType;
    typedef itk::ImageFileReader< ImageType >  ReaderType;
    typedef const ReaderType *ReaderTypePointer;
    
    void Execute(itk::Object *caller, const itk::EventObject & event)
    {
        Execute( (const itk::Object *)caller, event);
    }
    
    void Execute(const itk::Object * object, const itk::EventObject & event)
    {
        ReaderTypePointer m_reader = dynamic_cast< ReaderTypePointer >( object );
        if( typeid( event ) == typeid( itk::ProgressEvent ) )
        {
            
            std::cout << "Progressant..." << m_reader->GetProgress() << std::endl;
        }
        else
        {
            std::cout << "No s'ha invocat ProgressEvent" << std::endl;
        }
        
    
    }
};

/*
    Classe auxiliar per monitorejar el progr�s de la lectura d'una s�rie de fitxers
*/
class SeriesProgressCommand : public itk::Command 
{
public:
    typedef  SeriesProgressCommand   Self;
    typedef  itk::Command             Superclass;
    typedef  itk::SmartPointer<Self>  Pointer;
    itkNewMacro( Self );
protected:
    SeriesProgressCommand() {};
public:
  
    typedef Volume::ItkImageType ImageType;
    typedef itk::ImageSeriesReader< ImageType >  SeriesReaderType;
    typedef const SeriesReaderType *SeriesReaderTypePointer;
    
    void Execute(itk::Object *caller, const itk::EventObject & event)
    {
        Execute( (const itk::Object *)caller, event);
    }
    
    void Execute(const itk::Object * object, const itk::EventObject & event)
    {
        SeriesReaderTypePointer m_seriesReader = dynamic_cast< SeriesReaderTypePointer >( object );
        if( typeid( event ) == typeid( itk::ProgressEvent ) )
        {
            
            std::cout << "Progressant..." << m_seriesReader->GetProgress() << std::endl;
        }
        else
        {
            std::cout << "No s'ha invocat ProgressEvent" << std::endl;
        }
    }
};

/**
    Aquesta classe s'ocupa de les tasques d'obrir tot tipus de fitxer

@author Grup de Gr�fics de Girona ( GGG )
*/


class Input : public QObject {
Q_OBJECT
public:    
    
    Input();
    ~Input();
    
    /// Carrega un volum a partir del nom de fitxer que se li passi
    bool openFile(const char *fileName);
    /// Lector de s�ries dicom donat un directori que les cont�
    bool readSeries( const char *dirPath );
    
    /// Retorna un Volum
    Volume* getData() const { return m_volumeData; }; 

    // Aix� fa petar aplicaci�
    //itk::QtSignalAdaptor *m_progressSignalAdaptor;
public slots:
    void slotProgress()
    {
        emit progress( (int)( m_seriesReader->GetProgress() * 100 ) );
    }
signals:
    /// Indica el progr�s en % de la lectura del fitxer
    void progress( int );
private:
    
    typedef Volume::ItkImageType ImageType;
    typedef itk::ImageFileReader< ImageType >  ReaderType;
    typedef ReaderType::Pointer    ReaderTypePointer;

    typedef itk::ImageSeriesReader< ImageType >     SeriesReaderType;
    typedef itk::GDCMImageIO                        ImageIOType;
    typedef itk::GDCMSeriesFileNames                NamesGeneratorType;    

    /// El lector de s�ries dicom
    SeriesReaderType::Pointer m_seriesReader;
    /// El lector est�ndar de fitxers singulars, normalment servir� per llegir *.mhd's
    ReaderTypePointer    m_reader;
    /// Les dades llegides en format de volum
    Volume* m_volumeData;    
    /// el lector de DICOM
    ImageIOType::Pointer m_gdcmIO;
    /// el generador dels noms dels fitxers DICOM d'un directori
    NamesGeneratorType::Pointer m_namesGenerator;

    /// m�tode temporal per printar tags dicom
    void printTag( std::string tag , std::string name );

    /// Demana per un tag DICOM que ser� retornat en format string. Retorna true si es troba el tag al header DICOM
    bool queryTagAsString( std::string tag , std::string &result );
    
    /// Es dedica a proporcionar al volum la informaci� que ens d�na el DICOM
    void setVolumeInformation();

    /// a partir dels direction cosines d'un eix ens d�na l'orientaci� referent al pacient en string
    char *getOrientation( double vector[3] );
};





};  

#endif
