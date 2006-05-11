/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGIMAGELIST_H
#define UDGIMAGELIST_H
#define HAVE_CONFIG_H 1

#include <cond.h>
#include "image.h"
#include <string>
#include <list>

/* AQUESTA CLASSE NOMES SERA ACCEDIDA PER MES D'UN THREAD A LA VEGADA PER AIXO NO S'HAN IMPLEMENTAT SEMAFORS
  */
namespace udg {

/** Classe per manipular una llista d'objectes image
@author marc
*/
class ImageList{
public:

    ///constructor de la classe
    ImageList();

    ///destructor de la classe
    ~ImageList();
    
    /** Insereix una imatge a la llista
     * @param  Imatge a inserir
     */
    void insert(Image);
        
     ///Aquesta accio situa l'iterador de la llista al primer element de la llista abans de comen�ar a llegir s'ha d'invocar aquest m�tode
    void firstImage();
    
    /// l'iterador passa a apuntar al seg�ent element
    void nextImage();
    
    /** Indica si s'ha arribat al final de la llista
     * @return boolea indicant si s'ha arribat al final de la llista
     */
    bool end();    

    /** retorna el n�mero d'imatges de la llista
     * @return  n�mero d'imatges de la llista
     */
    int count();
  
    /** retorna un objecte image
     * @return objecte image
     */
    Image getImage();
    
    /// Aquest m�tode buida la llista
    void clear();
         
 private :
 
    list<Image> m_imageList;     
    list<Image>::iterator m_iterator;
    
};
};//end namespace udg

#endif
