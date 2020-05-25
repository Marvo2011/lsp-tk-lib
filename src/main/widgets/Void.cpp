/*
 * Void.cpp
 *
 *  Created on: 11 апр. 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/tk.h>
#include <lsp-plug.in/common/debug.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t Void::metadata      = { "Void", &Widget::metadata };
        
        Void::Void(Display *dpy):
            Widget(dpy),
            sConstraints(&sProperties)
        {
            pClass          = &metadata;
        }
        
        Void::~Void()
        {
        }

        status_t Void::init()
        {
            status_t res = Widget::init();
            if (res != STATUS_OK)
                return res;

            sConstraints.bind("size.constraints", &sStyle);

            Style *sclass = style_class();
            if (sclass != NULL)
                sConstraints.init(sclass, -1, -1, -1, -1);

            return res;
        }

        void Void::render(ws::ISurface *s, bool force)
        {
            if ((sSize.nWidth > 0) && (sSize.nHeight > 0))
            {
                lsp::Color bg_color(sBgColor);
                s->fill_rect(sSize.nLeft, sSize.nTop, sSize.nWidth, sSize.nHeight, bg_color);

                // Debug padding
                ws::rectangle_t xr;
                bg_color.set_rgb24(0);
                sPadding.leave(&xr, &sSize, sScaling.get());
                s->fill_frame(
                    xr.nLeft, xr.nTop, xr.nWidth, xr.nHeight,
                    sSize.nLeft, sSize.nTop, sSize.nWidth, sSize.nHeight,
                    bg_color
                );
            }
        }

        void Void::size_request(ws::size_limit_t *r)
        {
            // Add external size constraints
            float scaling = lsp_max(0.0f, sScaling.get());
            sConstraints.compute(r, scaling);
            lsp_trace("this=%p, w={%d, %d}, h={%d, %d}", this, int(r->nMinWidth), int(r->nMaxWidth), int(r->nMinHeight), int(r->nMaxHeight));
        }

        void Void::property_changed(Property *prop)
        {
            Widget::property_changed(prop);

            if (sConstraints.is(prop))
                query_resize();
        }

    } /* namespace tk */
} /* namespace lsp */
