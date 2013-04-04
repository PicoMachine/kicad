/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
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

#include <base_struct.h>
#include <layers_id_colors_and_visibility.h>

#include <view/view.h>
#include <view/view_rtree.h>
#include <gal/definitions.h>
#include <gal/graphics_abstraction_layer.h>
#include <painter.h>

#include <profile.h>

using namespace KiGfx;

const unsigned int VIEW::VIEW_MAX_LAYERS = 64;

void VIEW::AddLayer( int aLayer, bool aDisplayOnly )
{
    if( m_layers.find( aLayer ) == m_layers.end() )
    {
        m_layers[aLayer] = VIEW_LAYER();
        m_layers[aLayer].id     = aLayer;
        m_layers[aLayer].items  = new VIEW_RTREE();
        m_layers[aLayer].renderingOrder = aLayer;
        m_layers[aLayer].enabled        = true;
        m_layers[aLayer].isDirty        = false;
        m_layers[aLayer].displayOnly    = aDisplayOnly;
    }

    sortLayers();
}


void VIEW::Add( VIEW_ITEM* aItem )
{
    int layers[VIEW_MAX_LAYERS], layers_count;

    aItem->ViewGetLayers( layers, layers_count );

    for( int i = 0; i < layers_count; i++ )
    {
        VIEW_LAYER* l = &m_layers[layers[i]];
        l->items->Insert( aItem );
        l->dirtyExtents.Merge( aItem->ViewBBox() );
    }

    if( m_dynamic )
        aItem->viewAssign( this );
}


void VIEW::Remove( VIEW_ITEM* aItem )
{
    if( m_dynamic )
        aItem->m_view = NULL;

// fixme: this is so sloooow!
    for( LayerMapIter i = m_layers.begin(); i != m_layers.end(); ++i )
    {
        VIEW_LAYER* l = & ( ( *i ).second );
        l->items->Remove( aItem );
    }
}


// stupid C++... python lamda would do this in one line
template <class Container>
struct queryVisitor
{
    typedef typename Container::value_type item_type;

    queryVisitor( Container& aCont, int aLayer ) :
        m_cont( aCont ), m_layer( aLayer )
    {
    }

    void operator()( VIEW_ITEM* aItem )
    {
        if( aItem->ViewIsVisible() )
            m_cont.push_back( VIEW::LayerItemPair( aItem, m_layer ) );
    }

    Container&  m_cont;
    int         m_layer;
};


int VIEW::Query( const BOX2I& aRect, std::vector<LayerItemPair>& aResult )
{
    if( m_orderedLayers.empty() )
        return 0;

    std::vector<VIEW_LAYER*>::reverse_iterator i;

    // execute queries in reverse direction, so that items that are on the top of
    // the rendering stack are returned first.
    for( i = m_orderedLayers.rbegin(); i != m_orderedLayers.rend(); ++i )
    {
        // ignore layers that do not contain actual items (i.e. the selection box, menus, floats)
        if( ( *i )->displayOnly )
            continue;

        queryVisitor<std::vector<LayerItemPair> > visitor( aResult, ( *i )->id );
        ( *i )->items->Query( aRect, visitor );
    }

    return aResult.size();
}


VIEW::VIEW( bool aIsDynamic, bool aUseGroups ) :
    m_scale ( 1.0 ),
    m_painter( NULL ),
    m_gal( NULL ),
    m_dynamic( aIsDynamic ),
    m_useGroups( aUseGroups )
{
}


VIEW::~VIEW()
{
}


VECTOR2D VIEW::ToWorld( const VECTOR2D& aCoord, bool aAbsolute ) const
{
    MATRIX3x3D matrix = m_gal->GetWorldScreenMatrix().Inverse();

    if( aAbsolute )
    {
        return VECTOR2D( matrix * aCoord );
    }
    else
    {
        return VECTOR2D( matrix.GetScale().x * aCoord.x, matrix.GetScale().y * aCoord.y );
    }
}


VECTOR2D VIEW::ToScreen( const VECTOR2D& aCoord, bool aAbsolute ) const
{
    MATRIX3x3D matrix = m_gal->GetWorldScreenMatrix();

    if( aAbsolute )
    {
        return VECTOR2D( matrix * aCoord );
    }
    else
    {
        return VECTOR2D( matrix.GetScale().x * aCoord.x, matrix.GetScale().y * aCoord.y );
    }
}


double VIEW::ToScreen( double aCoord, bool aAbsolute ) const
{
    VECTOR2D t( aCoord, 0 );

    return ToScreen( t, aAbsolute ).x;
}


void VIEW::CopySettings( const VIEW* aOtherView )
{
    // FIXME
}


void VIEW::SetGAL( GAL* aGal )
{
    m_gal = aGal;

    // force the new GAL to display the current viewport.
    SetCenter( m_center );
    SetScale( m_scale );
}


void VIEW::SetPainter( PAINTER* aPainter )
{
    m_painter = aPainter;
}


BOX2D VIEW::GetViewport() const
{
    BOX2D    rect;
    VECTOR2D screenSize = m_gal->GetScreenPixelSize();

    rect.SetOrigin( ToWorld( VECTOR2D( 0, 0 ) ) );
    rect.SetEnd( ToWorld( screenSize ) );

    return rect.Normalize();
}


void VIEW::SetViewport( const BOX2D& aViewport, bool aKeepAspect )
{
    VECTOR2D ssize  = ToWorld( m_gal->GetScreenPixelSize(), false );
    VECTOR2D centre = aViewport.Centre();
    VECTOR2D vsize  = aViewport.GetSize();
    double   zoom   = 1.0 / std::min( fabs( vsize.x / ssize.x ), fabs( vsize.y / ssize.y ) );

    SetCenter( centre );
    SetScale( GetScale() * zoom );
}


void VIEW::SetMirror( bool aMirrorX, bool aMirrorY )
{
    // FIXME
}


void VIEW::SetScale( double aScale )
{
    SetScale( aScale, m_center );
}


void VIEW::SetScale( double aScale, const VECTOR2D& aAnchor )
{
    VECTOR2D a = ToScreen( aAnchor );

    m_gal->SetZoomFactor( aScale );
    m_gal->ComputeWorldScreenMatrix();

    VECTOR2D delta = ToWorld( a ) - aAnchor;

    SetCenter( m_center - delta );
    m_scale = aScale;
}


void VIEW::SetCenter( const VECTOR2D& aCenter )
{
    m_center = aCenter;
    m_gal->SetLookAtPoint( m_center );
    m_gal->ComputeWorldScreenMatrix();
}


void VIEW::SetLayerVisible( int aLayer, bool aVisible )
{
    m_layers[aLayer].enabled = aVisible;
}


void VIEW::sortLayers()
{
    int n = 0;

    m_orderedLayers.resize( m_layers.size() );

    for( LayerMapIter i = m_layers.begin(); i != m_layers.end(); ++i )
        m_orderedLayers[n++] = &i->second;

    sort( m_orderedLayers.begin(), m_orderedLayers.end(), compareRenderingOrder );
}


void VIEW::SetLayerOrder( int aLayer, int aRenderingOrder )
{
    m_layers[aLayer].renderingOrder = aRenderingOrder;
    sortLayers();
}


struct VIEW::drawItem
{
    drawItem( VIEW* aView, int aCurrentLayer ) :
        count( 0 ), countCached( 0 ), currentLayer( aCurrentLayer ), time( 0 ), view( aView )
    {
    }

    void operator()( VIEW_ITEM* aItem )
    {
        BOX2I    tmp;
        uint64_t ts  = rdtsc();
        GAL*     gal = view->GetGAL();

        if( view->m_useGroups )
        {
            int group = aItem->m_cachedGroup;

            if( group >= 0 && aItem->ViewIsVisible() )
            {
                gal->DrawGroup( group );
                countCached++;
            }
            else
            {
                group = gal->BeginGroup();
                aItem->m_cachedGroup = group;
                aItem->ViewDraw( 0, gal, tmp );
                gal->EndGroup();
                gal->DrawGroup( group );
            }
        }
        else if( aItem->ViewIsVisible() )
        {
            if( !( view->m_painter
                   && view->m_painter->Draw( static_cast<EDA_ITEM*>( aItem ), currentLayer ) ) )
            {
                // Fallback, if there is no painter or painter does not know how to draw aItem
                aItem->ViewDraw( currentLayer, gal, tmp );
            }
        }

        time += rdtsc() - ts;
        count++;
    }

    int      count;
    int      countCached;
    int      currentLayer;
    uint64_t time;
    VIEW*    view;
};


void VIEW::redrawRect( const BOX2I& aRect )
{
    int          totalItems = 0, totalCached = 0;
    uint64_t     totalDrawTime = 0;
    prof_counter totalCycles, totalRealTime;

    prof_start( &totalRealTime, false );
    prof_start( &totalCycles, true );

    BOOST_FOREACH( VIEW_LAYER* l, m_orderedLayers )
    {
        if( l->enabled )
        {
            drawItem drawFunc( this, l->id );

            m_gal->SetLayerDepth( (double) l->renderingOrder );
            l->items->Query( aRect, drawFunc );
            l->isDirty = false;

            totalItems    += drawFunc.count;
            totalDrawTime += drawFunc.time;
            totalCached   += drawFunc.countCached;
        }
    }

    prof_end( &totalCycles );
    prof_end( &totalRealTime );

    wxLogDebug( "Redraw::items %d (%d cached), %.1f ms/frame (%.0f FPS), draw/geometry ratio: %.1f%%",
            totalItems, totalCached, (double) totalRealTime.value / 1000.0,
            1000000.0 / (double) totalRealTime.value,
            (double) totalDrawTime / (double) totalCycles.value * 100.0 );
}


struct VIEW::unlinkItem
{
    void operator()( VIEW_ITEM* aItem )
    {
        aItem->m_view = NULL;
    }
};


void VIEW::Clear()
{
    BOX2I r;

    r.SetMaximum();

    for( LayerMapIter i = m_layers.begin(); i != m_layers.end(); ++i )
    {
        VIEW_LAYER* l = &( ( *i ).second );
        unlinkItem v;
        if( m_dynamic )
            l->items->Query( r, v );

        l->items->RemoveAll();
    }
}


void VIEW::Redraw()
{
    VECTOR2D screenSize = m_gal->GetScreenPixelSize();
    BOX2I    rect( ToWorld( VECTOR2D( 0, 0 ) ),
                   ToWorld( screenSize ) - ToWorld( VECTOR2D( 0, 0 ) ) );

    rect.Normalize();
    redrawRect( rect );
}


VECTOR2D VIEW::GetScreenPixelSize() const
{
    return m_gal->GetScreenPixelSize();
}


void VIEW::invalidateItem( VIEW_ITEM* aItem, int aUpdateFlags )
{
    int layer_indices[VIEW_MAX_LAYERS], layer_count;

    aItem->ViewGetLayers( layer_indices, layer_count );

    for( int i = 0; i < layer_count; i++ )
    {
        VIEW_LAYER* l = &m_layers[layer_indices[i]];

        l->dirtyExtents =
            l->isDirty ? aItem->ViewBBox() : l->dirtyExtents.Merge( aItem->ViewBBox() );

        if( aUpdateFlags & VIEW_ITEM::GEOMETRY )
        {
            l->items->Remove( aItem );
            l->items->Insert( aItem );    /* reinsert */

            if( m_useGroups )
                aItem->m_cachedGroup = -1;
        }
    }

    if( m_useGroups && aItem->m_cachedGroup >= 0 )
    {
        m_gal->DeleteGroup( aItem->m_cachedGroup );
        aItem->m_cachedGroup = -1;
    }
}


struct VIEW::clearItemCache
{
    clearItemCache( VIEW* aView ) :
        view( aView )
    {
    }

    void operator()( VIEW_ITEM* aItem )
    {
        if( aItem->m_cachedGroup >= 0 )
        {
            view->GetGAL()->DeleteGroup( aItem->m_cachedGroup );
            aItem->m_cachedGroup = -1;
        }
    }

    VIEW* view;
};


void VIEW::clearGroupCache()
{
    BOX2I r;

    r.SetMaximum();

    for( LayerMapIter i = m_layers.begin(); i != m_layers.end(); ++i )
    {
        VIEW_LAYER* l = & ( ( *i ).second );
        clearItemCache visitor( this );
        l->items->Query( r, visitor );
    };
}
