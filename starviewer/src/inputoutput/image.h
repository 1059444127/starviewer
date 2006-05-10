/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGIMAGE_H
#define UDGIMAGE_H

#include <string>

namespace udg {

/** Classe image encarregada de guardar la informaci� d'una imatge
@author marc
*/
class Image{
public:
    
    /// Constructor de la classe
    Image();
       
    /// destructor de la classe
    ~Image();

    /** Inserta el UID de la Imatge 
     * @param  UID de la imatge
     */  
    void setSoPUID (std::string );
    
    /** Inserta el UID de la s�rie al qual pertany la imatge
     * @param  UID de la s�rie a la que pertany la imatge
     */
    void setSeriesUID (std::string );
    
    /** Inserta el path de la imatge a l'ordinador local
     * @param path de la imatge
     */
    void setImagePath (std::string );
    
    /** Inserta el UID de la l'estudi al qual pertany la imatge
     * @param UID  UID de la l'estudi a la que pertany la imatge
     */
    void setStudyUID (std::string );
    
    /** Inserta el nom de la imatge
     * @param name [in] nom de la imatge
     */
    void setImageName (std::string );
    
    /** Inserta el n�mero d'imatge
     * @param N�mero que ocupa la imatge dins la s�rie
     */
    void setImageNumber (int);
    
    /** Inserta el n�mero de bytes que ocupa la imatge
     * @param  bytes de la imatge
     */
    void setImageSize (int);
   
    /** Retorna el UID de la Imatge 
     * @return  UID de la imatge
     */
    std::string getSoPUID();
    
    /** Retorna el UID de la s�rie al qual pertany la imatge
     * @return UID de la s�rie a la que pertany la imatge
     */
    std::string getSeriesUID();
    
    /** Retorna el path de la imatge a l'ordinador local
     * @return path de la imatge
     */ 
    std::string getImagePath ();
    
    /** Retorna el UID de l'estudi al qual pertany la imatge
     * @return UID de la l'estudi a la que pertany la imatge
     */
    std::string getStudyUID();
    
    /** Retorna el nom de la imatge
     * @return el nom de la imatge
     */
    std::string getImageName();
    
    /** retorna el n�mero d'imatge
     * @return Retorna el n�mero que ocupa la imatge dins la s�rie
     */
    int getImageNumber();
    
    /** retorna el n�mero de bytes que ocupa la imatge
     * @return retorna el n�mero de bytes de la imatge
     */
    int getImageSize();
       
private:
    //variables que guarden la informaci� de la imatge
    std::string m_SoPUID;
    std::string m_seriesUID;
    std::string m_imagePath;
    std::string m_studyUID;
    std::string m_imageName;
    int m_imageNumber;
    int m_imageSize;

};

};

#endif
