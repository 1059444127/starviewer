/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include <string>
#include "readdicomdir.h"

#include "ofstring.h"
#include "dcmtk/config/osconfig.h" /* make sure OS specific configuration is included first */
#include "dcdeftag.h" //provide the information for the tags
#include "status.h"
#include "study.h"
#include "studylist.h"
#include "series.h"
#include "serieslist.h"
#include "studymask.h"

namespace udg {

ReadDicomdir::ReadDicomdir()
{
    dicomdir = NULL;    

}

Status ReadDicomdir::open( std::string dicomdirPath )
{
    Status state;
    
    //no existeix cap comanda per tancar un dicomdir, quan en volem obrir un de nou, l'�nica manera d'obrir un nou dicomdir, �s a trav�s del construtor de DcmDicomDir, passant el path per par�metre, per aix� si ja existia un Dicomdir ober, fem un delete, per tancar-lo
    if ( dicomdir != NULL) delete dicomdir;

    dicomdir = new DcmDicomDir( dicomdirPath.c_str() );

    return state.setStatus( dicomdir->error() );
}

//El dicomdir segueix una estructura d'abre on tenim n pacients, que tenen n estudis, que cont� n series, i que cont� n imatges, per llegir la informaci� hem d'accedir a trav�s d'aquesta estructura d'arbre, primer llegim el primer pacient, amb el primer pacient, podem accedir el segon nivell de l'arbre, els estudis del pacient, i anar fent aix� fins arribar al nivell de baix de tot, les imatges,
Status ReadDicomdir::readStudies( StudyList &studyList , StudyMask studyMask )
{
    Status state;
    
    DcmDirectoryRecord *root = &( dicomdir->getRootRecord() );//accedim a l'estructura d'arbres del dicomdir
    DcmDirectoryRecord *patientRecord = root->getSub( 0 );//accedim al primer pacient
    OFString text;
    Study study;
    
    if ( dicomdir == NULL ) return state.setStatus( "Error: Not open dicomfile" , false , 1302 ); //FER RETORNAR STATUS AMB ERROR

    //En aquest primer while accedim al patient Record a nivell de dades de pacient
    while ( patientRecord != NULL )
    {
        //Nom pacient
        patientRecord->findAndGetOFStringArray( DCM_PatientsName , text );
        study.setPatientName( text.c_str() );
        //Id pacient
        patientRecord->findAndGetOFStringArray( DCM_PatientID , text );
        study.setPatientId( text.c_str() );
        
        DcmDirectoryRecord *studyRecord = patientRecord->getSub( 0 );//indiquem que volem el primer estudi del pacient

        //en aquest while accedim a les dades de l'estudi
        while ( studyRecord != NULL )
        {
            
            //Id estudi
            studyRecord->findAndGetOFStringArray( DCM_StudyID , text );
            study.setStudyId( text.c_str() );
          
            //Hora estudi
            studyRecord->findAndGetOFStringArray( DCM_StudyTime , text );
            study.setStudyTime( text.c_str() );

            //Data estudi
            studyRecord->findAndGetOFStringArray( DCM_StudyDate , text );
            study.setStudyDate( text.c_str() );

            //Descripci� estudi
            studyRecord->findAndGetOFStringArray( DCM_StudyDescription , text );
            study.setStudyDescription( text.c_str() );

            //accession number
            studyRecord->findAndGetOFStringArray( DCM_AccessionNumber , text );
            study.setAccessionNumber( text.c_str() );

            //obtenim el UID de l'estudi
            studyRecord->findAndGetOFStringArray( DCM_StudyInstanceUID , text );
            study.setStudyUID( text.c_str() );

            if ( matchStudyMask( study , studyMask ) ) //comprovem si l'estudi compleix la m�scara de cerca que ens han passat
            {
                studyList.insert( study );   
            }
            
            studyRecord = patientRecord->nextSub( studyRecord ); //accedim al seg�ent estudi del pacient
        }

        patientRecord = root->nextSub( patientRecord ); //accedim al seg�ent pacient del dicomdir 
    }
    
    return state.setStatus( dicomdir->error() );
} 

//Per trobar les s�ries d'une estudi haurem de recorre tots els estudis dels pacients, que hi hagi en el dicomdir, fins que obtinguem l'estudi amb el UID sol�licitat una vegada trobat, podrem accedir a la seva informacio de la s�rie
Status ReadDicomdir::readSeries( std::string studyUID , SeriesList &seriesList )
{
    Status state;
    DcmDirectoryRecord *root = &( dicomdir->getRootRecord() );//accedim a l'estructura d'abres del dicomdir
    DcmDirectoryRecord *patientRecord = root->getSub( 0 );//accedim al primer pacient
    DcmDirectoryRecord *studyRecord;
    OFString text;
    Series series;
    std::string studyUIDRecord;
    bool trobat = false;
    
    //Accedim a nivell de pacient
    while ( patientRecord != NULL && !trobat )
    {
        studyRecord = patientRecord->getSub( 0 );//indiquem que volem el primer estudi del pacient

        while (  studyRecord != NULL && !trobat )
        {
            text.clear();
            studyUIDRecord.clear();
            studyRecord->findAndGetOFStringArray( DCM_StudyInstanceUID , text );//obtenim el UID de l'estudi al qual estem posicionats
            studyUIDRecord.insert( 0 , text.c_str() );
            if ( studyUIDRecord == studyUID ) //busquem l'estudi que continguin el mateix UID
            { 
                trobat = true;   
            }
            else studyRecord = patientRecord->nextSub( studyRecord );//si no trobem accedim al seguent estudi del pacient
        }

        if ( !trobat ) patientRecord = root->nextSub( patientRecord ); //accedim al seg�ent pacient
    }

    if ( trobat )//si hem trobat l'estudi amb el UID que cerc�vem
    {
        DcmDirectoryRecord *seriesRecord = studyRecord->getSub( 0 ); //seleccionem la serie de l'estudi que cont� el studyUID que cerc�vem
        while ( seriesRecord != NULL )
        {
            //N�mero de s�rie
            seriesRecord->findAndGetOFStringArray( DCM_SeriesNumber , text );
            series.setSeriesNumber( text.c_str() );

            //UID Serie
            seriesRecord->findAndGetOFStringArray( DCM_SeriesInstanceUID , text );
            series.setSeriesUID( text.c_str() );

            //Modalitat s�rie            
            seriesRecord->findAndGetOFStringArray( DCM_Modality , text );
            series.setSeriesModality( text.c_str() );            
            
            seriesList.insert( series );//inserim a la llista de s�rie
            
            seriesRecord = studyRecord->nextSub( seriesRecord ); //accedim al seg�ent estudi de la s�rie
        } 
    }   
    
    return state.setStatus( dicomdir->error() );
}

//Per fer el match seguirem els criteris del PACS
bool ReadDicomdir::matchStudyMask( Study study , StudyMask studyMask )
{
    if ( !matchStudyMaskStudyId( studyMask.getStudyId() , study.getStudyId() ) ) return false;

    if ( !matchStudyMaskPatientId( studyMask.getPatientId() , study.getPatientId() ) ) return false;

    if ( !matchStudyMaskDate( studyMask.getStudyDate() , study.getStudyDate() ) ) return false;

    if ( !matchStudyMaskPatientName( studyMask.getPatientName() , study.getPatientName() ) ) return false;
    
    return true;
}

bool ReadDicomdir::matchStudyMaskStudyId( std::string studyMaskStudyId , std:: string studyStudyId )
{
    if ( studyMaskStudyId.length() > 0 )
    { //si hi ha m�scara d'estudi Id
      //en el cas del StudiId seguim criteri del pacs, nom�s faran match els ID que concordin amb el de la m�scara, no podem fer wildcard
        if ( studyStudyId == studyMaskStudyId )
        {   
            return true;
        }
        else 
        {
            return false;
        }
    }
    
    return true;
}

bool ReadDicomdir::matchStudyMaskPatientId( std::string studyMaskPatientId , std:: string studyPatientId )
{
    if ( studyMaskPatientId.length() > 0 )
    { //si hi ha m�scara Patient Id
      //en el cas del PatienId seguim criteri del pacs, nom�s faran match els ID que concordin amb el de la m�scara, no podem fer wildcard
        studyMaskPatientId = upperString( studyMaskPatientId );
        
        if ( studyPatientId == studyMaskPatientId )
        {   
            return true;
        }
        else 
        {
            return false;
        }
    }
    
    return true;
}

bool ReadDicomdir::matchStudyMaskDate( std::string studyMaskDate , std::string studyDate )
{

    if ( studyMaskDate.length() > 0 ) 
    { //Si hi ha m�scara de data
      //la m�scara de la data per DICOM segueix els formats :
      // -  "YYYYMMDD-YYYYMMDD", per indicar un rang de dades
      // - "-YYYYMMDD" per buscar estudis amb la data m�s petita o igual
      // - "YYYYMMDD-" per buscar estudis amb la data m�s gran o igual
      // - "YYYYMMDD" per buscar estudis d'aquella data
      // Hurem de mirar quin d'aquest formats �s la nostre m�scara
    
        if (  studyMaskDate.length() == 8 ) // cas YYYYMMDDD
        {
            if ( studyMaskDate == studyDate )
            {
                return true;
            }   
            else return false;
        }
        else if (  studyMaskDate.length() == 9 ) 
        {
            if (  studyMaskDate.at( 0 ) == '-' ) // cas -YYYYMMDD
            {
                if ( studyMaskDate.substr( 1 , 8 ) >= studyDate )
                {
                    return true;
                }
                else return false;
            }
            else if ( studyMaskDate.at( 8 ) == '-' ) // cas YYYYMMDD-
            {
                if ( studyMaskDate.substr( 0 , 8 ) <= studyDate )
                {
                    return true;
                }
                else return false;
            }
        }
        else if ( studyMaskDate.length() == 17 ) // cas YYYYMMDD-YYYYMMDD
        {
            if ( studyMaskDate.substr( 0 , 8 ) <= studyDate && 
                 studyMaskDate.substr( 9 , 8 ) >= studyDate )
            {
                return true;
            }
            else return false;
        }
        return false;
    }

    return true;
}

bool ReadDicomdir::matchStudyMaskPatientName( std::string studyMaskPatientName , std::string studyPatientName )
{
    std:: string lastPatientName , firstPatientName;

    if ( studyMaskPatientName.length() > 0 )
    { //En Pacs la m�scara del nom t� el seg�ent format Cognoms*Nom*
      //Seguint els criteris del PACS la cerca es fa en wildcard, �s a dir no cal que els dos string sigui igual mentre que la m�scara del nom del pacient estigui continguda dins studyPatientName n'hi ha suficient
        studyMaskPatientName = upperString( studyMaskPatientName );
        lastPatientName = studyMaskPatientName.substr( 0 , studyMaskPatientName.find_first_of ( "*" ) );   

        if ( lastPatientName.length() > 0)
        {
            if ( studyPatientName.find ( lastPatientName ) == std::string::npos ) return false; //comprovem si el nom del pacient conte el cognom
        }
    
        if ( studyMaskPatientName.find_first_of( "*" ) < studyMaskPatientName.length() ) //si la m�scara tamb� contem el nom del pacient
        {
            firstPatientName = studyMaskPatientName.substr( studyMaskPatientName.find_first_of ( "*" ) + 1 , studyMaskPatientName.length() - studyMaskPatientName.find_first_of ( "*" ) -2 );  //ignorem el * de final del Nom
            
            if ( studyPatientName.find ( firstPatientName ) == std::string::npos ) return false;
        }
        
        return true;
    }

    return true;

}

std::string ReadDicomdir::upperString( std:: string original )
{
    for ( unsigned int i = 0; i < original.length(); i++ )
    {
        original[i] = toupper( original[i] );
    }

    return original;
}

ReadDicomdir::~ReadDicomdir()
{
}

}
