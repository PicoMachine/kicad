/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2016 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * https://www.gnu.org/licenses/gpl-3.0.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef __sim_plot_frame__
#define __sim_plot_frame__

/**
@file Subclass of SIM_PLOT_FRAME_BASE, which is generated by wxFormBuilder. */

#include "sim_plot_frame_base.h"
#include "sim_types.h"

#include <kiway_player.h>
#include <dialogs/dialog_sim_settings.h>

#include <wx/event.h>

#include <list>
#include <memory>
#include <map>

class SCH_EDIT_FRAME;
class SCH_COMPONENT;

class SPICE_SIMULATOR;
class NETLIST_EXPORTER_PSPICE_SIM;
class SIM_PLOT_PANEL;
class TUNER_SLIDER;

/// @todo description
class TRACE_DESC
{
public:
    TRACE_DESC( const NETLIST_EXPORTER_PSPICE_SIM& aExporter, const wxString& aName,
            SIM_PLOT_TYPE aType, const wxString& aParam );

    ///> Modifies an existing TRACE_DESC simulation type
    TRACE_DESC( const NETLIST_EXPORTER_PSPICE_SIM& aExporter,
            const TRACE_DESC& aDescription, SIM_PLOT_TYPE aNewType )
        : TRACE_DESC( aExporter, aDescription.GetName(), aNewType, aDescription.GetParam() )
    {
    }

    const wxString& GetTitle() const
    {
        return m_title;
    }

    const wxString& GetName() const
    {
        return m_name;
    }

    const wxString& GetParam() const
    {
        return m_param;
    }

    SIM_PLOT_TYPE GetType() const
    {
        return m_type;
    }

    const wxString& GetSpiceVector() const
    {
        return m_spiceVector;
    }

private:
    // Three basic parameters
    ///> Name of the measured net/device
    wxString m_name;

    ///> Type of the signal
    SIM_PLOT_TYPE m_type;

    ///> Name of the signal parameter
    wxString m_param;

    // Generated data
    ///> Title displayed in the signal list/plot legend
    wxString m_title;

    ///> Spice vector name
    wxString m_spiceVector;
};

/** Implementing SIM_PLOT_FRAME_BASE */
class SIM_PLOT_FRAME : public SIM_PLOT_FRAME_BASE
{
    public:
        /** Constructor */
        SIM_PLOT_FRAME( KIWAY* aKiway, wxWindow* aParent );
        ~SIM_PLOT_FRAME();

        void StartSimulation();
        void StopSimulation();
        bool IsSimulationRunning();

        /**
         * @brief Creates a new plot panel for a given simulation type and adds it to the main
         * notebook.
         * @param aSimType is requested simulation type.
         * @return The new plot panel.
         */
        SIM_PLOT_PANEL* NewPlotPanel( SIM_TYPE aSimType );

        void AddVoltagePlot( const wxString& aNetName );
        void AddCurrentPlot( const wxString& aDeviceName, const wxString& aParam );

        void AddTuner( SCH_COMPONENT* aComponent );
        void RemoveTuner( TUNER_SLIDER* aTuner );

        SIM_PLOT_PANEL* CurrentPlot() const;

    private:
        /**
         * @brief Adds a new plot to the current panel.
         * @param aName is the device/net name.
         * @param aType describes the type of plot.
         * @param aParam is the parameter for the device/net (e.g. I, Id, V).
         */
        void addPlot( const wxString& aName, SIM_PLOT_TYPE aType, const wxString& aParam );

        /**
         * @brief Removes a plot with a specific title.
         * @param aPlotName is the full plot title (e.g. I(Net-C1-Pad1)).
         */
        void removePlot( const wxString& aPlotName );

        void updateNetlistExporter();

        /**
         * @brief Updates plot in a particular SIM_PLOT_PANEL. If the panel does not contain
         * the plot, it will be added.
         * @param aDescriptor contains the plot description.
         * @param aPanel is the panel that should receive the update.
         * @return True if a plot was successfully added/updated.
         */
        bool updatePlot( const TRACE_DESC& aDescriptor, SIM_PLOT_PANEL* aPanel );

        /**
         * @brief Updates the list of currently plotted signals.
         */
        void updateSignalList();

        /**
         * @brief Fills the tuners area with the ones related to the current plot.
         */
        void updateTuners();

        /**
         * @brief Updates the cursor values list.
         */
        void updateCursors();

        SIM_PLOT_TYPE GetXAxisType( SIM_TYPE aType ) const;

        // Menu handlers
        void menuNewPlot( wxCommandEvent& aEvent ) override;

        void menuExit( wxCommandEvent& event ) override
        {
            Close();
        }

        void menuSaveImage( wxCommandEvent& event ) override;
        void menuSaveCsv( wxCommandEvent& event ) override;
        void menuZoomIn( wxCommandEvent& event ) override;
        void menuZoomOut( wxCommandEvent& event ) override;
        void menuZoomFit( wxCommandEvent& event ) override;
        void menuShowGrid( wxCommandEvent& event ) override;
        void menuShowGridUpdate( wxUpdateUIEvent& event ) override;
        void menuShowLegend( wxCommandEvent& event ) override;
        void menuShowLegendUpdate( wxUpdateUIEvent& event ) override;
        void menuShowCoords( wxCommandEvent& event ) override;
        void menuShowCoordsUpdate( wxUpdateUIEvent& event ) override;

        // Event handlers
        void onPlotChanged( wxNotebookEvent& event ) override;

        void onSignalDblClick( wxCommandEvent& event ) override;
        void onSignalRClick( wxMouseEvent& event ) override;

        void onSimulate( wxCommandEvent& event ) override;
        void onSettings( wxCommandEvent& event ) override;
        void onAddSignal( wxCommandEvent& event ) override;
        void onProbe( wxCommandEvent& event ) override;
        void onTune( wxCommandEvent& event ) override;

        void onClose( wxCloseEvent& aEvent );

        void onCursorUpdate( wxCommandEvent& aEvent );
        void onSimUpdate( wxCommandEvent& aEvent );
        void onSimReport( wxCommandEvent& aEvent );
        void onSimStarted( wxCommandEvent& aEvent );
        void onSimFinished( wxCommandEvent& aEvent );

        SCH_EDIT_FRAME* m_schematicFrame;
        std::unique_ptr<NETLIST_EXPORTER_PSPICE_SIM> m_exporter;
        std::unique_ptr<SPICE_SIMULATOR> m_simulator;

        typedef std::map<wxString, TRACE_DESC> TRACE_MAP;
        typedef std::list<TUNER_SLIDER*> TUNER_LIST;

        struct PLOT_INFO
        {
            TUNER_LIST m_tuners;
            TRACE_MAP m_traces;
        };

        std::map<SIM_PLOT_PANEL*, PLOT_INFO> m_plots;

        // Trick to preserve settings between runs
        DIALOG_SIM_SETTINGS m_settingsDlg;

        // Right click context menu for signals in the listbox
        class SIGNAL_CONTEXT_MENU : public wxMenu
        {
            public:
                SIGNAL_CONTEXT_MENU( const wxString& aSignal, SIM_PLOT_FRAME* aPlotFrame );

            private:
                void onMenuEvent( wxMenuEvent& aEvent );

                const wxString& m_signal;
                SIM_PLOT_FRAME* m_plotFrame;

                enum SIGNAL_CONTEXT_MENU_EVENTS
                {
                    HIDE_SIGNAL,
                    SHOW_CURSOR,
                    HIDE_CURSOR
                };
        };

};

// Commands
wxDECLARE_EVENT( EVT_SIM_UPDATE, wxCommandEvent );
wxDECLARE_EVENT( EVT_SIM_REPORT, wxCommandEvent );

// Notifications
wxDECLARE_EVENT( EVT_SIM_STARTED, wxCommandEvent );
wxDECLARE_EVENT( EVT_SIM_FINISHED, wxCommandEvent );

#endif // __sim_plot_frame__
