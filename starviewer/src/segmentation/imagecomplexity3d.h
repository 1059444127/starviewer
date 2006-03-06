/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGIMAGECOMPLEXITY3D_H
#define UDGIMAGECOMPLEXITY3D_H

// STL
#include <list>
#include <queue> // priority_queue

// ITK

#include <itkImage.h>
// Extracci� de Regions: "7.4 Extracting Regions" 199-201 The ITK Software Guide
//#include <itkRegionOfInterestImageFilter.h>

// per generar l'histograma desde l'imatge
//#include <itkScalarImageToGenerator.h>

// Per iterar en la imatge de sortida
#include <itkImageRegionIterator.h>
// Per iterar en la imatge d'entrada
#include <itkImageRegionConstIterator.h>
// Perqu� la iteraci� del contour sigui un p�l m�s eficient
#include <itkImageRegionIteratorWithIndex.h>
// per el m�nim, m�xim, mitjana i stdDev de la imatge
#include <itkStatisticsImageFilter.h>

#include "imagecomplexity3dnode.h"
#include "imagecomplexity3dregion.h"

class ImageComplexity3DHistogram;
namespace udg {

/**
Image Complexity 3D. M�tode de segmentaci� construit en base al canal d'informaci� existent entre les caracter�stiques m�s b�siques dels voxels: el gradient/valor de propietat/intensitat i la seva posici� en l'espai. A partir d'aquest canal hi ha dos fases basades en la maximitzaci� de la Informaci� M�tua.

En la primera fase es parteix la imatge en regions relativament homog�nies fent servir una partici� d'espai bin�ria ( BSP ).

En la segona fase es segmenta la imatge a partir del clustering dels bins de l'histograma.

Les regions resultants de la primera fase es poden unir fent servir l'histograma obtingut en la segona fase.

@author Grup de Gr�fics de Girona  ( GGG )
*/

template <typename TInputImage>
class ImageComplexity3D
{
public:

    enum CutType{ Bipartition, Octree };

    enum StopCriteriaType{ NumberOfRegions, ErrorProbability, MutualInformationRatio, MutualInformation, NormalizedMutualInformation , MutualInformationPercent , NumberOfColours };
    
    typedef TInputImage InputImageType;
    typedef typename InputImageType::Pointer InputImageTypePointer;
    typedef itk::ImageRegionConstIterator< InputImageType > InputImageIteratorType;

    typedef InputImageType OutputImageType;
    typedef itk::ImageRegionIterator< OutputImageType > OutputImageIteratorType;
    typedef itk::ImageRegionIteratorWithIndex< OutputImageType > OutputImageIteratorWithIndexType;
    
    typedef typename OutputImageType::Pointer OutputImagePointerType;
    typedef typename OutputImageType::IndexType OutputImageIndexType;
    typedef typename OutputImageType::SizeType OutputImageSizeType;

    typedef typename InputImageType::PixelType PixelType;
    typedef typename InputImageType::IndexType IndexType;

    typedef ImageComplexity3DRegion< InputImageType > RegionType;
    typedef udg::ImageComplexity3DNode< RegionType > NodeType;
    typedef std::priority_queue< NodeType > NodePriorityQueue;
    typedef std::list< RegionType > SplitRegionsList;
    typedef typename SplitRegionsList::iterator SplitRegionListIteratorType;

    typedef typename RegionType::ItkImageRegionType ItkRegionType;
    
    typedef std::list< RegionType > ImageRegionSetType;

    typedef struct{
        RegionType region, fusion;
        double mutualInformation;
    }MergeNode;
    
    typedef std::list< MergeNode > MergeNodesListType;
    typedef typename MergeNodesListType::iterator MergeNodesListIteratorType;
    
    // Ho fem aix� ja que podria ser que el tipus de regi� canvies amb el merge
    typedef std::list< RegionType > MergeRegionsList;
    typedef struct{
        double entropy;
        unsigned area;
    }HN; // No m'agrada el nom, per� de mentres queda aix�

    
    ImageComplexity3D();

    ~ImageComplexity3D();

    /// Aplica el m�tode. Retorna true si tot va b�, false altrament
    bool applyMethod();
    /// Obtenim la imatge resultant. El par�metre type ens indica si volem la imatge amb els clusters (1) o nom�s amb els contorns de les regions particionades (2)
    /// Nou par�metre, 3: clustering a partir d'histograma sense fer mitjana
    OutputImageType* getOutputImage( int type = 1 );
    

//    Introducci� de par�metres
    /// Li assignem la imatge d'entrada
//    void setInputImage( InputImageType* inputImage );
    void setInputImage( TInputImage* inputImage );
    void setSplitStopCriteria( char test ){ m_splitStopCriteria = test; } // 0,1,2,3,4
    void setMergeStopCriteria( char test ){ m_mergeStopCriteria = test; } // 0,1,2,3,4,5
    void setHistogramBins( unsigned int bins ){ m_histogramBins = bins; }
    void setUniformEntropy( double UH ){ m_uniformEntropy = UH; } // hauria de ser entre 0.0 - 1.0
    void setCut( CutType cut ){ m_cut = cut; }
    void setSplitCoefficient( double coefficient ){ m_splitCoefficient = coefficient; }
    void setMergeCoefficient( double coefficient ){ m_mergeCoefficient = coefficient; }
    /// printa l'estat de les variables
    void print()
    {
        std::cout << "Nombre de Regions: " << m_numberOfRegions << std::endl; 
        std::cout << "Entropia de la imatge: " << m_imageEntropy << std::endl; 
        std::cout << "MI Imatge-Regions: " << m_mutualInformationImageRegions  << std::endl; 
        std::cout << "Entropia condicional Imatge-Regions: " << m_conditionalEntropyImageRegions << std::endl; 
        std::cout << "Entropia de les regions: " << m_regionsEntropy << std::endl;     
        std::cout << "Entropia Conjunta: " << m_jointEntropyImageRegions << std::endl;     
        std::cout << "MI normalitzada: " << m_normalizedMutualInformation << std::endl; 
        std::cout << "Mida dels candidats(split): " << m_splitCandidates.size() << std::endl;
        std::cout << "Mida de les fulles(split): " << m_splitLeaves.size() << std::endl;
        
        std::cout << "Mida dels candidats(merge): " << m_mergeCandidates.size() << std::endl;
        std::cout << "Mida de les fulles(merge): " << m_mergeLeaves.size() << std::endl;
    }


private:

    typedef itk::StatisticsImageFilter< InputImageType > StatisticsImageFilterType;
    /// Pels c�lculs de valor m�xim de la imatge
    typename StatisticsImageFilterType::Pointer m_imageStatisticsFilter;
    
// estructura espec�fica pel fill del merge
    typedef struct{
        double minimum,maximum,average;
    }fillStruct;
    
    /// Mida total del volum, == # de voxels
    unsigned m_size;

    /// Verifica els par�metres d'entrada
    bool input();
    /// Inicialitza les dades per l'algorisme de particionament
    void initSplitData();
    /// Inicialitza les dades per l'algorisme de fusi� de bins
    void initMergeData();
    /// Aplica l'algorisme split & merge
    void comput();
    /// Crea les particions de la imatge
    void imagePartition();
    /// Crea el clustering a partir de l'histograma un cop obtingudes les region
    void histogramQuantization();
    /// Ens calcula l'entropia i l'�rea d'una regi� que vagi de from a to
    HN mergeEntropy(const unsigned from, const unsigned to);
    
    ///
    double mergeEntropyMinimum();
    
    /// Construeix la imatge final a partir de les regions
    void output();
    /// Actualitza la imatge de sortida. El par�metre ens indica si la imatge de sortida nom�s mostrar� les regions trobades o fusionar� els colors.  1=fusio, 2= contorn 3= clustering histograma
    void update(int type = 1);
    /// Calcula la mitjana d'una determinada regio de la imatge d'entrada
    PixelType average( ItkRegionType itkRegion );
    /// Omple una regi� de la imatge de sortida amb un determinat valor 
    void fill( ItkRegionType itkRegion, PixelType value );

    /// Aquest �s el fill del merge. Retorna el nombre de pixels pintats (area)
    unsigned fill( fillStruct* filling );
    //unsigned fill(const double minimum, const double maximum, const double data);
    
    /// Repinta la imatge en el cas del clustering de l'histograma, calcula els nous colors amb qu� repintar la imatge ( clustering histograma )
    void repaint();
    /*! 
    Dibuixa el contorn de la regi� demanada sobre una imatge 
    \TODO 1. li podr�em incloure opcions de fer nom�s el contorn , contorn+avg, contorn+imatge original , etc, 2. Treure els warnings que surten per comparar signed i unsigned
    */
    void contour( ItkRegionType itkRegion );


    /// Passa totes les regions candidates de "l'arbre" de nodes al conjunt final de regions que definiran la imatge particionada
    void splitCandidatesToLeaves();

    /// Indica si l'algorisme de particionament ha de continuar excutant-se o s'ha d'aturar
    bool splitTest();
    /// Indica si l'algorisme de fusi� de l'histograma ha de continuar excutant-se o s'ha d'aturar
    bool mergeTest();
    /// Agafa el millor node candidat a ser tallat i fa l'insert() de cada tall.
    void split();
    
    /** Inserta una regi� al conjunt final o la segueix tallant.
        Si la regi� arriba al coeficient, passa al conjunt final
        sin� es talla
        Si no s'ha tallar, passa al conjunt final
        sin� tenim posem el nou node (cont� les regions tallades) a la llista de candidats
    */
    bool insert( RegionType region );

    /// C�lcul de propoci� en el proc�s de particionament 
    double computSplitProportion();
    /// C�lcul de propoci� en el proc�s de fusi�
    double computMergeProportion();
    /// La imatge original que volem partir
    TInputImage*            m_inputImage;

    /// La imatge resultant
    OutputImagePointerType             m_outputImage;

//--  mesures de Teoria de la Informaci� ----------------------------------------------------

    unsigned m_numberOfRegions; // N , nombre de regions tallades
    /// H(C)  Entropia del canal d'entrada, �s a dir, la imatge sencera, per tant �s constant.[hC]
    double m_imageEntropy,
    /// I(C;R), informaci� m�tua d'un moment concret entre la imatge i les particions actuals.[miCR]
    m_mutualInformationImageRegions,
    /// H(C|R) Entropia Condicional d'un moment concret entre la imatge i les particions actuals.[hcCR]
    m_conditionalEntropyImageRegions,
    /// H(R) Entropia de les Regions en un moment concret. [hR]
    m_regionsEntropy,
    // H(C,R) = H(C) + H(R) - I(C;R) Entropia Conjunta, la f�rmula b�sica que ho relaciona tot. [hjCR]
    m_jointEntropyImageRegions,
    // I(C;R) / H(C,R) Informaci� M�tua Normalitzada. [pih]
    m_normalizedMutualInformation;
    
    // MERGE
    // noms temporals
    unsigned m_merge_n;
    double m_merge_hC, m_merge_miCR, m_merge_miCRMax, m_merge_hcCR, m_merge_hR, m_merge_hjCR, m_merge_pih;
    /*
    unsigned n;          // N 
             REAL hC,             // H(C)
	             miCR,           // I(C;R)
		        miCRMax,        // I(C;R) Maximum,  �s l'inicial abans de fer clusters
			   hcCR,		    // H(C|R)
			   hR,             // H(R)
			   hjCR,           // H(C,R) = H(C) + H(R) - I(C;R) 
		        pih;       // I(C;R) / H(C,R)		   
    */

//--  fi mesures de Teoria de la Informaci� ---------------------------------------------

    /// Conjunt de regions final, la uni� de les quals composen la imatge original
    SplitRegionsList m_splitLeaves;
    /// Cua amb prioritats. Cont� els nodes candidats a millor regi� de tall, ordenats per informaci� m�tua
    NodePriorityQueue m_splitCandidates;

    
    /// �s la variable clau per arribar a parar l'algorisme. L'algorisme para quan aquesta �s major o igual que 1 o no hi ha m�s candidats. Aquest valor es calcula a cada split de l'algorisme. Segons el criteri d'aturada ( m_splitStopCriteria ) el seu c�lcul es dur� a terme d'una manera determinada.
    double m_proportion;

    ///
    //No recordo, pero pr d'error NO. Deu contenir el valor minim de MI trobat fins a les hores, o be, el valor minim a assolir per acabar algun test amb un % de MI. Ser� 0 si el criteri d'aturada �s diferent d'1 ( Pr-Error ) sin� es calcula amb una formuleta
    double m_miMin;

    /// Ens serveix per saber si les dades de sortida s'han d'actualitzar o no.
    int m_lastUpdate;

    /// Histograma de la imatge original
    ImageComplexity3DHistogram* m_imageHistogram;
    
    // -----------------------------------------------------------------------------------------------
    // MERGE
    // -----------------------------------------------------------------------------------------------
    
    ///
    void insert( MergeNodesListIteratorType p);
    ///
    MergeNodesListIteratorType select();
    ///
    bool update( MergeNodesListIteratorType p );
    ///
    void mergeCandidatesToLeaves();
    
    /// Conjunt final de regions de l'histograma de clustering
    MergeRegionsList m_mergeLeaves;
    /// Matriu de m_histogramBins x m_numberOfRegions. Aglomera els histogrames de cada regi� trobada durant l'split per realitzar el clustering dels bins dels histogrames
    unsigned* m_clusteringMatrix;
    
    /// Llista de candidats de la fase de merge 
    MergeNodesListType m_mergeCandidates;
    
    ///
    unsigned mergeArea( const unsigned i , const unsigned from, const unsigned to);
    ///
    void fusion( MergeNodesListIteratorType from , MergeNodesListIteratorType to );

    // MISC
    /// Neteja totes les estructures de dades per poder comen�ar l'algorisme
    void clean();
// Par�metres d'entrada =======================================================================

    /// Nombre de bins que volem de l'histograma
    unsigned int m_histogramBins;
    /// M�nim i m�xim valor de l'histograma
    double m_histogramMin, m_histogramMax;
    // Els minims i m�xims de l'histograma no els entrem com a par�metre, els calculem directament a trav�s de la imatge (itkMinMaxCalculator)

    /// Entropia normalitzada que volem assolir de cada regi�
    double m_uniformEntropy;
    /// Ens indica el tipu de tall: Bipartition, Octree
    CutType m_cut;
    /// decideix quin o quins par�metres optimitzar ( criteris de parada ) per l'split: #Regions, Pr-Error, Pr-MI, MI, NMI.
    char m_splitStopCriteria;
    /// decideix quin o quins par�metres optimitzar ( criteris de parada ) pel merge: #Regions, Pr-Error, Pr-MI, MI, NMI.
    char m_mergeStopCriteria;
    /// Donat un criteri d'aturada concret, aquest es el seu tipic epsilon corresponen (threshold). Llavors segons el criteri d'aturada el coeficient tindr� un rang determinat
    double m_splitCoefficient;

    /// Donat un criteri d'aturada concret, aquest es el seu tipic epsilon corresponen (threshold). Llavors segons el criteri d'aturada el coeficient tindr� un rang determinat
    double m_mergeCoefficient;

};


};  //  end  namespace udg

#include "imagecomplexity3d.cpp"

#endif
