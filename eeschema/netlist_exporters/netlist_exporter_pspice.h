/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2013 jp.charras at wanadoo.fr
 * Copyright (C) 2013 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 1992-2015 KiCad Developers
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

#ifndef NETLIST_EXPORTER_PSPICE_H
#define NETLIST_EXPORTER_PSPICE_H

#include "netlist_exporter.h"
#include <map>

class SEARCH_STACK;

/**
 * Class NETLIST_EXPORTER_PSPICE
 * generates a PSPICE compatible netlist
 */
class NETLIST_EXPORTER_PSPICE : public NETLIST_EXPORTER
{
public:
    NETLIST_EXPORTER_PSPICE( NETLIST_OBJECT_LIST* aMasterList, PART_LIBS* aLibs, SEARCH_STACK* aPaths = NULL ) :
        NETLIST_EXPORTER( aMasterList, aLibs ), m_paths( aPaths )
    {
    }

    typedef std::map<wxString, int> NET_INDEX_MAP;
    typedef std::vector<wxString> PROBE_LIST;

    /**
     * Function WriteNetlist
     * writes to specified output file
     */
    bool WriteNetlist( const wxString& aOutFileName, unsigned aNetlistOptions );

    bool Format( OUTPUTFORMATTER* aOutputFormatter, int aCtl );

    const NET_INDEX_MAP& GetNetIndexMap() const
    {
        return m_netMap;
    }

    const PROBE_LIST& GetProbeList() const
    {
        return m_probes;
    }

private:
    NET_INDEX_MAP m_netMap;
    PROBE_LIST m_probes;
    SEARCH_STACK* m_paths;
};

#endif
