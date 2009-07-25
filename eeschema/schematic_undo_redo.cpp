/*************************************************************/
/*  eeschema: undo and redo functions for schem�atic editor  */
/*************************************************************/

#include "fctsys.h"
#include "common.h"
#include "confirm.h"
#include "class_drawpickedstruct.h"

#include "program.h"
#include "libcmp.h"
#include "general.h"
#include "protos.h"
#include "class_marker_sch.h"

/* Functions to undo and redo edit commands.
 *  commmands to undo are stored in CurrentScreen->m_UndoList
 *  commmands to redo are stored in CurrentScreen->m_RedoList
 *
 *  m_UndoList and m_RedoList handle a std::vector of PICKED_ITEMS_LIST
 *  Each PICKED_ITEMS_LIST handle a std::vector of pickers (class ITEM_PICKER),
 *  that store the list of schematic items that are concerned by the command to undo or redo
 *  and is created for each command to undo (handle also a command to redo).
 *  each picker has a pointer pointing to an item to undo or redo (in fact: deleted, added or modified),
 * and has a pointer to a copy of this item, when this item has been modified
 * (the old values of parameters are therefore saved)
 *
 *  there are 3 cases:
 *  - delete item(s) command
 *  - change item(s) command
 *  - add item(s) command
 *
 *  Undo command
 *  - delete item(s) command:
 *       =>  deleted items are moved in undo list
 *
 *  - change item(s) command
 *      => A copy of item(s) is made (a DrawPickedStruct list of wrappers)
 *      the .m_Link member of each wrapper points the modified item.
 *      the .m_Item member of each wrapper points the old copy of this item.
 *
 *  - add item(s) command
 *      =>A list of item(s) is made. The .m_Item member of each wrapper points the new item.
 *
 *  Redo command
 *  - delete item(s) old command:
 *      => deleted items are moved in EEDrawList list, and in
 *
 *  - change item(s) command
 *      => the copy of item(s) is moved in Undo list
 *
 *  - add item(s) command
 *      => The list of item(s) is used to create a deleted list in undo list(same as a delete command)
 *
 *  A problem is the hierarchical sheet handling.
 *  the data associated (subhierarchy, uno/redo list) is deleted only
 *  when the sheet is really deleted (i.e. when deleted from undo or redo list)
 *  This is handled by its destructor.
 */


/**************************************************************/
void SwapData( EDA_BaseStruct* aItem, EDA_BaseStruct* aImage )
/***************************************************************/

/* Used if undo / redo command:
 *  swap data between Item and its copy, pointed by its .m_Image member
 * swapped data is data modified by edition, so not all values are swapped
 */
{
    if( aItem == NULL || aImage == NULL )
    {
        wxMessageBox( wxT( "SwapData error: NULL pointer" ) );
        return;
    }

    switch( aItem->Type() )
    {
    case DRAW_POLYLINE_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (DrawPolylineStruct*) aItem )
        #define DEST   ( (DrawPolylineStruct*) aImage )
        break;

    case DRAW_JUNCTION_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (DrawJunctionStruct*) aItem )
        #define DEST   ( (DrawJunctionStruct*) aImage )
        EXCHG( SOURCE->m_Pos, DEST->m_Pos );
        break;

    case TYPE_SCH_LABEL:
    case TYPE_SCH_GLOBALLABEL:
    case TYPE_SCH_HIERLABEL:
    case TYPE_SCH_TEXT:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (SCH_TEXT*) aItem )
        #define DEST   ( (SCH_TEXT*) aImage )
        DEST->SwapData( SOURCE );
        break;

    case TYPE_SCH_COMPONENT:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (SCH_COMPONENT*) aItem )
        #define DEST   ( (SCH_COMPONENT*) aImage )
        DEST->SwapData( SOURCE );
        break;

    case DRAW_SEGMENT_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (EDA_DrawLineStruct*) aItem )
        #define DEST   ( (EDA_DrawLineStruct*) aImage )
        EXCHG( SOURCE->m_Start, DEST->m_Start );
        EXCHG( SOURCE->m_End, DEST->m_End );
        break;

    case DRAW_BUSENTRY_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (DrawBusEntryStruct*) aItem )
        #define DEST   ( (DrawBusEntryStruct*) aImage )
        EXCHG( SOURCE->m_Pos, DEST->m_Pos );
        EXCHG( SOURCE->m_Size, DEST->m_Size );
        break;

    case DRAW_SHEET_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (DrawSheetStruct*) aItem )
        #define DEST   ( (DrawSheetStruct*) aImage )
        DEST->SwapData( SOURCE );
        break;

    case DRAW_MARKER_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (MARKER_SCH*) aItem )
        #define DEST   ( (MARKER_SCH*) aImage )
        EXCHG( SOURCE->m_Pos, DEST->m_Pos );
        break;

    case DRAW_HIERARCHICAL_PIN_SHEET_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (Hierarchical_PIN_Sheet_Struct*) aItem )
        #define DEST   ( (Hierarchical_PIN_Sheet_Struct*) aImage )
        EXCHG( SOURCE->m_Edge, DEST->m_Edge );
        EXCHG( SOURCE->m_Shape, DEST->m_Shape );
        break;

    case DRAW_NOCONNECT_STRUCT_TYPE:
        #undef SOURCE
        #undef DEST
        #define SOURCE ( (DrawNoConnectStruct*) aItem )
        #define DEST   ( (DrawNoConnectStruct*) aImage )
        EXCHG( SOURCE->m_Pos, DEST->m_Pos );
        break;

    case DRAW_PART_TEXT_STRUCT_TYPE:
        break;

    // not directly used in schematic:
    default:
        DisplayInfoMessage( NULL, wxT( "SwapData() error: unexpected type" ) );
        break;
    }
}


/***********************************************************************/
void WinEDA_SchematicFrame::SaveCopyInUndoList( SCH_ITEM* ItemToCopy,
                                                int       flag_type_command )
/***********************************************************************/

/** function SaveCopyInUndoList
 * Create a copy of the current schematic item, and put it in the undo list.
 *
 *  flag_type_command =
 *      IS_CHANGED
 *      IS_NEW
 *      IS_DELETED
 *      IS_WIRE_IMAGE
 *
 *  If it is a delete command, items are put on list with the .Flags member set to IS_DELETED.
 *  When it will be really deleted, the EEDrawList and the subhierarchy will be deleted.
 *  If it is only a copy, the EEDrawList and the subhierarchy must NOT be deleted.
 *
 *  Note:
 *  Edit wires and busses is a bit complex.
 *  because when a new wire is added, modifications in wire list
 *  (wire concatenation) there are modified items, deleted items and new items
 *  so flag_type_command is IS_WIRE_IMAGE: the struct ItemToCopy is a list of wires
 *  saved in Undo List (for Undo or Redo commands, saved wires will be exchanged with current wire list
 */
{
    SCH_ITEM*          CopyOfItem;
    PICKED_ITEMS_LIST* commandToUndo = new PICKED_ITEMS_LIST();

    commandToUndo->m_UndoRedoType = flag_type_command;
    ITEM_PICKER        itemWrapper( ItemToCopy, flag_type_command );

    if( ItemToCopy )
    {
        switch( flag_type_command )
        {
        case IS_CHANGED:        /* Create a copy of schematic */
            if( ItemToCopy->Type() == DRAW_PICK_ITEM_STRUCT_TYPE )
            {
                DrawPickedStruct* PickedList = (DrawPickedStruct*) ItemToCopy;
                while( PickedList )
                {
                    CopyOfItem = DuplicateStruct( (SCH_ITEM*) PickedList->m_PickedStruct );
                    CopyOfItem->m_Flags = flag_type_command;
                    itemWrapper.m_Item  = CopyOfItem;
                    itemWrapper.m_Link  = (SCH_ITEM*) PickedList->m_PickedStruct;
                    commandToUndo->PushItem( itemWrapper );
                    PickedList = PickedList->Next();
                }
            }
            else
            {
                CopyOfItem = DuplicateStruct( ItemToCopy );
                itemWrapper.m_Item = CopyOfItem;
                itemWrapper.m_Link = ItemToCopy;
                commandToUndo->PushItem( itemWrapper );
            }
            break;

        case IS_NEW:
            if( ItemToCopy->Type() == DRAW_PICK_ITEM_STRUCT_TYPE )
            {
                DrawPickedStruct* PickedList = (DrawPickedStruct*) ItemToCopy;
                while( PickedList )
                {
                    CopyOfItem = (SCH_ITEM*) PickedList->m_PickedStruct;
                    PickedList->m_PickedStruct = NULL;
                    PickedList->m_Flags = flag_type_command;
                    PickedList = PickedList->Next();
                    itemWrapper.m_Item = CopyOfItem;
                    commandToUndo->PushItem( itemWrapper );
                }
            }
            else
            {
                commandToUndo->PushItem( itemWrapper );
            }
            break;

        case IS_NEW | IS_CHANGED:   // when more than one item, some are new, some are changed
            if( ItemToCopy->Type() == DRAW_PICK_ITEM_STRUCT_TYPE )
            {
                DrawPickedStruct* PickedList = (DrawPickedStruct*) ItemToCopy;
                while( PickedList )
                {
                    CopyOfItem = (SCH_ITEM*) PickedList->m_PickedStruct;
                    PickedList->m_PickedStruct = NULL;
                    PickedList = PickedList->Next();
                    itemWrapper.m_Item = CopyOfItem;
                    itemWrapper.m_UndoRedoStatus = PickedList->m_Flags;
                    commandToUndo->PushItem( itemWrapper );
                }
            }
            else
            {
                commandToUndo->PushItem( itemWrapper );
            }
            break;

        case IS_WIRE_IMAGE:
            commandToUndo->PushItem( itemWrapper );
            break;

        case IS_DELETED:
            ItemToCopy->m_Flags = flag_type_command;
            if( ItemToCopy->Type() == DRAW_PICK_ITEM_STRUCT_TYPE )
            {
                DrawPickedStruct* PickedList = (DrawPickedStruct*) ItemToCopy;
                while( PickedList )
                {
                    CopyOfItem = (SCH_ITEM*) PickedList->m_PickedStruct;
                    CopyOfItem->m_Flags = flag_type_command;
                    PickedList->m_Flags = flag_type_command;
                    PickedList = PickedList->Next();
                    itemWrapper.m_Item = CopyOfItem;
                    commandToUndo->PushItem( itemWrapper );
                }
            }
            else
            {
                commandToUndo->PushItem( itemWrapper );
            }
            break;

        default:
        {
            wxString msg;
            msg.Printf( wxT( "SaveCopyInUndoList() error (unknown code %X)" ), flag_type_command );
            DisplayError( this, msg );
        }
        break;
        }
    }
    /* Save the copy in undo list */
    GetScreen()->PushCommandToUndoList( commandToUndo );

    /* Clear redo list, because after new save there is no redo to do */
    GetScreen()->ClearUndoORRedoList( GetScreen()->m_RedoList );
}


/** function SaveCopyInUndoList
 * @param aItemsList = a PICKED_ITEMS_LIST of items to save
 * @param aTypeCommand = type of comand ( IS_CHANGED, IS_NEW, IS_DELETED ...
 */
void WinEDA_SchematicFrame::SaveCopyInUndoList( PICKED_ITEMS_LIST& aItemsList, int aTypeCommand )
{
    SCH_ITEM*          CopyOfItem;
    PICKED_ITEMS_LIST* commandToUndo = new PICKED_ITEMS_LIST();

    commandToUndo->m_UndoRedoType = aTypeCommand;
    ITEM_PICKER        itemWrapper;

    for( unsigned ii = 0; ii < aItemsList.GetCount(); ii++ )
    {
        SCH_ITEM* ItemToCopy = (SCH_ITEM*) aItemsList.GetItemData( ii );
        int       command    = aItemsList.GetItemStatus( ii );
        if( command == 0 )
        {
            command = aTypeCommand;
        }
        itemWrapper.m_Item = ItemToCopy;
        itemWrapper.m_UndoRedoStatus = command;
        switch( command )
        {
        case IS_CHANGED:        /* Create a copy of schematic */
            CopyOfItem = DuplicateStruct( ItemToCopy );
            itemWrapper.m_Item = CopyOfItem;
            itemWrapper.m_Link = ItemToCopy;
            commandToUndo->PushItem( itemWrapper );
            break;

        case IS_NEW:
        case IS_NEW | IS_CHANGED:   // when more than one item, some are new, some are changed
            commandToUndo->PushItem( itemWrapper );
            break;

        case IS_DELETED:
            ItemToCopy->m_Flags = IS_DELETED;
            commandToUndo->PushItem( itemWrapper );
            break;

        default:
        {
            wxString msg;
            msg.Printf( wxT( "SaveCopyInUndoList() error (unknown code %X)" ), command );
            DisplayError( this, msg );
        }
        break;
        }
    }

    /* Save the copy in undo list */
    GetScreen()->PushCommandToUndoList( commandToUndo );

    /* Clear redo list, because after new save there is no redo to do */
    GetScreen()->ClearUndoORRedoList( GetScreen()->m_RedoList );
}


/**********************************************************/
bool WinEDA_SchematicFrame::GetSchematicFromRedoList()
/**********************************************************/

/* Redo the last edition:
 *  - Save the current schematic in undo list
 *  - Get the old version
 *  @return false if nothing done, else true
 */
{
    if( GetScreen()->GetRedoCommandCount() == 0 )
        return false;


    /* Get the old wrapper and put it in UndoList */
    PICKED_ITEMS_LIST* List = GetScreen()->PopCommandFromRedoList();
    GetScreen()->PushCommandToUndoList( List );

    /* Redo the command: */
    PutDataInPreviousState( List );

    CurrentDrawItem = NULL;
    GetScreen()->SetModify();
    SetSheetNumberAndCount();
    ReCreateHToolbar();
    SetToolbars();

    return true;
}


/***************************************************************************/
void WinEDA_SchematicFrame::PutDataInPreviousState( PICKED_ITEMS_LIST* aList )
/***************************************************************************/

/* Used in undo or redo command.
 *  Put data pointed by List in the previous state, i.e. the state memorised by List
 */
{
    SCH_ITEM* item;
    SCH_ITEM* alt_item;

    for( unsigned ii = 0; ii < aList->GetCount(); ii++  )
    {
        ITEM_PICKER itemWrapper = aList->GetItemWrapper( ii );
        item = (SCH_ITEM*) itemWrapper.m_Item;
        wxASSERT ( item  );
        SCH_ITEM*   image = (SCH_ITEM*) itemWrapper.m_Link;
        switch( itemWrapper.m_UndoRedoStatus )
        {
        case IS_CHANGED:    /* Exchange old and new data for each item */
            SwapData( item, image );
            break;

        case IS_NEW:        /* new items are deleted */
            aList->m_UndoRedoType = IS_DELETED;
            aList->SetItemStatus( IS_DELETED, ii );
            GetScreen()->RemoveFromDrawList( item );
            item->m_Flags = IS_DELETED;
            break;

        case IS_DELETED:    /* deleted items are put in EEdrawList, as new items */
            aList->m_UndoRedoType = IS_NEW;
            aList->SetItemStatus( IS_NEW, ii );
            item->SetNext( GetScreen()->EEDrawList );
            GetScreen()->EEDrawList = item;
            item->m_Flags = 0;
            break;

        case IS_WIRE_IMAGE:
            /* Exchange the current wires and the old wires */
            alt_item = GetScreen()->ExtractWires( false );
            aList->SetItem( alt_item, ii );
            while( item )
            {
                SCH_ITEM* nextitem = item->Next();
                item->SetNext( GetScreen()->EEDrawList );
                GetScreen()->EEDrawList = item;
                item->m_Flags = 0;
                item = nextitem;
            }

            break;

        default:
        {
            wxString msg;
            msg.Printf( wxT(
                            "PutDataInPreviousState() error (unknown code %X)" ),
                        itemWrapper.m_UndoRedoStatus );
            DisplayError( this, msg );
        }
        break;
        }
    }
}


/**********************************************************/
bool WinEDA_SchematicFrame::GetSchematicFromUndoList()
/**********************************************************/

/** Function GetSchematicFromUndoList
 *  Undo the last edition:
 *  - Save the current schematic in Redo list
 *  - Get an old version of the schematic
 *  @return false if nothing done, else true
 */
{
    if( GetScreen()->GetUndoCommandCount() <= 0 )
        return false;

    /* Get the old wrapper and put it in RedoList */
    PICKED_ITEMS_LIST* List = GetScreen()->PopCommandFromUndoList();
    GetScreen()->PushCommandToRedoList( List );
    /* Undo the command */
    PutDataInPreviousState( List );

    CurrentDrawItem = NULL;
    GetScreen()->SetModify();
    SetSheetNumberAndCount();
    ReCreateHToolbar();
    SetToolbars();

    return true;
}


/*********************************************************/
void SCH_SCREEN::ClearUndoORRedoList( UNDO_REDO_CONTAINER& aList, int aItemCount )
/*********************************************************/

/** Function ClearUndoORRedoList
 * free the undo or redo list from List element
 *  Wrappers are deleted.
 *  datas pointed by wrappers are deleted if not flagged IS_NEW
 *  because they are copy of used data or they are not in use (DELETED)
 * @param aList = the UNDO_REDO_CONTAINER to clear
 * @param aItemCount = the count of items to remove. < 0 for all items
 * items are removed from the beginning of the list.
 * So this function can be called to remove old commands
 */
{
    int CmdType;

    if( aItemCount == 0 )
        return;

    unsigned icnt = aList.m_CommandsList.size();
    if( aItemCount > 0 )
        icnt = aItemCount;
    for( unsigned ii = 0; ii < icnt; ii++ )
    {
        if( aList.m_CommandsList.size() == 0 )
            break;
        PICKED_ITEMS_LIST* curr_cmd = aList.m_CommandsList[0];
        aList.m_CommandsList.erase( aList.m_CommandsList.begin() );
        CmdType = curr_cmd->m_UndoRedoType;

        // Delete items is they are not flagged IS_NEW
        while( 1 )
        {
            ITEM_PICKER     wrapper = curr_cmd->PopItem();
            EDA_BaseStruct* item    = wrapper.m_Item;
            if( item == NULL ) // No more item in list.
                break;
            if( wrapper.m_UndoRedoStatus == IS_WIRE_IMAGE )
            {
                while( item )
                {
                    EDA_BaseStruct* nextitem = item->Next();
                    delete          item;
                    item = nextitem;
                }
            }
            else
            {
                if( (wrapper.m_UndoRedoStatus & IS_NEW) == 0 )
                {
                    delete item;
                }
            }
        }

        delete curr_cmd;    // Delete command
    }
}


/*****************************************/
void SCH_SCREEN::ClearUndoRedoList()
/*****************************************/

/* free the undo and the redo lists
 */
{
    ClearUndoORRedoList( m_UndoList );
    ClearUndoORRedoList( m_RedoList );
}


/***********************************************************/
void SCH_SCREEN::PushCommandToUndoList( PICKED_ITEMS_LIST* aItem )
/************************************************************/

/* Put newitem in head of undo list
 *  Deletes olds items if > count max.
 */
{
    m_UndoList.PushCommand( aItem );

    /* Delete the extra items, if count max reached */
    int extraitems = GetUndoCommandCount() - m_UndoRedoCountMax;
    if( extraitems > 0 ) // Delete the extra items
        ClearUndoORRedoList( m_UndoList, extraitems );
}


/***********************************************************/
void SCH_SCREEN::PushCommandToRedoList( PICKED_ITEMS_LIST* aItem )
/***********************************************************/
{
    m_RedoList.PushCommand( aItem );

    /* Delete the extra items, if count max reached */
    int extraitems = GetRedoCommandCount() - m_UndoRedoCountMax;
    if( extraitems > 0 ) // Delete the extra items
        ClearUndoORRedoList( m_RedoList, extraitems );
}
