/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2007 Dick Hollenbeck, dick@softplc.com
 * Copyright (C) 2004-2007 Kicad Developers, see change_log.txt for contributors.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html 
 * or you may search the http://www.gnu.org website for the version 2 license, 
 * or you may write to the Free Software Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "collectors.h"  
#include "pcbnew.h"             // class BOARD


/*  This module contains out of line member functions for classes given in 
    collectors.h.  Those classes augment the functionality of class WinEDA_PcbFrame.
*/


// see collectors.h
const KICAD_T GENERAL_COLLECTOR::AllBoardItems[] = {
    
    // there are some restrictions on the order of items in the general case.
    // all items in m_Drawings for instance should be contiguous.
    TYPETEXTE, 
    TYPEDRAWSEGMENT, 
    TYPEMARKER,
    TYPECOTATION,
    TYPEMIRE,
    TYPEVIA,
    TYPETRACK,
    TYPEPAD,
    TYPETEXTEMODULE,
    TYPEMODULE,
    TYPEZONE,
	TYPEZONE_CONTAINER,
    EOT
};    


/*
const KICAD_T GENERAL_COLLECTOR::PrimaryItems[] = {
    TYPETEXTE, 
    TYPEDRAWSEGMENT, 
    TYPEMARKER,
    TYPECOTATION,
    TYPEVIA,
    TYPETRACK,
    TYPEMODULE,
    EOT
};
*/


const KICAD_T GENERAL_COLLECTOR::AllButZones[] = {
    TYPETEXTE, 
    TYPEDRAWSEGMENT, 
    TYPEMARKER,
    TYPECOTATION,
    TYPEMIRE,
    TYPEVIA,
    TYPETRACK,
    TYPEPAD,
    TYPETEXTEMODULE,
    TYPEMODULE,
    EOT
};
    

const KICAD_T GENERAL_COLLECTOR::ModuleItems[] = {
    TYPEMODULE,
    EOT
};


const KICAD_T GENERAL_COLLECTOR::PadsOrModules[] = { 
    TYPEPAD, 
    TYPEMODULE, 
    EOT 
};


const KICAD_T GENERAL_COLLECTOR::PadsOrTracks[] = { 
    TYPEPAD, 
    TYPEVIA,
    TYPETRACK,
    TYPEZONE,
    EOT 
};


const KICAD_T GENERAL_COLLECTOR::ModulesAndTheirItems[] = { 
    TYPETEXTEMODULE,
    TYPEEDGEMODULE,
    TYPEPAD, 
    TYPEMODULE, 
    EOT 
};


const KICAD_T GENERAL_COLLECTOR::Tracks[] = {
    TYPETRACK,
    TYPEVIA,
    EOT
};


/**
 * Function Inspect
 * is the examining function within the INSPECTOR which is passed to the 
 * Iterate function.  Searches and collects all the objects that the old 
 * function PcbGeneralLocateAndDisplay() would find, except that it keeps all
 * that it finds and does not do any displaying. 
 *
 * @param testItem An EDA_BaseStruct to examine.
 * @param notUsed The const void* testData.
 * @return SEARCH_RESULT - SEARCH_QUIT if the Iterator is to stop the scan,
 *   else SCAN_CONTINUE;
 */ 
SEARCH_RESULT GENERAL_COLLECTOR::Inspect( EDA_BaseStruct* testItem, const void* notUsed )
{
    BOARD_ITEM* item = (BOARD_ITEM*) testItem;
    MODULE*     module = NULL;

#if 0   // debugging
    static int breakhere = 0;
    switch( item->Type() )
    {
    case TYPEPAD:
        {
            MODULE* m = (MODULE*) item->GetParent();
            if( m->GetReference() == wxT("Y2") )
            {
                breakhere++;
            }
        }
        break;
    case TYPEVIA:
        breakhere++;
        break;
    case TYPETRACK:
        breakhere++;
        break;
    case TYPEZONE:
        breakhere++;
        break;
    case TYPETEXTE: 
        breakhere++;
        break;
    case TYPEDRAWSEGMENT: 
        breakhere++;
        break;
    case TYPECOTATION:
        breakhere++;
        break;
    case TYPETEXTEMODULE:
        {            
            TEXTE_MODULE* tm = (TEXTE_MODULE*) item;
            if( tm->m_Text == wxT("10uH") )
            {
                breakhere++;
            }
        }            
        break;
    case TYPEMODULE:
        {
            MODULE* m = (MODULE*) item;
            if( m->GetReference() == wxT("C98") )
            {
                breakhere++;
            }
        }
        break;
        
    default:
        breakhere++;
        break;
    }
#endif


    switch( item->Type() )
    {
    case TYPEPAD:
        // if pad is a thru hole, then it can be visible when its parent module is not.
        if( ((D_PAD*)item)->m_Attribut != SMD )    // a hole is present, so multiple layers
        {
            // there are no pad specific visibility controls at this time.
            // proceed to the common tests below, but without the parent module test, 
            // by leaving module==NULL
        }
        else    // smd, so use common test below
            module = (MODULE*) item->GetParent();
        break;
        
    case TYPEVIA:
        break;
    case TYPETRACK:
        break;
    case TYPEZONE:
        break;
    case TYPEZONE_CONTAINER:
        break;
    case TYPETEXTE: 
        break;
    case TYPEDRAWSEGMENT: 
        break;
    case TYPECOTATION:
        break;
        
    case TYPETEXTEMODULE:
        module = (MODULE*) item->GetParent();
        
        if( m_Guide->IgnoreMTextsMarkedNoShow() && ((TEXTE_MODULE*)item)->m_NoShow )
            goto exit;
        
        if( module )
        {
            if( m_Guide->IgnoreMTextsOnCopper() && module->GetLayer()==COPPER_LAYER_N )
                goto exit;
            
            if( m_Guide->IgnoreMTextsOnCmp() && module->GetLayer()==LAYER_CMP_N )
                goto exit;
        }
        break;
        
    case TYPEMODULE:
        module = (MODULE*) item;
        break;
        
    default:
        break;
    }
    
    // common tests:

    if( module )    // true from case TYPEPAD, TYPETEXTEMODULE, or TYPEMODULE
    {
        if( m_Guide->IgnoreModulesOnCu() && module->GetLayer()==COPPER_LAYER_N )
            goto exit;
        
        if( m_Guide->IgnoreModulesOnCmp() && module->GetLayer()==LAYER_CMP_N )
            goto exit;
    }

    
    if( item->IsOnLayer( m_Guide->GetPreferredLayer() ) || m_Guide->IgnorePreferredLayer() )
    {
        int layer = item->GetLayer();

        // Modules and their subcomponents: text and pads are not sensitive to the layer 
        // visibility controls.  They all have their own separate visibility controls        
        if( module || m_Guide->IsLayerVisible( layer ) || !m_Guide->IgnoreNonVisibleLayers() )
        {
            if( !m_Guide->IsLayerLocked(layer) || !m_Guide->IgnoreLockedLayers() )
            {
                if( !item->IsLocked() || !m_Guide->IgnoreLockedItems() )
                {
                    if( item->HitTest( m_RefPos ) )
                    {
                        Append( item );
                        goto exit;
                    }
                }
            }
        }
    }

    
    if( m_Guide->IncludeSecondary() )
    {
        // for now, "secondary" means "tolerate any layer".  It has
        // no effect on other criteria, since there is a separate "ignore" control for 
        // those in the COLLECTORS_GUIDE
        
        int layer = item->GetLayer();
        
        // Modules and their subcomponents: text and pads are not sensitive to the layer 
        // visibility controls.  They all have their own separate visibility controls        
        if( module || m_Guide->IsLayerVisible( layer ) || !m_Guide->IgnoreNonVisibleLayers() )
        {
            if( !m_Guide->IsLayerLocked(layer) || !m_Guide->IgnoreLockedLayers() )
            {
                if( !item->IsLocked() || !m_Guide->IgnoreLockedItems() )
                {
                    if( item->HitTest( m_RefPos ) )
                    {
                        Append2nd( item );
                        goto exit;
                    }
                }
            }
        }
    }
    
exit:
    return SEARCH_CONTINUE;     // always when collecting
}


// see collectors.h 
void GENERAL_COLLECTOR::Collect( BOARD_ITEM* aItem, const KICAD_T aScanList[],
              const wxPoint& aRefPos, const COLLECTORS_GUIDE& aGuide )
{
    Empty();        // empty the collection, primary criteria list
    Empty2nd();     // empty the collection, secondary criteria list

    // remember guide, pass it to Inspect()
    SetGuide( &aGuide );

    SetScanTypes( aScanList );
    
    // remember where the snapshot was taken from and pass refPos to
    // the Inspect() function.
    SetRefPos( aRefPos );

    // visit the board or module with the INSPECTOR (me).
    aItem->Visit(   this,       // INSPECTOR* inspector
                    NULL,       // const void* testData, not used here 
                    m_ScanTypes);
    
    SetTimeNow();               // when snapshot was taken

    // record the length of the primary list before concatonating on to it.
    m_PrimaryLength = m_List.size();
    
    // append 2nd list onto end of the first list 
    for( unsigned i=0;  i<m_List2nd.size();  ++i )
        Append( m_List2nd[i] );
    
    Empty2nd();
}


//EOF
