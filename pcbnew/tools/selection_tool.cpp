/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <class_board.h>
#include <class_board_item.h>
#include <class_track.h>
#include <class_module.h>

#include <wxPcbStruct.h>
#include <collectors.h>
#include <confirm.h>
#include <dialog_find.h>

#include <class_draw_panel_gal.h>
#include <view/view_controls.h>
#include <view/view_group.h>
#include <painter.h>

#include <tool/tool_event.h>
#include <tool/tool_manager.h>

#include "selection_tool.h"
#include "selection_area.h"
#include "bright_box.h"
#include "common_actions.h"

SELECTION_TOOL::SELECTION_TOOL() :
        TOOL_INTERACTIVE( "pcbnew.InteractiveSelection" ),
        SelectedEvent( TC_MESSAGE, TA_ACTION, "pcbnew.InteractiveSelection.selected" ),
        UnselectedEvent( TC_MESSAGE, TA_ACTION, "pcbnew.InteractiveSelection.unselected" ),
        ClearedEvent( TC_MESSAGE, TA_ACTION, "pcbnew.InteractiveSelection.cleared" ),
        m_frame( NULL ), m_additive( false ), m_multiple( false ),
        m_editModules( false ), m_locked( true )
{
    m_selArea = new SELECTION_AREA;
    m_selection.group = new KIGFX::VIEW_GROUP;
}


SELECTION_TOOL::~SELECTION_TOOL()
{
    delete m_selArea;
    delete m_selection.group;
}


void SELECTION_TOOL::Reset( RESET_REASON aReason )
{
    if( aReason == TOOL_BASE::MODEL_RELOAD )
        // Remove pointers to the selected items from containers
        // without changing their properties (as they are already deleted)
        m_selection.clear();
    else
        // Restore previous properties of selected items and remove them from containers
        clearSelection();

    m_frame = getEditFrame<PCB_BASE_FRAME>();
    m_locked = true;

    // Reinsert the VIEW_GROUP, in case it was removed from the VIEW
    getView()->Remove( m_selection.group );
    getView()->Add( m_selection.group );

    setTransitions();
}


int SELECTION_TOOL::Main( TOOL_EVENT& aEvent )
{
    // Main loop: keep receiving events
    while( OPT_TOOL_EVENT evt = Wait() )
    {
        // Should selected items be added to the current selection or
        // become the new selection (discarding previously selected items)
        m_additive = evt->Modifier( MD_SHIFT );

        // single click? Select single object
        if( evt->IsClick( BUT_LEFT ) )
        {
            if( evt->Modifier( MD_CTRL ) && !m_editModules )
            {
                highlightNet( evt->Position() );
            }
            else
            {
                if( !m_additive )
                    clearSelection();

                selectCursor( evt->Position() );
            }
        }

        // right click? if there is any object - show the context menu
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            if( m_selection.Empty() )
                selectCursor( evt->Position() );

            generateMenu();
        }

        // double click? Display the properties window
        else if( evt->IsDblClick( BUT_LEFT ) )
        {
            if( m_selection.Empty() )
                selectCursor( evt->Position() );

            m_toolMgr->RunAction( COMMON_ACTIONS::properties );
        }

        // drag with LMB? Select multiple objects (or at least draw a selection box) or drag them
        else if( evt->IsDrag( BUT_LEFT ) )
        {
            if( m_additive )
            {
                selectMultiple();
            }
            else if( m_selection.Empty() )
            {
                // There is nothing selected, so try to select something
                if( !selectCursor( getView()->ToWorld( getViewControls()->GetMousePosition() ), false ) )
                {
                    // If nothings has been selected or user wants to select more
                    // draw the selection box
                    selectMultiple();
                }
                else
                {
                    m_toolMgr->InvokeTool( "pcbnew.InteractiveEdit" );
                }
            }

            else
            {
                // Check if dragging has started within any of selected items bounding box
                if( selectionContains( evt->Position() ) )
                {
                    // Yes -> run the move tool and wait till it finishes
                    m_toolMgr->InvokeTool( "pcbnew.InteractiveEdit" );
                }
                else
                {
                    // No -> clear the selection list
                    clearSelection();
                }
            }
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectionCursor ) )
        {
            // GetMousePosition() is used, as it is independent of snapping settings
            selectCursor( getView()->ToWorld( getViewControls()->GetMousePosition() ) );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::find ) )
        {
            find( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::findMove ) )
        {
            findMove( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::selectItem ) )
        {
            SelectItem( *evt );
        }

        else if( evt->IsAction( &COMMON_ACTIONS::unselectItem ) )
        {
            UnselectItem( *evt );
        }

        else if( evt->IsCancel() || evt->Action() == TA_UNDO_REDO ||
                 evt->IsAction( &COMMON_ACTIONS::selectionClear ) )
        {
            clearSelection();
        }
    }

    // This tool is supposed to be active forever
    assert( false );

    return 0;
}


void SELECTION_TOOL::AddMenuItem( const TOOL_ACTION& aAction, const SELECTION_CONDITION& aCondition )
{
    assert( aAction.GetId() > 0 );    // Check if the action was registered before in ACTION_MANAGER

    m_menu.Add( aAction );
    m_menuConditions.push_back( aCondition );
}


void SELECTION_TOOL::AddSubMenu( CONTEXT_MENU* aMenu, const wxString& aLabel, const SELECTION_CONDITION& aCondition )
{
    m_menu.AppendSubMenu( aMenu, aLabel );
    m_menuConditions.push_back( aCondition );
}


void SELECTION_TOOL::toggleSelection( BOARD_ITEM* aItem )
{
    if( aItem->IsSelected() )
    {
        unselect( aItem );

        // Inform other potentially interested tools
        TOOL_EVENT unselectEvent( UnselectedEvent );
        m_toolMgr->ProcessEvent( unselectEvent );
    }
    else
    {
        if( !m_additive )
            clearSelection();

        // Prevent selection of invisible or inactive items
        if( selectable( aItem ) )
        {
            select( aItem );

            // Inform other potentially interested tools
            TOOL_EVENT selectEvent( SelectedEvent );
            m_toolMgr->ProcessEvent( selectEvent );
        }
    }
}


bool SELECTION_TOOL::selectCursor( const VECTOR2I& aWhere, bool aAllowDisambiguation )
{
    BOARD_ITEM* item;
    GENERAL_COLLECTORS_GUIDE guide = m_frame->GetCollectorsGuide();
    GENERAL_COLLECTOR collector;

    // Preferred types (they have the priority when if they are covered by a bigger item)
    const KICAD_T types[] = { PCB_TRACE_T, PCB_VIA_T, PCB_LINE_T,
                              PCB_MODULE_EDGE_T, PCB_MODULE_TEXT_T, EOT };

    if( m_editModules )
        collector.Collect( getModel<BOARD>(), GENERAL_COLLECTOR::ModuleItems,
                           wxPoint( aWhere.x, aWhere.y ), guide );
    else
        collector.Collect( getModel<BOARD>(), GENERAL_COLLECTOR::AllBoardItems,
                           wxPoint( aWhere.x, aWhere.y ), guide );

    switch( collector.GetCount() )
    {
    case 0:
        if( !m_additive )
            clearSelection();

        return false;

    case 1:
        toggleSelection( collector[0] );

        return true;

    default:
        // Remove unselectable items
        for( int i = collector.GetCount() - 1; i >= 0; --i )
        {
            if( !selectable( collector[i] ) )
                collector.Remove( i );
        }

        // Check if among the selection candidates there is only one instance of preferred type
        item = prefer( collector, types );
        if( item )
        {
            toggleSelection( item );

            return true;
        }

        // Let's see if there is still disambiguation in selection..
        if( collector.GetCount() == 1 )
        {
            toggleSelection( collector[0] );

            return true;
        }

        else if( aAllowDisambiguation && collector.GetCount() > 1 )
        {
            item = disambiguationMenu( &collector );

            if( item )
            {
                toggleSelection( item );

                return true;
            }
        }
        break;
    }

    return false;
}


bool SELECTION_TOOL::selectMultiple()
{
    bool cancelled = false;     // Was the tool cancelled while it was running?
    m_multiple = true;          // Multiple selection mode is active
    KIGFX::VIEW* view = getView();
    getViewControls()->SetAutoPan( true );

    view->Add( m_selArea );

    while( OPT_TOOL_EVENT evt = Wait() )
    {
        if( evt->IsCancel() )
        {
            cancelled = true;
            break;
        }

        if( evt->IsDrag( BUT_LEFT ) )
        {
            if( !m_additive )
                clearSelection();

            // Start drawing a selection box
            m_selArea->SetOrigin( evt->DragOrigin() );
            m_selArea->SetEnd( evt->Position() );
            m_selArea->ViewSetVisible( true );
            m_selArea->ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY );
        }

        if( evt->IsMouseUp( BUT_LEFT ) )
        {
            // Mark items within the selection box as selected
            std::vector<KIGFX::VIEW::LAYER_ITEM_PAIR> selectedItems;
            BOX2I selectionBox = m_selArea->ViewBBox();
            view->Query( selectionBox, selectedItems );         // Get the list of selected items

            std::vector<KIGFX::VIEW::LAYER_ITEM_PAIR>::iterator it, it_end;

            for( it = selectedItems.begin(), it_end = selectedItems.end(); it != it_end; ++it )
            {
                BOARD_ITEM* item = static_cast<BOARD_ITEM*>( it->first );

                // Add only those items that are visible and fully within the selection box
                if( !item->IsSelected() && selectable( item ) &&
                        selectionBox.Contains( item->ViewBBox() ) )
                {
                    select( item );
                }
            }

            // Do not display information about selected item,as there is more than one
            m_frame->SetCurItem( NULL );

            if( !m_selection.Empty() )
            {
                // Inform other potentially interested tools
                TOOL_EVENT selectEvent( SelectedEvent );
                m_toolMgr->ProcessEvent( selectEvent );
            }

            break;  // Stop waiting for events
        }
    }

    // Stop drawing the selection box
    m_selArea->ViewSetVisible( false );
    view->Remove( m_selArea );
    m_multiple = false;         // Multiple selection mode is inactive
    getViewControls()->SetAutoPan( false );

    return cancelled;
}


void SELECTION_TOOL::setTransitions()
{
    Go( &SELECTION_TOOL::Main, COMMON_ACTIONS::selectionActivate.MakeEvent() );
    Go( &SELECTION_TOOL::CursorSelection, COMMON_ACTIONS::selectionCursor.MakeEvent() );
    Go( &SELECTION_TOOL::ClearSelection, COMMON_ACTIONS::selectionClear.MakeEvent() );
    Go( &SELECTION_TOOL::SelectItem, COMMON_ACTIONS::selectItem.MakeEvent() );
    Go( &SELECTION_TOOL::UnselectItem, COMMON_ACTIONS::unselectItem.MakeEvent() );
    Go( &SELECTION_TOOL::find, COMMON_ACTIONS::find.MakeEvent() );
    Go( &SELECTION_TOOL::findMove, COMMON_ACTIONS::findMove.MakeEvent() );
}


bool SELECTION_TOOL::CheckLock()
{
    if( !m_locked || m_editModules )
        return false;

    bool containsLocked = false;

    // Check if the selection contains locked items
    for( int i = 0; i < m_selection.Size(); ++i )
    {
        BOARD_ITEM* item = m_selection.Item<BOARD_ITEM>( i );

        switch( item->Type() )
        {
        case PCB_MODULE_T:
            if( static_cast<MODULE*>( item )->IsLocked() )
                containsLocked = true;
            break;

        case PCB_MODULE_EDGE_T:
        case PCB_MODULE_TEXT_T:
            if( static_cast<MODULE*>( item->GetParent() )->IsLocked() )
                containsLocked = true;
            break;

        default:    // suppress warnings
            break;
        }
    }

    if( containsLocked &&
        !IsOK( m_frame, _( "Selection contains locked items. Do you want to continue?" ) ) )
    {
        return true;
    }

    m_locked = false;

    return false;
}


int SELECTION_TOOL::CursorSelection( TOOL_EVENT& aEvent )
{
    selectCursor( getView()->ToWorld( getViewControls()->GetMousePosition() ) );
    setTransitions();

    return 0;
}


int SELECTION_TOOL::ClearSelection( TOOL_EVENT& aEvent )
{
    clearSelection();
    setTransitions();

    return 0;
}

int SELECTION_TOOL::SelectItem( TOOL_EVENT& aEvent )
{
    // Check if there is an item to be selected
    BOARD_ITEM* item = static_cast<BOARD_ITEM*>( aEvent.Parameter() );

    if( item )
    {
        select( item );

        // Inform other potentially interested tools
        TOOL_EVENT select( SelectedEvent );
        m_toolMgr->ProcessEvent( select );
    }

    setTransitions();

    return 0;
}

int SELECTION_TOOL::UnselectItem( TOOL_EVENT& aEvent )
{
    // Check if there is an item to be selected
    BOARD_ITEM* item = static_cast<BOARD_ITEM*>( aEvent.Parameter() );

    if( item )
    {
        unselect( item );

        // Inform other potentially interested tools
        TOOL_EVENT unselect( UnselectedEvent );
        m_toolMgr->ProcessEvent( unselect );
    }

    setTransitions();

    return 0;
}

void SELECTION_TOOL::findCallback( BOARD_ITEM* aItem )
{
    clearSelection();

    if( aItem )
        toggleSelection( aItem );

    m_frame->GetGalCanvas()->ForceRefresh();
}


int SELECTION_TOOL::find( TOOL_EVENT& aEvent )
{
    DIALOG_FIND dlg( m_frame );
    dlg.EnableWarp( false );
    dlg.SetCallback( boost::bind( &SELECTION_TOOL::findCallback, this, _1 ) );
    dlg.ShowModal();
    setTransitions();

    return 0;
}


int SELECTION_TOOL::findMove( TOOL_EVENT& aEvent )
{
    MODULE* module = m_frame->GetModuleByName();

    if( module )
    {
        clearSelection();
        toggleSelection( module );
        m_toolMgr->InvokeTool( "pcbnew.InteractiveEdit" );
    }

    setTransitions();

    return 0;
}


void SELECTION_TOOL::clearSelection()
{
    if( m_selection.Empty() )
        return;

    KIGFX::VIEW_GROUP::const_iter it, it_end;

    // Restore the initial properties
    for( it = m_selection.group->Begin(), it_end = m_selection.group->End(); it != it_end; ++it )
    {
        BOARD_ITEM* item = static_cast<BOARD_ITEM*>( *it );

        item->ViewSetVisible( true );
        item->ClearSelected();
    }
    m_selection.clear();

    m_frame->SetCurItem( NULL );
    m_locked = true;

    // Inform other potentially interested tools
    TOOL_EVENT clearEvent( ClearedEvent );
    m_toolMgr->ProcessEvent( clearEvent );
}


BOARD_ITEM* SELECTION_TOOL::disambiguationMenu( GENERAL_COLLECTOR* aCollector )
{
    BOARD_ITEM* current = NULL;
    boost::shared_ptr<BRIGHT_BOX> brightBox;
    CONTEXT_MENU menu;

    int limit = std::min( 10, aCollector->GetCount() );

    for( int i = 0; i < limit; ++i )
    {
        wxString text;
        BOARD_ITEM* item = ( *aCollector )[i];
        text = item->GetSelectMenuText();
        menu.Add( text, i );
    }

    menu.SetTitle( _( "Clarify selection" ) );
    SetContextMenu( &menu, CMENU_NOW );

    while( OPT_TOOL_EVENT evt = Wait() )
    {
        if( evt->Action() == TA_CONTEXT_MENU_UPDATE )
        {
            if( current )
                current->ClearBrightened();

            int id = *evt->GetCommandId();

            // User has pointed an item, so show it in a different way
            if( id >= 0 && id < limit )
            {
                current = ( *aCollector )[id];
                current->SetBrightened();
            }
            else
            {
                current = NULL;
            }
        }
        else if( evt->Action() == TA_CONTEXT_MENU_CHOICE )
        {
            boost::optional<int> id = evt->GetCommandId();

            // User has selected an item, so this one will be returned
            if( id && ( *id >= 0 ) )
                current = ( *aCollector )[*id];

            break;
        }

        // Draw a mark to show which item is available to be selected
        if( current && current->IsBrightened() )
        {
            brightBox.reset( new BRIGHT_BOX( current ) );
            getView()->Add( brightBox.get() );
            // BRIGHT_BOX is removed from view on destruction
        }
    }

    return current;
}


BOARD_ITEM* SELECTION_TOOL::pickSmallestComponent( GENERAL_COLLECTOR* aCollector )
{
    int count = aCollector->GetPrimaryCount();     // try to use preferred layer

    if( 0 == count )
        count = aCollector->GetCount();

    for( int i = 0; i < count; ++i )
    {
        if( ( *aCollector )[i]->Type() != PCB_MODULE_T )
            return NULL;
    }

    // All are modules, now find smallest MODULE
    int minDim = 0x7FFFFFFF;
    int minNdx = 0;

    for( int i = 0; i < count; ++i )
    {
        MODULE* module = (MODULE*) ( *aCollector )[i];

        int lx = module->GetBoundingBox().GetWidth();
        int ly = module->GetBoundingBox().GetHeight();

        int lmin = std::min( lx, ly );

        if( lmin < minDim )
        {
            minDim = lmin;
            minNdx = i;
        }
    }

    return (*aCollector)[minNdx];
}


bool SELECTION_TOOL::selectable( const BOARD_ITEM* aItem ) const
{
    // Is high contrast mode enabled?
    bool highContrast = getView()->GetPainter()->GetSettings()->GetHighContrast();

    if( highContrast )
    {
        bool onActive = false;          // Is the item on any of active layers?
        int layers[KIGFX::VIEW::VIEW_MAX_LAYERS], layers_count;

        // Filter out items that do not belong to active layers
        const std::set<unsigned int>& activeLayers = getView()->GetPainter()->
                                                     GetSettings()->GetActiveLayers();
        aItem->ViewGetLayers( layers, layers_count );

        for( int i = 0; i < layers_count; ++i )
        {
            if( activeLayers.count( layers[i] ) > 0 ) // Item is on at least one of the active layers
            {
                onActive = true;
                break;
            }
        }

        if( !onActive ) // We do not want to select items that are in the background
            return false;
    }

    BOARD* board = getModel<BOARD>();

    switch( aItem->Type() )
    {
    case PCB_VIA_T:
        {
            // For vias it is enough if only one of layers is visible
            LAYER_ID top, bottom;

            static_cast<const VIA*>( aItem )->LayerPair( &top, &bottom );

            return board->IsLayerVisible( top ) || board->IsLayerVisible( bottom );
        }
        break;

    case PCB_MODULE_T:
        if( aItem->IsOnLayer( F_Cu ) && board->IsElementVisible( MOD_FR_VISIBLE ) )
            return !m_editModules;

        if( aItem->IsOnLayer( B_Cu ) && board->IsElementVisible( MOD_BK_VISIBLE ) )
            return !m_editModules;

        return false;

        break;

    case PCB_MODULE_TEXT_T:
        if( m_multiple && !m_editModules )
            return false;
        break;

    // These are not selectable
    case PCB_MODULE_EDGE_T:
    case PCB_PAD_T:
        return m_editModules;

    case NOT_USED:
    case TYPE_NOT_INIT:
        return false;

    default:    // Suppress warnings
        break;
    }

    // All other items are selected only if the layer on which they exist is visible
    return board->IsLayerVisible( aItem->GetLayer() );
}


void SELECTION_TOOL::select( BOARD_ITEM* aItem )
{
    // Modules are treated in a special way - when they are selected, we have to mark
    // all the parts that make the module as selected
    if( aItem->Type() == PCB_MODULE_T )
    {
        MODULE* module = static_cast<MODULE*>( aItem );
        module->RunOnChildren( boost::bind( &SELECTION_TOOL::selectVisually, this, _1 ) );
    }

    selectVisually( aItem );
    ITEM_PICKER picker( aItem );
    m_selection.items.PushItem( picker );

    if( m_selection.Size() == 1 )
    {
        // Set as the current item, so the information about selection is displayed
        m_frame->SetCurItem( aItem, true );
    }
    else if( m_selection.Size() == 2 )  // Check only for 2, so it will not be
    {                                   // called for every next selected item
        // If multiple items are selected, do not show the information about the selected item
        m_frame->SetCurItem( NULL, true );
    }
}


void SELECTION_TOOL::unselect( BOARD_ITEM* aItem )
{
    // Modules are treated in a special way - when they are selected, we have to
    // unselect all the parts that make the module, not the module itself
    if( aItem->Type() == PCB_MODULE_T )
    {
        MODULE* module = static_cast<MODULE*>( aItem );
        module->RunOnChildren( boost::bind( &SELECTION_TOOL::unselectVisually, this, _1 ) );
    }

    unselectVisually( aItem );

    int itemIdx = m_selection.items.FindItem( aItem );
    if( itemIdx >= 0 )
        m_selection.items.RemovePicker( itemIdx );

    if( m_selection.Empty() )
    {
        m_frame->SetCurItem( NULL );
        m_locked = true;
    }

    // Inform other potentially interested tools
    TOOL_EVENT unselected( UnselectedEvent );
    m_toolMgr->ProcessEvent( unselected );
}


void SELECTION_TOOL::selectVisually( BOARD_ITEM* aItem ) const
{
    m_selection.group->Add( aItem );

    // Hide the original item, so it is shown only on overlay
    aItem->ViewSetVisible( false );
    aItem->SetSelected();
}


void SELECTION_TOOL::unselectVisually( BOARD_ITEM* aItem ) const
{
    m_selection.group->Remove( aItem );

    // Restore original item visibility
    aItem->ViewSetVisible( true );
    aItem->ClearSelected();
}


bool SELECTION_TOOL::selectionContains( const VECTOR2I& aPoint ) const
{
    const unsigned GRIP_MARGIN = 20;
    VECTOR2D margin = getView()->ToWorld( VECTOR2D( GRIP_MARGIN, GRIP_MARGIN ), false );

    // Check if the point is located within any of the currently selected items bounding boxes
    for( unsigned int i = 0; i < m_selection.items.GetCount(); ++i )
    {
        BOARD_ITEM* item = m_selection.Item<BOARD_ITEM>( i );
        BOX2I itemBox = item->ViewBBox();
        itemBox.Inflate( margin.x, margin.y );    // Give some margin for gripping an item

        if( itemBox.Contains( aPoint ) )
            return true;
    }

    return false;
}


void SELECTION_TOOL::highlightNet( const VECTOR2I& aPoint )
{
    KIGFX::RENDER_SETTINGS* render = getView()->GetPainter()->GetSettings();
    GENERAL_COLLECTORS_GUIDE guide = m_frame->GetCollectorsGuide();
    GENERAL_COLLECTOR collector;
    int net = -1;

    // Find a connected item for which we are going to highlight a net
    collector.Collect( getModel<BOARD>(), GENERAL_COLLECTOR::PadsTracksOrZones,
                       wxPoint( aPoint.x, aPoint.y ), guide );
    bool enableHighlight = ( collector.GetCount() > 0 );

    // Obtain net code for the clicked item
    if( enableHighlight )
        net = static_cast<BOARD_CONNECTED_ITEM*>( collector[0] )->GetNetCode();

    if( enableHighlight != render->GetHighlight() || net != render->GetHighlightNetCode() )
    {
        render->SetHighlight( enableHighlight, net );
        getView()->UpdateAllLayersColor();
    }
}


BOARD_ITEM* SELECTION_TOOL::prefer( GENERAL_COLLECTOR& aCollector, const KICAD_T aTypes[] ) const
{
    BOARD_ITEM* preferred = NULL;

    int typesNr = 0;
    while( aTypes[typesNr++] != EOT );      // count number of types, excluding the sentinel (EOT)

    for( int i = 0; i < aCollector.GetCount(); ++i )
    {
        KICAD_T type = aCollector[i]->Type();

        for( int j = 0; j < typesNr - 1; ++j )      // Check if the item's type is in our list
        {
            if( aTypes[j] == type )
            {
                if( preferred == NULL )
                {
                    preferred = aCollector[i];  // save the first matching item
                    break;
                }
                else
                {
                    return NULL;  // there is more than one preferred item, so there is no clear choice
                }
            }
        }
    }

    return preferred;
}


void SELECTION_TOOL::generateMenu()
{
    // Create a copy of the master context menu
    m_menuCopy = m_menu;

    assert( m_menuCopy.GetMenuItemCount() == m_menuConditions.size() );

    // Filter out entries that does not apply to the current selection
    for( int i = m_menuCopy.GetMenuItemCount() - 1; i >= 0; --i )
    {
        if( !m_menuConditions[i]( m_selection ) )
        {
            wxMenuItem* item = m_menuCopy.FindItemByPosition( i );
            m_menuCopy.Destroy( item );
        }
    }

    if( m_menuCopy.GetMenuItemCount() > 0 )
        SetContextMenu( &m_menuCopy, CMENU_NOW );
}


void SELECTION::clear()
{
    items.ClearItemsList();
    group->Clear();
}
