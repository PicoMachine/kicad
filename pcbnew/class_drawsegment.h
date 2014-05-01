/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
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

/**
 * @file class_drawsegment.h
 * @brief Class to handle a graphic segment.
 */

#ifndef CLASS_DRAWSEGMENT_H_
#define CLASS_DRAWSEGMENT_H_

#include <class_board_item.h>
#include <PolyLine.h>
#include <math_for_graphics.h>
#include <trigo.h>
#include <common.h>


class LINE_READER;
class EDA_DRAW_FRAME;
class MODULE;
class MSG_PANEL_ITEM;


class DRAWSEGMENT : public BOARD_ITEM
{
protected:
    int         m_Width;        ///< thickness of lines ...
    wxPoint     m_Start;        ///< Line start point or Circle and Arc center
    wxPoint     m_End;          ///< Line end point or circle and arc start point

    STROKE_T    m_Shape;        ///< Shape: line, Circle, Arc
    int         m_Type;         ///< Used in complex associations ( Dimensions.. )
    double      m_Angle;        ///< Used only for Arcs: Arc angle in 1/10 deg
    wxPoint     m_BezierC1;     ///< Bezier Control Point 1
    wxPoint     m_BezierC2;     ///< Bezier Control Point 2

    std::vector<wxPoint>    m_BezierPoints;
    std::vector<wxPoint>    m_PolyPoints;

public:
    DRAWSEGMENT( BOARD_ITEM* aParent = NULL, KICAD_T idtype = PCB_LINE_T );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~DRAWSEGMENT();

    /// skip the linked list stuff, and parent
    const DRAWSEGMENT& operator = ( const DRAWSEGMENT& rhs );

    void SetWidth( int aWidth )             { m_Width = aWidth; }
    int GetWidth() const                    { return m_Width; }

    /**
     * Function SetAngle
     * sets the angle for arcs, and normalizes it within the range 0 - 360 degrees.
     * @param aAngle is tenths of degrees, but will soon be degrees.
     */
    void SetAngle( double aAngle );     // encapsulates the transition to degrees
    double GetAngle() const { return m_Angle; }

    void SetType( int aType )                       { m_Type = aType; }
    int GetType() const                             { return m_Type; }

    void SetShape( STROKE_T aShape )                { m_Shape = aShape; }
    STROKE_T GetShape() const                       { return m_Shape; }

    void SetBezControl1( const wxPoint& aPoint )    { m_BezierC1 = aPoint; }
    const wxPoint& GetBezControl1() const           { return m_BezierC1; }

    void SetBezControl2( const wxPoint& aPoint )    { m_BezierC2 = aPoint; }
    const wxPoint& GetBezControl2() const           { return m_BezierC2; }

    void SetPosition( const wxPoint& aPos )         { m_Start = aPos; }     // override
    const wxPoint& GetPosition() const              { return m_Start; }     // override

    /**
     * Function GetStart
     * returns the starting point of the graphic
     */
    const wxPoint& GetStart() const         { return m_Start; }
    void SetStart( const wxPoint& aStart )  { m_Start = aStart; }
    void SetStartY( int y )                 { m_Start.y = y; }
    void SetStartX( int x )                 { m_Start.x = x; }

    /**
     * Function GetEnd
     * returns the ending point of the graphic
     */
    const wxPoint& GetEnd() const           { return m_End; }
    void SetEnd( const wxPoint& aEnd )      { m_End = aEnd; }
    void SetEndY( int y )                   { m_End.y = y; }
    void SetEndX( int x )                   { m_End.x = x; }

    // Some attributes are read only, since they are "calculated" from
    // m_Start, m_End, and m_Angle.
    // No Set...() function for these attributes.

    const wxPoint& GetCenter() const        { return m_Start; }
    const wxPoint& GetArcStart() const      { return m_End; }
    const wxPoint GetArcEnd() const;

    /**
     * function GetArcAngleStart()
     * @return the angle of the starting point of this arc, between 0 and 3600 in 0.1 deg
     */
    double GetArcAngleStart() const;

    /**
     * Function GetRadius
     * returns the radius of this item
     * Has meaning only for arc and circle
     */
    int GetRadius() const
    {
        double radius = GetLineLength( m_Start, m_End );
        return KiROUND( radius );
    }

    /**
     * Initialize the start arc point. can be used for circles
     * to initialize one point of the cicumference
     */
    void SetArcStart( const wxPoint& aArcStartPoint )
    { m_End = aArcStartPoint; }

    /** For arcs and circles:
     */
    void SetCenter( const wxPoint& aCenterPoint ) { m_Start = aCenterPoint; }

    /**
     * Function GetParentModule
     * returns a pointer to the parent module, or NULL if DRAWSEGMENT does not
     * belong to a module.
     * @return MODULE* - pointer to the parent module or NULL.
     */
    MODULE* GetParentModule() const;

    const std::vector<wxPoint>& GetBezierPoints() const { return m_BezierPoints; };
    const std::vector<wxPoint>& GetPolyPoints() const   { return m_PolyPoints; };

    void SetBezierPoints( const std::vector<wxPoint>& aPoints )
    {
        m_BezierPoints = aPoints;
    }

    void SetPolyPoints( const std::vector<wxPoint>& aPoints )
    {
        m_PolyPoints = aPoints;
    }

    void Copy( DRAWSEGMENT* source );

    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
               GR_DRAWMODE aDrawMode, const wxPoint& aOffset = ZeroOffset );

    virtual void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList );

    virtual const EDA_RECT GetBoundingBox() const;

    virtual bool HitTest( const wxPoint& aPosition );

    /** @copydoc BOARD_ITEM::HitTest(const EDA_RECT& aRect,
     *                               bool aContained = true, int aAccuracy ) const
     */
    bool HitTest( const EDA_RECT& aRect, bool aContained = true, int aAccuracy = 0 ) const;

    wxString GetClass() const
    {
        return wxT( "DRAWSEGMENT" );
    }

    /**
     * Function GetLength
     * returns the length of the track using the hypotenuse calculation.
     * @return double - the length of the track
     */
    double  GetLength() const
    {
        return GetLineLength( GetStart(), GetEnd() );
    }

    virtual void Move( const wxPoint& aMoveVector )
    {
        m_Start += aMoveVector;
        m_End   += aMoveVector;
    }

    virtual void Rotate( const wxPoint& aRotCentre, double aAngle );

    virtual void Flip( const wxPoint& aCentre );

    /**
     * Function TransformShapeWithClearanceToPolygon
     * Convert the track shape to a closed polygon
     * Used in filling zones calculations
     * Circles and arcs are approximated by segments
     * @param aCornerBuffer = a buffer to store the polygon
     * @param aClearanceValue = the clearance around the pad
     * @param aCircleToSegmentsCount = the number of segments to approximate a circle
     * @param aCorrectionFactor = the correction to apply to circles radius to keep
     * clearance when the circle is approximated by segment bigger or equal
     * to the real clearance value (usually near from 1.0)
     */
    void TransformShapeWithClearanceToPolygon( CPOLYGONS_LIST& aCornerBuffer,
                                               int             aClearanceValue,
                                               int             aCircleToSegmentsCount,
                                               double          aCorrectionFactor ) const;

    virtual wxString GetSelectMenuText() const;

    virtual BITMAP_DEF GetMenuImage() const { return  add_dashed_line_xpm; }

    virtual EDA_ITEM* Clone() const;

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const { ShowDummy( os ); } // override
#endif
};

#endif  // CLASS_DRAWSEGMENT_H_
