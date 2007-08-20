/***************/
/* hotkeys.cpp */
/***************/

#include "fctsys.h"

#include "gr_basic.h"

#include "common.h"
#include "pcbnew.h"
#include "id.h"
#include "hotkeys_basic.h"

#include "protos.h"

/* Routines locales */

enum hotkey_id_commnand {
	HK_NOT_FOUND = 0,
	HK_RESET_LOCAL_COORD,
	HK_HELP,
	HK_ZOOM_IN,
	HK_ZOOM_OUT,
	HK_ZOOM_REDRAW,
	HK_ZOOM_CENTER,
	HK_DELETE,
	HK_BACK_SPACE,
	HK_ROTATE_FOOTPRINT,
	HK_MOVE_FOOTPRINT,
	HK_DRAG_FOOTPRINT,
	HK_FLIP_FOOTPRINT,
	HK_LOCK_UNLOCK_FOOTPRINT,
	HK_ADD_VIA, HK_END_TRACK,
	HK_SAVE_BOARD, HK_LOAD_BOARD,
	HK_SWITCH_UNITS, HK_SWITCH_TRACK_DISPLAY_MODE,
	HK_FIND_ITEM
};


/* local variables */
/* Hotkey list: */
static Ki_HotkeyInfo HkSavefile(wxT("Save board"), HK_SAVE_BOARD, 'S' + GR_KB_CTRL);
static Ki_HotkeyInfo HkLoadfile(wxT("Load board"), HK_LOAD_BOARD, 'L' + GR_KB_CTRL);
static Ki_HotkeyInfo HkFindItem(wxT("Find Item"), HK_FIND_ITEM, 'F' + GR_KB_CTRL);
static Ki_HotkeyInfo HkBackspace(wxT("Delete track segment"), HK_BACK_SPACE, WXK_BACK);
static Ki_HotkeyInfo HkAddVia(wxT("Add Via"), HK_ADD_VIA, 'V');
static Ki_HotkeyInfo HkEndTrack(wxT("End Track"), HK_END_TRACK, WXK_END);
static Ki_HotkeyInfo HkFlipFootprint(wxT("Flip Footprint"), HK_FLIP_FOOTPRINT, 'F');
static Ki_HotkeyInfo HkRotateFootprint(wxT("Rotate Footprint"), HK_ROTATE_FOOTPRINT, 'R');
static Ki_HotkeyInfo HkMoveFootprint(wxT("Move Footprint"), HK_MOVE_FOOTPRINT, 'M');
static Ki_HotkeyInfo HkDragFootprint(wxT("Drag Footprint"), HK_DRAG_FOOTPRINT, 'G');
static Ki_HotkeyInfo HkLock_Unlock_Footprint(wxT("Lock/Unlock Footprint"), HK_LOCK_UNLOCK_FOOTPRINT, 'L');
static Ki_HotkeyInfo HkDelete(wxT("Delete Track or Footprint"), HK_DELETE, WXK_DELETE);
static Ki_HotkeyInfo HkResetLocalCoord(wxT("Reset local coord."), HK_RESET_LOCAL_COORD, ' ');
static Ki_HotkeyInfo HkZoomCenter(wxT("Zoom Center"), HK_ZOOM_CENTER, WXK_F4);
static Ki_HotkeyInfo HkZoomRedraw(wxT("Zoom Redraw"), HK_ZOOM_REDRAW, WXK_F3);
static Ki_HotkeyInfo HkZoomOut(wxT("Zoom Out"), HK_ZOOM_OUT, WXK_F2);
static Ki_HotkeyInfo HkZoomIn(wxT("Zoom In"), HK_ZOOM_IN, WXK_F1);
static Ki_HotkeyInfo HkHelp(wxT("Help: this message"), HK_HELP, '?');
static Ki_HotkeyInfo HkSwitchUnits(wxT("Switch Units"), HK_SWITCH_UNITS, 'U');
static Ki_HotkeyInfo HkTrackDisplayMode(wxT("Track Display Mode"), HK_SWITCH_TRACK_DISPLAY_MODE, 'F');


// List of hotkey descriptors for pcbnew
static Ki_HotkeyInfo *s_board_edit_Hotkey_List[] = {
	&HkHelp,
	&HkZoomIn, &HkZoomOut, &HkZoomRedraw, &HkZoomCenter,
	&HkResetLocalCoord, &HkSwitchUnits, &HkTrackDisplayMode,
	&HkDelete, &HkBackspace,
	&HkAddVia, &HkEndTrack,
	&HkMoveFootprint, &HkFlipFootprint,
	&HkRotateFootprint, &HkDragFootprint,
	&HkLock_Unlock_Footprint,
	&HkSavefile, &HkLoadfile, &HkFindItem,
	NULL
};

static Ki_HotkeyInfo *s_module_edit_Hotkey_List[] = {
	&HkHelp,
	&HkZoomIn, &HkZoomOut, &HkZoomRedraw, &HkZoomCenter,
	&HkSwitchUnits, &HkResetLocalCoord,
	&HkDelete, &HkBackspace,
	NULL
};


/***********************************************************/
void WinEDA_PcbFrame::OnHotKey( wxDC* DC, int hotkey,
                                EDA_BaseStruct* DrawStruct )
/***********************************************************/
/* Hot keys. Some commands are relatives to the item under the mouse cursor
	Commands are case insensitive
	Zoom commands are not managed here
*/

{
	
    bool PopupOn = GetScreen()->GetCurItem()
                   && GetScreen()->GetCurItem()->m_Flags;
                   
    bool ItemFree = (GetScreen()->GetCurItem() == 0 )
                    || (GetScreen()->GetCurItem()->m_Flags == 0);

    if( hotkey == 0 )
        return;

    MODULE* module = NULL;

	// Remap the control key Ctrl A (0x01) to GR_KB_CTRL + 'A' (easier to handle...)
	if ( (hotkey & GR_KB_CTRL) != 0 ) hotkey += 'A' - 1;
	/* Convert lower to upper case (the usual toupper function has problem with non ascii codes like function keys */
	if( (hotkey >= 'a') && (hotkey <= 'z') ) hotkey += 'A' - 'a';
	
	int CommandCode = GetCommandCodeFromHotkey(hotkey, s_board_edit_Hotkey_List);
    switch( CommandCode )
    {
		default:
		case HK_NOT_FOUND:
			return;
			break;
		
		case HK_HELP:	// Display Current hotkey list
			DisplayHotkeyList(this, s_board_edit_Hotkey_List);
			break;

		case HK_ZOOM_IN:
		case HK_ZOOM_OUT:
		case HK_ZOOM_REDRAW:
		case HK_ZOOM_CENTER:
			break;

		case HK_RESET_LOCAL_COORD:     /*Reset the relative coord  */
			GetScreen()->m_O_Curseur = GetScreen()->m_Curseur;
			break;


		case HK_SWITCH_UNITS:
			g_UnitMetric = (g_UnitMetric == INCHES ) ? MILLIMETRE : INCHES;
			break;

		case HK_SWITCH_TRACK_DISPLAY_MODE:
			DisplayOpt.DisplayPcbTrackFill ^= 1; DisplayOpt.DisplayPcbTrackFill &= 1;
			GetScreen()->SetRefreshReq();
			break;

		case HK_DELETE:
			OnHotkeyDeleteItem( DC, DrawStruct );
			break;

		case HK_BACK_SPACE:
			if( m_ID_current_state == ID_TRACK_BUTT && GetScreen()->m_Active_Layer <= CMP_N )
			{
				bool ItemFree = (GetScreen()->GetCurItem() == NULL )
								|| (GetScreen()->GetCurItem()->m_Flags == 0);
				if( ItemFree )
				{
					// no track is currently being edited - select a segment and remove it.
					DrawStruct = PcbGeneralLocateAndDisplay();

					// don't let backspace delete modules!!
					if( DrawStruct && (DrawStruct->m_StructType == TYPETRACK
									   || DrawStruct->m_StructType == TYPEVIA) )
						Delete_Segment( DC, (TRACK*) DrawStruct );
					GetScreen()->SetModify();
				}
				else if( GetScreen()->GetCurItem()->m_StructType == TYPETRACK  )
				{
					// then an element is being edited - remove the last segment.
					GetScreen()->SetCurItem(Delete_Segment( DC, (TRACK*) GetScreen()->GetCurItem() ) );
					GetScreen()->SetModify();
				}
			}
			break;

		case HK_END_TRACK:
			DrawPanel->MouseToCursorSchema();
			End_Route( (TRACK*) (GetScreen()->GetCurItem()), DC );
			break;

		case HK_FIND_ITEM:
		{
			wxCommandEvent evt;
			evt.SetId( ID_FIND_ITEMS );
			Process_Special_Functions( evt );
		}
			break;

		case HK_LOAD_BOARD:
		{
			// try not to duplicate save, load code etc.
			wxCommandEvent evt;
			evt.SetId( ID_LOAD_FILE );
			Files_io( evt );
		}
			break;

		case HK_SAVE_BOARD:
		{
			// try not to duplicate save, load code etc.
			wxCommandEvent evt;
			evt.SetId( ID_SAVE_BOARD );
			Files_io( evt );
		}
			break;

		case HK_ADD_VIA:      // Switch to alternate layer and Place a via if a track is in progress
			if( m_ID_current_state != ID_TRACK_BUTT )
				return;
			if( ItemFree )
			{
				Other_Layer_Route( NULL, DC );
				break;
			}
			if( GetScreen()->GetCurItem()->m_StructType != TYPETRACK )
				return;
			if( (GetScreen()->GetCurItem()->m_Flags & IS_NEW) == 0 )
				return;
			Other_Layer_Route( (TRACK*) GetScreen()->GetCurItem(), DC );
			if( DisplayOpt.ContrastModeDisplay )
				GetScreen()->SetRefreshReq();
			break;

			// Footprint edition:
		case HK_LOCK_UNLOCK_FOOTPRINT:      // toggle module "MODULE_is_LOCKED" status:
			// get any module, locked or not locked and toggle its locked status
			if( ItemFree )
				module = Locate_Prefered_Module( m_Pcb, CURSEUR_OFF_GRILLE | VISIBLE_ONLY );
			else if( GetScreen()->GetCurItem()->m_StructType == TYPEMODULE )
				module = (MODULE*) GetScreen()->GetCurItem();
			if( module )
			{
				GetScreen()->SetCurItem(module);
				module->SetLocked( !module->IsLocked() );
				module->Display_Infos( this );
			}
			break;

		case HK_DRAG_FOOTPRINT:       // Start move (and drag) module
		case HK_MOVE_FOOTPRINT:       // Start move module
			if( PopupOn )
				break;

		case HK_ROTATE_FOOTPRINT:       // Rotation
		case HK_FLIP_FOOTPRINT:       // move to other side
			if( ItemFree )
			{
				module = Locate_Prefered_Module( m_Pcb,
												 CURSEUR_OFF_GRILLE | IGNORE_LOCKED | VISIBLE_ONLY
	#if defined (USE_MATCH_LAYER)
												 | MATCH_LAYER
	#endif
						 );
				if( module == NULL )      // no footprint found
				{
					module = Locate_Prefered_Module( m_Pcb, CURSEUR_OFF_GRILLE );
					if( module )      
					{
						// a footprint is found, but locked or on an other layer
						if( module->IsLocked() )
						{
							wxString msg;
							
							msg.Printf( _("Footprint %s found, but locked"),
								module->m_Reference->m_Text.GetData() );
							
							DisplayInfo( this, msg );
						}
						module = NULL;
					}
				}
			}
			else if( GetScreen()->GetCurItem()->m_StructType == TYPEMODULE )
			{
				module = (MODULE*) GetScreen()->GetCurItem();

				// @todo: might need to add a layer check in if() below
				if( (GetScreen()->GetCurItem()->m_Flags == 0)
				   && module->IsLocked() )
					module = NULL; // do not move, rotate ... it.
			}
			if( module == NULL )
				break;

			/*  I'd like to make sending to EESCHEMA edge triggered, but the
				simple mouse click on a module when the arrow icon is in play
				does not set GetCurItem() at this time, nor does a mouse click
				when the local ratsnest icon is in play set GetCurItem(), and these
				actions also call SendMessageToEESCHEMA().
			if( GetScreen()->GetCurItem() != module )
				*/
			{
				// Send the module via socket to EESCHEMA's search facility.
				SendMessageToEESCHEMA( module );
				
				GetScreen()->SetCurItem(module);
			}

			switch( CommandCode )
			{
				case HK_ROTATE_FOOTPRINT:          // Rotation
					Rotate_Module( DC, module, 900, TRUE );
					break;

				case HK_FLIP_FOOTPRINT:          // move to other side
					Change_Side_Module( module, DC );
					break;

				case HK_DRAG_FOOTPRINT:          // Start move (and drag) module
					g_Drag_Pistes_On = TRUE;
					// fall through
				case HK_MOVE_FOOTPRINT:          // Start move module
					StartMove_Module( module, DC );
					break;
			}
			module->Display_Infos( this );
			break;
    }
}


/***********************************************************/
void WinEDA_ModuleEditFrame::OnHotKey( wxDC* DC, int hotkey,
                                       EDA_BaseStruct* DrawStruct )
/***********************************************************/

/* Gestion des commandes rapides (Raccourcis claviers) concernant l'element
 *  sous le courseur souris
 *  Les majuscules/minuscules sont indifferenciees
 */
{
    if( hotkey == 0 )
        return;

	/* Convert lower to upper case (the usual toupper function has problem with non ascii codes like function keys */
	if( (hotkey >= 'a') && (hotkey <= 'z') ) hotkey += 'A' - 'a';

	int CommandCode = GetCommandCodeFromHotkey(hotkey, s_module_edit_Hotkey_List);
    switch( CommandCode )
    {
		default:
		case HK_NOT_FOUND:
			return;
			break;
		
		case HK_HELP:	// Display Current hotkey list
			DisplayHotkeyList(this, s_module_edit_Hotkey_List);
			break;

		case HK_RESET_LOCAL_COORD:     /*Reset the relative coord  */
			GetScreen()->m_O_Curseur = GetScreen()->m_Curseur;
			break;


		case HK_SWITCH_UNITS:
			g_UnitMetric = (g_UnitMetric == INCHES ) ? MILLIMETRE : INCHES;
			break;

		case HK_ZOOM_IN:
		case HK_ZOOM_OUT:
		case HK_ZOOM_REDRAW:
		case HK_ZOOM_CENTER:
			break;
    }
}


/******************************************************************************/
bool WinEDA_PcbFrame::OnHotkeyDeleteItem( wxDC* DC, EDA_BaseStruct* DrawStruct )
/******************************************************************************/

/* Efface l'item pointe par la souris, en reponse a la touche "Del"
 *  Effet dependant de l'outil selectionne:
 *      Outil trace de pistes
 *          Efface le segment en cours ou la piste si pas d'element
 *      Outil module:
 *          Efface le module.
 */
{
    bool ItemFree = (GetScreen()->GetCurItem() == NULL )
                    || (GetScreen()->GetCurItem()->m_Flags == 0);

    switch( m_ID_current_state )
    {
    case ID_TRACK_BUTT:
        if( GetScreen()->m_Active_Layer > CMP_N )
            return FALSE;
        if( ItemFree )
        {
            DrawStruct = PcbGeneralLocateAndDisplay();
            if( DrawStruct && DrawStruct->m_StructType != TYPETRACK )
                return FALSE;
            Delete_Track( DC, (TRACK*) DrawStruct );
        }
        else if( GetScreen()->GetCurItem()->m_StructType == TYPETRACK  )
        {
            GetScreen()->SetCurItem(
                Delete_Segment( DC, (TRACK*) GetScreen()->GetCurItem() ) );
            GetScreen()->SetModify();
            return TRUE;
        }
        break;

    case ID_COMPONENT_BUTT:
        if( ItemFree )
        {
            MODULE* module = Locate_Prefered_Module( m_Pcb, CURSEUR_ON_GRILLE );
            if( module == NULL )
                return FALSE;
            if( !IsOK( this, _( "Delete module?" ) ) )
                return FALSE;
            RemoveStruct( module, DC );
        }
        else
            return FALSE;
        break;

    default:
        return FALSE;
    }

    GetScreen()->SetModify();
    GetScreen()->SetCurItem(NULL);
    return TRUE;
}
