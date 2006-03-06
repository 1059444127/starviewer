/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGIMAGECOMPLEXITY3DREGION_H
#define UDGIMAGECOMPLEXITY3DREGION_H

// ITK

#include <itkImageRegion.h>

#include "imagecomplexity3dhistogram.h"
//#include "extendeditkhistogram.h"
//#include "ICHistogram.h"
// STL
#include <list>

// CPP
#include <iostream> // cerr

namespace udg {

/**
Regio dels volums per a l'algorisme ImageComplexity3D.
La regi� guarda les coordenades x,y,z respecte l'origen 0,0,0 de la imatge i l'amplada de cada eix
Tamb� es guarda el respectiu histograma de la regi�, per� no pas les dades de la regi� que representa

@author Grup de Gr�fics de Girona  ( GGG )
*/
template < typename TInputImage >
class ImageComplexity3DRegion
{
public:

    typedef  TInputImage InputImageType;
    typedef typename InputImageType::Pointer    InputImagePointerType;
    typedef typename InputImageType::PixelType InputImagePixelType;
    
    typedef std::list< ImageComplexity3DRegion > ImageRegionSetType;

    // Aquests enum estan copiats tal qual, + endavant veurem si cal canviar-los o incl�s is s�n necessaris i en podem prescindidr completament
    typedef enum {NO_COMPUTED, CONSTANT, EQUAL, PROPORTIONAL} COPY_DATA;
    typedef enum { Any = 0,   Coordenates = 1,  Level = 2,       Volume = 4,
              Shape = 8, Orientation = 16, FirstData = 32, All = 63} SortType;


    typedef itk::ImageRegion<3> ItkImageRegionType;
    typedef ItkImageRegionType::IndexType IndexType;
    typedef ItkImageRegionType::SizeType SizeType;

//    typedef ExtendedItkHistogram< InputImageType > HistogramType;
//    typedef ExtendedItkHistogram< double > HistogramType;
    
    ImageComplexity3DRegion();
    ImageComplexity3DRegion( const ImageComplexity3DRegion& region );

    ~ImageComplexity3DRegion();

    /// Ens d�na la regi� del tipus itk
    ItkImageRegionType getItkRegion() const
    {
        return m_region;
    }
    /// Indiquem el valor de la coordenada d'origen X
    inline void setX( unsigned int value )
    {
        m_region.SetIndex( 0 , value );
    }
    /// Indiquem el valor de la coordenada d'origen Y
    inline void setY( unsigned int value )
    {
        m_region.SetIndex( 1 , value );
    }
    /// Indiquem el valor de la coordenada d'origen Z
    inline void setZ( unsigned int value )
    {
        m_region.SetIndex( 2 , value );
    }
    /// Indiquem l'or�gen de coordenades
    inline void setOrigin( unsigned int x, unsigned int y, unsigned int z )
    {
        m_region.SetIndex( 0 , x );
        m_region.SetIndex( 1 , y );
        m_region.SetIndex( 2 , z );
    }
    /// Indiquem l'or�gen de coordenades amb un objecte �ndex
    inline void setOrigin( const IndexType& index )
    {
        m_region.SetIndex( index);
    }

    /// Indiquem l'amplada ( X )
    inline void setWidth( unsigned int value )
    {
        m_region.SetSize( 0 , value );
    }
    /// Indiquem l'al�ada ( Y )
    inline void setHeight( unsigned int value )
    {
        m_region.SetSize( 1 , value );
    }
    /// Indiquem la profunditat ( Z )
    inline void setDepth( unsigned int value )
    {
        m_region.SetSize( 2 , value );
    }
    /// Indiquem amplada, al�ada i profunditat
    inline void setSize( unsigned int width, unsigned int height, unsigned int depth )
    {
        m_region.SetSize( 0 , width );
        m_region.SetSize( 1 , height );
        m_region.SetSize( 2 , depth );
    }
    /// Indiquem mides amb l'objecte Size
    inline void setSize( const SizeType& size )
    {
        m_region.SetSize( size );
    }
    /// Retorna l'�ndex de coordenades de la regi�
    inline const IndexType & getOrigin()
    {
        return m_region.GetIndex();
    }
    /// Retorna origen X
    inline unsigned int getX() const
    {
        return m_region.GetIndex( 0 );
    }
    /// Retorna origen Y
    inline unsigned int getY() const
    {
        return m_region.GetIndex( 1 );
    }
    /// Retorna origen Z
    inline unsigned int getZ() const
    {
        return m_region.GetIndex( 2 );
    }
    /// Retorna la mida dels eixos
    inline const SizeType & getSize()
    {
        return m_region.GetSize();
    }
    /// Obtenim l'amplada de la regi�
    inline unsigned int getWidth() const
    {
        return m_region.GetSize( 0 );
    }
    /// Obtenim l'al�ada de la regi�
    inline unsigned int getHeight() const
    {
        return m_region.GetSize( 1 );
    }
    /// Obtenim la profunditat de la regi�
    inline unsigned int getDepth() const
    {
        return m_region.GetSize( 2 );
    }


    // ----------------------------------------------------------------------------
    // CAUTION!!!!! THIS IS COPY PASTE
    // ----------------------------------------------------------------------------

    // Sort ***********************************************************************

    /// Retorna quin criteri d'ordenaci� seguim
    inline static char getSort()
    {
        return m_sort;
    }
    /// Indiquem el tipus d'ordenaci�
    inline static void setSort( SortType sort )
    {
        if (sort == Any || sort > All) std::cerr << "ImageComplexity3DRegion::setSort :: Out of range" << std::endl;

        m_sort = sort;
    }

    // Information ****************************************************************

    /// Ens retorna el volum de la regi�
    inline unsigned volume()
    {
        return m_region.GetSize( 0 ) * m_region.GetSize( 1 ) * m_region.GetSize( 2 );
    }
    /// L'usuari decideix si dues regions amb = area hande considerar-se iguals o depenent de
    /// la forma
    inline double shape(bool orientation = false)
    {
        double s =  m_region.GetSize( 1 )  /  m_region.GetSize( 0 ) / m_region.GetSize( 2 );

        if ( !orientation && s < 1) s = 1 / s;

        return s;
    }

    /// Ens diu si la regi� �s buida
    inline bool isEmpty()
    {
        return volume() == 0;
    }
    /// Ens retorna l'entropia de la regi� calculada a partir del seu histograma
    double getEntropy();
    
    /// Retorna la entropia de la regi� que li hem imposat, aquesta no est� calculada a partir de l'histograma
    double getProxyEntropy() const
    {
        return m_proxyEntropy;
    }
    /// Assignem l'entropia no calculada a partir de l'histograma
    void setProxyEntropy( double value )
    {
        m_proxyEntropy = value;
    }
    /// Aquesta �s l'�rea imposada, tamb� anomenat freq��ncia
    unsigned getArea() const
    {
        return m_area;
    }
    /// Aquesta �s l'area imposada, tamb� anomenat freq��ncia
    void setArea( unsigned value )
    {
        m_area = value;
    }
    
    /**
        Controla el nivell d'uniformitat que es vol entre 0..1 ( Uniformitat total = Entropia m�xima ).
        L'algorisme pot acabar encara que no s'assoleixi l'entropia m�xima ( = 1 )
    */
    double coefficientUH();

    /// Assignem la proporci� local
    inline void setLocalProportion( double prop )
    {        m_localProportion = prop;    }
    /// Assignem la proporci� global : en el proces de merge es el MaxRange
    inline void setGlobalProportion( double prop )
    {        m_globalProportion = prop;    }
    /// Retorna la proporci� local
    inline double getLocalProportion( ) const
    {        return m_localProportion;    }
    /// Assignem la proporci� global
    inline double getGlobalProportion( ) const
    {        return m_globalProportion;    }
    
    inline void setMinimumRange( double value )
    {
        m_minimumRange = value;
    }
    inline double getMinimumRange( ) const
    {
        return m_minimumRange;
    }
    /// Retorna el level
    inline unsigned getLevel()
    {
        return m_level;
    }
    /// retorna l'histograma de la regi�
    
    inline ImageComplexity3DHistogram& getHistogram()
    {
        return m_histogram;
    }
    

    /// Copiem una ImageComplexity3DRegion
    inline ImageComplexity3DRegion& setImageComplexity3DRegion(const ImageComplexity3DRegion& region)
    {
        return *this = region;
    }
    /// Indiquem la regi� ( indexs i amplades )
    inline void setRegion( const ItkImageRegionType & imageregion )
    {
        m_region = imageregion;
    }
    
    /// Indiquem l'histograma
    inline void setHistogram( const /*HISTOGRAM*/ ImageComplexity3DHistogram &histogram )
    {
        m_histogram = histogram;
    }
    /*
    inline void setHistogram( ImageComplexity3DHistogram* histogram )
    {
        m_histogram.copy( *histogram );
//        m_histogram.setHistogram( histogram );
    }
    */
    /// Indiquem l'histograma
/*    inline void setHistogram( HistogramType* histogram )
    {
//        m_histogram = histogram;

        m_histogram->copy( *histogram );

    }
*/
    /// Indiquem el level
    inline void setLevel(const unsigned level)
    {
        m_level = level;
    }

    // Operators ******************************************************************

    /// Operador de c�pia
    ImageComplexity3DRegion& operator= (const ImageComplexity3DRegion& region);
    /// Operador de Comparaci�
    bool operator< (ImageComplexity3DRegion& region);

    /// Realitza les particions d'una regi�
    //double bipartition( InputImageType* imageData, ImageRegionSetType& set);
    double bipartition( TInputImage* imageData, ImageRegionSetType& set);

    /// Printa informaci� sobre l'objecte
    void print()
    {
        std::cout << "Origin: " << getOrigin() << std::endl;
        std::cout << "Size: " << getSize() << std::endl;
        std::cout << "Level:" << m_level << std::endl;
        std::cout << "Local Proportion: " << m_localProportion << std::endl;
        std::cout << "Global Proportion: " << m_globalProportion << std::endl;
        std::cout << "Entropy:" << getEntropy() << std::endl;
        std::cout << "Area:" << getArea() << std::endl;
        std::cout << std::flush;
    }
private:
    /// Defineix un �ndex ( x, y , z ) i la mida al llarg d'aquests
    ItkImageRegionType m_region;
    /// Histograma de la regi�
    ImageComplexity3DHistogram m_histogram;
    //HISTOGRAM m_histogram;
//     typename HistogramType::Pointer m_histogram;
    
    /// Nivell de recursi�
    unsigned m_level;
    /// Tipus de criteri d'ordenaci�, com� per a totes les regions
    static SortType m_sort;
    /// Proporci� local i global, i minRange
    double m_localProportion, m_globalProportion, m_minimumRange;
    /// Diu quants bins de l'histograma estan ocupats, �s a dir, quants hi ha que no s�n 0
    unsigned fullBins();
    /// Entropia imposada; no es calcula a partir de l'histograma
    double m_proxyEntropy;
    /// Area de la regi�
    unsigned m_area;
};

};  //  end  namespace udg

#include "imagecomplexity3dregion.cpp"

#endif

    // Cut ************************************************************************

    /*
    ImageComplexity3DRegion& cutX(const unsigned x, ImageRegionSetType& set, const COPY_DATA type = PROPORTIONAL);

    ImageComplexity3DRegion& cutY(const unsigned x, ImageRegionSetType& set, const COPY_DATA type = PROPORTIONAL);

    ImageComplexity3DRegion& cutXY(const unsigned x, const unsigned y,               ImageRegionSetType& set, const COPY_DATA type = PROPORTIONAL);

    ImageComplexity3DRegion& quadtree(ImageRegionSetType& set, const bool hard = false,                const COPY_DATA type = PROPORTIONAL);

    double quadtree(const double* data, const unsigned width, ImageRegionSetType& set, const bool hard = true);
    */

