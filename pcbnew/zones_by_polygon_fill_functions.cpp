/////////////////////////////////////////////////////////////////////////////
// Name:        zones_by_polygon_fill_functions.cpp
/////////////////////////////////////////////////////////////////////////////
/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2009 Jean-Pierre Charras <jean-pierre.charras@gipsa-lab.inpg.fr>
 * Copyright (C) 2007 Kicad Developers, see change_log.txt for contributors.
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

#include <wx/progdlg.h>
#include "fctsys.h"
#include "appl_wxstruct.h"
#include "common.h"
#include "class_drawpanel.h"
#include "pcbnew.h"
#include "wxPcbStruct.h"
#include "zones.h"


/**
 * Function Delete_Zone_Fill
 * Remove the zone fillig which include the segment aZone, or the zone which have the given time stamp.
 * A zone is a group of segments which have the same TimeStamp
 * @param aZone = zone segment within the zone to delete. Can be NULL
 * @param aTimestamp = Timestamp for the zone to delete, used if aZone == NULL
 */
void WinEDA_PcbFrame::Delete_Zone_Fill( SEGZONE* aZone, long aTimestamp )
{
    bool          modify  = false;
    unsigned long TimeStamp;

    if( aZone == NULL )
        TimeStamp = aTimestamp;
    else
        TimeStamp = aZone->m_TimeStamp; // Save reference time stamp (aZone will be deleted)

    SEGZONE* next;
    for( SEGZONE* zone = GetBoard()->m_Zone; zone != NULL; zone = next )
    {
        next = zone->Next();
        if( zone->m_TimeStamp == TimeStamp )
        {
            modify = TRUE;
            /* remove item from linked list and free memory */
            zone->DeleteStructure();
        }
    }

    // Now delete the outlines of the corresponding copper areas (deprecated)
    for( int ii = 0; ii < GetBoard()->GetAreaCount(); ii++ )
    {
        ZONE_CONTAINER* zone = GetBoard()->GetArea( ii );
        if( zone->m_TimeStamp == TimeStamp )
        {
            modify = TRUE;
            zone->m_FilledPolysList.clear();
            zone->m_FillSegmList.clear();
            zone->m_IsFilled = false;
        }
    }

    if( modify )
    {
        OnModify();
        DrawPanel->Refresh();
    }
}


/***************************************************************************************/
int WinEDA_PcbFrame::Fill_Zone( ZONE_CONTAINER* zone_container, bool verbose )
/***************************************************************************************/

/**
 * Function Fill_Zone
 *  Calculate the zone filling for the outline zone_container
 *  The zone outline is a frontier, and can be complex (with holes)
 *  The filling starts from starting points like pads, tracks.
 * If exists, the old filling is removed
 * @param zone_container = zone to fill
 * @param verbose = true to show error messages
 * @return error level (0 = no error)
 */
{
    wxString msg;

    MsgPanel->EraseMsgBox();

    if( GetBoard()->ComputeBoundingBox() == false )
    {
        if( verbose )
            wxMessageBox( wxT( "Board is empty!" ) );
        return -1;
    }

    // Shows the net
    g_Zone_Default_Setting.m_NetcodeSelection = zone_container->GetNet();
    msg = zone_container->GetNetName();

    if( msg.IsEmpty() )
        msg = wxT( "No net" );
    Affiche_1_Parametre( this, 22, _( "NetName" ), msg, RED );

    wxBusyCursor dummy;     // Shows an hourglass cursor (removed by its destructor)

    zone_container->m_FilledPolysList.clear();
    Delete_Zone_Fill( NULL, zone_container->m_TimeStamp );
    zone_container->BuildFilledPolysListData( GetBoard() );

    OnModify();

    return 0;
}


int WinEDA_PcbFrame::Fill_All_Zones( bool verbose )
/**
 * Function Fill_All_Zones
 *  Fill all zones on the board
 * The old fillings are removed
 * @param verbose = true to show error messages
 * @return error level (0 = no error)
 */
{
    int errorLevel = 0;
    int areaCount = GetBoard()->GetAreaCount();
    wxBusyCursor dummyCursor;

    wxProgressDialog progressDialog( wxT( "Fill All Zones" ),
                                     wxT( "Starting zone fill..." ),
                                     areaCount+2, this,
                                     wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT );
    progressDialog.SetMinSize( wxSize( 400, 100 ) );

    // Remove segment zones
    GetBoard()->m_Zone.DeleteAll();

    int ii;
    for( ii = 0; ii < areaCount; ii++ )
    {
        ZONE_CONTAINER* zoneContainer = GetBoard()->GetArea( ii );
        wxString str;
        str.Printf( wxT( "Filling zone %d out of %d (net %s)..." ),
                    ii+1, areaCount, GetChars( zoneContainer->GetNetName() ) );

        if( !progressDialog.Update( ii+1, str ) )
            break;

        errorLevel = Fill_Zone( zoneContainer, verbose );

        if( errorLevel && !verbose )
            break;
    }
    progressDialog.Update( ii+2, wxT( "Updating ratsnest..." ) );
    test_connexions( NULL );

    // Recalculate the active ratsnest, i.e. the unconnected links
    Tst_Ratsnest( NULL, 0 );
    DrawPanel->Refresh( true );

    return errorLevel;
}


