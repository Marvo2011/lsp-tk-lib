/*
 * Box.cpp
 *
 *  Created on: 20 июн. 2017 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/tk.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t Box::metadata   =   { "Box", &WidgetContainer::metadata     };

        Box::Box(Display *dpy):
            WidgetContainer(dpy),
            sSpacing(&sProperties),
            sHomogeneous(&sProperties),
            sOrientation(&sProperties)
        {
            pClass          = &metadata;
        }
        
        Box::~Box()
        {
            do_destroy();
        }

        status_t Box::init()
        {
            status_t result = WidgetContainer::init();
            if (result != STATUS_OK)
                return result;



            return STATUS_OK;
        }

        void Box::destroy()
        {
            do_destroy();
            WidgetContainer::destroy();
        }

        void Box::do_destroy()
        {
            size_t n_items  = vItems.size();
            for (size_t i=0; i<n_items; ++i)
            {
                // Get widget
                cell_t *w = vItems.uget(i);
                if (w->pWidget == NULL)
                    continue;

                unlink_widget(w->pWidget);
                w->pWidget = NULL;
            }

            vItems.flush();
        }

        bool Box::hidden_widget(const cell_t *w)
        {
            if ((w == NULL) || (w->pWidget == NULL))
                return true;
            return !w->pWidget->visibility()->get();
        }

        status_t Box::visible_items(lltl::parray<cell_t> *out, lltl::darray<cell_t> *list)
        {
            // Estimate number of visible items
            for (size_t i=0, n=list->size(); i<n; ++i)
            {
                // Get widget
                cell_t *w = list->uget(i);
                if (w->pWidget == NULL)
                    continue;
                if (!w->pWidget->visibility()->get())
                    continue;

                // Add visible widget to list
                if (!out->add(w))
                    return STATUS_NO_MEM;
            }

            return STATUS_OK;
        }

        Widget *Box::find_widget(ssize_t x, ssize_t y)
        {
            for (size_t i=0, n=vItems.size(); i<n; ++i)
            {
                cell_t *w = vItems.uget(i);
                if (hidden_widget(w))
                    continue;
                if (Position::inside(&w->s, x, y))
                    return w->pWidget;
            }

            return NULL;
        }

        void Box::property_changed(Property *prop)
        {
            Widget::property_changed(prop);

            if (sSpacing.is(prop))
                query_resize();
            if (sHomogeneous.is(prop))
                query_resize();
            if (sOrientation.is(prop))
                query_resize();
        }

        void Box::render(ws::ISurface *s, bool force)
        {
            // Check dirty flag
            if (nFlags & REDRAW_SURFACE)
                force = true;

            // Estimate palette
            lsp::Color bg_color(sBgColor);

            // Render child widgets
            lltl::parray<cell_t>    visible;
            status_t res    = visible_items(&visible, &vItems);
            if (res != STATUS_OK)
            {
                s->fill_rect(sSize.nLeft, sSize.nTop, sSize.nWidth, sSize.nHeight, bg_color);
                return;
            }

            // Draw background if needed
            if ((visible.is_empty()) && (force))
            {
                s->fill_rect(sSize.nLeft, sSize.nTop, sSize.nWidth, sSize.nHeight, bg_color);
                return;
            }

            // Draw items
            for (size_t i=0, n=visible.size(); i<n; ++i)
            {
                cell_t *wc = visible.uget(i);
                Widget *w = wc->pWidget;

                if ((force) || (w->redraw_pending()))
                {
                    // Fill unused space with background
                    if (force)
                    {
                        bg_color.copy(w->bg_color()->color());
                        s->fill_frame(
                            wc->a.nLeft, wc->a.nTop, wc->a.nWidth, wc->a.nHeight,
                            wc->s.nLeft, wc->s.nTop, wc->s.nWidth, wc->s.nHeight,
                            bg_color
                        );
                    }
                    w->render(s, force);
                    w->commit_redraw();
                }
            }
        }

        status_t Box::add(Widget *widget)
        {
            cell_t *cell = vItems.append();
            if (cell == NULL)
                return STATUS_NO_MEM;

            cell->r.nMinWidth   = -1;
            cell->r.nMinHeight  = -1;
            cell->r.nMaxWidth   = -1;
            cell->r.nMaxHeight  = -1;
            cell->a.nLeft       = 0;
            cell->a.nTop        = 0;
            cell->a.nWidth      = 0;
            cell->a.nHeight     = 0;
            cell->s.nLeft       = 0;
            cell->s.nTop        = 0;
            cell->s.nWidth      = 0;
            cell->s.nHeight     = 0;
            cell->pWidget       = widget;

            if (widget != NULL)
                widget->set_parent(this);

            query_resize();
            return STATUS_SUCCESS;
        }

        status_t Box::remove(Widget *child)
        {
            size_t n            = vItems.size();
            for (size_t i=0; i<n; ++i)
            {
                cell_t *cell        = vItems.uget(i);
                if (cell->pWidget == child)
                {
                    if (!vItems.remove(i))
                        return STATUS_UNKNOWN_ERR;
                    query_resize();
                    child->set_parent(NULL);
                    return STATUS_OK;
                }
            }

            return STATUS_NOT_FOUND;
        }

        status_t Box::remove_all()
        {
            if (vItems.size() <= 0)
                return STATUS_OK;

            lltl::darray<cell_t> tmp;
            vItems.swap(&tmp);

            for (size_t i=0, n=tmp.size(); i<n; ++i)
            {
                cell_t *cell        = tmp.uget(i);
                if (cell->pWidget != NULL)
                    cell->pWidget->set_parent(NULL);
            }

            tmp.flush();
            query_resize();
            return STATUS_OK;
        }

        status_t Box::allocate_homogeneous(const ws::rectangle_t *r, lltl::parray<cell_t> &visible)
        {
            return STATUS_OK;
        }

        status_t Box::allocate_proportional(const ws::rectangle_t *r, lltl::parray<cell_t> &visible)
        {
            // Get the final area to perform allocation
            float scaling       = lsp_max(0.0f, sScaling.get());
            ssize_t spacing     = scaling * sSpacing.get();
            bool horizontal     = sOrientation.horizontal();

            ssize_t n_left      = (horizontal) ? r->nWidth : r->nHeight;
            n_left             -= spacing * (visible.size() - 1);
            size_t n_size       = n_left;

            // FIRST PASS: Initialize widgets with their minimum widths
            // Estimate number of expanded widgets and space used by them
            lltl::parray<cell_t>    expand;
            size_t n_expand     = 0;

            for (size_t i=0, n=visible.size(); i<n; ++i)
            {
                // Get widget
                cell_t *w =          visible.uget(i);

                // Request size limit and padding of the widget
                w->pWidget->get_size_limits(&w->r);
                w->pWidget->get_padding(&w->p);

                if (horizontal)
                {
                    w->a.nWidth         = w->p.nLeft + w->p.nRight;     // Add padding to allocation
                    w->a.nWidth        += lsp_max(0, w->r.nMinWidth);   // Add minimum width to allocation
                    w->a.nHeight        = r->nHeight;                   // All allocations have same height for horizontal box
                    n_left             -= w->a.nWidth;

                    // Estimate number of expanded widgets and their allocated space
                    if (w->pWidget->allocation()->hexpand())
                    {
                        n_expand           += w->a.nWidth;
                        if (!expand.add(w))
                            return STATUS_NO_MEM;
                    }
                }
                else // vertical
                {
                    w->a.nHeight        = w->p.nTop + w->p.nBottom;     // Add padding to allocation
                    w->a.nHeight       += lsp_max(0, w->r.nMinHeight);  // Add minimum height to allocation
                    w->a.nWidth         = r->nWidth;                    // All allocation have same width for vertical box
                    n_left             -= w->a.nHeight;

                    // Estimate number of expanded widgets and their allocated space
                    if (w->pWidget->allocation()->vexpand())
                    {
                        n_expand           += w->a.nHeight;
                        if (!expand.add(w))
                            return STATUS_NO_MEM;
                    }
                }
            }

            // SECOND PASS: Split unused space between widgets
            if (n_left > 0)
            {
                ssize_t total = 0;

                if (expand.is_empty())
                {
                    // Split unused space between all visible widgets
                    for (size_t i=0, n=visible.size(); i<n; ++i)
                    {
                        // Get widget
                        cell_t *w = visible.uget(i);

                        if (horizontal)
                        {
                            ssize_t delta   = (w->a.nWidth * n_left) / n_size;
                            w->a.nWidth    += delta;
                            total          += delta;
                        }
                        else // vertical
                        {
                            ssize_t delta   = (w->a.nHeight * n_left) / n_size;
                            w->a.nHeight   += delta;
                            total          += delta;
                        }
                    }
                }
                else if (n_expand == 0) // All widgets have minimum width 0
                {
                    // Split unused space between all expanded widgets
                    ssize_t delta   = n_left / expand.size();
                    for (size_t i=0, n=expand.size(); i<n; ++i)
                    {
                        // Get widget
                        cell_t *w       = expand.uget(i);

                        if (horizontal)
                        {
                            w->a.nWidth    += delta;
                            total          += delta;
                        }
                        else // vertical
                        {
                            w->a.nHeight   += delta;
                            total          += delta;
                        }
                    }
                }
                else
                {
                    // Split unused space between all expanded widgets
                    // Distribute space proportionally to the current area size
                    for (size_t i=0, n=expand.size(); i<n; ++i)
                    {
                        // Get widget
                        cell_t *w       = expand.uget(i);

                        if (horizontal)
                        {
                            ssize_t delta   = (w->a.nWidth * n_left) / n_expand;
                            w->a.nWidth    += delta;
                            total          += delta;
                        }
                        else // vertical
                        {
                            ssize_t delta   = (w->a.nHeight * n_left) / n_expand;
                            w->a.nHeight   += delta;
                            total          += delta;
                        }
                    }
                } // n_expand

                n_left     -= total;
            }

            // FOURTH PASS: utilize still unallocated pixels between all visible widgets
            while (n_left > 0)
            {
                for (size_t i=0, n=visible.size(); i<n; ++i)
                {
                    // Get widget
                    cell_t *w = visible.uget(i);
                    if (horizontal)
                        w->a.nWidth     ++;
                    else // vertical
                        w->a.nHeight    ++;

                    if ((n_left--) <= 0)
                        break;
                }
            }

            // Now we have n_left=0, now need to generate proper Left and Top coordinates of widget
            ssize_t l = r->nLeft, t = r->nTop; // Left-Top corner

            // Now completely apply geometry to each allocation
            for (size_t i=0, n=visible.size(); i<n; ++i)
            {
                // Get widget
                cell_t *w       = visible.uget(i);

                // Initial coordinates
                w->a.nLeft      = l;
                w->a.nTop       = t;
                Padding::enter(&w->s, &w->a, &w->p); // Estimate real space used by widget

                // Compute coordinates of next cell
                if (horizontal)
                {
                    if ((i + 1) < n)
                        w->a.nWidth    += spacing; // Add spacing to fill gaps between cells on rendering
                    l              += w->a.nWidth;
                }
                else // vertical
                {
                    if ((i + 1) < n)
                        w->a.nHeight   += spacing; // Add spacing to fill gaps between cells on rendering
                    t              += w->a.nHeight;
                }
            }

            return STATUS_OK;
        }

        void Box::realize_widgets(lltl::parray<cell_t> &visible)
        {
            ws::rectangle_t r;

            for (size_t i=0, n=visible.size(); i<n; ++i)
            {
                // Get widget
                cell_t *w       = visible.uget(i);

                // Allocated widget area may be too large, restrict it with size constraints
                SizeConstraints::apply(&r, &w->s, &w->r);

                // Estimate the real widget allocation size
                ssize_t xw      = (w->pWidget->allocation()->hfill()) ? r.nWidth    : lsp_limit(w->r.nMinWidth,  0, r.nWidth );
                ssize_t xh      = (w->pWidget->allocation()->vfill()) ? r.nHeight   : lsp_limit(w->r.nMinHeight, 0, r.nHeight);

                // Update location of the widget
                w->s.nLeft     += lsp_max(0, w->s.nWidth  - r.nWidth ) >> 1;
                w->s.nTop      += lsp_max(0, w->s.nHeight - r.nHeight) >> 1;
                w->s.nWidth     = xw;
                w->s.nHeight    = xh;

                // Realize the widget
//                lsp_trace("realize id=%d, parameters = {%d, %d, %d, %d}", int(i), int(w->s.nLeft), int(w->s.nTop), int(w->s.nWidth), int(w->s.nHeight));
                w->pWidget->realize(&w->s);
                w->pWidget->query_draw();
            }
        }

        void Box::realize(const ws::rectangle_t *r)
        {
            // Make a copy of current widget list
            lltl::darray<cell_t>    items;
            if (items.add(&vItems))
            {
                lltl::parray<cell_t>    visible;
                status_t res    = visible_items(&visible, &items);

                // Realize child widgets
                if ((res == STATUS_OK) && (visible.size() > 0))
                {
                    res = (sHomogeneous.get()) ?
                        allocate_homogeneous(r, visible) :
                        allocate_proportional(r, visible);
                }

                // Update state of all widgets
                if (res == STATUS_OK)
                {
                    realize_widgets(visible);
                    vItems.swap(&items);
                }
            }

            // Call parent method to realize
            WidgetContainer::realize(r);
        }

        void Box::size_request(ws::size_limit_t *r)
        {
            r->nMinWidth    = -1;
            r->nMinHeight   = -1;
            r->nMaxWidth    = -1;
            r->nMaxHeight   = -1;

            size_t n_items  = vItems.size();
            if (n_items <= 0)
                return;

            ssize_t e_width = 0, e_height = 0; // Estimated width and height

            // Estimate self size
            for (size_t i=0; i<n_items; ++i)
            {
                // Get widget
                cell_t *w = vItems.at(i);
                if (hidden_widget(w))
                    continue;

                // Perform size request
                w->r.nMinWidth      = -1;
                w->r.nMinHeight     = -1;
                w->r.nMaxWidth      = -1;
                w->r.nMaxHeight     = -1;
                if (w->pWidget == NULL)
                    continue;

                w->pWidget->size_request(&w->r);
                w->pWidget->padding()->get(&w->p);
//                lsp_trace("size_request id=%d, parameters = {%d, %d, %d, %d}",
//                    int(i), int(w->r.nMinWidth), int(w->r.nMinHeight), int(w->r.nMaxWidth), int(w->r.nMaxHeight));

                // Analyze widget class
                ssize_t x_width     = w->p.nLeft + w->p.nRight;
                ssize_t x_height    = w->p.nTop  + w->p.nBottom;
                if (w->r.nMinWidth >= 0)
                    x_width            += w->r.nMinWidth;
                if (w->r.nMinHeight >= 0)
                    x_height           += w->r.nMinHeight;

                if (enOrientation == O_HORIZONTAL)
                {
                    if (x_height > e_height)
                        e_height        = x_height;
                    e_width            += x_width;
                    if (i > 0)
                        e_width        += nSpacing;
                }
                else // VBOX
                {
                    if (x_width > e_width)
                        e_width         = x_width;
                    e_height           += x_height;
                    if (i > 0)
                        e_height       += nSpacing;
                }
            }

            r->nMinWidth        = e_width;
            r->nMinHeight       = e_height;
        }
    
    } /* namespace tk */
} /* namespace lsp */
