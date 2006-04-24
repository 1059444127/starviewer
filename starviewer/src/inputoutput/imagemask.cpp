#include "imagemask.h"
#include "status.h"



namespace udg{

/** Constructor 
 */
 
ImageMask::ImageMask()
{
    m_imageMask = new DcmDataset;
    retrieveLevel();
    
    //afegim els camps obligatoris
    setStudyUID( "" );
    setSeriesUID( "" );
    
}

/** This action especified that the query search, will use the retrieve level I. For any doubts about this retrieve level and the query/retrieve fields,
 consult DICOMS's documentation in Chapter 4, C.6.2.1
*/
//Per cada cerca a la m�scara s'ha d'espeficiar el nivell al que anirem a buscar les dades, en aquest cas s'especifica image
void ImageMask:: retrieveLevel()
{

    char val[15];
   
    DcmElement *elem = newDicomElement(DCM_QueryRetrieveLevel);

    
    strcpy(val,"IMAGE");
    elem->putString(val);

    m_imageMask->insert(elem, OFTrue);
    if (m_imageMask->error() != EC_Normal) {
        printf("cannot insert tag: ");
    }

}

/** Set SeriesUID.
  *              @param Series UID
  *              @return The status of the action
  */
Status ImageMask:: setSeriesUID( std::string seriesUID )
{
    Status state;

    DcmElement *elem = newDicomElement(DCM_SeriesInstanceUID);

    elem->putString( seriesUID.c_str() );
    if (elem->error() != EC_Normal)
    {
        return state.setStatus(error_MaskSeriesUID);
    }
    

    //insert the tag SERIES UID in the search mask    
    m_imageMask->insert(elem, OFTrue);
    if (m_imageMask->error() != EC_Normal) {
        return state.setStatus(error_MaskSeriesUID);
    }

    return state.setStatus(correct);
}


/** set the StudyId of the images
  *              @param   Study instance UID the study to search. If this parameter is null it's supose that any mask is applied at this field
  *              @return The state of the action
  */
Status ImageMask:: setStudyUID( std::string studyUID )
{
    Status state;

    DcmElement *elem = newDicomElement(DCM_StudyInstanceUID);

    elem->putString( studyUID.c_str() );
    if (elem->error() != EC_Normal)
    {
        return state.setStatus(error_MaskStudyUID);
    }
    

    //insert the tag STUDY UID in the search mask    
    m_imageMask->insert(elem, OFTrue);
    if (m_imageMask->error() != EC_Normal) {
        return state.setStatus(error_MaskStudyUID);
    }

    return state.setStatus(correct);
}

/** Set ImageNumber.
  *              @param image Number
  *              @return The status of the action
  */
Status ImageMask:: setImageNumber( std::string imgNum )
{
    Status state;

    DcmElement *elem = newDicomElement(DCM_InstanceNumber);

    elem->putString( imgNum.c_str() );
    if (elem->error() != EC_Normal)
    {
        return state.setStatus(error_MaskInstanceNumber);
    }

    //insert the tag SERIES UID in the search mask    
    m_imageMask->insert(elem, OFTrue);
    if (m_imageMask->error() != EC_Normal) {
        return state.setStatus(error_MaskInstanceNumber);
    }
    return state.setStatus(correct);
}

std::string ImageMask::getStudyUID()
{
    const char * UID=NULL;
    std::string studyUID;
    
    DcmTagKey studyUIDTagKey (DCM_StudyInstanceUID);
    OFCondition ec;
    ec = m_imageMask->findAndGetString( studyUIDTagKey, UID, OFFalse );;
    
    if (UID != NULL) studyUID.insert(0,UID);
        
    return studyUID;
}

std::string ImageMask::getSeriesUID()
{
    const char * UID = NULL;
    std::string seriesUID;
    
    DcmTagKey seriesUIDTagKey (DCM_SeriesInstanceUID);
    OFCondition ec;
    ec = m_imageMask->findAndGetString( seriesUIDTagKey, UID, OFFalse );;
    
    if (UID != NULL) seriesUID.insert(0,UID);
        
    return seriesUID;
}

std::string ImageMask::getImageNumber()
{
    const char * number = NULL;
    std::string imageNumber;
    
    DcmTagKey instanceNumberTagKey (DCM_InstanceNumber);
    OFCondition ec;
    ec = m_imageMask->findAndGetString( instanceNumberTagKey, number, OFFalse );;
    
    if (number != NULL) imageNumber.insert(0, number);
        
    return imageNumber;
}

/**  Return the generated image mask
              @return returns the image mask
*/
DcmDataset* ImageMask::getImageMask()
{
    return m_imageMask;
}

};

